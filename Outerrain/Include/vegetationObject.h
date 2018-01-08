#pragma once
#include "mesh.h"
#include "valueField.h"

class GameObject;

enum TreeType
{
	PineTree,
	Broadleaf
};

typedef struct Specie
{
public:
	TreeType type;
	Vector2 heightData; // x: base value; z : range value
	Vector2 slopeData; // x: base value; z : range value
	Vector2 wetnessData; // x: base value; z : range value

	const char* name;
	ScalarField2D densityField;

	Specie() : type(PineTree), heightData(Vector2(0.0)), slopeData(Vector2(0.0)), wetnessData(Vector2(0.0)), name(NULL)
	{
		densityField = ScalarField2D(0, 0, 0, 0);
	}

	Specie(TreeType sp, Vector2 height, Vector2 slope, Vector2 wetness, const char* n, ScalarField2D s) :
		type(sp), heightData(height), slopeData(slope), wetnessData(wetness), name(n), densityField(s) {}
} Specie;

class VegetationObject
{
protected:
	float radius = 1.0f;

public:
	VegetationObject();

	// Chaque arbre ou type d'arbre aura ses propriétés. 
	// Les fonctions suivantes retournent une 'densité'
	// Pour ce type en fonction de facteurs (height, slope...)
	// Les paramètres doivent être normalisés [0, 1].
	float SlopeDensityFactor(const Specie s, float);
	float HeightDensityFactor(const Specie s, float);
	float WetnessDensityFactor(const Specie s, float);

	float ComputeDensityFactor(Specie s, float height, float slope, float wetness);

	GameObject* GetGameObject(Specie s);

	void SetRadius(float r);
	float GetRadius();
};
