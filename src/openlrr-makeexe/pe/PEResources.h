// PEResources.h : (unused)
//

#pragma once

#include "PECommon.h"


namespace PE
{; // !<---

/////////////////////////////////////////////////////////////////////
// Forward declarations

struct ResourceDirectory;
struct ResourceEntry;


/////////////////////////////////////////////////////////////////////
// Structures

struct ResourceEntry
{
	IMAGE_RESOURCE_DIRECTORY_ENTRY entry;
	std::shared_ptr<ResourceDirectory> subDirectory;
	IMAGE_RESOURCE_DATA_ENTRY data;
};

struct ResourceDirectory
{
	IMAGE_RESOURCE_DIRECTORY directory;
	std::vector<ResourceEntry> entries;
};

struct ResourceId
{
	uint16_t Id;
	std::string Name;
	bool NameIsString;

	ResourceId() : Id(0), Name(), NameIsString(false)
	{
	}
	ResourceId(const ResourceEntry& entry) : Id(0), Name(), NameIsString(entry.entry.NameIsString)
	{
		assert(entry.entry.NameIsString == 0); // Names not supported
		this->Id = entry.entry.Id;
		this->Name = "";
		this->NameIsString = (entry.entry.NameIsString != 0);
	}
};

struct Resource
{
	ResourceId Type;
	ResourceId Name;
	ResourceId Lang;
	uint32_t OffsetToData;
	uint32_t Size;
	uint32_t CodePage;
};

}
