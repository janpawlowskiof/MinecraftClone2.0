#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

//klasa bazowa kostki
class Block
{
public:
	//id klocka
	int id = -1;
	//koordynaty w œwiecie
	int world_x = 0, world_y = 0, world_z = 0;
	//pozycja textur na boku
	int side_x = 0, side_y = 0;
	//pozycja textur na górze
	int top_x = 0, top_y = 0;
	//pozycja textur na dole
	int bottom_x = 0, bottom_y = 0;
	//czy klicek jest nieprzejrzysty
	bool opaque = true;
	//czy klocek jest szescianem
	bool cube = true;
	//które œcianki s¹ widoczne
	bool face_visible[6] = { true,true,true,true,true,true };

	Block() {};
	Block(int world_x, int world_y, int world_z)
	{
		this->world_x = world_x;
		this->world_y = world_y;
		this->world_z = world_z;

		//przezroczyste klocki sa zawsze widoczne
		if (!opaque) {
			face_visible[Block::NORTH] = true;
			face_visible[Block::SOUTH] = true;
			face_visible[Block::WEST] = true;
			face_visible[Block::EAST] = true;
			face_visible[Block::TOP] = true;
			face_visible[Block::BOTTOM] = true;
		}
	}
	//zwraca liczbê trójk¹tów które w obecnym stanie bêdzie rysowa³ klocek;
	virtual int GetNumberOfTriangles();
	//		WA¯NE		//
	//wpisuje swoje widoczne vertices w miejsce gdzie wskazuje target i zwraca pierwszy adres za wpisanymi danymi
	virtual float* CreateModel(float* target);
	//wnum kierunków œwiata
	enum Direction
	{
		//	z+
		NORTH,
		//	z-
		SOUTH,
		//	x+
		EAST,
		//	x-
		WEST,
		//	y+
		TOP,
		//	y-
		BOTTOM
	};
private:
};


//					definicje bloków				//
namespace blk
{
	class Air : public Block
	{
	public:
		Air(int world_x, int world_y, int world_z) : Block(world_x, world_y, world_z) { id = 0; opaque = false; }
		int GetNumberOfTriangles() override { return 0; }
		float* CreateModel(float* target) override { return target; }
	private:
	};

	class Stone : public Block
	{
	public:
		Stone(int world_x, int world_y, int world_z) : Block(world_x, world_y, world_z)
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
		Grass(int world_x, int world_y, int world_z) : Block(world_x, world_y, world_z)
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
		Dirt(int world_x, int world_y, int world_z) : Block(world_x, world_y, world_z)
		{
			id = 3;
			side_x = 2; side_y = 0;
			top_x = 2; top_y = 0;
			bottom_x = 2; bottom_y = 0;
		}
	}; 
};