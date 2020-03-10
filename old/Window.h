#pragma once

#include "Camera.h"
//#include "Shader.h"
//#include "Render.h"

class Window
{

public:
	Window();
	unsigned int CreateTexture(const std::string& strTexturePath, Camera* pCamera);
	void scroll_callback(double xoffset, double yOffset, Camera* pCamera);
	void mouse_callback(double xpos, double ypos, Camera* pCamera);
	void framebuffer_size_callback(int width, int height, Camera* pCamera);
	void processInput(Camera* pCamera);

public:
	// settings
	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;
	// timing
	double deltaTime = 0.0f;    // time between current frame and last frame
	double lastFrame = 0.0f;
	bool lightMoving = false;
	GLFWwindow* window;
};