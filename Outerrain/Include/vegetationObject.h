#pragma once
#include "mesh.h"

class GameObject;

class VegetationObject
{
protected:

public:
	VegetationObject();

	// Chaque arbre ou type d'arbre aura ses propri�t�s. 
	// Les fonctions suivantes retournent une 'densit�'
	// Pour ce type en fonction de facteurs (height, slope...)
	// Les param�tres doivent �tre normalis�s [0, 1].
	float SlopeDensityFactor(float);
	float HeightDensityFactor(float);
	float WetnessDensityFactor(float);
	float StreamPowerDensityFactor(float);

	virtual GameObject* GetGameObject();
};
