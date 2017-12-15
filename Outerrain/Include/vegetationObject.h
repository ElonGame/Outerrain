#pragma once
#include "mesh.h"

class GameObject;

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
	float SlopeDensityFactor(float);
	float HeightDensityFactor(float);
	float WetnessDensityFactor(float);
	float StreamPowerDensityFactor(float);

	float ComputeDensityFactor(float height, float slope, float wetness, float streampower);

	GameObject* GetGameObject();

	void SetRadius(float r);
	float GetRadius();
};
