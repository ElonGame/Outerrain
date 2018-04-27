#include "layerfield.h"
#include "heightfield.h"

/*
This class represents a heightfield made of several layers : bedrock, sediments, water etc...
This model is useful when trying to do an ecosystem simulation and study events such as fires, wind, water, lighting strikes...
Several ones are implemented here, but not all. If you want to know more about this, you can read the following papers :
	- https://hal.archives-ouvertes.fr/hal-01518967/document
	- https://perso.liris.cnrs.fr/eric.galin/Articles/2017-sparse-vegetation-terrains.pdf
*/

/*
\brief
*/
LayerField::LayerField(int nx, int ny, const Box2D& bbox) : ValueField(nx, ny, bbox, LayerStack(0.0))
{
}

/*
\brief
*/
LayerField::LayerField(int nx, int ny, const Box2D& bbox, float value) : ValueField(nx, ny, bbox, LayerStack(value))
{
}

/*
\brief
*/
LayerField::LayerField(const std::string& filePath, float blackAltitude, float whiteAltitude, int nx, int ny, const Box2D& bbox) : LayerField(nx, ny, bbox)
{
	HeightField hf(filePath, blackAltitude, whiteAltitude, nx, ny, bbox);
	for (int i = 0; i < hf.SizeX(); i++)
	{
		for (int j = 0; j < hf.SizeY(); j++)
			Set(i, j, LayerStack(hf.Get(i, j)));
	}
}

/*
\brief
*/
Vector3 LayerField::Vertex(int i, int j) const
{
	Vector2 pos2D = ValueField::Vertex(i, j);
	Vector3 ret = Vector3(pos2D.x, 0.0, pos2D.y);
	ret.y = Get(i, j).Height();
	return ret;
}
