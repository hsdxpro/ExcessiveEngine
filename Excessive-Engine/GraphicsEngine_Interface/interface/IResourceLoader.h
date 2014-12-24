#pragma once
#include "../Common/src/BasicTypes.h"


namespace graphics {

class IMesh;
class IMaterial;
class ITexture;

////////////////////////////////////////////////////////////////////////////////
/// Loads various mesh, material and texture formats into resource objects.
//
////////////////////////////////////////////////////////////////////////////////
class IResourceLoader {
public:
	////////////////////////////////////
	// Mesh functions

	/// Load a mesh from file
	/// Supported formats are the following: none.
	/// \return Indicates success.
	virtual bool loadMesh(IMesh* mesh, const wchar_t* filePath) = 0;

	/// Generate a teapot mesh.
	/// \return Indicates success.
	virtual bool genMesh_Teapot(IMesh* mesh, float radius, int tesselation) = 0;

	/// Generate a box.
	/// \return Indicates success.
	virtual bool getMesh_Box(IMesh* mesh, float length, float width, float heigh) = 0;

	/// Generate a sphere.
	/// \return Indicates success.
	virtual bool genMesh_Sphere(IMesh*, float radius, int tesselation) = 0;

	/// Generate a cylinder.
	/// \return Indicates success.
	virtual bool genMesh_Cylinder(IMesh* mesh, float radius, float height, int num_sides) = 0;


	////////////////////////////////////
	// Material functions

	/// Load a material file.
	/// Supported formats are: none.
	/// \return Indicates success.
	virtual bool loadMaterial(IMaterial* material, const wchar_t* filePath) = 0;


	////////////////////////////////////
	// Texture functions

	/// Load an image file into a texture.
	/// Supported format are: none.
	/// \return Indicates success.
	virtual bool loadTexture(ITexture* texture, const wchar_t* filePath) = 0;

	/// Generate a checker pattern.
	/// \return Indicates success.
	virtual bool genTexture_Checker(ITexture* texture, int div_x, int div_y, u32 color1, u32 color2) = 0;

};


} // namespace graphics