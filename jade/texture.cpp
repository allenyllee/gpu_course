#include "texture.h"
#include "../image.h"
#include "../file_utility.h"

namespace jade
{
	Texture::Texture(TextureManager* _manager)
		: texBuf(0), manager(_manager)
	{

	}

	Texture::Texture()
		: texBuf(0), manager(0)
	{

	}

	Texture::~Texture()
	{
		if(manager)
			manager->RemoveTexture(path);
		delete [] texBuf;
	}

	bool Texture::Load(RenderDevice* device, const std::string& path)
	{
		int width, height;
		unsigned char* imgbuf;

		bool loadStat = LoadTGA(path.c_str(), &imgbuf, &width, &height);

		HWTexture2D* _hwTexture = NULL;

		HWTexture2D::Desc desc;
		desc.arraySize = 0;
		desc.format = TEX_FORMAT_RGBA8;
		desc.width = width;
		desc.height = height;
		desc.mipLevels = (unsigned int) TotalMipLevels(width, height);
		desc.generateMipmap = true;

		SubresourceData data;
		data.buf = imgbuf;

		RenderDevice::error_t err = device->CreateTexture2D(&desc, &data, &_hwTexture);
		this->hwTexture = _hwTexture;
		
		return loadStat;
	}

	TextureManager::TextureManager(RenderDevice* _device)
	{
		device = _device;
		Texture* whiteTexture = new Texture();

		unsigned char white[] = {255, 255, 255, 255};

		HWTexture2D::Desc desc;
		desc.arraySize = 0;
		desc.format = TEX_FORMAT_RGBA8;
		desc.width = 1;
		desc.height = 1;
		desc.mipLevels = 1;
		desc.generateMipmap = true;

		SubresourceData data;
		data.buf = white;
		HWTexture2D* hwTexture = NULL;
		RenderDevice::error_t err = device->CreateTexture2D(&desc, &data, &hwTexture);

		whiteTexture->hwTexture = hwTexture;

		texTable["sys:white"] = whiteTexture;
	}

	Texture* TextureManager::Load(const std::string& path)
	{
		int width, height;
		unsigned char* imgbuf;

		if(texTable.find(path) != texTable.end())
		{
			return texTable[path];
		}

		bool success = LoadTGA(path.c_str(), &imgbuf, &width, &height);

		if(!success)
			return NULL;

		Texture* texture = new Texture(this);
		texTable[path] = texture;
		texture->path = path;
		texture->texBuf = imgbuf;

		HWTexture2D* hwTexture = NULL;

		HWTexture2D::Desc desc;
		desc.arraySize = 0;
		desc.format = TEX_FORMAT_RGBA8;
		desc.width = width;
		desc.height = height;
		desc.mipLevels = (unsigned int) TotalMipLevels(width, height);
		desc.generateMipmap = true;

		SubresourceData data;
		data.buf = imgbuf;

		RenderDevice::error_t err = device->CreateTexture2D(&desc, &data, &hwTexture);
		texture->hwTexture = hwTexture;

		return texture;
	}

	void TextureManager::RemoveTexture(const std::string& path)
	{
		if(texTable.find(path) != texTable.end())
		{
			 texTable.erase(path);
		}
	}

}