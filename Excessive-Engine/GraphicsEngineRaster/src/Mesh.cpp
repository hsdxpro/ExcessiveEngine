#include "Mesh.h"

#include <iostream> // only for debug!!


////////////////////////////////////////////////////////////////////////////////
// Constructor & destructor

Mesh::Mesh() {
	refcount = 1;
}

Mesh::~Mesh() {
	std::cout << "Mesh @" << this << ": kthxbai";
}


////////////////////////////////////////////////////////////////////////////////
// lifecycle

void Mesh::acquire() {
	refcount++;
}

void Mesh::release() {
	refcount--;
	if (refcount == 0) {
		delete this;
	}
}


////////////////////////////////////////////////////////////////////////////////
// load

void Mesh::load(const char* file_path) {
	
}


void Mesh::load(const wchar_t* file_path) {

}