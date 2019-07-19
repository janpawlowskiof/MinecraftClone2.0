#pragma once
#include "SimpleBlock.h"

class ComplexBlock : public SimpleBlock
{
public:
	ComplexBlock();
	~ComplexBlock();
	virtual float* CreateModel(float* target) = 0;
	//virtual void NeighbourDeleted() = 0;
};

namespace blk
{
	/*class Dirt : public ComplexBlock
	{
	public:
		Dirt() { id = blk_id::dirt; }
		float* CreateModel(float* target)
		{

		}
	};*/
}