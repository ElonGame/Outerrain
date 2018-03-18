#pragma once
#include "vec.h"

struct EcosystemInstance
{
public:
	Vector3 position;
	float fieldFactor;
	int age;

	EcosystemInstance(const Vector3& p, float f, int a) : position(p), fieldFactor(f), age(a)
	{
		
	}
};
