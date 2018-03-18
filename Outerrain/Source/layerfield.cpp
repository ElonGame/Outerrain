#include "layerfield.h"

/*
\brief
*/
LayerField::LayerField(int nx, int ny, const Box2D& bbox)
{
	bedrock = Scalarfield2D(nx, ny, bbox, 1.0);

	vegetation = Scalarfield2D(nx, ny, bbox, 0.0);
	snow = Scalarfield2D(nx, ny, bbox, 0.0);
	water = Scalarfield2D(nx, ny, bbox, 0.0);
	sediments = Scalarfield2D(nx, ny, bbox, 0.0);
}

/*
\brief
*/
LayerField::LayerField(int nx, int ny, const Box2D& bbox, float value)
{
	bedrock = Scalarfield2D(nx, ny, bbox, value);

	vegetation = Scalarfield2D(nx, ny, bbox, 0.0);
	snow = Scalarfield2D(nx, ny, bbox, 0.0);
	water = Scalarfield2D(nx, ny, bbox, 0.0);
	sediments = Scalarfield2D(nx, ny, bbox, 0.0);
}

/*
\brief
*/
LayerField::LayerField(const std::string& filePath, float blackAltitude, float whiteAltitude, int nx, int ny, const Box2D& bbox)
{
	bedrock = Scalarfield2D(filePath, blackAltitude, whiteAltitude, nx, ny, bbox);

	vegetation = Scalarfield2D(nx, ny, bbox, 0.0);
	snow = Scalarfield2D(nx, ny, bbox, 0.0);
	water = Scalarfield2D(nx, ny, bbox, 0.0);
	sediments = Scalarfield2D(nx, ny, bbox, 0.0);
}


/*
	Papers :
		- https://hal.archives-ouvertes.fr/hal-01518967/document
		- https://hal.inria.fr/hal-01262376/document
		- https://perso.liris.cnrs.fr/eric.galin/Articles/2016-environmental-objects.pdf
*/