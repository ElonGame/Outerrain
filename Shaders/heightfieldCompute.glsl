#version 430

#ifdef COMPUTE_SHADER

layout(binding = 0, std430) readonly buffer InputData
{
    int inputData[];
};

layout(binding = 1, std430) writeonly buffer OutputData
{
    int outputData[];
};


/* Useful functions */
bool Inside(int i, int j, int nx)
{
	if (i < 0 || i >= nx || j < 0 || j >= nx)
		return false;
	return true;
}

int ToIndex1D(int i, int j, int nx)
{
	return i * nx + j;
}

float ChangeBase(float min, float max, float newMin, float newMax, float val)
{
	return (newMax - newMin) * (val - min) / (max - min);
}

const int minRange = 0;
const int maxRange = 100000;

/* Main program */
layout(local_size_x = 1024) in;
void main()
{
	uint id = gl_GlobalInvocationID.x;
	int nx = 256;
	int amplitude = 1000;
	float tanThresholdAngle = 1200;
	float cellDistX = 2.0;
	int newHeight = inputData[id];
    if(id >= inputData.length())
        return;
	
	int maxZDiff = 0;
	int neiIndex = -1;
	int i = int(id) / nx;
	int j = int(id) % nx;
	for (int k = -1; k <= 1; k++)
	{
		for (int l = -1; l <= 1; l++)
		{
			if ((k == 0 && l == 0) || Inside(i + k, j + l, nx) == false)
				continue;
			int z = inputData[id] - inputData[ToIndex1D(i + k, j + l, nx)];
			if (z > maxZDiff)
			{
				maxZDiff = z;
				neiIndex = ToIndex1D(i + k, j + l, nx);
			}
		}
	}
	
	if (float(maxZDiff) / cellDistX > tanThresholdAngle)
	{
		newHeight -= amplitude;
		atomicAdd(outputData[neiIndex], amplitude);
	}
	
	outputData[id] = newHeight;
}

#endif
