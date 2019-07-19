#include "SimpleBlock.h"
#include <iostream>

SimpleBlock::SimpleBlock(unsigned char id)
{
	this->id = id;
	if (id == blk_id::air_id || id == 255)
		SetFlag(OPAQUE, false);
	else
		SetFlag(OPAQUE, true);
}


SimpleBlock::~SimpleBlock()
{
}

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

float * SimpleBlock::CreateModel(float* target, int world_x, int world_y, int world_z)
{
	if (face_visible == 0)
		return target;

	int side_x = 0, side_y = 0, top_x = 0, top_y = 0, bottom_x = 0, bottom_y = 0;
	switch (id)
	{
	case blk_id::dirt_id:
		side_x = top_x = bottom_x = 2;
		side_y = top_y = bottom_y = 0;
		break;
	case blk_id::stone_id:
		side_x = top_x = bottom_x = 1;
		side_y = top_y = bottom_y = 0;
		break;
	case blk_id::air_id:
	default:
		return target;
	}
	if (GetFaceVisible(NORTH))
	{
		target = CreateNorthFace(target, world_x, world_y, world_z, side_x, side_y);
	}
	if (GetFaceVisible(SOUTH))
	{
		target = CreateSouthFace(target, world_x, world_y, world_z, side_x, side_y);
	}
	if (GetFaceVisible(WEST))
	{
		target = CreateWestFace(target, world_x, world_y, world_z, side_x, side_y);
	}
	if (GetFaceVisible(EAST))
	{
		target = CreateEastFace(target, world_x, world_y, world_z, side_x, side_y);
	}
	if (GetFaceVisible(TOP))
	{
		target = CreateTopFace(target, world_x, world_y, world_z, top_x, top_y);
	}
	if (GetFaceVisible(BOTTOM))
	{
		target = CreateBottomFace(target, world_x, world_y, world_z, bottom_x, bottom_y);
	}
	return target;
}
