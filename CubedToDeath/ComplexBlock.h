#pragma once
#include "SimpleBlock.h"
#include <iostream>

class ComplexBlock : public SimpleBlock
{
public:
	ComplexBlock() 
	{
		SetFlag(OPAQUE, false);
		SetFlag(COMPLEX, true);
	}
	virtual ~ComplexBlock();
	virtual float* CreateModel(float* target, int world_x, int world_y, int world_z) = 0;
	virtual int GetNumberOfTriangles() = 0;
	//virtual void NeighbourDeleted() = 0;
};

namespace blk
{
	class Torch : public ComplexBlock
	{
	public:
		Direction direction = BOTTOM;

		Torch() { id = blk_id::torch_id; }

		int GetNumberOfTriangles() override
		{
			return 8;
		}


		float* CreateModel(float* target, int world_x, int world_y, int world_z) override
		{
			const int texture_x = 0, texture_y = 5;
			const float width = 1.0f/16.0f;
			const float vertices[] = {
			world_x + 0.0f, world_y + 0.0f, world_z + 0.5f + width, texture_x *m_unit, (texture_y + 1) *m_unit, 0, 0, 1,
			world_x + 1.0f, world_y + 0.0f, world_z + 0.5f + width, (texture_x + 1) *m_unit, (texture_y + 1) *m_unit, 0, 0, 1,
			world_x + 1.0f, world_y + 1.0f, world_z + 0.5f + width, (texture_x + 1) *m_unit, texture_y *m_unit, 0, 0, 1,

			world_x + 0.0f, world_y + 0.0f, world_z + 0.5f + width, texture_x *m_unit, (texture_y + 1) *m_unit, 0, 0, 1,
			world_x + 1.0f, world_y + 1.0f, world_z + 0.5f + width, (texture_x + 1) *m_unit, texture_y *m_unit, 0, 0, 1,
			world_x + 0.0f, world_y + 1.0f, world_z + 0.5f + width, texture_x *m_unit, texture_y *m_unit, 0, 0, 1,

			world_x + 0.0f, world_y + 0.0f, world_z + 0.5f - width, texture_x*m_unit, (texture_y + 1)*m_unit, 0, 0, -1,
			world_x + 1.0f, world_y + 1.0f, world_z + 0.5f - width, (texture_x + 1) *m_unit, texture_y*m_unit, 0, 0, -1,
			world_x + 1.0f, world_y + 0.0f, world_z + 0.5f - width, (texture_x + 1) *m_unit, (texture_y + 1)*m_unit, 0, 0, -1,

			world_x + 0.0f, world_y + 0.0f, world_z + 0.5f - width, texture_x *m_unit, (texture_y + 1)*m_unit, 0, 0, -1,
			world_x + 0.0f, world_y + 1.0f, world_z + 0.5f - width, texture_x *m_unit, texture_y*m_unit, 0, 0, -1,
			world_x + 1.0f, world_y + 1.0f, world_z + 0.5f - width, (texture_x + 1) *m_unit, texture_y*m_unit, 0, 0, -1,

			world_x + 0.5f + width, world_y + 0.0f, world_z + 0.0f, texture_x*m_unit, (texture_y + 1)*m_unit, 1, 0, 0,
			world_x + 0.5f + width, world_y + 1.0f, world_z + 1.0f, (texture_x + 1)*m_unit, texture_y*m_unit, 1, 0, 0,
			world_x + 0.5f + width, world_y + 0.0f, world_z + 1.0f, (texture_x + 1)*m_unit, (texture_y + 1)*m_unit, 1, 0, 0,

			world_x + 0.5f + width, world_y + 0.0f, world_z + 0.0f, texture_x*m_unit, (texture_y + 1)*m_unit, 1, 0, 0,
			world_x + 0.5f + width, world_y + 1.0f, world_z + 0.0f, texture_x*m_unit, texture_y*m_unit, 1, 0, 0,
			world_x + 0.5f + width, world_y + 1.0f, world_z + 1.0f, (texture_x + 1)*m_unit, texture_y*m_unit, 1, 0, 0,

			world_x + 0.5f - width, world_y + 0.0f, world_z + 0.0f, texture_x *m_unit, (texture_y + 1)*m_unit, -1, 0, 0,
			world_x + 0.5f - width, world_y + 0.0f, world_z + 1.0f, (texture_x + 1) *m_unit, (texture_y + 1)*m_unit, -1, 0, 0,
			world_x + 0.5f - width, world_y + 1.0f, 1 + world_z, (texture_x + 1) *m_unit, texture_y*m_unit, -1, 0, 0,

			world_x + 0.5f - width, world_y + 0.0f, world_z + 0.0f, texture_x *m_unit, (texture_y + 1)*m_unit, -1, 0, 0,
			world_x + 0.5f - width, world_y + 1.0f, world_z + 1.0f, (texture_x + 1) *m_unit, texture_y*m_unit, -1, 0, 0,
			world_x + 0.5f - width, world_y + 1.0f, world_z + 0.0f, texture_x *m_unit, texture_y*m_unit, -1, 0, 0,
			};
			std::copy(vertices, vertices + sizeof(vertices) / sizeof(float), target);
			return target + sizeof(vertices) / sizeof(float);
		}
	};
}