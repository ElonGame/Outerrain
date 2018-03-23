#pragma once
#include "vec.h"
#include "box2D.h"
#include "noise.h"

class TerraNode
{
protected:
	Box2D box;

public:
	TerraNode();
	TerraNode(const Box2D& b);

	virtual float Value(const Vector2& p) const = 0;
	virtual Vector2 Gradient(const Vector2& p) const;
};

class TerrafBm : public TerraNode
{
protected:
	static PerlinNoise noise;
	float amplitude;
	float frequency;
	int octave;

public:
	TerrafBm(float a, float f, int o);

	virtual float Value(const Vector2& p) const;
};

class TerraVertex : public TerraNode
{
protected:
	Vector2 center;
	float radius;
	float maxValue;

public:
	TerraVertex(const Vector2& c, float r, float m);

	virtual float Value(const Vector2& p) const;
};

class TerraTree
{
protected:
	TerraNode* root;

public:
	TerraTree(TerraNode* r);

	float Value(const Vector2& p) const;
	Vector2 Gradient(const Vector2& p) const;
};
