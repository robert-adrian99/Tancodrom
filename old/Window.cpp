#include "Window.h"

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void Window::processInput(Camera* pCamera)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		pCamera->ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		pCamera->ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		pCamera->ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		pCamera->ProcessKeyboard(RIGHT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		pCamera->ProcessKeyboard(UP, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		pCamera->ProcessKeyboard(DOWN, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		pCamera->Reset(width, height);
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
	{
		lightMoving = true;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		lightMoving = false;
	}

}

Window::Window()
{
	window = glfwCreateWindow(Window::SCR_WIDTH, Window::SCR_HEIGHT, "Tancodrom", NULL, NULL);

	deltaTime = 0.0f;
	lastFrame = 0.0f;
	lightMoving = false;
}



// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void Window::framebuffer_size_callback(int width, int height, Camera* pCamera)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	pCamera->Reshape(width, height);
}

void Window::mouse_callback(double xpos, double ypos, Camera* pCamera)
{
	pCamera->MouseControl((float)xpos, (float)ypos);
}

void Window::scroll_callback(double xoffset, double yOffset, Camera* pCamera)
{
	pCamera->ProcessMouseScroll((float)yOffset);
}


unsigned int Window::CreateTexture(const std::string& strTexturePath, Camera* pCamera)
{
	unsigned int textureId = -1;

	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load(strTexturePath.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "Failed to load texture: " << strTexturePath << std::endl;
	}
	stbi_image_free(data);

	return textureId;
}



