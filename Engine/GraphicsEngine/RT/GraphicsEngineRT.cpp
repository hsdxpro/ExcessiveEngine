#include "GraphicsEngineRT.h"
#include "Vec3.h"
#include "Ray.h"
#include "Scene.h"
#include "mymath\mm_quat_func.h"
#include "Core\InputCore.h"
#include <windows.h>

#ifdef WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

void GraphicsEngineRT_Richard::initScene()
{
	//scene->addSphere(Vec3(0, 0, 40), 15, Vec3(1, 1, 1), 0.00);
	//scene->addSphere(Vec3(0, 0, 8000), 15, Vec3(1, 1, 1), 0.00); // Test 1
	//scene->addSphere(Vec3(0, 0, 4000), 15, Vec3(1, 1, 1), 0.00); // Test2
	scene->addSphere(Vec3(0, 0, 40), 15, Vec3(1, 1, 1), 0.00); // Test2
	scene->addPointLight(Vec3(0, 0, 0), Vec3(1, 1, 1), Vec3(1, 1, 1), 1.0, 1.0);

	//generate spheres randomly
	//srand(time(NULL));
	//for (int i = 0; i < 10; i++)
	//{
	//	float posx = (rand() % 81) - 40;
	//	float posy = (rand() % 26) - 25;
	//	float posz = (rand() % 101) + 50;
	//
	//	float radius = rand() % 13 + 3;
	//
	//
	//	float R = (rand() % 81 + 30) / 255.f;
	//	float G = (rand() % 81 + 30) / 255.f;
	//	float B = (rand() % 81 + 30) / 255.f;
	//
	//	scene->addSphere(Vec3(posx, posy, posz), radius, Vec3(R, G, B), 0.08);
	//}
	//
	////add Point light to the scene
	//scene->addPointLight(Vec3(30, 40, 100), Vec3(255, 255, 255), Vec3(255, 255, 255), 0.8, 1.0);
}

Ray GraphicsEngineRT_Richard::getRayAtPixel(float pixelX, float pixelY, int screenWidth, int screenHeight, float aspectRatio)
{
	//Screen Space
	float screenX = pixelX;
	float screenY = pixelY;

	//View Proj Space
	float vProjX = screenX * 2 / screenWidth - 1;
	float vProjY = -screenY * 2 / screenHeight + 1;
	float vProjZ = 0.01;

	
	//View Space
	float Vz = (cam.GetFarPlane() * (1 - cam.GetNearPlane())) / (vProjZ * (cam.GetFarPlane() - cam.GetNearPlane()));
	float Vx = vProjX * Vz * aspectRatio * tanf(cam.GetFOVRad() / 4.0f);
	float Vy = vProjY * Vz * tanf(cam.GetFOVRad() / 4.0f);

	Ray pixelRay;

	const mm::vec3& pos = cam.GetPos();

	pixelRay.origin = Vec3(pos.x, pos.y, pos.z);
	//transform that dir to camera rot space
	mm::vec3 dirVec = mm::vec3(Vx, Vy, Vz);

	dirVec = mm::rotate_vector(cam.GetRot(), dirVec);

	//XMVECTOR dirVec = { Vx,Vy,Vz };
	//dirVec = XMVector3TransformNormal(dirVec, getRotMat());

	pixelRay.dir = Vec3(dirVec.x, dirVec.y, dirVec.z);// = Vec3(XMVectorGetX(dirVec), XMVectorGetY(dirVec), XMVectorGetZ(dirVec));
	return pixelRay;
}

extern "C"
EXPORT IGraphicsEngine* CreateGraphicsEngineRT_Richard(const rGraphicsEngineRT_Richard& d) 
{
	return new GraphicsEngineRT_Richard(d);
}



GraphicsEngineRT_Richard::GraphicsEngineRT_Richard(const rGraphicsEngineRT_Richard& d)
:targetWindow(d.targetWindow)
{
	scene = new Randominezz::Scene(this, &cam);
	initScene();

	backBuffer = new Color[targetWindow->GetNumClientPixels()];
}

GraphicsEngineRT_Richard::~GraphicsEngineRT_Richard() 
{
	delete backBuffer;
}

void GraphicsEngineRT_Richard::Release() 
{
	delete this;
}

graphics::IScene* GraphicsEngineRT_Richard::CreateScene() {
	return 0;
}

graphics::IMesh* GraphicsEngineRT_Richard::CreateMesh() 
{
	return nullptr;
}

graphics::IMaterial* GraphicsEngineRT_Richard::CreateMaterial() 
{
	return nullptr;
}

graphics::ITexture* GraphicsEngineRT_Richard::CreateTexture() 
{
	return nullptr;
}

Camera* GraphicsEngineRT_Richard::CreateCam() 
{
	return nullptr;
}

void GraphicsEngineRT_Richard::Update(float deltaTime) 
{
	for (size_t i = 0; i < targetWindow->GetClientH(); ++i)
	{
		for (size_t j = 0; j < targetWindow->GetClientW(); ++j)
		{
			const int nMultiSampleX = 1;
			const int nMultiSampleY = 1;

			Vec3 color = Vec3(0, 0, 0);
			
			//const float renderBoxDimensionFromCenter = 150;
			//
			float nCount = 0;
			//if (j > targetWindow->GetClientW() * 0.5f - renderBoxDimensionFromCenter && i > targetWindow->GetClientH() * 0.5 - renderBoxDimensionFromCenter &&
			//	j < targetWindow->GetClientW() * 0.5f + renderBoxDimensionFromCenter && i < targetWindow->GetClientH() * 0.5 + renderBoxDimensionFromCenter)
			for (float yOffset = 1.f / nMultiSampleY / 2; yOffset < 1; yOffset += 1.f / nMultiSampleY)
			{
				for (float xOffset = 1.f / nMultiSampleX / 2; xOffset < 1; xOffset += 1.f / nMultiSampleX)
				{
					++nCount;
					color.add(scene->getColorAtPixel(j + xOffset, i + yOffset, targetWindow->GetClientW(), targetWindow->GetClientH()));
				}
			}
			
			color.x /= nCount;
			color.y /= nCount;
			color.z /= nCount;
			

			backBuffer[j + i * targetWindow->GetClientW()].R = color.x * 255;
			backBuffer[j + i * targetWindow->GetClientW()].G = color.y * 255;
			backBuffer[j + i * targetWindow->GetClientW()].B = color.z * 255;
			backBuffer[j + i * targetWindow->GetClientW()].A = 255;
		}
	}

	if(GetAsyncKeyState('W'))
	{
		(int&)Randominezz::gBRDF += 1;
		(int&)Randominezz::gBRDF %= 4;
	}

	targetWindow->SetClientPixels(backBuffer);
}

void GraphicsEngineRT_Richard::AddLayer(const Layer& layer) 
{

}

void GraphicsEngineRT_Richard::RemoveLayer(size_t index) 
{

}

size_t GraphicsEngineRT_Richard::GetNumLayers() const 
{
	return 0;
}

void GraphicsEngineRT_Richard::SetNumLayers(size_t num_layers) 
{

}

auto GraphicsEngineRT_Richard::GetLayer(size_t index) -> Layer& 
{
	static Layer l;
	return l;
}