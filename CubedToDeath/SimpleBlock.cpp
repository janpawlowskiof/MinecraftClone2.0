#include "SimpleBlock.h"
#include <iostream>
#include "HitInfo.h"
#include "ComplexBlock.h"
#include "Chunk.h"
#include "Vertex.h"
#include "TextureInfo.h"

SimpleBlock::SimpleBlock(unsigned char id)
{
	this->id = id;

	switch (id)
	{
	case blk_id::air_id:
	case blk_id::leaves_id:
		SetFlag(OPAQUE, false);
		break;
	case blk_id::water_id:
		SetFlag(OPAQUE, false);
		SetFlag(FLUID, true);
		break;
	default:
		SetFlag(OPAQUE, true);
	}
}


SimpleBlock::~SimpleBlock()
{
}

//											vertex format														//
//	world_x, world_y, world_z,		texture_x, texture_y,		normal_x, normal_y, normal_z,		texture_id,		overlay_id,			colorization.rgb	//

//wpisuje swoje widoczne vertices w miejsce gdzie wskazuje target i zwraca pierwszy adres za wpisanymi danymi

static void CreateNorthFace(std::vector<Vertex>& vertices, int world_x, int world_y, int world_z, TextureInfo texture_info, TextureInfo overlay_info, glm::vec3 color01, glm::vec3 color11, float count = 1)
{
	const glm::vec3 normal(0, 0, 1);
	const glm::vec3 tangent(1, 0, 0);
	vertices.push_back(Vertex(glm::vec3(world_x + 0.0f, world_y + 0.0f, world_z + count), glm::vec2(0 * m_unit, (0 + 1) * m_unit), normal, tangent,texture_info, overlay_info, color01));
	vertices.push_back(Vertex(glm::vec3(world_x + count, world_y + 0.0f, world_z + count), glm::vec2((0 + 1) * m_unit, (0 + 1) * m_unit), normal, tangent,texture_info, overlay_info, color11));
	vertices.push_back(Vertex(glm::vec3(world_x + count, world_y + 1.0f, world_z + count), glm::vec2((0 + 1) * m_unit, 0 * m_unit), normal, tangent,texture_info, overlay_info, color11));
	vertices.push_back(Vertex(glm::vec3(world_x + 0.0f, world_y + 0.0f, world_z + count), glm::vec2(0 * m_unit, (0 + 1) * m_unit), normal, tangent,texture_info, overlay_info, color01));
	vertices.push_back(Vertex(glm::vec3(world_x + count, world_y + 1.0f, world_z + count), glm::vec2((0 + 1) * m_unit, 0 * m_unit), normal, tangent,texture_info, overlay_info, color11));
	vertices.push_back(Vertex(glm::vec3(world_x + 0.0f, world_y + 1.0f, world_z + count), glm::vec2(0 * m_unit, 0 * m_unit), normal, tangent,texture_info, overlay_info, color01));
	//std::copy(vertices, vertices + sizeof(vertices) / sizeof(float), target);
	//return target + sizeof(vertices) / sizeof(float);
}

static void CreateTopFace(std::vector<Vertex>& vertices, int world_x, int world_y, int world_z, TextureInfo texture_info, TextureInfo overlay_info, glm::vec3 color00, glm::vec3 color01, glm::vec3 color10, glm::vec3 color11, float count = 1)
{
	const glm::vec3 normal(0, 1, 0);
	const glm::vec3 tangent(0, 0, 1);
	vertices.push_back(Vertex(glm::vec3(world_x + 0.0f, world_y + 1.0f, world_z + 0.0f), glm::vec2(0 * m_unit, (0) * m_unit), normal, tangent,texture_info, overlay_info, color00));
	vertices.push_back(Vertex(glm::vec3(world_x + 0.0f, world_y + 1.0f, world_z + count), glm::vec2((0) * m_unit, (0 + 1) * m_unit), normal, tangent,texture_info, overlay_info, color01));
	vertices.push_back(Vertex(glm::vec3(world_x + count, world_y + 1.0f, world_z + count), glm::vec2((0 + 1) * m_unit, (0 + 1) * m_unit), normal, tangent,texture_info, overlay_info, color11));
	vertices.push_back(Vertex(glm::vec3(world_x + 0.0f, world_y + 1.0f, world_z + 0.0f), glm::vec2(0 * m_unit, (0) * m_unit), normal, tangent,texture_info, overlay_info, color00));
	vertices.push_back(Vertex(glm::vec3(world_x + count, world_y + 1.0f, world_z + count), glm::vec2((0 + 1) * m_unit, (0 + 1) * m_unit), normal, tangent,texture_info, overlay_info, color11));
	vertices.push_back(Vertex(glm::vec3(world_x + count, world_y + 1.0f, world_z + 0.0f), glm::vec2((0 + 1) * m_unit, 0 * m_unit), normal, tangent,texture_info, overlay_info, color10));
}

static void CreateBottomFace(std::vector<Vertex>& vertices, int world_x, int world_y, int world_z, TextureInfo texture_info, TextureInfo overlay_info, glm::vec3 color00, glm::vec3 color01, glm::vec3 color10, glm::vec3 color11, float count = 1)
{
	const glm::vec3 normal(0, -1, 0);
	const glm::vec3 tangent(0, 0, -1);
	vertices.push_back(Vertex(glm::vec3(world_x + 0.0f, world_y + 0.0f, world_z + 0.0f), glm::vec2(0 * m_unit, (0) * m_unit), normal, tangent,texture_info, overlay_info, color00));
	vertices.push_back(Vertex(glm::vec3(world_x + count, world_y + 0.0f, world_z + count), glm::vec2((0 + 1) * m_unit, (0 + 1) * m_unit), normal, tangent,texture_info, overlay_info, color11));
	vertices.push_back(Vertex(glm::vec3(world_x + 0.0f, world_y + 0.0f, world_z + count), glm::vec2((0) * m_unit, (0 + 1) * m_unit), normal, tangent,texture_info, overlay_info, color01));
	vertices.push_back(Vertex(glm::vec3(world_x + 0.0f, world_y + 0.0f, world_z + 0.0f), glm::vec2(0 * m_unit, (0) * m_unit), normal, tangent,texture_info, overlay_info, color00));
	vertices.push_back(Vertex(glm::vec3(world_x + count, world_y + 0.0f, world_z + 0.0f), glm::vec2((0 + 1) * m_unit, 0 * m_unit), normal, tangent,texture_info, overlay_info, color10));
	vertices.push_back(Vertex(glm::vec3(world_x + count, world_y + 0.0f, world_z + count), glm::vec2((0 + 1) * m_unit, (0 + 1) * m_unit), normal, tangent,texture_info, overlay_info, color11));
}

static void CreateSouthFace(std::vector<Vertex>& vertices, int world_x, int world_y, int world_z, TextureInfo texture_info, TextureInfo overlay_info, glm::vec3 color00, glm::vec3 color10, float count = 1)
{
	const glm::vec3 normal(0, 0, -1);
	const glm::vec3 tangent(-1, 0, 0);
	vertices.push_back(Vertex(glm::vec3(world_x + 0.0f, world_y + 0.0f, world_z + 0.0f), glm::vec2(0 * m_unit, (0 + 1) * m_unit), normal, tangent,texture_info, overlay_info, color00));
	vertices.push_back(Vertex(glm::vec3(world_x + count, world_y + 1.0f, world_z + 0.0f), glm::vec2((0 + 1) * m_unit, 0 * m_unit), normal, tangent,texture_info, overlay_info, color10));
	vertices.push_back(Vertex(glm::vec3(world_x + count, world_y + 0.0f, world_z + 0.0f), glm::vec2((0 + 1) * m_unit, (0 + 1) * m_unit), normal, tangent,texture_info, overlay_info, color10));

	vertices.push_back(Vertex(glm::vec3(world_x + 0.0f, world_y + 0.0f, world_z + 0.0f), glm::vec2(0 * m_unit, (0 + 1) * m_unit), normal, tangent,texture_info, overlay_info, color00));
	vertices.push_back(Vertex(glm::vec3(world_x + 0.0f, world_y + 1.0f, world_z + 0.0f), glm::vec2(0 * m_unit, 0 * m_unit), normal, tangent,texture_info, overlay_info, color00));
	vertices.push_back(Vertex(glm::vec3(world_x + count, world_y + 1.0f, world_z + 0.0f), glm::vec2((0 + 1) * m_unit, 0 * m_unit), normal, tangent,texture_info, overlay_info, color10));
}

static void CreateWestFace(std::vector<Vertex>& vertices, int world_x, int world_y, int world_z, TextureInfo texture_info, TextureInfo overlay_info, glm::vec3 color10, glm::vec3 color11, float count = 1)
{
	const glm::vec3 normal(1, 0, 0);
	const glm::vec3 tangent(0, -1, 0);
	vertices.push_back(Vertex(glm::vec3(world_x + count, world_y + 0.0f, world_z + 0.0f), glm::vec2(0 * m_unit, (0 + 1) * m_unit), normal, tangent,texture_info, overlay_info, color10));
	vertices.push_back(Vertex(glm::vec3(world_x + count, world_y + 1.0f, world_z + count), glm::vec2((0 + 1) * m_unit, 0 * m_unit), normal, tangent,texture_info, overlay_info, color11));
	vertices.push_back(Vertex(glm::vec3(world_x + count, world_y + 0.0f, world_z + count), glm::vec2((0 + 1) * m_unit, (0 + 1) * m_unit), normal, tangent,texture_info, overlay_info, color11));

	vertices.push_back(Vertex(glm::vec3(world_x + count, world_y + 0.0f, world_z + 0.0f), glm::vec2(0 * m_unit, (0 + 1) * m_unit), normal, tangent,texture_info, overlay_info, color10));
	vertices.push_back(Vertex(glm::vec3(world_x + count, world_y + 1.0f, world_z + 0.0f), glm::vec2(0 * m_unit, 0 * m_unit), normal, tangent,texture_info, overlay_info, color10));
	vertices.push_back(Vertex(glm::vec3(world_x + count, world_y + 1.0f, world_z + count), glm::vec2((0 + 1) * m_unit, 0 * m_unit), normal, tangent,texture_info, overlay_info, color11));
}

static void CreateEastFace(std::vector<Vertex>& vertices, int world_x, int world_y, int world_z, TextureInfo texture_info, TextureInfo overlay_info, glm::vec3 color10, glm::vec3 color11, float count = 1)
{
	const glm::vec3 normal(-1, 0, 0);
	const glm::vec3 tangent(0, 1, 0);
	vertices.push_back(Vertex(glm::vec3(world_x, world_y + 0.0f, world_z + 0.0f), glm::vec2(1 * m_unit, (0 + 1) * m_unit), normal, tangent,texture_info, overlay_info, color10));
	vertices.push_back(Vertex(glm::vec3(world_x, world_y + 0.0f, world_z + count), glm::vec2((0) * m_unit, (0 + 1) * m_unit), normal, tangent,texture_info, overlay_info, color11));
	vertices.push_back(Vertex(glm::vec3(world_x, world_y + 1.0f, world_z + count), glm::vec2((0) * m_unit, 0 * m_unit), normal, tangent,texture_info, overlay_info, color11));

	vertices.push_back(Vertex(glm::vec3(world_x, world_y + 0.0f, world_z + 0.0f), glm::vec2(1 * m_unit, (0 + 1) * m_unit), normal, tangent,texture_info, overlay_info, color10));
	vertices.push_back(Vertex(glm::vec3(world_x, world_y + 1.0f, world_z + count), glm::vec2((0) * m_unit, 0 * m_unit), normal, tangent,texture_info, overlay_info, color11));
	vertices.push_back(Vertex(glm::vec3(world_x, world_y + 1.0f, world_z + 0.0f), glm::vec2(1 * m_unit, 0 * m_unit), normal, tangent,texture_info, overlay_info, color10));
}

static float* CreateNorthFace(float* target, int world_x, int world_y, int world_z, int texture_id, int overlay_id, glm::vec3 color01, glm::vec3 color11, float count = 1)
{
	const glm::vec3 normal(0, 0, 1);
	const glm::vec3 tangent(1, 0, 0);
	const float vertices[] = {
		world_x + 0.0f, world_y + 0.0f, world_z + count, 0 * m_unit, (0 + 1) * m_unit, xyz(normal), xyz(tangent), texture_id, overlay_id,xyz(color01),
		world_x + count, world_y + 0.0f, world_z + count, (0 + 1) * m_unit, (0 + 1) * m_unit, xyz(normal), xyz(tangent),texture_id, overlay_id,xyz(color11),
		world_x + count, world_y + 1.0f, world_z + count, (0 + 1) * m_unit, 0 * m_unit, xyz(normal), xyz(tangent),texture_id, overlay_id,xyz(color11),

		world_x + 0.0f, world_y + 0.0f, world_z + count, 0 * m_unit, (0 + 1) * m_unit, xyz(normal), xyz(tangent),texture_id, overlay_id,xyz(color01),
		world_x + count, world_y + 1.0f, world_z + count, (0 + 1) * m_unit, 0 * m_unit, xyz(normal), xyz(tangent),texture_id, overlay_id,xyz(color11),
		world_x + 0.0f, world_y + 1.0f, world_z + count, 0 * m_unit, 0 * m_unit, xyz(normal), xyz(tangent),texture_id, overlay_id, xyz(color01) };
	std::copy(vertices, vertices + sizeof(vertices) / sizeof(float), target);
	return target + sizeof(vertices) / sizeof(float);
}
//wpisuje swoje widoczne vertices w miejsce gdzie wskazuje target i zwraca pierwszy adres za wpisanymi danymi
static float* CreateSouthFace(float* target, int world_x, int world_y, int world_z, int texture_id, int overlay_id, glm::vec3 color00, glm::vec3 color10, float count = 1)
{
	const glm::vec3 normal(0, 0, -1);
	const glm::vec3 tangent(-1, 0, 0);
	const float vertices[] = {
		world_x + 0.0f, world_y + 0.0f, world_z + 0.0f, 0 * m_unit, (0 + 1) * m_unit, xyz(normal), xyz(tangent),texture_id,overlay_id,xyz(color00),
		world_x + count, world_y + 1.0f, world_z + 0.0f, (0 + 1) * m_unit, 0 * m_unit, xyz(normal), xyz(tangent),texture_id,overlay_id,xyz(color10),
		world_x + count, world_y + 0.0f, world_z + 0.0f, (0 + 1) * m_unit, (0 + 1) * m_unit, xyz(normal), xyz(tangent),texture_id,overlay_id,xyz(color10),

		world_x + 0.0f, world_y + 0.0f, world_z + 0.0f, 0 * m_unit, (0 + 1) * m_unit, xyz(normal), xyz(tangent),texture_id,overlay_id,xyz(color00),
		world_x + 0.0f, world_y + 1.0f, world_z + 0.0f, 0 * m_unit, 0 * m_unit, xyz(normal), xyz(tangent),texture_id,overlay_id,xyz(color00),
		world_x + count, world_y + 1.0f, world_z + 0.0f, (0 + 1) * m_unit, 0 * m_unit, xyz(normal), xyz(tangent),texture_id,overlay_id,xyz(color10),
	};
	std::copy(vertices, vertices + sizeof(vertices) / sizeof(float), target);
	return target + sizeof(vertices) / sizeof(float);
}
//wpisuje swoje widoczne vertices w miejsce gdzie wskazuje target i zwraca pierwszy adres za wpisanymi danymi
static float* CreateTopFace(float* target, int world_x, int world_y, int world_z, int texture_id, int overlay_id, glm::vec3 color00, glm::vec3 color01, glm::vec3 color10, glm::vec3 color11, float count = 1)
{
	const glm::vec3 normal(0, 1, 0);
	const glm::vec3 tangent(0, 0, 1);
	const float vertices[] = {
		world_x + 0.0f, world_y + 1.0f, world_z + 0.0f, 0 * m_unit, (0) * m_unit, xyz(normal), xyz(tangent),texture_id,overlay_id,xyz(color00),
		world_x + 0.0f, world_y + 1.0f, world_z + count, (0) * m_unit, (0 + 1) * m_unit, xyz(normal), xyz(tangent),texture_id,overlay_id,xyz(color01),
		world_x + count, world_y + 1.0f, world_z + count, (0 + 1) * m_unit, (0 + 1) * m_unit, xyz(normal), xyz(tangent),texture_id,overlay_id,xyz(color11),

		world_x + 0.0f, world_y + 1.0f, world_z + 0.0f, 0 * m_unit, (0) * m_unit, xyz(normal), xyz(tangent),texture_id,overlay_id,xyz(color00),
		world_x + count, world_y + 1.0f, world_z + count, (0 + 1) * m_unit, (0 + 1) * m_unit, xyz(normal), xyz(tangent),texture_id,overlay_id,xyz(color11),
		world_x + count, world_y + 1.0f, world_z + 0.0f, (0 + 1) * m_unit, 0 * m_unit, xyz(normal), xyz(tangent),texture_id,overlay_id,xyz(color10),
	};
	std::copy(vertices, vertices + sizeof(vertices) / sizeof(float), target);
	return target + sizeof(vertices) / sizeof(float);
}
//wpisuje swoje widoczne vertices w miejsce gdzie wskazuje target i zwraca pierwszy adres za wpisanymi danymi
static float* CreateBottomFace(float* target, int world_x, int world_y, int world_z, int texture_id, int overlay_id, glm::vec3 color00, glm::vec3 color01, glm::vec3 color10, glm::vec3 color11, float count = 1)
{
	const glm::vec3 normal(0, -1, 0);
	const glm::vec3 tangent(0, 0, -1);
	const float vertices[] = {
		world_x + 0.0f, world_y + 0.0f, world_z + 0.0f, 0 * m_unit, (0) * m_unit, xyz(normal), xyz(tangent),texture_id,overlay_id,xyz(color00),
		world_x + count, world_y + 0.0f, world_z + count, (0 + 1) * m_unit, (0 + 1) * m_unit, xyz(normal), xyz(tangent),texture_id,overlay_id,xyz(color11),
		world_x + 0.0f, world_y + 0.0f, world_z + count, (0) * m_unit, (0 + 1) * m_unit, xyz(normal), xyz(tangent),texture_id,overlay_id,xyz(color01),

		world_x + 0.0f, world_y + 0.0f, world_z + 0.0f, 0 * m_unit, (0) * m_unit, xyz(normal), xyz(tangent),texture_id,overlay_id,xyz(color00),
		world_x + count, world_y + 0.0f, world_z + 0.0f, (0 + 1) * m_unit, 0 * m_unit, xyz(normal), xyz(tangent),texture_id,overlay_id,xyz(color10),
		world_x + count, world_y + 0.0f, world_z + count, (0 + 1) * m_unit, (0 + 1) * m_unit, xyz(normal), xyz(tangent),texture_id,overlay_id,xyz(color11),
	};
	std::copy(vertices, vertices + sizeof(vertices) / sizeof(float), target);
	return target + sizeof(vertices) / sizeof(float);
}
//wpisuje swoje widoczne vertices w miejsce gdzie wskazuje target i zwraca pierwszy adres za wpisanymi danymi
static float* CreateWestFace(float* target, int world_x, int world_y, int world_z, int texture_id, int overlay_id, glm::vec3 color10, glm::vec3 color11, float count = 1)
{
	const glm::vec3 normal(1, 0, 0);
	const glm::vec3 tangent(0, 1, 0);
	const float vertices[] = {
		world_x + count, world_y + 0.0f, world_z + 0.0f, 0 * m_unit, (0 + 1) * m_unit, xyz(normal), xyz(tangent),texture_id,overlay_id,xyz(color10),
		world_x + count, world_y + 1.0f, world_z + count, (0 + 1) * m_unit, 0 * m_unit, xyz(normal), xyz(tangent),texture_id,overlay_id,xyz(color11),
		world_x + count, world_y + 0.0f, world_z + count, (0 + 1) * m_unit, (0 + 1) * m_unit, xyz(normal), xyz(tangent),texture_id,overlay_id,xyz(color11),

		world_x + count, world_y + 0.0f, world_z + 0.0f, 0 * m_unit, (0 + 1) * m_unit, xyz(normal), xyz(tangent),texture_id,overlay_id,xyz(color10),
		world_x + count, world_y + 1.0f, world_z + 0.0f, 0 * m_unit, 0 * m_unit, xyz(normal), xyz(tangent),texture_id,overlay_id,xyz(color10),
		world_x + count, world_y + 1.0f, world_z + count, (0 + 1) * m_unit, 0 * m_unit, xyz(normal), xyz(tangent),texture_id,overlay_id,xyz(color11),
	};
	std::copy(vertices, vertices + sizeof(vertices) / sizeof(float), target);
	return target + sizeof(vertices) / sizeof(float);
}
//wpisuje swoje widoczne vertices w miejsce gdzie wskazuje target i zwraca pierwszy adres za wpisanymi danymi
static float* CreateEastFace(float* target, int world_x, int world_y, int world_z, int texture_id, int overlay_id, glm::vec3 color00, glm::vec3 color01, float count = 1)
{
	const glm::vec3 normal(-1, 0, 0);
	const glm::vec3 tangent(0, -1, 0);
	const float vertices[] = {
		world_x + 0.0f, world_y + 0.0f, world_z + 0.0f, 0 * m_unit, (0 + 1) * m_unit, xyz(normal), xyz(tangent),texture_id,overlay_id,xyz(color00),
		world_x + 0.0f, world_y + 0.0f, world_z + count, (0 + 1) * m_unit, (0 + 1) * m_unit, xyz(normal), xyz(tangent),texture_id,overlay_id,xyz(color01),
		world_x + 0.0f, world_y + 1.0f, world_z + count, (0 + 1) * m_unit, 0 * m_unit, xyz(normal), xyz(tangent),texture_id,overlay_id,xyz(color01),

		world_x + 0.0f, world_y + 0.0f, world_z + 0.0f, 0 * m_unit, (0 + 1) * m_unit, xyz(normal), xyz(tangent),texture_id,overlay_id,xyz(color00),
		world_x + 0.0f, world_y + 1.0f, world_z + count, (0 + 1) * m_unit, 0 * m_unit, xyz(normal), xyz(tangent),texture_id,overlay_id,xyz(color01),
		world_x + 0.0f, world_y + 1.0f, world_z + 0.0f, 0 * m_unit, 0 * m_unit, xyz(normal), xyz(tangent),texture_id,overlay_id,xyz(color00),
	};
	std::copy(vertices, vertices + sizeof(vertices) / sizeof(float), target);
	return target + sizeof(vertices) / sizeof(float);
}

static float* CreateTopFluidFace(float* target, int world_x, int world_y, int world_z, glm::vec3 color00, glm::vec3 color01, glm::vec3 color10, glm::vec3 color11)
{
	const float vertices[] = {
		world_x + 0.0f, world_y + 1.0f, world_z + 0.0f, 0 * m_unit, (0) * m_unit, 0, 1, 0, xyz(color00),
		world_x + 0.0f, world_y + 1.0f, world_z + 1.0f, (0) * m_unit, (0 + 1) * m_unit, 0, 1, 0, xyz(color01),
		world_x + 1.0f, world_y + 1.0f, world_z + 1.0f, (0 + 1) * m_unit, (0 + 1) * m_unit, 0, 1, 0, xyz(color11),

		world_x + 0.0f, world_y + 1.0f, world_z + 0.0f, 0 * m_unit, (0) * m_unit, 0, 1, 0, xyz(color00),
		world_x + 1.0f, world_y + 1.0f, world_z + 1.0f, (0 + 1) * m_unit, (0 + 1) * m_unit, 0, 1, 0, xyz(color11),
		world_x + 1.0f, world_y + 1.0f, world_z + 0.0f, (0 + 1) * m_unit, 0 * m_unit, 0, 1, 0, xyz(color10),
	};
	std::copy(vertices, vertices + sizeof(vertices) / sizeof(float), target);
	return target + sizeof(vertices) / sizeof(float);
}

float* SimpleBlock::CreateFluidModel(float* target, int world_x, int world_y, int world_z, glm::vec3 color00, glm::vec3 color01, glm::vec3 color10, glm::vec3 color11)
{
	if (face_visible == 0)
		return target;
	glm::vec3 color = glm::vec3(0, 1, 1);
	if (GetFaceVisible(TOP))
	{
		target = CreateTopFluidFace(target, world_x, world_y, world_z, color00, color01, color10, color11);
	}
	return target;
}

float* SimpleBlock::CreateSolidModel(float* target, int world_x, int world_y, int world_z, glm::vec3 color00, glm::vec3 color01, glm::vec3 color10, glm::vec3 color11, int x_count, int z_count)
{
	if (face_visible == 0)
		return target;

	int tex_side_id = -1, tex_top_id = -1, overlay_side_id = -1, overlay_top_id = -1;
	const glm::vec3 no_colorization(-1, -1, -1);

	switch (id)
	{
	case blk_id::dirt_id:
		tex_side_id = tex_top_id = tex_id::dirt;
		color00 = color01 = color10 = color11 = no_colorization;
		break;
	case blk_id::stone_id:
		tex_side_id = tex_top_id = tex_id::stone;
		color00 = color01 = color10 = color11 = no_colorization;
		break;
	case blk_id::wood_id:
		tex_side_id = tex_id::wood_side;
		tex_top_id = tex_id::wood_top;
		color00 = color01 = color10 = color11 = no_colorization;
		break;
	case blk_id::grass_id:
		tex_side_id = tex_id::dirt;
		overlay_side_id = tex_id::grass_side_overlay;
		overlay_top_id = tex_id::grass_top;
		break;
	case blk_id::leaves_id:
		tex_side_id = tex_top_id = tex_id::leaves;
		break;
	case blk_id::air_id:
	default:
		return target;
	}
	if (GetFaceVisible(NORTH))
	{
		target = CreateNorthFace(target, world_x, world_y, world_z, tex_side_id, overlay_side_id, color01, color11, z_count);
	}
	if (GetFaceVisible(SOUTH))
	{
		target = CreateSouthFace(target, world_x, world_y, world_z, tex_side_id, overlay_side_id, color00, color10, z_count);
	}
	if (GetFaceVisible(WEST))
	{
		target = CreateWestFace(target, world_x, world_y, world_z, tex_side_id, overlay_side_id, color10, color11, x_count);
	}
	if (GetFaceVisible(EAST))
	{
		target = CreateEastFace(target, world_x, world_y, world_z, tex_side_id, overlay_side_id, color00, color01, x_count);
	}
	if (GetFaceVisible(TOP))
	{
		target = CreateTopFace(target, world_x, world_y, world_z, tex_top_id, overlay_top_id, color00, color01, color10, color11);
	}
	if (GetFaceVisible(BOTTOM))
	{
		target = CreateBottomFace(target, world_x, world_y, world_z, tex_top_id, overlay_top_id, color00, color01, color10, color11);
	}
	return target;
}

void SimpleBlock::CreateSolidModel(std::vector<Vertex>& vertices, int world_x, int world_y, int world_z, glm::vec3 color00, glm::vec3 color01, glm::vec3 color10, glm::vec3 color11, int x_count, int z_count)
{
	TextureInfo texture_side;
	TextureInfo overlay_side;
	TextureInfo texture_top;
	TextureInfo overlay_top;

	const glm::vec3 no_colorization(-1, -1, -1);

	switch (id)
	{
	case blk_id::dirt_id:
		texture_side.texture_id = texture_top.texture_id = tex_id::dirt;
		texture_side.normal_id = texture_top.normal_id = tex_id::dirt_n;
		color00 = color01 = color10 = color11 = no_colorization;
		break;
	case blk_id::stone_id:
		texture_side.texture_id = texture_top.texture_id = tex_id::stone;
		texture_side.normal_id = texture_top.normal_id = tex_id::stone_n;
		color00 = color01 = color10 = color11 = no_colorization;
		break;
	case blk_id::wood_id:
		texture_side.texture_id = tex_id::wood_side;
		texture_side.normal_id = tex_id::wood_side_n;
		texture_side.specular_id = tex_id::wood_side_s;
		texture_top.texture_id = tex_id::wood_top;
		texture_top.normal_id = tex_id::wood_top_n;
		texture_top.specular_id = tex_id::wood_top_s;
		color00 = color01 = color10 = color11 = no_colorization;
		break;
	case blk_id::planks_id:
		texture_side.texture_id = texture_top.texture_id = tex_id::planks;
		texture_side.normal_id = texture_top.normal_id = tex_id::planks_n;
		texture_side.specular_id = texture_top.specular_id = tex_id::planks_s;
		color00 = color01 = color10 = color11 = no_colorization;
		break;
	case blk_id::gold_ore_id:
		texture_side.texture_id = texture_top.texture_id = tex_id::gold_ore;
		texture_side.normal_id = texture_top.normal_id = tex_id::gold_ore_n;
		texture_side.specular_id = texture_top.specular_id = tex_id::gold_ore_s;
		color00 = color01 = color10 = color11 = no_colorization;
		break;
	case blk_id::grass_id:
		texture_side.texture_id = tex_id::dirt;
		texture_side.normal_id = tex_id::dirt_n;
		overlay_side.texture_id = tex_id::grass_side_overlay;
		overlay_side.normal_id = tex_id::grass_side_overlay_n;
		overlay_top.texture_id = tex_id::grass_top;
		overlay_top.normal_id = tex_id::grass_top_n;
		break;
	case blk_id::leaves_id:
		overlay_side.texture_id = overlay_top.texture_id = tex_id::leaves;
		overlay_side.normal_id = overlay_top.normal_id = tex_id::leaves_n;
		break;
	case blk_id::air_id:
	default:
		return;
	}
	if (GetFaceVisible(NORTH))
	{
		CreateNorthFace(vertices, world_x, world_y, world_z, texture_side, overlay_side, color01, color11, z_count);
	}
	if (GetFaceVisible(SOUTH))
	{
		CreateSouthFace(vertices, world_x, world_y, world_z, texture_side, overlay_side, color00, color10, z_count);
	}
	if (GetFaceVisible(WEST))
	{
		CreateWestFace(vertices, world_x, world_y, world_z, texture_side, overlay_side, color10, color11, x_count);
	}
	if (GetFaceVisible(EAST))
	{
		CreateEastFace(vertices, world_x, world_y, world_z, texture_side, overlay_side, color00, color01, x_count);
	}
	if (GetFaceVisible(TOP))
	{
		CreateTopFace(vertices, world_x, world_y, world_z, texture_top, overlay_top, color00, color01, color10, color11);
	}
	if (GetFaceVisible(BOTTOM))
	{
		CreateBottomFace(vertices, world_x, world_y, world_z, texture_top, overlay_top, color00, color01, color10, color11);
	}
}

bool SimpleBlock::CheckRayCollision(glm::vec3 origin, glm::vec3 direction, int block_x, int block_y, int block_z, RayHitInfo& hit_info)
{
	return CheckRayVsAABB(origin, direction, glm::vec3(block_x, block_y, block_z), glm::vec3(1, 1, 1), hit_info);
}

bool SimpleBlock::CheckRayVsAABB(glm::vec3 origin, glm::vec3 direction, glm::vec3 position, glm::vec3 dimentions, RayHitInfo& hit_info)
{
	float hit_x, hit_y, hit_z;
	if (direction.x > 0 && ProjectRayOnPlaneYZ(position.x, hit_y, hit_z, origin, direction) && between(hit_y, position.y, position.y + dimentions.y) && between(hit_z, position.z, position.z + dimentions.z))
	{
		hit_info.Update(floor(position.x), floor(position.y), floor(position.z), floor(position.x) - 1, floor(position.y), floor(position.z), glm::length(origin - glm::vec3(position.x, hit_y, hit_z)));
		return true;
	}
	if (direction.x < 0 && ProjectRayOnPlaneYZ(position.x + dimentions.x, hit_y, hit_z, origin, direction) && between(hit_y, position.y, position.y + dimentions.y) && between(hit_z, position.z, position.z + dimentions.z))
	{
		hit_info.Update(floor(position.x), floor(position.y), floor(position.z), floor(position.x) + 1, floor(position.y), floor(position.z), glm::length(origin - glm::vec3(position.x + dimentions.x, hit_y, hit_z)));
		return true;
	}
	if (direction.z > 0 && ProjectRayOnPlaneXY(position.z, hit_x, hit_y, origin, direction) && between(hit_y, position.y, position.y + dimentions.y) && between(hit_x, position.x, position.x + dimentions.x))
	{
		hit_info.Update(floor(position.x), floor(position.y), floor(position.z), floor(position.x), floor(position.y), floor(position.z) - 1, glm::length(origin - glm::vec3(hit_x, hit_y, position.z)));
		return true;
	}
	if (direction.z < 0 && ProjectRayOnPlaneXY(position.z + dimentions.z, hit_x, hit_y, origin, direction) && between(hit_y, position.y, position.y + dimentions.y) && between(hit_x, position.x, position.x + dimentions.x))
	{
		hit_info.Update(floor(position.x), floor(position.y), floor(position.z), floor(position.x), floor(position.y), floor(position.z) + 1, glm::length(origin - glm::vec3(hit_x, hit_y, position.z + dimentions.z)));
		return true;
	}
	if (direction.y > 0 && ProjectRayOnPlaneXZ(position.y, hit_x, hit_z, origin, direction) && between(hit_z, position.z, position.z + dimentions.z) && between(hit_x, position.x, position.x + dimentions.x))
	{
		hit_info.Update(floor(position.x), floor(position.y), floor(position.z), floor(position.x), floor(position.y) - 1, floor(position.z), glm::length(origin - glm::vec3(hit_x, position.y, hit_z)));
		return true;
	}
	if (direction.y < 0 && ProjectRayOnPlaneXZ(position.y + dimentions.y, hit_x, hit_z, origin, direction) && between(hit_z, position.z, position.z + dimentions.z) && between(hit_x, position.x, position.x + dimentions.x))
	{
		hit_info.Update(floor(position.x), floor(position.y), floor(position.z), floor(position.x), floor(position.y) + 1, floor(position.z), glm::length(origin - glm::vec3(hit_x, position.y + dimentions.y, hit_z)));
		return true;
	}
	return false;
}

bool CheckAABBVsAABB(glm::vec3 aabb1_position, glm::vec3 aabb1_half_extends, glm::vec3 aabb2_position, glm::vec3 aabb2_half_extends)
{
	if (aabb2_position.x + aabb2_half_extends.x < aabb1_position.x - aabb1_half_extends.x ||
		aabb2_position.y + aabb2_half_extends.y < aabb1_position.y - aabb1_half_extends.y ||
		aabb2_position.z + aabb2_half_extends.z < aabb1_position.z - aabb1_half_extends.z ||
		aabb1_position.x + aabb1_half_extends.x < aabb2_position.x - aabb2_half_extends.x ||
		aabb1_position.y + aabb1_half_extends.y < aabb2_position.y - aabb2_half_extends.y ||
		aabb1_position.z + aabb1_half_extends.z < aabb2_position.z - aabb2_half_extends.z)
		return false;
	else
		return true;
}

bool SimpleBlock::CheckCylinderVsAABB(glm::vec3 cylinder_center, float cylinder_radius, float cylinder_height, glm::vec3 aabb_center, glm::vec3 aabb_helf_extends, glm::vec3& closest_point)
{
	//broad check done here
	if (!CheckAABBVsAABB(aabb_center, aabb_helf_extends, cylinder_center, glm::vec3(cylinder_radius, cylinder_height / 2, cylinder_radius)))
	{
		return false;
	}
	//narrow check done here
	aabb_center.y = cylinder_center.y;
	glm::vec3 clamped = glm::clamp(cylinder_center - aabb_center, -aabb_helf_extends, aabb_helf_extends);
	closest_point = aabb_center + clamped;
	glm::vec3 diffrence = cylinder_center - closest_point;
	return glm::length(diffrence) < cylinder_radius;
}

float SimpleBlock::ResolveCollisionVerticaly(glm::vec3 block_center, glm::vec3 hitbox_center, float hitbox_height)
{
	return block_center.y + 0.5f - (hitbox_center.y - hitbox_height / 2);
}

glm::vec3 SimpleBlock::ResolveCollision(glm::vec3 block_center, glm::vec3 hitbox_center, float hitbox_radius, float hitbox_height)
{
	glm::vec3 result = glm::vec3(0);

	float closest_distance = 10000;
	float x0 = (block_center.x + 0.5) - (hitbox_center.x - hitbox_radius);
	float x1 = (block_center.x - 0.5) - (hitbox_center.x + hitbox_radius);
	float z0 = (block_center.z + 0.5) - (hitbox_center.z - hitbox_radius);
	float z1 = (block_center.z - 0.5) - (hitbox_center.z + hitbox_radius);
	closest_distance = fmin(closest_distance, abs(x0));
	closest_distance = fmin(closest_distance, abs(x1));
	closest_distance = fmin(closest_distance, abs(z0));
	closest_distance = fmin(closest_distance, abs(z1));

	if (abs(x0) == closest_distance)
	{
		result.x += x0;
	}
	else if (abs(x1) == closest_distance)
	{
		result.x += x1;
	}
	else if (abs(z0) == closest_distance)
	{
		result.z += z0;
	}
	else if (abs(z1) == closest_distance)
	{
		result.z += z1;
	}

	return result;
}

SimpleBlock* SimpleBlock::CreateNew(int block_id, RayHitInfo hit_info)
{
	switch (block_id)
	{
	case blk_id::torch_id:
		return new blk::Torch(glm::ivec3(hit_info.place_x, hit_info.place_y, hit_info.place_z), glm::ivec3(hit_info.hit_x, hit_info.hit_y, hit_info.hit_z), hit_info.place_chunk);
	case blk_id::switch_id:
		return new blk::Switch(glm::ivec3(hit_info.place_x, hit_info.place_y, hit_info.place_z), glm::ivec3(hit_info.hit_x, hit_info.hit_y, hit_info.hit_z), hit_info.place_chunk);
	case blk_id::door_id:
		return new blk::Door(glm::ivec3(hit_info.place_x, hit_info.place_y, hit_info.place_z), glm::ivec3(hit_info.hit_x, hit_info.hit_y, hit_info.hit_z), hit_info.place_chunk);
	default:
		return new SimpleBlock(block_id);
	}
}

SimpleBlock* SimpleBlock::CreateNew(int block_id)
{
	return CreateNew(block_id, RayHitInfo());
}

glm::vec3 SimpleBlock::GetColor(int block_id)
{
	switch (block_id)
	{
	case blk_id::dirt_id:
	case blk_id::grass_id:
		return glm::vec3(120.0, 85, 58) / 255.0f;
	case blk_id::stone_id:
		return glm::vec3(116, 116, 116) / 255.0f;

	case blk_id::wood_id:
		return glm::vec3(104, 83, 50) / 255.0f;
	case blk_id::leaves_id:
		return glm::vec3(80, 144, 38) / 255.0f;
	case blk_id::water_id:
		return glm::vec3(0,0,1);
	case blk_id::air_id:
	default:
		return glm::vec3(0, 0, 0);
	}
}

SimpleBlock::Direction SimpleBlock::GetDirection(glm::ivec3 vec)
{
	if (vec.x >= 1)
		return EAST;
	if (vec.x <= -1)
		return WEST;
	if (vec.z <= -1)
		return SOUTH;
	if (vec.z >= 1)
		return NORTH;
	if (vec.y >= 1)
		return TOP;
	if (vec.y <= -1)
		return BOTTOM;

	return NORTH;
}

bool SimpleBlock::ProjectRayOnPlaneXZ(float plane_y, float& hit_x, float& hit_z, glm::vec3 origin, glm::vec3 direction)
{
	if (glm::dot(direction, glm::vec3(0, 1, 0)) == 0)
		return false;

	hit_x = direction.x / direction.y * (plane_y - origin.y) + origin.x;
	hit_z = direction.z / direction.y * (plane_y - origin.y) + origin.z;
	return true;
}

bool SimpleBlock::ProjectRayOnPlaneXY(float plane_z, float& hit_x, float& hit_y, glm::vec3 origin, glm::vec3 direction)
{
	if (glm::dot(direction, glm::vec3(0,0,1)) == 0)
		return false;

	hit_x = direction.x / direction.z * (plane_z - origin.z) + origin.x;
	hit_y = direction.y / direction.z * (plane_z - origin.z) + origin.y;
	return true;
}

bool SimpleBlock::ProjectRayOnPlaneYZ(float plane_x, float& hit_y, float& hit_z, glm::vec3 origin, glm::vec3 direction)
{
	if (glm::dot(direction, glm::vec3(1, 0, 0)) == 0)
		return false;

	hit_y = direction.y / direction.x * (plane_x - origin.x) + origin.y;
	hit_z = direction.z / direction.x * (plane_x - origin.x) + origin.z;

	return true;
}

void SimpleBlock::SaveBlockToFile(SimpleBlock* block, std::ofstream& save_file)
{
	save_file << block->id;
}

char* SimpleBlock::SaveBlockToFile(SimpleBlock* block, char* data)
{
	*data = block->id;
	return data + 1;
}


SimpleBlock* SimpleBlock::LoadBlockFromFile(glm::ivec3 position, Chunk* parent_chunk, char*& data)
{
	char block_id = *data;
	data++;
	switch (block_id)
	{
	case blk_id::torch_id:
		return new blk::Torch(position, parent_chunk, data);
	case blk_id::switch_id:
		return new blk::Switch(position, parent_chunk, data);
	case blk_id::door_id:
		return new blk::Door(position, parent_chunk, data);
	default:
		return new SimpleBlock(block_id);
	}
}
