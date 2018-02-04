#include "mainwindow.h"
#include "app.h"

int main()
{
	srand(1589);

	std::cout << "Welcome to Outerrain !" << std::endl;
	MainWindow mw = MainWindow(1280, 720);
	mw.Show();

	//App app(1280, 720, 4, 4);
	//app.Run();

	return 0;
}
