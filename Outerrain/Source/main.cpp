#include "mainwindow.h"


//#include "ray.h"
//#include "cameraOrbiter.h"
//#include <iostream>
//using namespace std;

int main()
{
	MainWindow mw = MainWindow(1280, 720);
	mw.Show();

	//CameraOrbiter orbiter = CameraOrbiter(Vector3(0), 10.0f, 1.0f, 5000.0f);
	//orbiter.SetFrameWidth(1280);
	//orbiter.SetFrameHeight(720);

	//Ray r = orbiter.PixelToRay(Vector2i(0, 254));
	//cout << r.origin << endl;
	//cout << r.direction << endl;
	//cin.get();

	return 0;
}
