////////////////////////////////////////////////////////////////////////////////
//	Zsiroskenyer Team
////////////////////////////////////////////////////////////////////////////////
//	This file contains general pipeline settings such as:
//		- Depth-stencil state
//		- Blending state
//		- Uhmmm... sooo... anything else that should be here
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cassert>
typedef unsigned char uint8_t;
////////////////////////////////////////////////////////////////////////////////
//	Depth-stencil state

enum class eComparisonFunc {
	NEVER = 1,
	LESS = 2,
	EQUAL = 3,
	LESS_EQUAL = 4,
	GREATER = 5,
	NOT_EQUAL = 6,
	GREATER_EQUAL = 7,
	ALWAYS = 8
};

enum class eStencilOp {
	KEEP = 1,
	ZERO = 2,
	REPLACE = 3,
	INCR_SAT = 4,
	DECR_SAT = 5,
	INVERT = 6,
	INCR = 7,
	DECR = 8
};

struct tDepthStencilDesc {
	bool depthEnable;
	bool depthWriteEnable;
	eComparisonFunc depthCompare;
	bool stencilEnable;
	uint8_t stencilReadMask;
	uint8_t stencilWriteMask;

	struct tStencilOpDesc {
		eStencilOp stencilFail;
		eStencilOp stencilPassDepthFail;
		eStencilOp stencilPass;
		eComparisonFunc stencilCompare;

		tStencilOpDesc() {
			stencilFail = eStencilOp::REPLACE;
			stencilPass = eStencilOp::REPLACE;
			stencilPassDepthFail = eStencilOp::REPLACE;
			stencilCompare = eComparisonFunc::ALWAYS;
		}
	} stencilOpFrontFace, stencilOpBackFace;

	tDepthStencilDesc() {
		depthEnable = true;
		depthWriteEnable = true;
		depthCompare = eComparisonFunc::LESS_EQUAL;
		stencilEnable = false;
		stencilReadMask = stencilWriteMask = 0;
	}
};



////////////////////////////////////////////////////////////////////////////////
//	Blending

enum class eBlendFactor {
	ZERO = 1,
	ONE = 2,
	SRC_COLOR = 3,
	INV_SRC_COLOR = 4,
	SRC_ALPHA = 5,
	INV_SRC_ALPHA = 6,
	DEST_ALPHA = 7,
	INV_DEST_ALPHA = 8,
	DEST_COLOR = 9,
	INV_DEST_COLOR = 10,
	SRC_ALPHA_SAT = 11,
	BLEND_FACTOR = 14,
	INV_BLEND_FACTOR = 15,
	SRC1_COLOR = 16,
	INV_SRC1_COLOR = 17,
	SRC1_ALPHA = 18,
	INV_SRC1_ALPHA = 19
};

enum class eBlendOp {
	ADD = 1,
	SUBTRACT = 2,
	REV_SUBTRACT = 3,
	MIN = 4,
	MAX = 5
};

enum class eBlendWriteMask : uint8_t {
	RED = 1,
	GREEN = 2,
	BLUE = 4,
	ALPHA = 8,
	ALL = (RED | GREEN | BLUE |  ALPHA),
};

struct tBlendDesc {
	bool alphaToCoverageEnable;
	bool independentBlendEnable;

	struct tRenderTargetBlend {
		bool enable;
		eBlendFactor srcBlend, destBlend;;
		eBlendOp blendOp;
		eBlendFactor srcBlendAlpha, destBlendAlpha;
		eBlendOp blendOpAlpha;
		uint8_t writeMask;

		tRenderTargetBlend() {
			enable = false;
			srcBlend = eBlendFactor::ONE;
			destBlend = eBlendFactor::ZERO;
			blendOp = eBlendOp::ADD;
			srcBlendAlpha = eBlendFactor::ONE;
			destBlendAlpha = eBlendFactor::ZERO;
			blendOpAlpha = eBlendOp::ADD;
			writeMask = (uint8_t)eBlendWriteMask::ALL;
		}
	} renderTargets[8];

	tBlendDesc() {
		alphaToCoverageEnable = false;
		independentBlendEnable = false;
	}

	tRenderTargetBlend& operator[](unsigned idx) {
		assert(idx < 8);
		return renderTargets[idx];
	}
};

////////////////////////////////////////////////////////////////////////////////
//	SamplerStates

enum class eFilter {
	POINT,
	LINEAR,
	ANISOTROPIC,
};

enum class eAddress {
	CLAMP,
	WRAP,
	MIRROR,
};

struct tSamplerStateDesc {
	eFilter filterMip;
	eFilter filterMin;
	eFilter filterMag;

	eAddress addressU;
	eAddress addressV;

	tSamplerStateDesc() :filterMip(eFilter::POINT), filterMin(eFilter::POINT), filterMag(eFilter::POINT), addressU(eAddress::WRAP), addressV(eAddress::WRAP){};
};