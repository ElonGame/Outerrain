#pragma once
#include "mesh.h"

class GameObject;

enum Specie
{
	PineTree,
	Broadleaf
};
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
	float StreamPowerDensityFactor(const Specie s, float);

	float ComputeDensityFactor(Specie s, float height, float slope, float wetness, float streampower);

	GameObject* GetGameObject(Specie s);

	void SetRadius(float r);
	float GetRadius();
};
