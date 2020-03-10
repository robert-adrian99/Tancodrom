#pragma once

#include "Shader.h"
//#include "Camera.h"

class Render
{
public:
	void renderScene(const Shader& shader);
	void renderFloor();
	void renderCube();

protected:
	unsigned int cubeVAO;
	unsigned int cubeVBO;
	unsigned int planeVAO;
};

