#include "terratree.h"

static const float epsilon = 1e-6;

TerraNode::TerraNode()
{
}

TerraNode::TerraNode(const Box2D& b) : box(b)
{

}

Vector2 TerraNode::Gradient(const Vector2& p) const
{
	float x = Value(Vector2(p[0] + epsilon, p[1])) - Value(Vector2(p[0] - epsilon, p[1]));
	float y = Value(Vector2(p[0], p[1] + epsilon)) - Value(Vector2(p[0], p[1] - epsilon));
	return Vector2(x, y) * (0.5 / epsilon);
}
