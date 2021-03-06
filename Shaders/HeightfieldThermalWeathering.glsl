#version 430

#ifdef COMPUTE_SHADER

layout(binding = 0, std430) coherent buffer HeightfieldDataFloat
{
	float floatingHeightBuffer[];
};

uniform int gridSize;
uniform float amplitude;
uniform float cellSize;
uniform float tanThresholdAngle;

bool Inside(int i, int j)
{
	if (i < 0 || i >= gridSize || j < 0 || j >= gridSize)
		return false;
	return true;
}

int ToIndex1D(int i, int j)
{
	return i * gridSize + j;
}

layout(local_size_x = 1024) in;
void main()
{
	uint id = gl_GlobalInvocationID.x;
	if(id >= integerHeightBuffer.length())
        return;
	
	int integerAmplitude = int(amplitude);
	float maxZDiff = 0;
	int neiIndex = -1;
	int i = int(id) / gridSize;
	int j = int(id) % gridSize;
	for (int k = -1; k <= 1; k += 2)
	{
		for (int l = -1; l <= 1; l += 2)
		{
			if (Inside(i + k, j + l) == false)
				continue;
			int index = ToIndex1D(i + k, j + l);
			float h = float(integerHeightBuffer[index]);
			float z = float(integerHeightBuffer[id] - h);
			if (z > maxZDiff)
			{
				maxZDiff = z;
				neiIndex = index;
			}
		}
	}
	if (maxZDiff / cellSize > tanThresholdAngle)
	{
		floatingHeightBuffer[id] = floatingHeightBuffer[id] - amplitude;
		floatingHeightBuffer[neiIndex] = floatingHeightBuffer[neiIndex] + amplitude;
	}
}

#endif
