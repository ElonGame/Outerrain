#pragma once
#include "mesh.h"
#include "valueField.h"
#include "scalarfield.h"

class GameObject;

enum TreeType
{
	PineTree,
	Broadleaf
};

class Specie
{
public:
	TreeType type;
	Vector2 heightData;		// x: base value; z : range value
	Vector2 slopeData;		// x: base value; z : range value
	Vector2 wetnessData;	// x: base value; z : range value

	const char* name;
	Scalarfield2D densityField;

	Specie() : type(PineTree), heightData(Vector2(0.0)), slopeData(Vector2(0.0)), wetnessData(Vector2(0.0)), name(NULL)
	{
		densityField = Scalarfield2D(0, 0, 0, 0);
	}

	Specie(TreeType sp, Vector2 height, Vector2 slope, Vector2 wetness, const char* n, Scalarfield2D s) :
		type(sp), heightData(height), slopeData(slope), wetnessData(wetness), name(n), densityField(s) {}
};

class VegetationObject
{
protected:
	float radius = 1.0f;

public:
	VegetationObject();

	// Fonction de transfert des paramètres de végétation
	float SlopeDensityFactor(const Specie& s, float) const;
	float HeightDensityFactor(const Specie& s, float) const;
	float WetnessDensityFactor(const Specie& s, float) const;

	float ComputeDensityFactor(const Specie& s, float height, float slope, float wetness) const;

	GameObject* GetGameObject(const Specie& s) const;
	void SetRadius(float r);
	float GetRadius() const;
};
