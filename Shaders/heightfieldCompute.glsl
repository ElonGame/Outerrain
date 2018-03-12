#version 430

#ifdef COMPUTE_SHADER

layout(binding = 0, std430) readonly buffer InputData
{
    float inputData[];
};

layout(binding = 1, std430) writeonly buffer OutputData
{
    float outputData[];
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


/* Main program */
layout(local_size_x = 1024) in;
void main()
{
	uint id = gl_GlobalInvocationID.x;
	int nx = 1024;
	float amplitude = 0.6f;
	float tanThresholdAngle = 0.6f;
	float cellDistX = 1.0;
	float newHeight = inputData[id];
    if(id >= inputData.length())
        return;
	
	float maxZDiff = 0.0f;
	int neiIndex = -1;
	int i = int(id) / nx;
	int j = int(id) % nx;
	for (int k = -1; k <= 1; k++)
	{
		for (int l = -1; l <= 1; l++)
		{
			if ((k == 0 && l == 0) || Inside(i + k, j + l, nx) == false)
				continue;
			float z = inputData[id] - inputData[ToIndex1D(i + k, j + l, nx)];
			if (z > maxZDiff)
			{
				maxZDiff = z;
				neiIndex = ToIndex1D(i + k, j + l, nx);
			}
		}
	}

	// Point is unstable : decrease matter
	if (maxZDiff / cellDistX > tanThresholdAngle)
	{
		float matter = amplitude;
		newHeight -= matter;
		
		// Add matter to lowest neighbour
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//!!!!!!! No atomicAdd for floats in glsl!!!!!!
		//outputData[neiIndex] = inputData[neiIndex] + matter;
	}
	
	outputData[id] = newHeight;
}

#endif
