#include "app.h"
#include "vector.h"

int main()
{
	std::cout << "BIENVENUE DANS OUTERRAIN !" << std::endl;

	App app = App(800, 600, 3, 3);
	app.Run();

	return 0;
}