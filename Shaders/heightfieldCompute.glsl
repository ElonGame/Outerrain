#version 430

#ifdef COMPUTE_SHADER

layout(binding = 0, std430) coherent buffer HeightfieldData
{
    int data[];
};

const int nx = 1024;

bool Inside(int i, int j)
{
	if (i < 0 || i >= nx || j < 0 || j >= nx)
		return false;
	return true;
}

int ToIndex1D(int i, int j)
{
	return i * nx + j;
}

layout(local_size_x = 1024) in;
void main()
{
	uint id = gl_GlobalInvocationID.x;
	if(id >= data.length())
        return;
	
	int amplitude = 1;
	float tanThresholdAngle = 0.6f;
	float cellDistX = 2.0f;

	float maxZDiff = 0;
	int neiIndex = -1;
	int i = int(id) / nx;
	int j = int(id) % nx;
	for (int k = -1; k <= 1; k += 2)
	{
		for (int l = -1; l <= 1; l += 2)
		{
			if (Inside(i + k, j + l) == false)
				continue;
			int index = ToIndex1D(i + k, j + l);
			float h = data[index]; 
			float z = data[id] - h;
			if (z > maxZDiff)
			{
				maxZDiff = z;
				neiIndex = index;
			}
		}
	}
	
	if (maxZDiff / cellDistX > tanThresholdAngle)
	{
		atomicAdd(data[id], -amplitude);
		atomicAdd(data[neiIndex], amplitude);
	}
}

#endif
