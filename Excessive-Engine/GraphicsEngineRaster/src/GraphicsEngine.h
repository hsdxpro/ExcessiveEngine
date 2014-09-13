#include "../GraphicsEngine_Interface/interface/IGraphicsEngine.h"



////////////////////////////////////////////////////////////////////////////////
/// GraphicsEngine implementation.
///
////////////////////////////////////////////////////////////////////////////////

class GraphicsEngine : public ge::IGraphicsEngine 
{
public:
	GraphicsEngine();
	~GraphicsEngine();

	void release() override;

	ge::IScene* createScene() override;
};