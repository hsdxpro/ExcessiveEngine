// CgShaderHelper.h by Zsíroskenyér Team 2014.01.21 11:07
// This helper class is for compiling, parsing nvidia cg shaders
// and parsing cgc.exe generated HLSL, GLSL files

#include "Cg/cg.h"

#include "PipelineState.h"
#include "zsString.h"
#include <unordered_map>
#include <list>

#include "VertexFormat.h"

// @TODO cgShaderHelper only supports 1 pass 1 tech per effect per cg file
class cCgShaderHelper {
public:
	// Compiling profiles for cg
	enum class eProfileCG {
		VS_5_0,
		HS_5_0,
		DS_5_0,
		GS_5_0,
		PS_5_0,
		VS_4_0,
		HS_4_0,
		DS_4_0,
		GS_4_0,
		PS_4_0,
		VS_3_0,
		PS_3_0,
		VS_2_0,
		PS_2_0,
	};

	// General domain indexing
	enum eDomainIdx : uint8_t {
		VS = 0,
		HS = 1,
		DS = 2,
		GS = 3,
		PS = 4,
		NDOMAINS = 5,
	};

	struct tCgInfo {
		bool domainsExist[NDOMAINS];
		zsString domainsEntry[NDOMAINS];
	};

	struct tSamplerInfo {
		uint16_t textureSlot;
		uint16_t samplerStateSlot;
	};

	struct tHLSLDesc {
		// < SamplerName, samplerInfo>
		std::unordered_map<zsString, tSamplerInfo> samplerInfo;
	};

	cCgShaderHelper(const zsString& shaderPath);

	bool CompileCg(const zsString& cgFilePath, const zsString& shaderOut, cCgShaderHelper::eProfileCG compileProfile);

	tHLSLDesc GetHLSLDesc(const zsString& hlslFilePath, const void* byteCode, size_t byteCodeSize);

	const std::list<zsString>& GetIncludedFilesPaths() const;
	std::unordered_map<zsString, tSamplerStateDesc> GetSamplerStates();
	const wchar_t* cCgShaderHelper::GetLastErrorMsg();
	const cCgShaderHelper::tCgInfo& GetDomainInfo();
	cVertexFormat GetVSInputFormat();

protected:
	// Error handling
	zsString lastErrorMsg;

	std::list<zsString> cgFileLines;
	std::list<zsString> includedFilesPaths;
	cCgShaderHelper::tCgInfo info;
};