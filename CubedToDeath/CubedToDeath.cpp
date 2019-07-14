#include <iostream>
#include "cubanCigar.h"

int main()
{
	CubanCigar* cuban = new CubanCigar();
	cuban->Run();
	delete cuban;
}
