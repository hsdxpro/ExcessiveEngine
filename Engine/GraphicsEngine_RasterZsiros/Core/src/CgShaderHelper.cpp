#include "CgShaderHelper.h"
#include "Cg/cgGL.h"

#include "StrUtil.h"
#include "FileUtil.h"

// Windows based because of cgc.exe
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <assert.h>

#include <d3d11shader.h>
#include <d3dcompiler.h>
#include "PlatformLibrary/Sys.h"

cCgShaderHelper::cCgShaderHelper(const zsString& shaderPath) {

	cFileUtil::ReplaceIncludeDirectives(shaderPath, cgFileLines, includedFilesPaths);

	// Setup context, for creation (Yeah silly, but use OpenGL cg state and params) because equal with dx...
	CGcontext con = cgCreateContext();
	cgGLRegisterStates(con);
	cgGLSetManageTextureParameters(con, CG_TRUE);
	
	// Create cg effect from file (load)
	assert(shaderPath.size() <= 256);
	char ansiShaderPath[256];
	cStrUtil::ToAnsi(shaderPath, ansiShaderPath, 256);
	CGeffect effect = cgCreateEffectFromFile(con, ansiShaderPath, nullptr);
	if (effect == nullptr) {
		lastErrorMsg = cgGetLastListing(con);

		// Free up
		cgDestroyEffect(effect);
		cgDestroyContext(con);
		return;
	}

	// Tech creation
	CGtechnique tech = cgGetFirstTechnique(effect);
	if (tech == nullptr) {
		lastErrorMsg = L"There is no Technique in shader: " + shaderPath;

		// Free up
		cgDestroyEffect(effect);
		cgDestroyContext(con);
		return;
	}

	// Pass creation
	CGpass pass = cgGetFirstPass(tech);
	if (pass == nullptr) {
		lastErrorMsg = L"There is no pass in shader: " + shaderPath;
		// Free up
		cgDestroyEffect(effect);
		cgDestroyContext(con);
		return;
	}

	CGprogram shaderPrograms[NDOMAINS];
		shaderPrograms[VS] = cgGetPassProgram(pass, CGdomain::CG_VERTEX_DOMAIN);
		shaderPrograms[HS] = cgGetPassProgram(pass, CGdomain::CG_TESSELLATION_CONTROL_DOMAIN);
		shaderPrograms[DS] = cgGetPassProgram(pass, CGdomain::CG_TESSELLATION_EVALUATION_DOMAIN);
		shaderPrograms[GS] = cgGetPassProgram(pass, CGdomain::CG_GEOMETRY_DOMAIN);
		shaderPrograms[PS] = cgGetPassProgram(pass, CGdomain::CG_FRAGMENT_DOMAIN);


	// Domain infos
	for (uint8_t i = 0; i < NDOMAINS; i++) {
		// Domain existence
		info.domainsExist[i] = shaderPrograms[i] != nullptr;

		// if exist save entry name
		if (info.domainsExist[i]) 
			info.domainsEntry[i] = cgGetProgramString(shaderPrograms[i], CGenum::CG_PROGRAM_ENTRY);
	}

	// Free up
	cgDestroyEffect(effect);
	cgDestroyContext(con);
}

bool cCgShaderHelper::CompileCg(const zsString& cgFilePath, const zsString& shaderOut, cCgShaderHelper::eProfileCG compileProfile) 
{
	// Paths
	zsString cgcExePath = Sys::GetExeDir() + "cgc.exe";
	zsString entryAndProfile;
	switch (compileProfile)
	{
	case eProfileCG::VS_5_0:
		entryAndProfile = L"-profile vs_5_0 -entry VS_MAIN";
		break;
	case eProfileCG::HS_5_0:
		entryAndProfile = L"-profile hs_5_0 -entry HS_MAIN";
		break;
	case eProfileCG::DS_5_0:
		entryAndProfile = L"-profile ds_5_0 -entry DS_MAIN";
		break;
	case eProfileCG::GS_5_0:
		entryAndProfile = L"-profile gs_5_0 -entry GS_MAIN";
		break;
	case eProfileCG::PS_5_0:
		entryAndProfile = L"-profile ps_5_0 -entry PS_MAIN";
		break;
	case eProfileCG::VS_4_0:
		entryAndProfile = L"-profile vs_4_0 -entry VS_MAIN";
		break;
	case eProfileCG::HS_4_0:
		entryAndProfile = L"-profile hs_4_0 -entry HS_MAIN";
		break;
	case eProfileCG::DS_4_0:
		entryAndProfile = L"-profile ds_4_0 -entry DS_MAIN";
		break;
	case eProfileCG::GS_4_0:
		entryAndProfile = L"-profile gs_4_0 -entry GS_MAIN";
		break;
	case eProfileCG::PS_4_0:
		entryAndProfile = L"-profile ps_4_0 -entry PS_MAIN";
		break;
	case eProfileCG::VS_3_0:
		entryAndProfile = L"-profile vs_3_0 -entry VS_MAIN";
		break;
	case eProfileCG::PS_3_0:
		entryAndProfile = L"-profile ps_3_0 -entry PS_MAIN";
		break;
	case eProfileCG::VS_2_0:
		entryAndProfile = L"-profile vs_2_0 -entry VS_MAIN";
		break;
	case eProfileCG::PS_2_0:
		entryAndProfile = L"-profile ps_2_0 -entry PS_MAIN";
		break;
	}
	//cgc.exe proba.fx -profile vs_5_0 -entry MAIN -o proba.vs
	zsString shellParams = cgcExePath + L" " + cgFilePath + L" " + entryAndProfile + L" -unroll all count=50 -o " + shaderOut;
	//zsString shellParams = cgcExePath + L" " + cgFilePath + L" " + entryAndProfile + L" -unroll all -inline all -o " + shaderOut;
	//zsString shellParams = cgcExePath + L" " + cgFilePath + L" " + entryAndProfile + L" -unroll none -o " + shaderOut;

	// Process info
	STARTUPINFOW StartupInfo;
	memset(&StartupInfo, 0, sizeof(StartupInfo));
	StartupInfo.cb = sizeof(StartupInfo);
	PROCESS_INFORMATION ProcessInfo;

	// LPWCSTR to LPWSTR
	wchar_t params[512];
	wcscpy(params, shellParams.c_str());

	// Start cgc.exe and Generate .hlsl from .cg
	BOOL appStarted = CreateProcessW(cgcExePath.c_str(), params, nullptr, nullptr, false, 0, nullptr, nullptr, &StartupInfo, &ProcessInfo);
	if (!appStarted) {
		lastErrorMsg = zsString(L"cannot find cg compiler @") + cgcExePath;
		return false;
	}

	WaitForSingleObject(ProcessInfo.hProcess, INFINITE);

	// CgC fail !?
	if ( ! cFileUtil::isFileExits(shaderOut))
	{
		lastErrorMsg = L"\n\nThe CG Compiler don't know what the fucking hell are you doing with your shader, can't compile";
		return false;
	}

	return true;
}

cCgShaderHelper::tHLSLDesc cCgShaderHelper::GetHLSLDesc(const zsString& hlslFilePath, const void* byteCode, size_t byteCodeSize) {

	// Result
	tHLSLDesc result;

	// Reflect HLSL Sm (4.0 - 5.0) shader
	ID3D11ShaderReflection* reflector;
	D3DReflect(byteCode, byteCodeSize, IID_ID3D11ShaderReflection, (void**)&reflector);
	D3D11_SHADER_DESC shaderDesc;
	reflector->GetDesc(&shaderDesc);


	std::unordered_map<zsString, uint16_t> textureSlots;

	// Collect samplers, texture names and their slot indexing
	for (size_t i = 0; i < shaderDesc.BoundResources; i++)
	{
		D3D11_SHADER_INPUT_BIND_DESC bindDesc;
		reflector->GetResourceBindingDesc((UINT)i, &bindDesc);

		switch (bindDesc.Type) {
		case D3D_SIT_SAMPLER: result.samplerInfo[bindDesc.Name + 1].samplerStateSlot = bindDesc.BindPoint; break;
		case D3D_SIT_TEXTURE: textureSlots[bindDesc.Name] = bindDesc.BindPoint;
		}
	}


	// Parse hlsl code for samplers, textures
	std::ifstream hlslFile(hlslFilePath);

	auto lines = cFileUtil::GetLines(hlslFile);
	for (auto it = lines.begin(); it != lines.end(); it++) {
		const zsString& row = *it;

		// Match samplers with textures
		int chPos = cStrUtil::Find(row, L".Sample");
		if (chPos >= 0) {
			zsString textureName;
			zsString samplerName;

			// "].Sample"
			// Found arrays of samplers...
			if (row[chPos - 1] == ']') {
				// Example : _pout._color = _TMP23[0].Sample(_shadowMaps[1], _In._tex01);
				int leftBracketPos = cStrUtil::FindLeft(row, chPos, '[');

				textureName = cStrUtil::SubStrLeft(row, leftBracketPos - 1, '_');
				samplerName = cStrUtil::SubStrRight(row, chPos + 9, '[', -1);
			}
			else {
				// Example : _pout._color = _TMP23.Sample(_diffuseTex, _In._tex01);
				textureName = cStrUtil::SubStrLeft(row, chPos - 1, '_');

				// Go to parenthesis '(' Noob solution, good solution is to start from chPos...
				auto parenthesisIdx = cStrUtil::Find(row, '(');
				assert(parenthesisIdx > 0);

				samplerName = cStrUtil::SubStrRight(row, parenthesisIdx + 2, ',', -1);
			}

			auto it = textureSlots.find(textureName);
			if (it != textureSlots.end())
			{
				result.samplerInfo[samplerName].textureSlot = it->second;
			}
			
		}
	}

	hlslFile.close();

	return result;
}

const std::list<zsString>& cCgShaderHelper::GetIncludedFilesPaths() const {
	return includedFilesPaths;
}

std::unordered_map<zsString, tSamplerStateDesc> cCgShaderHelper::GetSamplerStates() {
	std::unordered_map<zsString, tSamplerStateDesc> result;

	// Lines that contains "sampler" and "=", contains sampler states under that
	const zsString words[2] = { L"sampler", L"=" };
	auto samplerLineIndices = cStrUtil::GetLinesContainingAllStr(cgFileLines, words, 2);

	// For each sampler line that uses SamplerStates 
	//for (auto it = samplerLineIndices.begin(); it != samplerLineIndices.end(); it++) {
	for (auto lineIdx : samplerLineIndices) {
		// Sampler Name
		
		const zsString& _samplerName = *std::next(cgFileLines.begin(), lineIdx);
		zsString samplerName = _samplerName;
		cStrUtil::TrimBorder(samplerName, ' ');
		cStrUtil::Between(samplerName, ' ', ' ');


		const auto samplerStateLines = cStrUtil::GetLines(cgFileLines, lineIdx + 1, L";");

		// TODO Noob mode : assume one state per line
		// ex.
		// MipFilter = POINT,
		// MinFilter = POINT,
		// MagFilter = POINT,

		tSamplerStateDesc desc;

		// For each of the above lines
		for (auto state : samplerStateLines) {
			const zsString& row = *std::next(cgFileLines.begin(), state);
			if (row.size() == 0)
				continue;

			// ex. "MipFilter = POINT,", split, trim to "MipFilter", "POINT", then lower those
			std::list<zsString> parts = cStrUtil::SplitAt(row, '=');
			const wchar_t borders[5] = { ' ', ',', '\t', '{', '}'};
			cStrUtil::TrimBorder(parts, borders, 5);
			cStrUtil::ToUpper(parts);

			auto it = parts.begin();
			const zsString& left = *it++;
			const zsString& right = *it;

			if (left == L"MIPFILTER") {
				if (right == L"POINT") {
					desc.filterMip = eFilter::POINT;
				}
				else if (right == L"LINEAR") {
					desc.filterMip = eFilter::LINEAR;
				}
				else if (right == L"ANISOTROPIC") {
					desc.filterMip = eFilter::ANISOTROPIC;
				}

			}
			else if (left == L"MINFILTER") {
				if (right == L"POINT") {
					desc.filterMin = eFilter::POINT;
				}
				else if (right == L"LINEAR") {
					desc.filterMin = eFilter::LINEAR;
				}
				else if (right == L"ANISOTROPIC") {
					desc.filterMin = eFilter::ANISOTROPIC;
				}

			}
			else if (left == L"MAGFILTER") {
				if (right == L"POINT") {
					desc.filterMag = eFilter::POINT;
				}
				else if (right == L"LINEAR") {
					desc.filterMag = eFilter::LINEAR;
				}
				else if (right == L"ANISOTROPIC") {
					desc.filterMag = eFilter::ANISOTROPIC;
				}
			}
			else if (left == L"ADDRESSU") {
				if (right == L"CLAMP") {
					desc.addressU = eAddress::CLAMP;
				}
				else if (right == L"WRAP") {
					desc.addressU = eAddress::WRAP;
				}
				else if (right == L"MIRROR") {
					desc.addressU = eAddress::MIRROR;
				}
			}
			else if (left == L"ADDRESSV") {
				if (right == L"CLAMP") {
					desc.addressV = eAddress::CLAMP;
				}
				else if (right == L"WRAP") {
					desc.addressV = eAddress::WRAP;
				}
				else if (right == L"MIRROR") {
					desc.addressV = eAddress::MIRROR;
				}
			}
		}
		result[samplerName] = desc;
	}
	return result;
}


const wchar_t* cCgShaderHelper::GetLastErrorMsg() {
	return (lastErrorMsg.size() == 0 ) ? nullptr : lastErrorMsg.c_str();
}

cVertexFormat cCgShaderHelper::GetVSInputFormat()
{
	// That assert can occur not defining technique in the file, OR ! using constant buffer like constant.camPos not declared in the file :(
	assert(info.domainsExist[cCgShaderHelper::VS]);

	// Parse input Layout... from VERTEX_SHADER
	// - 1. search for vertexShader Entry name ex:"VS_MAIN(", get return value, for example struct name VS_OUT, or valid params like float4
	// - 2. search for VS_OUT, get lines under that, while line != "};"
	// - 3. extract VERTEX DECLARATION from those lines

	// TODOOOOOOOOOOOOOOOOOOOOOOO Found info.domainsEntry[VS] && Find'(' but next character is not')'
	zsString vsEntryLine = cStrUtil::GetWordAfter(cgFileLines, info.domainsEntry[VS] + L"(");
	std::list<zsString> vsInStructLines = cStrUtil::GetLinesBetween(cgFileLines, vsEntryLine, L"};");

	// (inputLayout)  Vertex shader input format
	std::vector<cVertexFormat::Attribute> attribs;
	cVertexFormat::Attribute tmpAttrib;
	uint16_t attribIdx = 0;

	for (auto str : vsInStructLines) {
	
		if (cStrUtil::Find(str, ':') < 0)
			continue;

		// not empty line... Parse Vertex Declaration
		if (str.size() != 0) {
			char semanticNames[10][32]; // Max 10 semantic, each 32 word length
			char semanticIndex[3]; // 999 max

			cStrUtil::GetWordBetween(str, ':', ';', semanticNames[attribIdx]);
			cStrUtil::GetNumberFromEnd(semanticNames[attribIdx], semanticIndex);
			cStrUtil::CutNumberFromEnd(semanticNames[attribIdx]);

			if (strcmp(semanticNames[attribIdx], "POSITION") == 0)
				tmpAttrib.semantic = cVertexFormat::POSITION;
			else if (strcmp(semanticNames[attribIdx], "NORMAL") == 0)
				tmpAttrib.semantic = cVertexFormat::NORMAL;
			else if (strcmp(semanticNames[attribIdx], "COLOR") == 0)
				tmpAttrib.semantic = cVertexFormat::COLOR;
			else if (strcmp(semanticNames[attribIdx], "TEXCOORD") == 0)
				tmpAttrib.semantic = cVertexFormat::TEXCOORD;
			else
			{
				lastErrorMsg = zsString("Zsiroskenyer Engine does not support that vertex shader input format semantic name :( -> ") + zsString(semanticNames[attribIdx]);
				assert(0);
			}


			if (cStrUtil::Find(str, L"float4") >= 0) {
				tmpAttrib.bitsPerComponent = cVertexFormat::_32_BIT;
				tmpAttrib.nComponents = 4;
				tmpAttrib.type = cVertexFormat::FLOAT;
			}
			else if (cStrUtil::Find(str, L"float3") >= 0) {
				tmpAttrib.bitsPerComponent = cVertexFormat::_32_BIT;
				tmpAttrib.nComponents = 3;
				tmpAttrib.type = cVertexFormat::FLOAT;
			}
			else if (cStrUtil::Find(str, L"float2") >= 0) {
				tmpAttrib.bitsPerComponent = cVertexFormat::_32_BIT;
				tmpAttrib.nComponents = 2;
				tmpAttrib.type = cVertexFormat::FLOAT;
			}
			else if (cStrUtil::Find(str, L"float") >= 0) {
				tmpAttrib.bitsPerComponent = cVertexFormat::_32_BIT;
				tmpAttrib.nComponents = 1;
				tmpAttrib.type = cVertexFormat::FLOAT;
			}
			else
			{
				lastErrorMsg = L"Cg file parsing: Failed to determine vertex shader input attribute format type (float1, 2, 3, 4) supported lol";
				assert(0);
			}

			attribs.push_back(tmpAttrib);
		}

		attribIdx++;
	}

	// Vertex Shader input format
	cVertexFormat vsInputFormat;
	vsInputFormat.Create(attribs);

	return vsInputFormat;
}

const cCgShaderHelper::tCgInfo& cCgShaderHelper::GetDomainInfo() {
	return info;
}