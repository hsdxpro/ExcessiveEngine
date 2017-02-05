////////////////////////////////////////////////////////////////////////////////
//	GraphicsEngine/src/GraphicsEngine.h
//	2013.oct.10, Zsiroskenyer Team, Péter Kardos
////////////////////////////////////////////////////////////////////////////////
//	This is the core class of the graphics engine, that holds together all the
//	stuff.
////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "GraphicsEntity.h"
#include "GraphicsEngine/Raster/Scene.h"
#include "GraphicsEngine/IScene.h"
#include "GraphicsEngine/IGraphicsEngine.h"
#include "GraphicsEngine/Raster/GraphicsEngineRaster.h"
#include "GraphicsEngine/RasterZsiros/GraphicsApiD3D11/src/GraphicsApiD3D11.h"
#include "ShadowMap.h"

#include <set>
#include <deque>
#include <array>
#include <unordered_map>
//	Errors
enum class eGraphicsResult : signed int {
	OK = 0,
	ERROR_UNKNOWN,
	ERROR_OUT_OF_MEMORY,
	ERROR_FILE_NOT_FOUND,
	ERROR_INVALID_CALL,
	ERROR_INVALID_ARG
};

struct GraphicsEngineRasterZsirosDesc
{
	Window*		targetWindow;
	eGapiType	gapiType;
	RectNormed	renderRegion;
};

// scene rendering states
struct tRenderState {
	// hdr
	struct HdrRendering {
		bool enabled;
		int lensEffect;
		int starEffect;
	} hdr;

	// motion blur
	struct MotionBlur {
		int quality; // 0 disabled
	} motionBlur;

	// depth of field
	struct DepthOfField {
		int quaility; // 0 disabled
		float focusDistance; // negative: auto
	} depthOfField;

	// ambient occlusion
	struct AmbientOcclusion {
		static enum {
			DISABLE = 0,
			SSAO = 1,
			// SSDO = 2,
			// HBAO = 3,
			// SAO = 4,
		} method;
		int quality;
	};
};

class ITexture2D;
class IShaderManager;
class cResourceManager;
class ITexture2D;
class IShaderProgram;
class IWindow;

class IGeometryBuilder;
class IGeometryRef;

class IGraphicsEntity;
class IGraphicsLight;



////////////////////////////////////////////////////////////////////////////////
//	GraphicsScene
class cGraphicsScene// : public graphics::IScene
{
	friend class cGraphicsEngine;
public:
	// constructor & destructor
	cGraphicsScene(cGraphicsEngine& parent, tRenderState state = tRenderState());
	~cGraphicsScene();

	// entities & lights
	//IGraphicsEntity* CreateEntity(const wchar_t* geomPath, const wchar_t* mtlPath);
	//IGraphicsEntity* CreateEntity(IGeometryRef* customGeom, const wchar_t* mtlPath);
	//void DeleteEntity(const IGraphicsEntity* entity);
	//IGraphicsLight* CreateLight();
	//void DeleteLight(const IGraphicsLight* light);

	// scene state
	ICamera* GetCamera() { return &camera; }

	//tRenderState& GetState();
	//void Clear();
	//
	//// scene order
	//void MoveUp();
	//void MoveDown();
	//void MoveTop();
	//void MoveBottom();

	//std::pair<graphics::ILight, cShadowMap>* AddLight();

	const std::unordered_set<std::pair<graphics::ILight*, cShadowMap>*>& GetLights() const { return lights; }

private:
	// information for rendering pipeline
	tRenderState state;
	//Scene sceneManager;
	cGraphicsEngine& parent;
	Camera camera;

	// Used by friend classes (Hdr, motionBlur)
	float luminanceAdaptation;
	Matrix44 lastCameraMatrix;

	std::unordered_set<std::pair<graphics::ILight*, cShadowMap>*> lights;
};


////////////////////////////////////////////////////////////////////////////////
//	GraphicsEngine
class cGraphicsEngine : public IGraphicsEngine {
	friend class cGraphicsScene;
	class cDeferredRenderer;
	class cHDRProcessor;
	class cPostProcessor;
	class cShadowRenderer;
public:
	// lifecycle (creation, destruction)
	//cGraphicsEngine(IWindow* targetWindow, unsigned screenWidth, unsigned screenHeight, tGraphicsConfig config);
	cGraphicsEngine(const GraphicsEngineRasterZsirosDesc& d);
	cGraphicsEngine(const cGraphicsEngine&) = delete;
	~cGraphicsEngine();
	cGraphicsEngine& operator=(const cGraphicsEngine&) = delete;
	void Release() override;

	// rendering pipeline
	void Update(float elapsed = 0.0f) override;

	bool ResizeRenderTargets(unsigned width, unsigned height) override;
	eGraphicsResult ReloadShaders();
	
	// scene & geom management
	//IGeometryBuilder* CreateCustomGeometry();
	//graphics::IScene* CreateScene(tRenderState state = tRenderState());

	//void DeleteScene(const graphics::IScene* scene);

	// Configurate graphics engine
	//eGraphicsResult SetConfig(tGraphicsConfig config);

	const char*			GetLastErrorMessage();
	cResourceManager*	GetResourceManager();

	// Create stuff
	Scene* CreateScene() override;
	Mesh* CreateMesh() override;
	Material* CreateMaterial() override;
	Texture* CreateTexture() override;
	Camera* CreateCam() override;

	// scene & layer system
	// DEPRECATED
	void AddLayer(const Layer& layer) override;
	void RemoveLayer(size_t index) override;
	size_t GetNumLayers() const override;
	void SetNumLayers(size_t num_layers) override;
	Layer& GetLayer(size_t index) override;

	// configure pipeline
	// description must be in json format.
	// describes the nodes making the pipeline
	bool SetPipeline(const char* description);

	Window* GetTargetWindow() override;

private:
	void RenderScene(Scene& scene, ITexture2D* target, float elapsed);
	void RenderForward();
	void RenderDeferred();

	void LoadShaders();
	void UnloadShaders();

	void ReloadBuffers();

	// render states
	static const tDepthStencilDesc depthStencilDefault;
	static const tBlendDesc blendDefault;

	std::vector<Layer> layers;

	Window* targetWindow;
	RectNormed	renderRegion;

	// rendering stuff
	cGraphicsScene* sceneManager; // temporary storage for current scene state (copied from cScene)
	Camera* camera;
	float elapsed;
	ITexture2D* curSceneBuffer;
	std::array<ITexture2D*,2> hdrTextures; // Global hdr texture to preserve hdr values across post processes

	// state
	unsigned screenWidth, screenHeight;
	//tGraphicsConfig config;

	// sub-compnents: rendering & graphical
	cGraphicsApiD3D11* gApi;
	class cDeferredRenderer* deferredRenderer;
	class cHDRProcessor* hdrProcessor;
	class cPostProcessor* postProcessor;
	class cShadowRenderer* shadowRenderer;

	cResourceManager* resourceManager;
	std::set<Scene*> graphicsScenes;
	std::deque<Scene*> graphicsSceneOrder;
	
	// shaders
	IShaderProgram* shaderScreenCopy;

	// misc
	std::string lastErrorMessage;

	float luminanceAdaptation;

	////////////////////////////////////////////////
	// --- --- --- --- SUBCLASSES --- --- --- --- //
	////////////////////////////////////////////////

	///////////////////////////////////////
	// Deferred renderer helper subclass
	class cDeferredRenderer {
		friend class cGraphicsEngine;
	public:
		// constructor
		cDeferredRenderer(cGraphicsEngine& parent);
		cDeferredRenderer(const cDeferredRenderer&) = delete;
		~cDeferredRenderer();
		cDeferredRenderer& operator=(const cDeferredRenderer&) = delete;

		void RenderComposition(Scene& scene);
		

		eGraphicsResult Resize(unsigned width, unsigned height);

		ITexture2D* GetCompositionBuffer();
		ITexture2D* GetDepthStencilBuffer();
		ITexture2D* GetDepthBuffer();
		ITexture2D* GetGBuffer(uint8_t idx);

	private:
		void ReloadShaders();
		void UnloadShaders();
		eGapiResult Cleanup();
		eGapiResult ReallocBuffers();

		ITexture2D* gBuffer[3];
		ITexture2D* compositionBuffer;
		ITexture2D* depthBuffer;
		ITexture2D* depthBufferCopy;
		ITexture2D* aoBuffer, *aoBlurHelperBuffer, *randomTexture;

		cGraphicsApiD3D11* gApi;
		IShaderProgram* shaderGBuffer;
		IShaderProgram *shaderDirectional, *shaderPoint, *shaderSpot, *shaderAmbient, *shaderSky;
		IShaderProgram* shaderSSDO, *shaderSAO, *shaderSSAO, *shaderHBAO, *shaderHBAOblurHor, *shaderHBAOblurVer;
		cGraphicsEngine& parent;

		IVertexBuffer *vbSpot, *vbPoint;
		IIndexBuffer *ibSpot, *ibPoint;

		// Remove that also..
		ITexture2D* motionBlurredBuffer;

		unsigned bufferWidth, bufferHeight;
	};

	///////////////////////////////////////
	// General post-processor helper class
	class cPostProcessor {
		friend class cGraphicsEngine;
	public:
		cPostProcessor(cGraphicsEngine& parent);
		cPostProcessor(const cPostProcessor&) = delete;
		~cPostProcessor();
		cPostProcessor& operator=(const cPostProcessor&) = delete;

		void ProcessMB(float frameDeltaTime, const Camera& cam, Scene& scene, float aspectRatio);
		void ProcessDOF(float frameDeltaTime, const Camera& cam, float aspectRatio);
		void ProcessFXAA();
		void ProcessSSAR(const Camera& cam, float aspectRatio);
		void ProcessSSVR(const Camera& cam, float aspectRatio);

		// Set inputs
		void SetInputMB(ITexture2D* color, ITexture2D* depth);
		void SetInputDOF(ITexture2D* color, ITexture2D* depth);
		void SetInputFXAA(ITexture2D* color); // If FXAA shader defines that it takes luminance from alpha channel, then pass appropriate texture please..
		void SetInputSSAR(ITexture2D* color, ITexture2D* depth, ITexture2D* normal);
		void SetInputSSVR(ITexture2D* color, ITexture2D* depth, ITexture2D* normal);

		// Set outputs
		void SetOutputMB(ITexture2D* color, ITexture2D* velocity2D, ITexture2D* depth);
		void SetOutputDOF(ITexture2D* color);
		void SetOutputFXAA(ITexture2D* color);
		void SetOutputSSAR(ITexture2D* color);
		void SetOutputSSVR(ITexture2D* color);

	private:
		void		ReloadShaders(); // Reload belonging shaders
		void		UnloadShaders(); // Destroy shaders
		void		Cleanup();		 // Clean up resources that class uses
		eGapiResult ReallocBuffers();// Realloc buffers that class uses

	private:
		// Shaders
		IShaderProgram *shaderMB ,	*shaderMBCamera2DVelocity, *shaderMBObject2DVelocity,	// Motion blur shaders
						*shaderDOF, *shaderDOFBokeh, *shaderFocalPlaneAdaption,				// Depth of field shaders
					   *shaderFXAA,															// FXAA shaders
					   *shaderSSAR, *shaderSSVR;											// Screen Space Reflections( Analytic and Variance)

		// Input textures
		ITexture2D* inputTexColor, *inputTexDepth, *inputTexNormal;

		// Output textures
		ITexture2D* outputTexColor, *outputTexVelocity2D, *outputTexDepthStencil;

		// Focal plane adaption on gpu (1x1) resolution
		ITexture2D* focalPlaneTexA, *focalPlaneTexB;

		// Motion blur vars
		mm::mat4 lastViewMat;
		std::unordered_map<graphics::IEntity*, mm::mat4> entityPrevWorldViewProjs;

		cGraphicsEngine& parent;
		cGraphicsApiD3D11* gApi;
	};

	///////////////////////////////////////
	// HDR post-processor helper class
	class cHDRProcessor {
		friend class cGraphicsEngine;
	public:
		cHDRProcessor(cGraphicsEngine& parent);
		~cHDRProcessor();

		void Update(float elapsedSec = -1.0f);
		void ReloadShaders();

		void SetDestination(ITexture2D* dest);
		eGraphicsResult SetSource(ITexture2D* t);

	private:
		void LoadShaders();
		void UnloadShaders();
		void Cleanup();

		cGraphicsEngine& parent;
		cGraphicsApiD3D11* gApi;
		ITexture2D* source;
		ITexture2D* dest;
		ITexture2D* luminanceBuffer[10]; // for 10 buffers down from 512x512 ... 1x1
		ITexture2D* luminanceStaging;
		ITexture2D* blurBuffer;
		ITexture2D* downSampled;
		float avgLuminance;
		float adaptedLuminance;
		IShaderProgram *shaderLumSample, *shaderLumAvg, *shaderOverbright, *shaderBlurVert, *shaderBlurHoriz, *shaderCompose;
		unsigned sourceWidth, sourceHeight;
	};

	///////////////////////////////////////
	//	Shadow mapper helper class
	class cShadowRenderer {
	public:
		cShadowRenderer(cGraphicsEngine& parent);
		~cShadowRenderer();

		void RenderShadowMaps(Scene& sceneManager);
		void ReloadShaders();

	private:
		void LoadShaders();
		void UnloadShaders();
		void Cleanup();

		IShaderProgram* shaderDirectional;
		IShaderProgram* shaderSpot;
		IShaderProgram* shaderPoint;

		cGraphicsEngine& parent;
		cGraphicsApiD3D11* gApi;
	};
};


// Helper function for safely loading shaders
auto SafeLoadShader(cGraphicsApiD3D11* gApi, const wchar_t* shader)->IShaderProgram*;
