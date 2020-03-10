#pragma once

#include <stdlib.h> //necesare pentru citirea shaderStencilTesting-elor
#include <stdio.h>
#include <math.h> 

#include <glew.h>

#define GLM_FORCE_CTOR_INIT 
#include <GLM.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

#ifndef STB_IMAGE_IMPLEMENTATION
	#define STB_IMAGE_IMPLEMENTATION
	#include <stb_image.h>
#endif // !STB_IMAGE_IMPLEMENTATION

#include <vector>
#include "tiny_obj_loader.h"

#pragma comment (lib, "glfw3dll.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "OpenGL32.lib")

typedef struct {
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texcoords;
	std::vector<int> v_indices;
	std::vector<int> vn_indices;
	std::vector<int> vt_indices;

	std::vector<tinyobj::material_t> materials;

} MyMesh;