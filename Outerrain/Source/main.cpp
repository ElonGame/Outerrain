#include "mainwindow.h"
//#include "vld.h"

int main()
{
	srand(1589);

	std::cout << "Welcome to Outerrain !" << std::endl;
	MainWindow mw = MainWindow(1280, 720);
	mw.Show();

	return 0;
}
