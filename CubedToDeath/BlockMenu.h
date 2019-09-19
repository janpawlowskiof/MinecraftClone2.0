#pragma once
#include <map>
#include <string>
#include "Sprite.h"
#include "SimpleBlock.h"
#include <glm/glm.hpp>
#include "MyCraft.h"

struct BlockMenuItem
{
	BlockMenuItem(unsigned char id, std::string path, glm::vec2 position)
	{
		this->id = id;
		this->position = position;
		sprite = new Sprite(path);
	}
	~BlockMenuItem()
	{
		delete sprite;
	}
	unsigned char id;
	Sprite* sprite;
	glm::vec2 position;
};

class BlockMenu
{
public:
	BlockMenu()
	{

	}
	void Initialize()
	{
		background = new Sprite("res/background_bar.png");

		AddItem(blk_id::dirt_id, "res/block/dirt.png");
		AddItem(blk_id::stone_id, "res/block/stone.png");
		AddItem(blk_id::cobblestone_id, "res/block/cobblestone.png");
		AddItem(blk_id::grass_id, "res/block/grass_block_side.png");
		AddItem(blk_id::gold_ore_id, "res/block/gold_ore.png");
		AddItem(blk_id::iron_block_id, "res/block/iron_block.png");
		AddItem(blk_id::planks_id, "res/block/birch_planks.png");
		AddItem(blk_id::wood_id, "res/block/birch_log.png");
		AddItem(blk_id::leaves_id, "res/block/birch_leaves.png");
		AddItem(blk_id::redstone_block_id, "res/block/redstone_block.png");
		AddItem(blk_id::redstone_torch_id, "res/block/redstone_torch.png");
		AddItem(blk_id::redstone_id, "res/block/redstone_dust_line1.png");
	}
	void AddItem(unsigned char id, std::string path)
	{
		if (items_in_row >= 6)
		{
			items_in_row = 0;
			rows++;
		}
		glm::vec2 item_position = glm::vec2(2 * item_size * items_in_row, 2 * item_size * rows);
		block_sprite_map.emplace(id, new BlockMenuItem(id, path, item_position));

		items_in_row++;
	}
	void Render(float aspect_ratio, unsigned char selected_item)
	{
		background->Draw(aspect_ratio / 2.0f - TOTAL_WIDTH() / 2.0f - item_size, 0.5f - TOTAL_HEIGHT()/2.0f, TOTAL_WIDTH() + 2 * item_size, TOTAL_HEIGHT() + 2 * item_size);

		for (auto iterator : block_sprite_map)
		{
			auto item = iterator.second;
			if(item->id == selected_item)
				item->sprite->Draw(item->position.x + aspect_ratio / 2.0f - TOTAL_WIDTH() / 2.0f - 0.25f * item_size, -item->position.y + 0.5f + TOTAL_HEIGHT()/2.0f - 0.25f * item_size, 1.5f*item_size, 1.5f*item_size);
			else
				item->sprite->Draw(item->position.x + aspect_ratio / 2.0f - TOTAL_WIDTH() / 2.0f, -item->position.y + 0.5f + TOTAL_HEIGHT() / 2.0f, item_size, item_size);
		}
	}
	int SelectItem(glm::vec2 cursor_position, float aspect_ratio)
	{
		std::cout << "mx " << cursor_position.x << "   my " << cursor_position.y << "\n";
		for (auto iterator : block_sprite_map)
		{
			auto item = iterator.second;
			const float x = item->position.x + aspect_ratio / 2.0f - TOTAL_WIDTH() / 2.0f;
			const float y = -item->position.y + 1.0f / 2.0f + TOTAL_HEIGHT() / 2.0f;
			std::cout << "y " << y << "\n";

			if (cursor_position.x >= x && cursor_position.y >= y &&
				cursor_position.x <= x + item_size && cursor_position.y <= y + item_size)
			{
			std::cout << "Selected " << (int)item->id << "\n";
			return item->id;
			}
		}
		return -1;
	}

	~BlockMenu()
	{
		for (auto iterator : block_sprite_map)
			delete iterator.second;
		delete background;
	}

private:
	std::map<unsigned char, BlockMenuItem*> block_sprite_map;
	Sprite* background;
	float item_size = 1.0/16;
	int items_in_row = 0;
	int rows = 0;
	constexpr float TOTAL_WIDTH() { return  11*item_size; };
	constexpr float TOTAL_HEIGHT() { return  2 * item_size * (rows+1) - item_size; };
};