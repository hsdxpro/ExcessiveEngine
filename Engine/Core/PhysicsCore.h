#pragma once
#include "EngineCore.h"

class PhysicsCore
{
public:
	inline void SetLayeCollision(size_t ID0, size_t ID1, bool bEnableCollision) 
	{ 
		Core.SetLayeCollision(ID0, ID1, bEnableCollision); 
	}

	inline bool TraceClosestPoint(const mm::vec3& from, const mm::vec3& to, PhysicsTraceResult& traceResult_out, const PhysicsTraceParams& params = PhysicsTraceParams())
	{
		return Core.TraceClosestPoint_Physics(from, to, traceResult_out, params);
	}
};

extern PhysicsCore Physics;