#include "Block.h"
#include <iostream>

//bo textura ma 16x16 bloków
#define m_unit (1.0f/16.0f)

//											vertex format										//
//	world_x, world_y, world_z,		texture_x, texture_y,		normal_x, normal_y, normal_z	//

//wpisuje swoje widoczne vertices w miejsce gdzie wskazuje target i zwraca pierwszy adres za wpisanymi danymi
static float* CreateNorthFace(float* target, int world_x, int world_y, int world_z, int texture_x, int texture_y)
{
	float vertices[] = {
			world_x + 0.0f, world_y + 0.0f, world_z + 1.0f, texture_x *m_unit, (texture_y + 1) *m_unit, 0, 0, 1,
			world_x + 1.0f, world_y + 0.0f, world_z + 1.0f, (texture_x + 1) *m_unit, (texture_y + 1) *m_unit, 0, 0, 1,
			world_x + 1.0f, world_y + 1.0f, world_z + 1.0f, (texture_x + 1) *m_unit, texture_y *m_unit, 0, 0, 1,

			world_x + 0.0f, world_y + 0.0f, world_z + 1.0f, texture_x *m_unit, (texture_y + 1) *m_unit, 0, 0, 1,
			world_x + 1.0f, world_y + 1.0f, world_z + 1.0f, (texture_x + 1) *m_unit, texture_y *m_unit, 0, 0, 1,
			world_x + 0.0f, world_y + 1.0f, world_z + 1.0f, texture_x *m_unit, texture_y *m_unit, 0, 0, 1 };
	std::copy(vertices, vertices + sizeof(vertices) / sizeof(float), target);
	return target + sizeof(vertices) / sizeof(float);
}
//wpisuje swoje widoczne vertices w miejsce gdzie wskazuje target i zwraca pierwszy adres za wpisanymi danymi
static float* CreateSouthFace(float* target, int world_x, int world_y, int world_z, int texture_x, int texture_y)
{
	float vertices[] = {
		world_x + 0.0f, world_y + 0.0f, world_z + 0.0f, texture_x*m_unit, (texture_y + 1)*m_unit, 0, 0, -1,
		world_x + 1.0f, world_y + 1.0f, world_z + 0.0f, (texture_x + 1) *m_unit, texture_y*m_unit, 0, 0, -1,
		world_x + 1.0f, world_y + 0.0f, world_z + 0.0f, (texture_x + 1) *m_unit, (texture_y + 1)*m_unit, 0, 0, -1,

		world_x + 0.0f, world_y + 0.0f, world_z + 0.0f, texture_x *m_unit, (texture_y + 1)*m_unit, 0, 0, -1,
		world_x + 0.0f, world_y + 1.0f, world_z + 0.0f, texture_x *m_unit, texture_y*m_unit, 0, 0, -1,
		world_x + 1.0f, world_y + 1.0f, world_z + 0.0f, (texture_x + 1) *m_unit, texture_y*m_unit, 0, 0, -1,
	};
	std::copy(vertices, vertices + sizeof(vertices) / sizeof(float), target);
	return target + sizeof(vertices) / sizeof(float);
}
//wpisuje swoje widoczne vertices w miejsce gdzie wskazuje target i zwraca pierwszy adres za wpisanymi danymi
static float* CreateTopFace(float* target, int world_x, int world_y, int world_z, int texture_x, int texture_y)
{
	float vertices[] = {
		world_x + 0.0f, world_y + 1.0f, world_z + 0.0f, texture_x*m_unit, (texture_y)*m_unit, 0, 1, 0,
			world_x + 0.0f, world_y + 1.0f, world_z + 1.0f, (texture_x)*m_unit, (texture_y + 1)*m_unit, 0, 1, 0,
			world_x + 1.0f, world_y + 1.0f, world_z + 1.0f, (texture_x + 1)*m_unit, (texture_y + 1)*m_unit, 0, 1, 0,

			world_x + 0.0f, world_y + 1.0f, world_z + 0.0f, texture_x*m_unit, (texture_y)*m_unit, 0, 1, 0,
			world_x + 1.0f, world_y + 1.0f, world_z + 1.0f, (texture_x + 1)*m_unit, (texture_y + 1)*m_unit, 0, 1, 0,
			world_x + 1.0f, world_y + 1.0f, world_z + 0.0f, (texture_x + 1)*m_unit, texture_y*m_unit, 0, 1, 0,
	};
	std::copy(vertices, vertices + sizeof(vertices) / sizeof(float), target);
	return target + sizeof(vertices) / sizeof(float);
}
//wpisuje swoje widoczne vertices w miejsce gdzie wskazuje target i zwraca pierwszy adres za wpisanymi danymi
static float* CreateBottomFace(float* target, int world_x, int world_y, int world_z, int texture_x, int texture_y)
{
	float vertices[] = {
		world_x + 0.0f, world_y + 0.0f, world_z + 0.0f, texture_x*m_unit, (texture_y)*m_unit, 0, -1, 0,
			world_x + 1.0f, world_y + 0.0f, world_z + 1.0f, (texture_x + 1)*m_unit, (texture_y + 1)*m_unit, 0, -1, 0,
			world_x + 0.0f, world_y + 0.0f, world_z + 1.0f, (texture_x)*m_unit, (texture_y + 1)*m_unit, 0, -1, 0,

			world_x + 0.0f, world_y + 0.0f, world_z + 0.0f, texture_x*m_unit, (texture_y)*m_unit, 0, 1, 0,
			world_x + 1.0f, world_y + 0.0f, world_z + 0.0f, (texture_x + 1)*m_unit, texture_y*m_unit, 0, -1, 0,
			world_x + 1.0f, world_y + 0.0f, world_z + 1.0f, (texture_x + 1) *m_unit, (texture_y + 1)*m_unit, 0, -1, 0,
	};
	std::copy(vertices, vertices + sizeof(vertices) / sizeof(float), target);
	return target + sizeof(vertices) / sizeof(float);
}
//wpisuje swoje widoczne vertices w miejsce gdzie wskazuje target i zwraca pierwszy adres za wpisanymi danymi
static float* CreateWestFace(float* target, int world_x, int world_y, int world_z, int texture_x, int texture_y)
{
	float vertices[] = {
		world_x + 1.0f, world_y + 0.0f, world_z + 0.0f, texture_x*m_unit, (texture_y + 1)*m_unit, 1, 0, 0,
			world_x + 1.0f, world_y + 1.0f, world_z + 1.0f, (texture_x + 1)*m_unit, texture_y*m_unit, 1, 0, 0,
			world_x + 1.0f, world_y + 0.0f, world_z + 1.0f, (texture_x + 1)*m_unit, (texture_y + 1)*m_unit, 1, 0, 0,

			world_x + 1.0f, world_y + 0.0f, world_z + 0.0f, texture_x*m_unit, (texture_y + 1)*m_unit, 1, 0, 0,
			world_x + 1.0f, world_y + 1.0f, world_z + 0.0f, texture_x*m_unit, texture_y*m_unit, 1, 0, 0,
			world_x + 1.0f, world_y + 1.0f, world_z + 1.0f, (texture_x + 1)*m_unit, texture_y*m_unit, 1, 0, 0,
	};
	std::copy(vertices, vertices + sizeof(vertices) / sizeof(float), target);
	return target + sizeof(vertices) / sizeof(float);
}
//wpisuje swoje widoczne vertices w miejsce gdzie wskazuje target i zwraca pierwszy adres za wpisanymi danymi
static float* CreateEastFace(float* target, int world_x, int world_y, int world_z, int texture_x, int texture_y)
{
	float vertices[] = {
		world_x + 0.0f, world_y + 0.0f, world_z + 0.0f, texture_x *m_unit, (texture_y + 1)*m_unit, -1, 0, 0,
			world_x + 0.0f, world_y + 0.0f, world_z + 1.0f, (texture_x + 1) *m_unit, (texture_y + 1)*m_unit, -1, 0, 0,
			world_x + 0.0f, world_y + 1.0f, 1 + world_z, (texture_x + 1) *m_unit, texture_y*m_unit, -1, 0, 0,

			world_x + 0.0f, world_y + 0.0f, world_z + 0.0f, texture_x *m_unit, (texture_y + 1)*m_unit, -1, 0, 0,
			world_x + 0.0f, world_y + 1.0f, world_z + 1.0f, (texture_x + 1) *m_unit, texture_y*m_unit, -1, 0, 0,
			world_x + 0.0f, world_y + 1.0f, world_z + 0.0f, texture_x *m_unit, texture_y*m_unit, -1, 0, 0,
	};
	std::copy(vertices, vertices + sizeof(vertices) / sizeof(float), target);
	return target + sizeof(vertices) / sizeof(float);
}

float* Block::CreateModel(float* target)
{
	if (face_visible[NORTH])
	{
		target = CreateNorthFace(target, world_x, world_y, world_z, side_x, side_y);
	}
	if (face_visible[SOUTH])
	{
		target = CreateSouthFace(target, world_x, world_y, world_z, side_x, side_y);
	}
	if (face_visible[WEST])
	{
		target = CreateWestFace(target, world_x, world_y, world_z, side_x, side_y);
	}
	if (face_visible[EAST])
	{
		target = CreateEastFace(target, world_x, world_y, world_z, side_x, side_y);
	}
	if (face_visible[TOP])
	{
		target = CreateTopFace(target, world_x, world_y, world_z, side_x, side_y);
	}
	if (face_visible[BOTTOM])
	{
		target = CreateBottomFace(target, world_x, world_y, world_z, side_x, side_y);
	}
	return target;
}

int Block::GetNumberOfTriangles()
{
	//ka¿da œciana liczona 2 razy bo 2 trójk¹ty na œcianê
	int result = 0;
	for (int i = 0; i < 6; i++)
		result += face_visible[i] * 2;
	return result;
}

void Block::RecalculateVisibility()
{

}

