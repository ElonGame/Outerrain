#include "mainwindow.h"
#include "random.h"

int main()
{
	srand(1589);

	MainWindow mw = MainWindow(1280, 720);
	mw.Show();

	return 0;
}
