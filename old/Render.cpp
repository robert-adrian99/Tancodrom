#include "Render.h"
//#include "ObjLoader.h"

// renders the 3D scene
// --------------------
void Render::renderScene(const Shader& shader)
{
	// floor
	glm::mat4 model;
	shader.SetMat4("model", model);
	renderFloor();

	// cube
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, 1.75f, 0.0));
	model = glm::scale(model, glm::vec3(0.75f));
	shader.SetMat4("model", model);
	renderCube();

	//cub2
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.2f, 2.75f, 1.5));
	model = glm::scale(model, glm::vec3(0.25f));
	model = glm::rotate(model, 0.5f, glm::vec3(0.2f, 3.0f, 1.5f));
	shader.SetMat4("model", model);
	renderCube();

	//cub3

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-0.5f, 2.75f, -1.0));
	model = glm::scale(model, glm::vec3(0.25f));
	model = glm::rotate(model, 0.5f, glm::vec3(0.2f, 3.0f, 1.5f));
	shader.SetMat4("model", model);
	renderCube();

	//cub4

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-5.5f, 0.75f, 1.0));
	model = glm::scale(model, glm::vec3(1.25f));
	model = glm::rotate(model, 0.7f, glm::vec3(0.2f, 3.0f, 1.5f));
	shader.SetMat4("model", model);
	renderCube();

	//cub5
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0));
	model = glm::scale(model, glm::vec3(0.25f));
	model = glm::rotate(model, 0.8f, glm::vec3(0.2f, 3.0f, 1.5f));
	shader.SetMat4("model", model);
	renderCube();
}


void Render::renderFloor()
{
	unsigned int planeVBO;

	if (planeVAO == 0)
	{
		// set up vertex data (and buffer(s)) and configure vertex attributes
		float planeVertices[] = {
			// positions            // normals // texcoords
			0.0f, -0.5f,  25.0f, 0.0f, 1.0f, 0.0f,  25.0f, 0.0f,
			-25.0f, -0.5f,  25.0f, 0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
			-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f, 0.0f, 25.0f,

			25.0f, -0.5f,  25.0f, 0.0f, 1.0f, 0.0f,  25.0f, 0.0f,
			-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f, 0.0f, 25.0f,
			25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f, 25.0f, 25.0f
		};
		// plane VAO
		glGenVertexArrays(1, &planeVAO);
		glGenBuffers(1, &planeVBO);
		glBindVertexArray(planeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindVertexArray(0);
	}

	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}


void Render::renderCube()
{
	//MyMesh mesh = getObj();
	float tankVertexArray[58512]; // atatea vertexuri are obj-ul, nu merge cu .size()
	//std::cout << mesh.vertices.size();
	/*for (int i = 0; i < mesh.vertices.size(); i++)
	{
		tankVertexArray[i] = mesh.vertices[i];
	}*/

	// initialize (if necessary)
	if (cubeVAO == 0)
	{
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			1.0f, 1.0f, -1.0f,  0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
			1.0f, -1.0f, -1.0f,  0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			1.0f, 1.0f, -1.0f,  0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f, 1.0f, -1.0f,  0.0f, 0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			1.0f, 1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f, // top-right
			1.0f, 1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f, 1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f, 1.0f,  1.0f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f, 1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, 1.0f,  1.0f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			1.0f, 1.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f, // top-left
			1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
			1.0f, 1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right         
			1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
			1.0f, 1.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f, // top-left
			1.0f, -1.0f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
			1.0f, -1.0f, -1.0f,  0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
			1.0f, -1.0f,  1.0f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			1.0f, -1.0f,  1.0f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f, 1.0f, -1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
			1.0f, 1.0f , 1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
			1.0f, 1.0f, -1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right     
			1.0f, 1.0f,  1.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f, 1.0f, -1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
			-1.0f, 1.0f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f, 0.0f // bottom-left        
		};

		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// glBufferData(GL_ARRAY_BUFFER, sizeof(tankVertexArray), tankVertexArray, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}