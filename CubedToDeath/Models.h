#pragma once
#include <vector>
#include "Vertex.h"

static void CreateNorthFace(std::vector<Vertex>& vertices, float world_x, float world_y, float world_z, TextureInfo texture_info, TextureInfo overlay_info = TextureInfo() = TextureInfo(), glm::vec3 color01 = glm::vec3(-1), glm::vec3 color11 = glm::vec3(-1))
{
	const glm::vec3 normal(0, 0, 1);
	const glm::vec3 tangent(1, 0, 0);
	vertices.push_back(Vertex(glm::vec3(world_x + 0.0f, world_y + 0.0f, world_z), glm::vec2(0 * m_unit, (0 + 1) * m_unit), normal, tangent, texture_info, overlay_info, color01));
	vertices.push_back(Vertex(glm::vec3(world_x + 1, world_y + 0.0f, world_z), glm::vec2((0 + 1) * m_unit, (0 + 1) * m_unit), normal, tangent, texture_info, overlay_info, color11));
	vertices.push_back(Vertex(glm::vec3(world_x + 1, world_y + 1.0f, world_z), glm::vec2((0 + 1) * m_unit, 0 * m_unit), normal, tangent, texture_info, overlay_info, color11));
	vertices.push_back(Vertex(glm::vec3(world_x + 0.0f, world_y + 0.0f, world_z), glm::vec2(0 * m_unit, (0 + 1) * m_unit), normal, tangent, texture_info, overlay_info, color01));
	vertices.push_back(Vertex(glm::vec3(world_x + 1, world_y + 1.0f, world_z), glm::vec2((0 + 1) * m_unit, 0 * m_unit), normal, tangent, texture_info, overlay_info, color11));
	vertices.push_back(Vertex(glm::vec3(world_x + 0.0f, world_y + 1.0f, world_z), glm::vec2(0 * m_unit, 0 * m_unit), normal, tangent, texture_info, overlay_info, color01));
}

static void CreateTopFace(std::vector<Vertex>& vertices, float world_x, float world_y, float world_z, TextureInfo texture_info, TextureInfo overlay_info = TextureInfo(), glm::vec3 color00 = glm::vec3(-1), glm::vec3 color01 = glm::vec3(-1), glm::vec3 color10 = glm::vec3(-1), glm::vec3 color11 = glm::vec3(-1))
{
	const glm::vec3 normal(0, 1, 0);
	const glm::vec3 tangent(0, 0, 1);
	vertices.push_back(Vertex(glm::vec3(world_x + 0.0f, world_y, world_z + 0.0f), glm::vec2(0 * m_unit, (0) * m_unit), normal, tangent, texture_info, overlay_info, color00));
	vertices.push_back(Vertex(glm::vec3(world_x + 0.0f, world_y, world_z + 1), glm::vec2((0) * m_unit, (0 + 1) * m_unit), normal, tangent, texture_info, overlay_info, color01));
	vertices.push_back(Vertex(glm::vec3(world_x + 1, world_y, world_z + 1), glm::vec2((0 + 1) * m_unit, (0 + 1) * m_unit), normal, tangent, texture_info, overlay_info, color11));
	vertices.push_back(Vertex(glm::vec3(world_x + 0.0f, world_y, world_z + 0.0f), glm::vec2(0 * m_unit, (0) * m_unit), normal, tangent, texture_info, overlay_info, color00));
	vertices.push_back(Vertex(glm::vec3(world_x + 1, world_y, world_z + 1), glm::vec2((0 + 1) * m_unit, (0 + 1) * m_unit), normal, tangent, texture_info, overlay_info, color11));
	vertices.push_back(Vertex(glm::vec3(world_x + 1, world_y, world_z + 0.0f), glm::vec2((0 + 1) * m_unit, 0 * m_unit), normal, tangent, texture_info, overlay_info, color10));
}

static void CreateBottomFace(std::vector<Vertex>& vertices, float world_x, float world_y, float world_z, TextureInfo texture_info, TextureInfo overlay_info = TextureInfo(), glm::vec3 color00 = glm::vec3(-1), glm::vec3 color01 = glm::vec3(-1), glm::vec3 color10 = glm::vec3(-1), glm::vec3 color11 = glm::vec3(-1))
{
	const glm::vec3 normal(0, -1, 0);
	const glm::vec3 tangent(0, 0, -1);
	vertices.push_back(Vertex(glm::vec3(world_x + 0.0f, world_y + 0.0f, world_z + 0.0f), glm::vec2(0 * m_unit, (0) * m_unit), normal, tangent, texture_info, overlay_info, color00));
	vertices.push_back(Vertex(glm::vec3(world_x + 1, world_y + 0.0f, world_z + 1), glm::vec2((0 + 1) * m_unit, (0 + 1) * m_unit), normal, tangent, texture_info, overlay_info, color11));
	vertices.push_back(Vertex(glm::vec3(world_x + 0.0f, world_y + 0.0f, world_z + 1), glm::vec2((0) * m_unit, (0 + 1) * m_unit), normal, tangent, texture_info, overlay_info, color01));
	vertices.push_back(Vertex(glm::vec3(world_x + 0.0f, world_y + 0.0f, world_z + 0.0f), glm::vec2(0 * m_unit, (0) * m_unit), normal, tangent, texture_info, overlay_info, color00));
	vertices.push_back(Vertex(glm::vec3(world_x + 1, world_y + 0.0f, world_z + 0.0f), glm::vec2((0 + 1) * m_unit, 0 * m_unit), normal, tangent, texture_info, overlay_info, color10));
	vertices.push_back(Vertex(glm::vec3(world_x + 1, world_y + 0.0f, world_z + 1), glm::vec2((0 + 1) * m_unit, (0 + 1) * m_unit), normal, tangent, texture_info, overlay_info, color11));
}

static void CreateSouthFace(std::vector<Vertex>& vertices, float world_x, float world_y, float world_z, TextureInfo texture_info, TextureInfo overlay_info = TextureInfo(), glm::vec3 color00 = glm::vec3(-1), glm::vec3 color10 = glm::vec3(-1))
{
	const glm::vec3 normal(0, 0, -1);
	const glm::vec3 tangent(-1, 0, 0);
	vertices.push_back(Vertex(glm::vec3(world_x + 0.0f, world_y + 0.0f, world_z + 0.0f), glm::vec2(0 * m_unit, (0 + 1) * m_unit), normal, tangent, texture_info, overlay_info, color00));
	vertices.push_back(Vertex(glm::vec3(world_x + 1, world_y + 1.0f, world_z + 0.0f), glm::vec2((0 + 1) * m_unit, 0 * m_unit), normal, tangent, texture_info, overlay_info, color10));
	vertices.push_back(Vertex(glm::vec3(world_x + 1, world_y + 0.0f, world_z + 0.0f), glm::vec2((0 + 1) * m_unit, (0 + 1) * m_unit), normal, tangent, texture_info, overlay_info, color10));

	vertices.push_back(Vertex(glm::vec3(world_x + 0.0f, world_y + 0.0f, world_z + 0.0f), glm::vec2(0 * m_unit, (0 + 1) * m_unit), normal, tangent, texture_info, overlay_info, color00));
	vertices.push_back(Vertex(glm::vec3(world_x + 0.0f, world_y + 1.0f, world_z + 0.0f), glm::vec2(0 * m_unit, 0 * m_unit), normal, tangent, texture_info, overlay_info, color00));
	vertices.push_back(Vertex(glm::vec3(world_x + 1, world_y + 1.0f, world_z + 0.0f), glm::vec2((0 + 1) * m_unit, 0 * m_unit), normal, tangent, texture_info, overlay_info, color10));
}

static void CreateWestFace(std::vector<Vertex>& vertices, float world_x, float world_y, float world_z, TextureInfo texture_info, TextureInfo overlay_info = TextureInfo(), glm::vec3 color10 = glm::vec3(-1), glm::vec3 color11 = glm::vec3(-1))
{
	const glm::vec3 normal(1, 0, 0);
	const glm::vec3 tangent(0, -1, 0);
	vertices.push_back(Vertex(glm::vec3(world_x, world_y + 0.0f, world_z + 0.0f), glm::vec2(0 * m_unit, (0 + 1) * m_unit), normal, tangent, texture_info, overlay_info, color10));
	vertices.push_back(Vertex(glm::vec3(world_x, world_y + 1.0f, world_z + 1), glm::vec2((0 + 1) * m_unit, 0 * m_unit), normal, tangent, texture_info, overlay_info, color11));
	vertices.push_back(Vertex(glm::vec3(world_x, world_y + 0.0f, world_z + 1), glm::vec2((0 + 1) * m_unit, (0 + 1) * m_unit), normal, tangent, texture_info, overlay_info, color11));

	vertices.push_back(Vertex(glm::vec3(world_x, world_y + 0.0f, world_z + 0.0f), glm::vec2(0 * m_unit, (0 + 1) * m_unit), normal, tangent, texture_info, overlay_info, color10));
	vertices.push_back(Vertex(glm::vec3(world_x, world_y + 1.0f, world_z + 0.0f), glm::vec2(0 * m_unit, 0 * m_unit), normal, tangent, texture_info, overlay_info, color10));
	vertices.push_back(Vertex(glm::vec3(world_x, world_y + 1.0f, world_z + 1), glm::vec2((0 + 1) * m_unit, 0 * m_unit), normal, tangent, texture_info, overlay_info, color11));
}

static void CreateEastFace(std::vector<Vertex>& vertices, float world_x, float world_y, float world_z, TextureInfo texture_info, TextureInfo overlay_info = TextureInfo(), glm::vec3 color10 = glm::vec3(-1), glm::vec3 color11 = glm::vec3(-1))
{
	const glm::vec3 normal(-1, 0, 0);
	const glm::vec3 tangent(0, 1, 0);
	vertices.push_back(Vertex(glm::vec3(world_x + 0.0, world_y + 0.0f, world_z + 0.0f), glm::vec2(1 * m_unit, (0 + 1) * m_unit), normal, tangent, texture_info, overlay_info, color10));
	vertices.push_back(Vertex(glm::vec3(world_x + 0.0, world_y + 0.0f, world_z + 1), glm::vec2((0) * m_unit, (0 + 1) * m_unit), normal, tangent, texture_info, overlay_info, color11));
	vertices.push_back(Vertex(glm::vec3(world_x + 0.0, world_y + 1.0f, world_z + 1), glm::vec2((0) * m_unit, 0 * m_unit), normal, tangent, texture_info, overlay_info, color11));

	vertices.push_back(Vertex(glm::vec3(world_x + 0.0, world_y + 0.0f, world_z + 0.0f), glm::vec2(1 * m_unit, (0 + 1) * m_unit), normal, tangent, texture_info, overlay_info, color10));
	vertices.push_back(Vertex(glm::vec3(world_x + 0.0, world_y + 1.0f, world_z + 1), glm::vec2((0) * m_unit, 0 * m_unit), normal, tangent, texture_info, overlay_info, color11));
	vertices.push_back(Vertex(glm::vec3(world_x + 0.0, world_y + 1.0f, world_z + 0.0f), glm::vec2(1 * m_unit, 0 * m_unit), normal, tangent, texture_info, overlay_info, color10));
}