#include "TextureArray.h"
#include "Texture.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include "SimpleBlock.h"
#include <algorithm>
#include "MyCraft.h"

TextureArray::TextureArray(std::string path)
{
	//GLsizei width = 16;
	//GLsizei height = 16;

	glGenTextures(1, &texture_id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture_id);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	std::cout << "ver: " << glGetString(GL_VERSION) << "\n";
	float aniso = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
	std::cout << "max anisotropic level: " << aniso << "\n";
	glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_ANISOTROPY_EXT, std::min(aniso, std::stof(MyCraft::config_map["anidotropic_filtering"])));
	//std::cout << glGetError() << " \n";


	/*glTexImage3D(GL_TEXTURE_2D_ARRAY,
		0,                // level
		GL_RGBA8,         // Internal format
		texture_size, texture_size, layer_count, // width,height,depth
		0,                // border?
		GL_RGBA,          // format
		GL_UNSIGNED_BYTE, // type
		nullptr);          // pointer to data
	glTexImage3D(GL_TEXTURE_2D_ARRAY,
		1,                // level
		GL_RGBA8,         // Internal format
		texture_size / 2, texture_size / 2, layer_count, // width,height,depth
		0,                // border?
		GL_RGBA,          // format
		GL_UNSIGNED_BYTE, // type
		nullptr);
	glTexImage3D(GL_TEXTURE_2D_ARRAY,
		2,                // level
		GL_RGBA8,         // Internal format
		texture_size / 4, texture_size / 4, layer_count, // width,height,depth
		0,                // border?
		GL_RGBA,          // format
		GL_UNSIGNED_BYTE, // type
		nullptr);*/

	glTexStorage3D(GL_TEXTURE_2D_ARRAY,
		std::stoi(MyCraft::config_map["mipmap_count"]),
		GL_RGBA8,
		texture_size,
		texture_size,
		layer_count);


	LoadLayer(tex_id::dirt, "res/block/dirt.png");
	LoadLayer(tex_id::stone, "res/block/cobblestone.png");
	LoadLayer(tex_id::grass_side, "res/block/grass_block_side.png");
	LoadLayer(tex_id::grass_top, "res/block/grass_block_top.png");
	LoadLayer(tex_id::wood_side, "res/block/spruce_log.png");
	LoadLayer(tex_id::wood_top, "res/block/spruce_log_top.png");
	LoadLayer(tex_id::wood_top, "res/block/spruce_log_top.png");
	LoadLayer(tex_id::leaves, "res/block/spruce_leaves.png");
	LoadLayer(tex_id::torch_side, "res/block/torch.png");

	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
}

void TextureArray::Bind()
{
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture_id);
}

void TextureArray::LoadLayer(int layer, std::string path)
{
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
			0,                // Mipmap number
			0, 0, layer,		// xoffset, yoffset, zoffset
			width, height, 1, // width, height, depth
			GL_RGBA,          // format
			GL_UNSIGNED_BYTE, // type
			data);           // pointer to data
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}
