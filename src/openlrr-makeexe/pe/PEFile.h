// PEFile.h : PE file and section buffer classes for low level modification of executables.
//

#pragma once

#include "PECommon.h"


namespace PE
{; // !<---

/////////////////////////////////////////////////////////////////////
// Forward declarations and Typedefs

class PESection;
typedef std::shared_ptr<PESection>			PESectionSPtr;


/////////////////////////////////////////////////////////////////////
// PE Section class

class PESection
{
public:
	IMAGE_SECTION_HEADER Header;
	std::vector<uint8_t> Data;


public:
	PESection();


	#pragma region Accessors/Mutators

	inline std::string Name() const
	{
		return std::string(Header.Name, ::strnlen_s(Header.Name, sizeof(Header.Name)));
	}

	inline void SetName(const std::string& newName)
	{
		std::strncpy(Header.Name, newName.c_str(), sizeof(Header.Name));
	}

	#pragma endregion
	
	#pragma region Address offset conversion

	constexpr fileoff_t RVA2File(rva_t rva) const noexcept
	{
		return Header.PointerToRawData + (rva - Header.VirtualAddress);
	}

	constexpr sectionoff_t RVA2Section(rva_t rva) const noexcept
	{
		return (rva - Header.VirtualAddress);
	}

	constexpr rva_t File2RVA(fileoff_t fileOffset) const noexcept
	{
		return Header.VirtualAddress + (fileOffset - Header.PointerToRawData);
	}

	constexpr sectionoff_t File2Section(fileoff_t fileOffset) const noexcept
	{
		return (fileOffset - Header.PointerToRawData);
	}

	constexpr rva_t Section2RVA(sectionoff_t sectionOffset) const noexcept
	{
		return (sectionOffset + Header.VirtualAddress);
	}

	constexpr fileoff_t Section2File(sectionoff_t sectionOffset) const noexcept
	{
		return (sectionOffset + Header.PointerToRawData);
	}

	#pragma endregion
};


/////////////////////////////////////////////////////////////////////
// PE File class

class PEFile
{
private:
	FILE* m_file;
	std::string m_errorMessage;
	IMAGE_DOS_HEADER m_dos;
	IMAGE_NT_HEADERS32 m_nt;
	std::vector<PESectionSPtr> m_sections;


public:
	PEFile();
	PEFile(const tstring& filename);


	#pragma region Accessors

	constexpr bool IsOpen() const noexcept { return m_file != nullptr; }
	constexpr IMAGE_DOS_HEADER& DOSHeader() noexcept { return m_dos; }
	constexpr IMAGE_NT_HEADERS32& NTHeader() noexcept { return m_nt; }
	constexpr IMAGE_FILE_HEADER& FileHeader() noexcept { return m_nt.FileHeader; }
	constexpr IMAGE_OPTIONAL_HEADER32& OptionalHeader() noexcept { return m_nt.OptionalHeader; }
	constexpr const std::vector<PESectionSPtr>& Sections() const noexcept { return m_sections; }

	constexpr uint32_t NumberOfSections() const noexcept { return m_nt.FileHeader.NumberOfSections; }
	constexpr uint32_t SizeOfHeaders() const noexcept { return m_nt.OptionalHeader.SizeOfHeaders; }
	constexpr uint32_t ImageBase() const noexcept { return m_nt.OptionalHeader.ImageBase; }
	constexpr uint32_t FileAlignment() const noexcept { return m_nt.OptionalHeader.FileAlignment; }
	constexpr uint32_t SectionAlignment() const noexcept { return m_nt.OptionalHeader.SectionAlignment; }

	constexpr IMAGE_DATA_DIRECTORY& ImportDirectory() noexcept { return m_nt.OptionalHeader.DataDirectory[1]; }
	constexpr IMAGE_DATA_DIRECTORY& ResourceDirectory() noexcept { return m_nt.OptionalHeader.DataDirectory[2]; }

	constexpr const std::string& ErrorMessage() const noexcept { return m_errorMessage; }

	#pragma endregion

	#pragma region Address offset alignment

	/**
	 * Rounds the specified offset up to a multiple of SectionAlignment.
	 * @return The rounded up alignment, or input alignment if already a multiple.
	 */
	inline rva32_t VirtualAlign(rva32_t virtualOffset) const
	{
		const uint32_t align = m_nt.OptionalHeader.SectionAlignment;
		return ((virtualOffset + align - 1) / align) * align;
	}

	/**
	 * Rounds the specified offset up to a multiple of FileAlignment.
	 * @return The rounded up alignment, or input alignment if already a multiple.
	 */
	inline fileoff32_t FileAlign(fileoff32_t fileOffset) const
	{
		const uint32_t align = m_nt.OptionalHeader.FileAlignment;
		return ((fileOffset + align - 1) / align) * align;
	}

	#pragma endregion

	#pragma region Address offset conversion

	/**
	 * Converts an RVA offset to an absolute address offset.
	 * @return The RVA offset with the added ImageBase offset.
	 */
	constexpr address_t RVA2Address(rva_t rva) const noexcept
	{
		return (rva + m_nt.OptionalHeader.ImageBase);
	}

	/**
	 * Converts an RVA offset to a raw file offset, by finding the containing section.
	 * @return The raw file offset relative to the matched section RVA offset.
	 */
	inline fileoff_t RVA2File(rva_t rva) const
	{
		return this->GetSectionAtRVA(rva)->RVA2File(rva);
	}

	/**
	 * Converts an absolute address offset to an RVA offset.
	 * @return The absolute address offset with the subtracted ImageBase offset.
	 */
	constexpr rva_t Address2RVA(address_t address) const noexcept
	{
		return (address - m_nt.OptionalHeader.ImageBase);
	}

	/**
	 * Converts an absolute address offset to a raw file offset, by finding the containing section.
	 * @return The raw file offset relative to the matched section address offset.
	 */
	inline fileoff_t Address2File(address_t address) const
	{
		return this->RVA2File(this->Address2RVA(address));
	}

	/**
	 * Converts a raw file offset to an RVA offset, by finding the containing section.
	 * @return The RVA offset relative to the matched section file offset.
	 */
	inline rva_t File2RVA(fileoff_t fileOffset) const
	{
		return this->GetSectionAtFile(fileOffset)->File2RVA(fileOffset);
	}

	/**
	 * Converts a raw file offset to an absolute address offset, by finding the containing section.
	 * @return The absolute address offset relative to the matched section file offset.
	 */
	inline address_t File2Address(fileoff_t fileOffset) const
	{
		return this->RVA2Address(this->File2RVA(fileOffset));
	}

	#pragma endregion

	#pragma region Section Lookup

	/**
	 * Create and return a new PESection shared_ptr.
	 */
	PESectionSPtr CreateSection();

	/**
	 * Create and return a new PESection shared_ptr, with an assigned name.
	 */
	PESectionSPtr CreateSection(const std::string& name);

	/**
	 * Add a PESection to the end of the sections list.
	 */
	void AddSection(PESectionSPtr section);

	/**
	 * Insert a PESection at the specified index in the sections list.
	 */
	void InsertSectionAt(int32_t index, PESectionSPtr section);

	/**
	 * Remove the PESection at the specified index in the sections list.
	 */
	void RemoveSectionAt(int32_t index);

	/**
	 * Gets the PESection at the specified index in the sections list.
	 */
	PESectionSPtr GetSectionAt(int32_t index);

	/**
	 * Tests if the PESection exists in the sections list.
	 */
	bool ContainsSection(const PESectionSPtr& section) const;

	/**
	 * Tests if a PESection with the specified name exists in the sections list.
	 */
	bool ContainsSection(const std::string& name) const;

	/**
	 * Gets the index of the PESection in the sections list.
	 * @return Index of the section, or -1 if not found.
	 */
	int32_t IndexOfSection(const PESectionSPtr& section) const;

	/**
	 * Gets the index of a PESection with the specified name.
	 * @return Index of the section, or -1 if not found.
	 */
	int32_t IndexOfSection(const std::string& name) const;


	/**
	 * Gets the PESection with the specified name.
	 * @return The first matching section, or nullptr if not found.
	 */
	PESectionSPtr GetSection(const std::string& name) const;

	/**
	 * Gets the PESection thet contains the specified RVA offset.
	 * @return The first matching section, or nullptr if not found.
	 */
	PESectionSPtr GetSectionAtRVA(rva_t rva) const;

	/**
	 * Gets the PESection thet contains the specified absolute address offset.
	 * @return The first matching section, or nullptr if not found.
	 */
	PESectionSPtr GetSectionAtAddress(address_t address) const;

	/**
	 * Gets the PESection thet contains the specified raw file offset.
	 * @return The first matching section, or nullptr if not found.
	 */
	PESectionSPtr GetSectionAtFile(fileoff_t fileOffset) const;

	#pragma endregion

	#pragma region Load/Save

	/**
	 * Load a new PE file to work with.
	 * @return True on success, otherwise false with the reason stored in ErrorMessage().
	 */
	bool Load(const tstring& filename);

	/**
	 * Save changes made to a previously-loaded PE file.
	 * @return True on success, otherwise false with the reason stored in ErrorMessage().
	 */
	bool Save(const tstring& filename);

	/**
	 * Recalculates IMAGE_OPTIONAL_HEADER32.SizeOfImage, for when section locations/sizes have changed.
	 */
	void UpdateSizeOfImage(bool noshrink = false);

	#pragma endregion

private:
	#pragma region Load/Save helpers

	/**
	 * Helper to close a PE file during Load/Save functions.
	 */
	void Close() noexcept;

	/**
	 * Writes zero-bytes to the file stream to pad the remainder of start -> size based on the current position.
	 */
	size32_t WritePadding(fileoff32_t endOffset);

	#pragma endregion

	#pragma region Load/Save file wrappers

	fileoff32_t Tell();

	void Seek(fileoff_t offset, int32_t origin);

	size32_t Read(OUT void* buffer, size32_t size);

	size32_t Write(const void* buffer, size32_t size);
	
	#pragma endregion
};

}
