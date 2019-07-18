#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Block
{
public:
	int id = 0;
	int world_x = 0, world_y = 0, world_z = 0;
	int side_x = 0, side_y = 0;
	int top_x = 0, top_y = 0;
	int bottom_x = 0, bottom_y = 0;
	bool opaque = false;

	bool face_visible[6] = { true,true,true,true,true,true };
	//bitset<6> face_visible;

	virtual std::vector<float> GetModel()
	{
		return GetCubeModel(face_visible, world_x, world_y, world_z, side_x, side_y, top_x, top_y, bottom_x, bottom_y);
	}
	virtual void RecalculateVisibility();

	enum Direction
	{
		NORTH,
		SOUTH,
		EAST,
		WEST,
		TOP,
		BOTTOM
	};

	#define u (1.0f/16.0f)
	static std::vector<float> GetCubeModel(bool face_visible[6], int world_x, int world_y, int world_z, int side_x, int side_y, int top_x, int top_y, int bottom_x, int bottom_y)
	{
		std::vector<float> model;
		std::vector<float> face;
		if (face_visible[NORTH])
		{
			face = GetNorthFace(world_x, world_y, world_z, side_x, side_y);
			model.insert(model.end(), face.data(), face.data() + 48);
		}
		if (face_visible[SOUTH])
		{
			face = GetSouthFace(world_x, world_y, world_z, side_x, side_y);
			model.insert(model.end(), face.data(), face.data() + 48);
		}
		if (face_visible[WEST])
		{
			face = GetWestFace(world_x, world_y, world_z, side_x, side_y);
			model.insert(model.end(), face.data(), face.data() + 48);
		}
		if (face_visible[EAST])
		{
			face = GetEastFace(world_x, world_y, world_z, side_x, side_y);
			model.insert(model.end(), face.data(), face.data() + 48);
		}
		if (face_visible[TOP])
		{
			face = GetTopFace(world_x, world_y, world_z, top_x, top_y);
			model.insert(model.end(), face.data(), face.data() + 48);
		}
		if (face_visible[BOTTOM])
		{
			face = GetBottomFace(world_x, world_y, world_z, bottom_x, bottom_y);
			model.insert(model.end(), face.data(), face.data() + 48);
		}
		return model;
	}

	static std::vector<float> GetNorthFace(int world_x, int world_y, int world_z, int texture_x, int texture_y)
	{
		return std::vector<float>{
				world_x + 0.0f, world_y + 0.0f, world_z + 1.0f, texture_x* u, (texture_y + 1)* u, 0, 0, 1,
				world_x + 1.0f, world_y + 0.0f, world_z + 1.0f, (texture_x + 1)* u, (texture_y + 1)* u, 0, 0, 1,
				world_x + 1.0f, world_y + 1.0f, world_z + 1.0f, (texture_x + 1)* u, texture_y* u, 0, 0, 1,

				world_x + 0.0f, world_y + 0.0f, world_z + 1.0f, texture_x* u, (texture_y + 1)* u, 0, 0, 1,
				world_x + 1.0f, world_y + 1.0f, world_z + 1.0f, (texture_x + 1)* u, texture_y* u, 0, 0, 1,
				world_x + 0.0f, world_y + 1.0f, world_z + 1.0f, texture_x* u, texture_y* u, 0, 0, 1
		};
	}

	static std::vector<float> GetSouthFace(int world_x, int world_y, int world_z, int texture_x, int texture_y)
	{
		return std::vector<float>{
			world_x + 0.0f, world_y + 0.0f, world_z + 0.0f, texture_x* u, (texture_y + 1)* u, 0, 0, -1,
				world_x + 1.0f, world_y + 1.0f, world_z + 0.0f, (texture_x + 1) * u, texture_y* u, 0, 0, -1,
				world_x + 1.0f, world_y + 0.0f, world_z + 0.0f, (texture_x + 1) * u, (texture_y + 1)* u, 0, 0, -1,

				world_x + 0.0f, world_y + 0.0f, world_z + 0.0f, texture_x * u, (texture_y + 1)* u, 0, 0, -1,
				world_x + 0.0f, world_y + 1.0f, world_z + 0.0f, texture_x * u, texture_y* u, 0, 0, -1,
				world_x + 1.0f, world_y + 1.0f, world_z + 0.0f, (texture_x + 1) * u, texture_y* u, 0, 0, -1,
		};
	}

	static std::vector<float> GetTopFace(int world_x, int world_y, int world_z, int texture_x, int texture_y)
	{
		return std::vector<float>{
			world_x + 0.0f, 1.0f, world_z + 0.0f, texture_x* u, (texture_y)* u, 0, 1, 0,
				world_x + 0.0f, world_y + 1.0f, world_z + 1.0f, (texture_x)* u, (texture_y+1)* u, 0, 1, 0,
				world_x + 1.0f, world_y + 1.0f, world_z + 1.0f, (texture_x + 1)* u, (texture_y + 1)* u, 0, 1, 0,

				world_x + 0.0f, world_y + 1.0f, world_z + 0.0f, texture_x* u, (texture_y)* u, 0, 1, 0,
				world_x + 1.0f, world_y + 1.0f, world_z + 1.0f, (texture_x+1)* u, (texture_y+1)* u, 0, 1, 0,
				world_x + 1.0f, world_y + 1.0f, world_z + 0.0f, (texture_x + 1)* u, texture_y* u, 0, 1, 0,
		};
	}

	static std::vector<float> GetBottomFace(int world_x, int world_y, int world_z, int texture_x, int texture_y)
	{
		return std::vector<float>{
			world_x + 0.0f, world_y + 0.0f, world_z + 0.0f, texture_x* u, (texture_y)* u, 0, -1, 0,
				world_x + 1.0f, world_y + 0.0f, world_z + 1.0f, (texture_x + 1)* u, (texture_y + 1)* u, 0, -1, 0,
				world_x + 0.0f, world_y + 0.0f, world_z + 1.0f, (texture_x)* u, (texture_y + 1)* u, 0, -1, 0,

				world_x + 0.0f, world_y + 0.0f, world_z + 0.0f, texture_x* u, (texture_y)* u, 0, 1, 0,
				world_x + 1.0f, world_y + 0.0f, world_z + 0.0f, (texture_x + 1)* u, texture_y* u, 0, -1, 0,
				world_x + 1.0f, world_y + 0.0f, world_z + 1.0f, (texture_x + 1) * u, (texture_y + 1)* u, 0, -1, 0,
		};
	}

	static std::vector<float> GetWestFace(int world_x, int world_y, int world_z, int texture_x, int texture_y)
	{
		return std::vector<float>{
			world_x + 1.0f, world_y + 0.0f, world_z + 0.0f, texture_x* u, (texture_y + 1)* u, 1, 0, 0,
				world_x + 1.0f, world_y + 1.0f, world_z + 1.0f, (texture_x + 1)* u, texture_y* u, 1, 0, 0,
				world_x + 1.0f, world_y + 0.0f, world_z + 1.0f, (texture_x + 1)* u, (texture_y + 1)* u, 1, 0, 0,

				world_x + 1.0f, world_y + 0.0f, world_z + 0.0f, texture_x* u, (texture_y + 1)* u, 1, 0, 0,
				world_x + 1.0f, world_y + 1.0f, world_z + 0.0f, texture_x* u, texture_y* u, 1, 0, 0,
				world_x + 1.0f, world_y + 1.0f, world_z + 1.0f, (texture_x + 1)* u, texture_y* u, 1, 0, 0,
		};
	}

	static std::vector<float> GetEastFace(int world_x, int world_y, int world_z, int texture_x, int texture_y)
	{
		return std::vector<float>{
			world_x + 0.0f, world_y + 0.0f, world_z + 0.0f, texture_x * u, (texture_y + 1)* u, -1, 0, 0,
				world_x + 0.0f, world_y + 0.0f, world_z + 1.0f, (texture_x + 1) * u, (texture_y + 1)* u, -1, 0, 0,
				world_x + 0.0f, world_y + 1.0f, 1, (texture_x + 1) * u, texture_y* u, -1, 0, 0,

				world_x + 0.0f, world_y + 0.0f, world_z + 0.0f, texture_x * u, (texture_y + 1)* u, -1, 0, 0,
				world_x + 0.0f, world_y + 1.0f, world_z + 1.0f, (texture_x + 1) * u, texture_y* u, -1, 0, 0,
				world_x + 0.0f, world_y + 1.0f, world_z + 0.0f, texture_x * u, texture_y* u, -1, 0, 0,
		};
	}

private:

};

namespace blocks
{
	class Air : public Block
	{
	public:
		Air(){ id = 0; }
		std::vector<float> GetModel() override
		{
			return std::vector<float>();
		}
	private:
	};

	class Stone : public Block
	{
	public:
		Stone()
		{
			id = 1;
			top_x = 1; top_y = 0;
			side_x = 1; side_y = 0;
			bottom_x = 1; bottom_y = 0;
		}
	};

	class Grass : public Block
	{
	public:
		Grass()
		{
			id = 2;
			top_x = 8; top_y = 2;
			side_x = 3; side_y = 0;
			bottom_x = 2; bottom_y = 0;
		}
	};

	class Dirt : public Block
	{
	public:
		Dirt() 
		{
			id = 3;
			side_x = 2; side_y = 0;
			top_x = 2; top_y = 0;
			bottom_x = 2; bottom_y = 0;
		}
	}; 
};