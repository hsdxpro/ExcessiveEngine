// TagParser.h By Zsíroskenyér Team 2013.11.23 9:34
// For parsing tagged XML like files
#pragma once

#include "zsString.h"
#include <vector>

#define TAG_MAX_PROPERTIES 10
#define TAG_MAX_PROPERTIES_LENGTH 64
#define TAG_MAX_NAME_LENGTH 32
#define TAGPARSER_MAX_NESTED_LEVEL 16

struct tProperty {
	float AsFloat();

	void SetVal(char* str);

	tProperty();
	tProperty(char* str);
	~tProperty();
protected:
	char val[TAG_MAX_PROPERTIES_LENGTH];
};

struct tTagData {
	tTagData();
	tTagData(const zsString& name);
	~tTagData();

	void DestroyHierarchyRecursively();

	void AddChild(tTagData* t);
	void AddProperty(tProperty* prop);
	void SetName(const zsString& name);

	const zsString& GetName();
	tProperty* GetPropertyByIndex(size_t idx);

	std::vector<tTagData*>& GetChilds();
	tTagData* GetChildByIndex(size_t idx);
	size_t GetNChilds();

protected:

	void DestroyHierarchyRecursively_(tTagData *root);

protected:
	// Tag have childs
	std::vector<tTagData*> childs;

	// Tag have name
	zsString name;

	// Tag have properties like this tag -> <position x="0.0" y="1.0" z="0.0" />
	tProperty properties[TAG_MAX_PROPERTIES];
	size_t nProperties;
};

class cTagParser {
public:
	cTagParser(const zsString& filePah);
	~cTagParser();

	tTagData* GetRootNodeByIndex(size_t idx);
protected:
	// Nested Level starts from 1 !! they are root nodes
	size_t GetNestedLevel(const zsString& row);
	bool IsClosingNode(const zsString& row, size_t nodeNestedLevel);
	tProperty* GatherNodeProperties(const zsString& row, size_t nodeNestedLevel, size_t& nProperties);
	zsString GatherNodeName(const zsString& row, size_t nodeNestedLevel);
	tTagData* CreateNode(const zsString& row, size_t nodeNestedLevel);
protected:
	// The main file Node, under that you can find the file root nodes and ...
	tTagData *fileNode;
};