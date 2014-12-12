#pragma once

#include "../GraphicsEngine_Interface/interface/IResourceLoader.h"


////////////////////////////////////////////////////////////////////////////////
/// Loads various of mesh, material and texture formats into resource objects.
/// \see IResourceLoader
////////////////////////////////////////////////////////////////////////////////
class ResourceLoader : public graphics::IResourceLoader {
public:
	////////////////////////////////////
	// Mesh functions

	bool loadMesh(graphics::IMesh* mesh, const char* filePath) override;
	bool loadMesh(graphics::IMesh* mesh, const wchar_t* filePath) override;
	bool genMesh_Teapot(graphics::IMesh* mesh, float radius, int tesselation) override;
	bool getMesh_Box(graphics::IMesh* mesh, float length, float width, float heigh) override;
	bool genMesh_Sphere(graphics::IMesh*, float radius, int tesselation) override;
	bool genMesh_Cylinder(graphics::IMesh* mesh, float radius, float height, int num_sides) override;


	////////////////////////////////////
	// Material functions

	bool loadMaterial(graphics::IMaterial* material, const char* filePath) override;
	bool loadMaterial(graphics::IMaterial* material, const wchar_t* filePath) override;


	////////////////////////////////////
	// Texture functions

	bool loadTexture(graphics::ITexture* texture, const char* filePath) override;
	bool loadTexture(graphics::ITexture* texture, const wchar_t* filePath) override;
	bool genTexture_Checker(graphics::ITexture* texture, int div_x, int div_y, unsigned color1, unsigned color2) override;

};
