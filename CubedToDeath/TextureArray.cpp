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
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_R, GL_REPEAT);
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
	LoadLayer(tex_id::dirt_n, "res/block/dirt_n.png");
	LoadLayer(tex_id::stone, "res/block/stone.png");
	LoadLayer(tex_id::stone_n, "res/block/stone_n.png");
	LoadLayer(tex_id::cobblestone, "res/block/cobblestone.png");
	LoadLayer(tex_id::cobblestone_n, "res/block/cobblestone_n.png");
	LoadLayer(tex_id::grass_top, "res/block/grass_block_top.png");
	LoadLayer(tex_id::grass_top_n, "res/block/grass_top_n.png");
	LoadLayer(tex_id::grass_side_overlay, "res/block/grass_block_side_overlay.png");
	LoadLayer(tex_id::grass_side_overlay_n, "res/block/grass_block_side_overlay_n.png");
	LoadLayer(tex_id::wood_side, "res/block/birch_log.png");
	LoadLayer(tex_id::wood_side_n, "res/block/birch_log_n.png");
	LoadLayer(tex_id::wood_side_s, "res/block/birch_log_s.png");
	LoadLayer(tex_id::planks, "res/block/birch_planks.png");
	LoadLayer(tex_id::planks_n, "res/block/birch_planks_n.png");
	LoadLayer(tex_id::planks_s, "res/block/birch_planks_s.png");
	LoadLayer(tex_id::gold_ore, "res/block/gold_ore.png");
	LoadLayer(tex_id::gold_ore_n, "res/block/gold_ore_n.png");
	LoadLayer(tex_id::gold_ore_s, "res/block/gold_ore_s.png");
	LoadLayer(tex_id::iron_block, "res/block/iron_block.png");
	LoadLayer(tex_id::iron_block_n, "res/block/iron_block_n.png");
	LoadLayer(tex_id::iron_block_s, "res/block/iron_block_s.png");
	LoadLayer(tex_id::wood_top, "res/block/birch_log_top.png");
	LoadLayer(tex_id::wood_top_n, "res/block/birch_log_top_n.png");
	LoadLayer(tex_id::wood_top_s, "res/block/birch_log_top_s.png");
	LoadLayer(tex_id::leaves, "res/block/spruce_leaves.png");
	LoadLayer(tex_id::leaves_n, "res/block/orange_concrete_n.png");
	LoadLayer(tex_id::torch, "res/block/torch.png");
	LoadLayer(tex_id::torch_n, "res/block/torch_n.png");
	LoadLayer(tex_id::torch_s, "res/block/torch_s.png");
	LoadLayer(tex_id::redstone, "res/block/redstone_dust_line0.png");
	LoadLayer(tex_id::redstone_dot, "res/block/redstone_dust_dot.png");
	LoadLayer(tex_id::lever_base, "res/block/lever_base.png");
	LoadLayer(tex_id::lever_base_top, "res/block/lever_base_top.png");
	LoadLayer(tex_id::water, "res/block/tnt_top.png");
	LoadLayer(tex_id::door_top, "res/block/oak_door_top.png");
	LoadLayer(tex_id::door_bottom, "res/block/oak_door_bottom.png");

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
