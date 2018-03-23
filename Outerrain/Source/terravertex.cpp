#include "terratree.h"

TerraVertex::TerraVertex(const Vector2& c, float r, float m) : TerraNode(Box2D(c, r)), center(c), radius(r), maxValue(m)
{

}

float TerraVertex::Value(const Vector2& p) const
{
	return 0.0f;
}
