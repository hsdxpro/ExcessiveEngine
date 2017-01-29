#include "TagParser.h"
#include "common.h"
#include "FileUtil.h"
#include "StrUtil.h"

cTagParser::cTagParser(const zsString& filePath)
:fileNode(nullptr) {
	fileNode = new tTagData();

	// parse File
	//IFile* file = IFile::Create(filePath, eFileOpenMode::READ);
	std::ifstream file(filePath, std::ios_base::in);
	auto lines = cFileUtil::GetLines(file);

	size_t currNestedLevel = 1;
	tTagData *entryNodes[TAGPARSER_MAX_NESTED_LEVEL];
	// Main entry is file Node
	entryNodes[0] = fileNode;
	// fileNode
	//    |-----rootNode1
	//	  |-----rootNode2
	//    |			|-------asdNode
	//    |					   |----childNode...
	//    |-----rootNode4
	//    |-----rootNode5
	//

	for (auto row = lines.begin(); row != lines.end(); row++) 
	{
		currNestedLevel = GetNestedLevel(*row);

		// We found new entry point...
		if (! IsClosingNode(*row, currNestedLevel)) {
			tTagData *newNode = CreateNode(*row, currNestedLevel);
			entryNodes[currNestedLevel] = newNode;
			entryNodes[currNestedLevel - 1]->AddChild(newNode);
		}
	}
}

cTagParser::~cTagParser() {
	fileNode->DestroyHierarchyRecursively();
}

tTagData* cTagParser::GetRootNodeByIndex(size_t idx) {
	ASSERT(fileNode != nullptr);
	return fileNode->GetChildByIndex(idx);
}

zsString cTagParser::GatherNodeName(const zsString& row, size_t nodeNestedLevel) {
	zsString result;
	size_t index = nodeNestedLevel;
	while (row[index] != '>' && row[index] != ' ') {
		result += row[index];
		index++;
	}
	return result;
}

size_t cTagParser::GetNestedLevel(const zsString& row) {
	size_t nestedLevel = 0;
	while (row[nestedLevel] != '<') {
		nestedLevel++;
	}
	ASSERT(nestedLevel <= TAGPARSER_MAX_NESTED_LEVEL);
	return nestedLevel + 1;
}

bool cTagParser::IsClosingNode(const zsString& row, size_t nodeNestedLevel) {
	return row[nodeNestedLevel] == '/';
}

tTagData *cTagParser::CreateNode(const zsString& row, size_t nodeNestedLevel) {
	tTagData *node = new tTagData(GatherNodeName(row, nodeNestedLevel));
	size_t nProperties;
	tProperty *properties = GatherNodeProperties(row, nodeNestedLevel, nProperties);

	for (size_t i = 0; i < nProperties; i++) {
		node->AddProperty(&properties[i]);
	}
	SAFE_DELETE_ARRAY(properties);
	return node;
}

tProperty *cTagParser::GatherNodeProperties(const zsString& row, size_t nodeNestedLevel, size_t& nProperties) {
	// Properties have assign with " " so search for it

	// TODO O FUCK REMOVE IT
	char data[1024];
	cStrUtil::ToAnsi(row, data, 1024);

	nProperties = 0;
	size_t idx = nodeNestedLevel + 1; // safely can start from here inside '<' char
	tProperty *properties = new tProperty[TAG_MAX_PROPERTIES];
	size_t propIndex = 0;
	char prop[TAG_MAX_PROPERTIES_LENGTH];
	while (data[idx] != '>') {
		if (data[idx] == '"' || data[idx] == '\'') {
			size_t propCharIndex = 0;
			idx++; // enter "...
			while (data[idx] != '"' || data[idx] == '\'') {
				prop[propCharIndex] = data[idx];
				propCharIndex++;
				idx++;
			}
			prop[propCharIndex] = '\0';
			properties[propIndex].SetVal(prop);
			propIndex++;
			propCharIndex = 0;
			nProperties++;
		}
		idx++;
	}
	return properties;
}


///////////////////////////////////////////////////////////////////////////
//////////////////////////////// cTagData /////////////////////////////////
///////////////////////////////////////////////////////////////////////////
tTagData::tTagData() :nProperties(0) { 
	memset(properties, 0, sizeof(tProperty*)* TAG_MAX_PROPERTIES); 
}

tTagData::tTagData(const zsString& name) 
:nProperties(0) { 
	this->name = name; memset(properties, 0, sizeof(tProperty*)* TAG_MAX_PROPERTIES); 
}

tTagData::~tTagData() 
{}

void tTagData::DestroyHierarchyRecursively() {
	DestroyHierarchyRecursively_(this);
}

void tTagData::AddChild(tTagData* t) { 
	childs.push_back(t); 
}

void tTagData::AddProperty(tProperty* prop) {
	properties[nProperties] = *prop; nProperties++; 
}

void tTagData::SetName(const zsString& name) {
	this->name = name; 
}

const zsString& tTagData::GetName() { 
	return name;
}

tProperty* tTagData::GetPropertyByIndex(size_t idx) {
	if (idx < TAG_MAX_PROPERTIES) return &properties[idx]; else return nullptr; 
}

std::vector<tTagData*>& tTagData::GetChilds() {
	return childs; 
}

tTagData* tTagData::GetChildByIndex(size_t idx) { 
	return childs[idx]; 
}

size_t tTagData::GetNChilds() { 
	return childs.size(); 
}

void tTagData::DestroyHierarchyRecursively_(tTagData *root) {
	for (size_t i = 0; i < root->GetNChilds(); i++) {
		DestroyHierarchyRecursively_(root->GetChildByIndex(i));
	}
	delete root;
}

///////////////////////////////////////////////////////////////////////////
//////////////////////////////// tProperty /////////////////////////////////
///////////////////////////////////////////////////////////////////////////

tProperty::tProperty(char* str) {
	strcpy(val, str);
}

float tProperty::AsFloat() { 
	return (float)atof(val); 
}

void tProperty::SetVal(char* str) { 
	strcpy(val, str); 
}
