#include "app.h"
#include "vec.h"
#include <stdio.h>

int main()
{
	srand(1589);

	std::cout << "BIENVENUE DANS OUTERRAIN !" << std::endl;
	App app = App(1280, 720, 3, 3);
	app.Run();
	return 0;
}
