#pragma once
#include "EngineCore.h"

class PhysicsCore
{
public:
	__inline void SetLayerCollision(size_t ID0, size_t ID1, bool bEnableCollision) 
	{ 
		Core.SetLayerCollision(ID0, ID1, bEnableCollision); 
	}

	__inline bool TraceClosestPoint(const mm::vec3& from, const mm::vec3& to, rPhysicsTraceResult& traceResult_out, const rPhysicsTraceParams& params = rPhysicsTraceParams())
	{
		return Core.TraceClosestPoint_Physics(from, to, traceResult_out, params);
	}
};

extern PhysicsCore Physics;