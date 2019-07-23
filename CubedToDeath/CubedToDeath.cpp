#include <iostream>
#include "MyCraft.h"

int main()
{
	MyCraft* mycraft = new MyCraft();
	mycraft->Run();
	delete mycraft;
}
