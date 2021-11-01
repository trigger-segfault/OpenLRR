// PEFile.cpp : PE file and section buffer classes for low level modification of executables.
//

#include "PECommon.h"

#include "PEFile.h"


/////////////////////////////////////////////////////////////////////
// PE Section class

PE::PESection::PESection() : Header({ 0 }), Data()
{
}


/*std::string PE::PESection::Name() const
{
	return std::string(Header.Name, ::strnlen_s(Header.Name, sizeof(Header.Name)));
}

void PE::PESection::SetName(const std::string& newName)
{
	std::strncpy(Header.Name, newName.c_str(), sizeof(Header.Name));
}*/


/////////////////////////////////////////////////////////////////////
// PE File class

PE::PEFile::PEFile()
	: m_file(nullptr), m_dos({ 0 }), m_nt({ 0 }), m_sections()
{
}

PE::PEFile::PEFile(const tstring& filename)
	: PEFile()
{
	this->Load(filename);
}


#pragma region Section Lookup

PE::PESectionSPtr PE::PEFile::CreateSection()
{
	return std::make_shared<PESection>();
}

PE::PESectionSPtr PE::PEFile::CreateSection(const std::string& name)
{
	PESectionSPtr section = this->CreateSection();
	section->SetName(name);
	return section;
}

void PE::PEFile::AddSection(PESectionSPtr section)
{
	m_sections.push_back(section);
	m_nt.FileHeader.NumberOfSections = (uint16_t)m_sections.size();
}

void PE::PEFile::InsertSectionAt(int32_t index, PESectionSPtr section)
{
	m_sections.insert(m_sections.begin() + index, section);
	m_nt.FileHeader.NumberOfSections = (uint16_t)m_sections.size();
}

void PE::PEFile::RemoveSectionAt(int32_t index)
{
	m_sections.erase(m_sections.begin() + index);
	m_nt.FileHeader.NumberOfSections = (uint16_t)m_sections.size();
}

PE::PESectionSPtr PE::PEFile::GetSectionAt(int32_t index)
{
	return m_sections[index];
}

bool PE::PEFile::ContainsSection(const PESectionSPtr& section) const
{
	return this->IndexOfSection(section) != -1;
}

bool PE::PEFile::ContainsSection(const std::string& name) const
{
	return this->IndexOfSection(name) != -1;
}

int32_t PE::PEFile::IndexOfSection(const PESectionSPtr& section) const
{
	for (size_t i = 0; i < m_sections.size(); i++) {
		if (section == m_sections[i])
			return (int32_t)i;
	}
	return -1;
}

int32_t PE::PEFile::IndexOfSection(const std::string& name) const
{
	if (name.length() > sizeof(IMAGE_SECTION_HEADER::Name))
		return -1; // never exists, we don't want a false match while using strncmp

	for (size_t i = 0; i < m_sections.size(); i++) {
		auto& section = m_sections[i];
		const auto& header = section->Header;
		if (std::strncmp((const ascii*)header.Name, name.c_str(), sizeof(header.Name)) == 0)
			return (int32_t)i;
	}
	return -1;
}


PE::PESectionSPtr PE::PEFile::GetSection(const std::string& name) const
{
	int32_t index = this->IndexOfSection(name);
	if (index != -1)
		return m_sections[index];
	return nullptr;
}

PE::PESectionSPtr PE::PEFile::GetSectionAtRVA(rva_t rva) const
{
	if (rva < 0)
		return nullptr;

	for (auto& section : m_sections) {
		const auto& header = section->Header;
		if (((rva32_t)rva >= header.VirtualAddress) &&
			((rva32_t)rva < (header.VirtualAddress + header.VirtualSize)))
		{
			return section;
		}
	}
	return nullptr;
}

PE::PESectionSPtr PE::PEFile::GetSectionAtAddress(address_t address) const
{
	return this->GetSectionAtRVA(this->Address2RVA(address));
}

PE::PESectionSPtr PE::PEFile::GetSectionAtFile(fileoff_t fileOffset) const
{
	if (fileOffset < 0)
		return nullptr;

	for (auto& section : m_sections) {
		const auto& header = section->Header;
		if (((fileoff32_t)fileOffset >= header.PointerToRawData) &&
			((fileoff32_t)fileOffset < (header.PointerToRawData + header.SizeOfRawData)))
		{
			return section;
		}
	}
	return nullptr;
}

#pragma endregion

#pragma region Load/Save

bool PE::PEFile::Load(const tstring& filename)
{
	m_errorMessage = ""; // clear error message

	std::memset(&m_dos, 0, sizeof(m_dos)); // Clear fields
	std::memset(&m_nt, 0, sizeof(m_nt));
	m_sections.clear();

	m_file = _tfopen(filename.c_str(), _T("rb"));
	if (m_file == nullptr) {
		m_errorMessage = "Failed to open file for reading";
		return false;
	}

	try {
		this->Read(&m_dos, sizeof(m_dos));

		this->Read(&m_nt.Signature, sizeof(m_nt.Signature));
		this->Read(&m_nt.FileHeader, sizeof(m_nt.FileHeader));

		// Unexpected optional header size.
		if (m_nt.FileHeader.SizeOfOptionalHeader > sizeof(IMAGE_OPTIONAL_HEADER32))
			throw std::exception("Optional header size too large, not supported");

		this->Read(&m_nt.OptionalHeader, m_nt.FileHeader.SizeOfOptionalHeader);

		if (m_nt.OptionalHeader.NumberOfRvaAndSizes < 3) {
			throw std::exception("Optional header NumberOfRvaAndSizes too low, expected at least 3");
		}

		// Read section headers.
		m_sections.resize(m_nt.FileHeader.NumberOfSections);

		for (size_t i = 0; i < m_sections.size(); i++) {
			auto section = this->CreateSection();
			m_sections[i] = section;

			this->Read(&section->Header, sizeof(section->Header));
		}

		// Read section payloads.
		for (auto& section : m_sections) {
			section->Data.resize(this->FileAlign(section->Header.SizeOfRawData));

			this->Seek(section->Header.PointerToRawData, SEEK_SET);
			this->Read(section->Data.data(), section->Header.SizeOfRawData);
		}

		this->Close();
	}
	catch (const std::exception& ex) {
		m_errorMessage = ex.what();
		this->Close();
		//return false;
		throw;
	}

	return true;
}

bool PE::PEFile::Save(const tstring& filename)
{
	m_errorMessage = ""; // clear error message

	m_file = _tfopen(filename.c_str(), _T("wb"));
	if (m_file == nullptr) {
		m_errorMessage = "Failed to open file for writing";
		return false;
	}
	try {
		// Unexpected optional header size.
		if (m_nt.FileHeader.SizeOfOptionalHeader > sizeof(IMAGE_OPTIONAL_HEADER32))
			throw std::exception("Optional header size too large, not supported");

		// Write PE headers.
		this->Write(&m_dos, sizeof(m_dos));
		//this->Write(&m_nt, sizeof(m_nt));

		this->Write(&m_nt.Signature, sizeof(m_nt.Signature));
		this->Write(&m_nt.FileHeader, sizeof(m_nt.FileHeader));
		this->Write(&m_nt.OptionalHeader, m_nt.FileHeader.SizeOfOptionalHeader);

		// Write section headers.
		for (auto& section : m_sections) {
			const auto& header = section->Header;
			this->Write(&header, sizeof(header));
		}
		// Pad remainder of header size alignment.
		this->WritePadding(m_nt.OptionalHeader.SizeOfHeaders);

		// Write section payloads.
		for (auto& section : m_sections) {
			const auto& header = section->Header;
			this->Seek(header.PointerToRawData, SEEK_SET);
			this->Write(section->Data.data(), header.SizeOfRawData);

			// Pad remainder of file alignment until next section/or EOF.
			this->WritePadding(header.PointerToRawData + this->FileAlign(header.SizeOfRawData));
		}

		// Flush changes and close the file.
		this->Close();
	}
	catch (const std::exception& ex) {
		m_errorMessage = ex.what();
		this->Close();
		return false;
	}

	return true;
}

void PE::PEFile::UpdateSizeOfImage(bool noshrink)
{
	// SectionAlignment (0x1000) is assumed to be the minimum offset before starting the first section.
	// Not fully understood.
	uint32_t newSize = (noshrink ? m_nt.OptionalHeader.SizeOfImage : m_nt.OptionalHeader.SectionAlignment);

	for (const auto& section : m_sections) {
		const auto& header = section->Header;
		newSize = std::max(newSize, (header.VirtualAddress + header.VirtualSize));
	}
	m_nt.OptionalHeader.SizeOfImage = this->VirtualAlign(newSize);
}

#pragma endregion

#pragma region Load/Save helpers

void PE::PEFile::Close() noexcept
{
	if (m_file != nullptr) {
		std::fflush(m_file);
		std::fclose(m_file);
		m_file = nullptr;
	}
}

size32_t PE::PEFile::WritePadding(fileoff32_t endOffset)
{
	fileoff32_t currPos = this->Tell();
	if (endOffset < currPos)
		throw std::exception("Invalid padding endOffset less than current position");

	size32_t padding = (endOffset - currPos);

	if (padding != 0) {
		uint8_t* padBytes = new uint8_t[padding];
		std::memset(padBytes, 0, padding);
		this->Write(padBytes, padding);
		delete[] padBytes;
	}

	return padding;
}

#pragma endregion

#pragma region Load/Save file wrappers

fileoff32_t PE::PEFile::Tell()
{
	long result = std::ftell(m_file);
	if (result == -1)
		throw std::exception("ftell failed");
	return (fileoff32_t)result;
}
void PE::PEFile::Seek(fileoff_t offset, int32_t origin)// = SEEK_SET)
{
	if (std::fseek(m_file, (long)offset, origin) != 0)
		throw std::exception("fseek failed");
}

size32_t PE::PEFile::Read(OUT void* buffer, size32_t size)
{
	if (std::fread(buffer, size, 1, m_file) != 1)
		throw std::exception("fread failed");
	return size;
}

size32_t PE::PEFile::Write(const void* buffer, size32_t size)
{
	if (std::fwrite(buffer, size, 1, m_file) != 1)
		throw std::exception("fwrite failed");
	return size;
}

#pragma endregion

