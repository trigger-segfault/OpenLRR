// PESectionStream.h : PE class declaration for reading and writing section buffers.
//

#pragma once

#include "PECommon.h"
#include "PEFile.h"


namespace PE
{; // !<---

/////////////////////////////////////////////////////////////////////
// Classes

class PESectionStream
{
private:
	PESectionSPtr m_section;
	sectionoff32_t m_position;
	bool m_expandable;


public:
	PESectionStream(PESectionSPtr section, bool expandable = false);
	PESectionStream(PEFile& pefile, const std::string& name, bool expandable = false);


	#pragma region Accessors/Mutators

	inline PE::PESectionSPtr Section() noexcept { return m_section; }

	// Shorthand for Section()->Name()
	inline std::string Name() const { m_section->Name(); }

	// True if writing past the section size will increase the buffer size to make room.
	constexpr bool IsExpandable() const noexcept { return m_expandable; }
	constexpr void SetExpandable(bool expandable) noexcept { m_expandable = expandable; }
	
	#pragma endregion

	#pragma region Address I/O conversion

	inline address32_t TellAddress(const PEFile& pefile) const
	{
		return pefile.RVA2Address(this->TellRVA());
	}

	inline rva32_t TellRVA() const
	{
		return m_section->Section2RVA(this->Tell());
	}

	inline fileoff32_t TellFile() const
	{
		return m_section->Section2File(this->Tell());
	}

	inline sectionoff32_t TellSection() const
	{
		return this->Tell();
	}

	inline void SeekAddress(const PEFile& pefile, address_t address)
	{
		this->SeekRVA(pefile.Address2RVA(address));
	}

	inline void SeekRVA(rva_t rva)
	{
		this->Seek(m_section->RVA2Section(rva), SEEK_SET);
	}

	inline void SeekFile(fileoff_t fileOffset)
	{
		this->Seek(m_section->File2Section(fileOffset), SEEK_SET);
	}

	inline void SeekSection(sectionoff_t sectionOffset)
	{
		this->Seek(sectionOffset, SEEK_SET);
	}

	#pragma endregion

	#pragma region Stream I/O

	sectionoff32_t Tell() const;

	void Seek(sectionoff_t offset, int32_t origin);

	bool IsEOF() const;

	int32_t GetC();

	int32_t PutC(int32_t c);

	size32_t Read(OUT void* buffer, size32_t size);

	size32_t Write(const void* buffer, size32_t size);

	//void Flush();
	
	#pragma endregion

	#pragma region Extended stream I/O

	std::string ReadCString();

	size32_t WriteCString(const std::string& str);

	size32_t Length() const;

	void SetLength(size32_t newLength);

	#pragma endregion
};

}
