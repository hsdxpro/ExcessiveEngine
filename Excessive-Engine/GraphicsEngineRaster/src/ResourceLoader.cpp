#include "ResourceLoader.h"


////////////////////////////////////////////////////////////////////////////////
// Mesh functions

bool loadMesh(graphics::IMesh* mesh, const char* filePath) {
	return false;
}
bool loadMesh(graphics::IMesh* mesh, const wchar_t* filePath) {
	return false;
}
bool genMesh_Teapot(graphics::IMesh* mesh, float radius, int tesselation) {
	return false;
}
bool getMesh_Box(graphics::IMesh* mesh, float length, float width, float heigh) {
	return false;
}
bool genMesh_Sphere(graphics::IMesh*, float radius, int tesselation) {
	return false;
}
bool genMesh_Cylinder(graphics::IMesh* mesh, float radius, float height, int num_sides) {
	return false;
}


////////////////////////////////////////////////////////////////////////////////
// Material functions

bool loadMaterial(graphics::IMaterial* material, const char* filePath) {
	return false;
}
bool loadMaterial(graphics::IMaterial* material, const wchar_t* filePath) {
	return false;
}


////////////////////////////////////////////////////////////////////////////////
// Texture functions

bool loadTexture(graphics::ITexture* texture, const char* filePath) {
	return false;
}
bool loadTexture(graphics::ITexture* texture, const wchar_t* filePath) {
	return false;
}
bool genTexture_Checker(graphics::ITexture* texture, int div_x, int div_y, unsigned color1, unsigned color2) {
	return false;
}