#include <iostream>
#include "MyCraft.h"

int main()
{
	MyCraft* cuban = new MyCraft();
	cuban->Run();
	delete cuban;
}
