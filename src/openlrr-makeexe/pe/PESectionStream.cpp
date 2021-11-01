// PESectionStream.cpp : PE class definition for reading and writing section buffers.
//

#include "PEFile.h"

#include "PESectionStream.h"


/////////////////////////////////////////////////////////////////////
// Classes

PE::PESectionStream::PESectionStream(PESectionSPtr section, bool expandable)
	: m_section(section), m_position(0), m_expandable(expandable)
{
	if (m_section == nullptr)
		throw std::exception("section cannot be null");
}
PE::PESectionStream::PESectionStream(PEFile& pefile, const std::string& name, bool expandable)
	: m_section(nullptr), m_position(0), m_expandable(expandable)
{
	m_section = pefile.GetSection(name);
	if (m_section == nullptr)
		throw std::exception("section with name not found");
}


#pragma region Stream I/O

sectionoff32_t PE::PESectionStream::Tell() const
{
	return m_position;
}

void PE::PESectionStream::Seek(sectionoff_t offset, int32_t origin)
{
	sectionoff_t length = this->Length();
	switch (origin) {
	case SEEK_SET: break; // do nothing
	case SEEK_CUR: offset += m_position; break;
	case SEEK_END: offset += length; break;
	}

	if (offset < 0)
		offset = 0;
	else if (offset > length) {
		if (!m_expandable) {
			// Only cap offset when not expandable.
			offset = length;
		}
		else {
			// Otherwise support resizing buffer to fit offset.
			this->SetLength(offset);
		}
	}

	m_position = offset;
}

bool PE::PESectionStream::IsEOF() const
{
	return m_position >= this->Length();
}

int32_t PE::PESectionStream::GetC()
{
	if (m_position >= this->Length())
		return EOF;

	uint8_t cByte;
	this->Read(&cByte, sizeof(cByte));
	return cByte;
}

int32_t PE::PESectionStream::PutC(int32_t c)
{
	uint8_t cByte = (uint8_t)c;
	this->Write(&cByte, sizeof(cByte));
	return cByte;
}

size32_t PE::PESectionStream::Read(OUT void* buffer, size32_t size)
{
	if (m_position < 0 || m_position + size > this->Length())
		throw std::exception("section read failed");

	std::memcpy(buffer, m_section->Data.data() + m_position, size);
	m_position += size;
	return size;
}

size32_t PE::PESectionStream::Write(const void* buffer, size32_t size)
{
	if (m_position < 0)
		throw std::exception("section write failed");
	if (m_position + size > this->Length()) {
		if (!m_expandable)
			throw std::exception("section write failed");

		// Otherwise resize our buffer.
		this->SetLength(m_position + size);
	}

	std::memcpy(m_section->Data.data() + m_position, buffer, size);
	m_position += size;
	return size;
}

#pragma endregion

#pragma region Extended stream I/O

std::string PE::PESectionStream::ReadCString()
{
	std::string str = "";
	int32_t c;
	while ((c = this->GetC()) != EOF && c != '\0')
		str.append(1, (char)c);
	return str.c_str();
}

size32_t PE::PESectionStream::WriteCString(const std::string& str)
{
	return Write(str.c_str(), str.length() + 1);
}

size32_t PE::PESectionStream::Length() const
{
	return (size32_t)m_section->Data.size();
}

void PE::PESectionStream::SetLength(size32_t newLength)
{
	int32_t diff = (int32_t)(newLength - this->Length());
	m_section->Data.resize(newLength);
	m_section->Header.SizeOfRawData += diff;
	m_section->Header.VirtualSize   += diff;
}

#pragma endregion
