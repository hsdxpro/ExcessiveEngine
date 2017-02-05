#include "MeshComponent.h"
#include "GraphicsEngine\IEntity.h"
#include "PlatformLibrary\Sys.h"
#include "EngineCore.h"

void MeshComponent::SetTextureNormal(const std::string& contentPath)
{
	graphics::ITexture* texture = Core.GetGraphicsEngine()->CreateTexture();
	texture->Load(GetAssetsDir() + contentPath);

	// Load contentPath
	graphics::IMaterial* material = entity->GetMaterial();

	int nSubMaterials = material->GetNumSubMaterials();

	for (int i = 0; i < nSubMaterials; ++i)
	{
		graphics::IMaterial::SubMaterial& subMaterial = material->GetSubMaterial(i);

		subMaterial.t_normal = texture;
	}
}

void MeshComponent::SetTextureBaseColor(const std::string& contentPath)
{
	graphics::ITexture* texture = Core.GetGraphicsEngine()->CreateTexture();
	texture->Load(GetAssetsDir() + contentPath);

	// Load contentPath
	graphics::IMaterial* material = entity->GetMaterial();

	int nSubMaterials = material->GetNumSubMaterials();

	for (int i = 0; i < nSubMaterials; ++i)
	{
		graphics::IMaterial::SubMaterial& subMaterial = material->GetSubMaterial(i);

		subMaterial.t_diffuse = texture;
	}
}

void MeshComponent::SetTextureAO(const std::string& contentPath)
{
	graphics::ITexture* texture = Core.GetGraphicsEngine()->CreateTexture();
	texture->Load(GetAssetsDir() + contentPath);

	// Load contentPath
	graphics::IMaterial* material = entity->GetMaterial();

	int nSubMaterials = material->GetNumSubMaterials();

	for (int i = 0; i < nSubMaterials; ++i)
	{
		graphics::IMaterial::SubMaterial& subMaterial = material->GetSubMaterial(i);

		subMaterial.t_ao = texture;
	}
}