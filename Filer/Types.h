#pragma once

enum class SortDirsType : int
{
	First = 0,
	Last = 1,
	NoSpecify = 2,

	SortDirsTypeNum
};

enum class ColorRoleType : int
{
	Unknown = -1,

	Normal,
	Normal_Selected,
	Folder,
	Folder_Selected,
	ReadOnly,
	ReadOnly_Selected,
	Hidden,
	Hidden_Selected,
	System,
	System_Selected,

	Background,
	Selected_Background,

	ColorRoleTypeNum
};

const QDir::Filters FIX_FILTER_FLAGS = QDir::AllEntries | QDir::AccessMask | QDir::AllDirs | QDir::NoDot | QDir::NoDotDot;
const QDir::Filters DEFAULT_FILTER_FLAGS = FIX_FILTER_FLAGS;

enum class SectionType : int
{
	Unknown = -1,

	FileName = 0,
	FileType,
	FileSize,
	LastModified,

	SectionTypeNum
};
