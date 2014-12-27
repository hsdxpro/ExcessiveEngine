#pragma once
// Importing 3D models based on Assimp (Open Asset Import Library)

//COMMON INTERCHANGE FORMATS
//Collada(.dae)
//Blender 3D (.blend)
//3ds Max 3DS (.3ds)
//3ds Max ASE(.ase)
//Wavefront Object(.obj)
//Industry Foundation Classes(IFC / Step) (.ifc)
//XGL(.xgl, .zgl)
//Stanford Polygon Library(.ply)
//*AutoCAD DXF(.dxf)
//LightWave(.lwo)
//LightWave Scene(.lws)
//Modo(.lxo)
//Stereolithography(.stl)
//DirectX X(.x)
//AC3D(.ac)
//Milkshape 3D (.ms3d)
//* TrueSpace(.cob, .scn)

//MOTION CAPTURE FORMATS
//Biovision BVH(.bvh)
//* CharacterStudio Motion(.csm)

//GRAPHICS ENGINE FORMATS
//Ogre XML(.xml)
//Irrlicht Mesh(.irrmesh)
//* Irrlicht Scene(.irr)

//GAME FILE FORMATS
//Quake I(.mdl)
//Quake II(.md2)
//Quake III Mesh(.md3)
//Quake III Map / BSP(.pk3)
//* Return to Castle Wolfenstein(.mdc)
//Doom 3 (.md5*)
//*Valve Model(.smd, .vta)
//*Starcraft II M3(.m3)
//*Unreal(.3d)

//OTHER FILE FORMATS
//BlitzBasic 3D (.b3d)
//Quick3D(.q3d, .q3s)
//Neutral File Format(.nff)
//Sense8 WorldToolKit(.nff)
//Object File Format(.off)
//PovRAY Raw(.raw)
//Terragen Terrain(.ter)
//3D GameStudio(3DGS) (.mdl)
//3D GameStudio(3DGS) Terrain(.hmp)
//Izware Nendo(.ndo)

#include "BasicTypes.h"
#include "mymath\mymath.h"
#include <vector>

// Flags for importing
enum class eImporter3DFlag : u32 {
	VERT_ATTR_POS,
	VERT_ATTR_NORM,
	VERT_ATTR_TAN,
	VERT_ATTR_BITAN,
	VERT_ATTR_TEX0,
	VERT_INTERLEAVED,
	VERT_NON_INTERLEAVED,
	COUNT,
};

// Configuring importer for importing, for example u dont want tangent for output meshes
struct rImporter3DCfg {
		rImporter3DCfg() {	// Default importing all attributes in interleaved form
			flags.resize(6);
			flags[0] = eImporter3DFlag::VERT_ATTR_POS;
				flags[1] = eImporter3DFlag::VERT_ATTR_NORM;
				flags[2] = eImporter3DFlag::VERT_ATTR_TAN;
				flags[3] = eImporter3DFlag::VERT_ATTR_BITAN;
				flags[4] = eImporter3DFlag::VERT_ATTR_TEX0;
				flags[5] = eImporter3DFlag::VERT_INTERLEAVED;
		}

		rImporter3DCfg(const std::vector<eImporter3DFlag>& flags) :flags(flags){}

	std::vector<eImporter3DFlag> flags;
};


// Output meshes
struct rImporter3DMesh {
		rImporter3DMesh(): nVertices(0), vertexSize(0){}

	// Output material for mesh
	struct rMaterial {
			rMaterial(): faceStartIdx(std::numeric_limits<u32>::max()), faceEndIdx(std::numeric_limits<u32>::max()){}
		u32 faceStartIdx;
		u32 faceEndIdx;

		std::wstring texPathDiffuse;
		std::wstring texPathNormal;
	};

	// Vertices
	u32 nVertices;
	u32 vertexSize;
	std::vector<void*> vertexBuffers;

	// Indices
	std::vector<u32> indices;

	// Materials
	std::vector<rMaterial> materials;
};

// Importer output 
struct rImporter3DData {
	std::vector<rImporter3DMesh> meshes;
};

class Importer3D
{
public:
	static bool loadFile(const std::wstring& path, const rImporter3DCfg& cfg, rImporter3DData& data_out);
};