#include <stdio.h>
#include <iostream>
#include <vector>
// glm additional header to generate transformation matrices directly.
#include <glm/gtc/matrix_transform.hpp>
#include <cstring> // For memcopy depending on the platform.

#include "Renderer.h"

glm::mat4 heli1Model;

Renderer::~Renderer() {}

void Renderer::initTanksModel() {
	tanksModel = glm::translate(tanksModel, glm::vec3(-2.0, -0.3, -5.0));
	tanksModel = glm::rotate(tanksModel, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	tanksModel = glm::rotate(tanksModel, glm::radians(-45.0f), glm::vec3(0, 0, 1));
	tanksModel = glm::scale(glm::translate(tanksModel, glm::vec3(5.0, -1.0, 0.1)), glm::vec3(0.25f));
	tanksModel = glm::rotate(tanksModel, glm::radians(90.0f), glm::vec3(0, 0, 1));
}

void Renderer::initTank2Model() {
	tank2Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(3.5, 0, 15)), glm::vec3(0.5f));
}

void Renderer::initTank3Model() {
	tank3Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0, -0.5, 10)), glm::vec3(0.5f));
	tank3Model = glm::rotate(tank3Model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
}

void Renderer::initTank4Model() {
	tank4Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(7, 0, 0)), glm::vec3(0.5f));
	tank4Model = glm::rotate(tank4Model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
	tank4Model = glm::rotate(tank4Model, glm::radians(30.0f), glm::vec3(1, 0, 0));
}

void Renderer::initTank5Model() {
	tank5Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(-10, 0, 5)), glm::vec3(3.0f));
	tank5Model = glm::rotate(tank5Model, glm::radians(180.0f), glm::vec3(0, 1, 0));
	tank5Model = glm::rotate(tank5Model, glm::radians(-20.0f), glm::vec3(0, 0, 1));
}

void Renderer::initRampModel() {
	rampModel = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(-9.5, 0, 4.65)), glm::vec3(0.25f));
	rampModel = glm::rotate(rampModel, glm::radians(90.0f), glm::vec3(0, 0, 1));
	rampModel = glm::rotate(rampModel, glm::radians(90.0f), glm::vec3(1, 0, 0));
}

void Renderer::initRamp2Model() {
	ramp2Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(8, 0, -0.75)), glm::vec3(0.5f));
	ramp2Model = glm::rotate(ramp2Model, glm::radians(90.0f), glm::vec3(0, 0, 1));
	ramp2Model = glm::rotate(ramp2Model, glm::radians(90.0f), glm::vec3(1, 0, 0));
	// rampModel = glm::rotate(rampModel, glm::radians(-20.0f), glm::vec3(0, 0, 1));
}

void Renderer::initDroneModel() {
	droneModel = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0, 3, 7)), glm::vec3(7.0));
	droneModel = glm::rotate(droneModel, glm::radians(-45.0f), glm::vec3(1, 0, 0));
}

int sign(float x) {
	if (x >= 0) return 1;
	if (x < 0) return -1;
	return 0;
}

Renderer::Renderer(int width, int height) {
	initTanksModel();
	initTank2Model();
	initTank3Model();
	initTank4Model();
	initTank5Model();
	initRampModel();
	initRamp2Model();
	initDroneModel();
	// Initialize the timer.
	_timer = glfwGetTime();
	// Initialize random generator;
	Random::seed();
	// Setup projection matrix.
	//_camera.screen(width, height);
	pCamera = new Camera(width, height, glm::vec3(0.0, 1.0, 3.0));
	glm::mat4 projection = pCamera->GetProjectionMatrix();
	glm::mat4 view = pCamera->GetViewMatrix();

	const int renderWidth = (int)pCamera->renderSize()[0];
	const int renderHeight = (int)pCamera->renderSize()[1];
	const int renderHalfWidth = (int)(0.5f * pCamera->renderSize()[0]);
	const int renderHalfHeight = (int)(0.5f * pCamera->renderSize()[1]);
	_gbuffer = std::make_shared<Gbuffer>(renderWidth, renderHeight);
	_ssaoFramebuffer = std::make_shared<Framebuffer>(renderHalfWidth, renderHalfHeight, GL_RED, GL_UNSIGNED_BYTE, GL_RED, GL_LINEAR, GL_CLAMP_TO_EDGE);
	_ssaoBlurFramebuffer = std::make_shared<Framebuffer>(renderWidth, renderHeight, GL_RED, GL_UNSIGNED_BYTE, GL_RED, GL_LINEAR, GL_CLAMP_TO_EDGE);
	_sceneFramebuffer = std::make_shared<Framebuffer>(renderWidth, renderHeight, GL_RGBA, GL_FLOAT, GL_RGBA16F, GL_LINEAR, GL_CLAMP_TO_EDGE);
	_toneMappingFramebuffer = std::make_shared<Framebuffer>(renderWidth, renderHeight, GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA, GL_LINEAR, GL_CLAMP_TO_EDGE);
	_fxaaFramebuffer = std::make_shared<Framebuffer>(renderWidth, renderHeight, GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA, GL_LINEAR, GL_CLAMP_TO_EDGE);

	/*_gbuffer = std::make_shared<Gbuffer>(800, 600);
	_ssaoFramebuffer = std::make_shared<Framebuffer>(400, 300, GL_RED, GL_UNSIGNED_BYTE, GL_RED, GL_LINEAR, GL_CLAMP_TO_EDGE);
	_ssaoBlurFramebuffer = std::make_shared<Framebuffer>(800, 600, GL_RED, GL_UNSIGNED_BYTE, GL_RED, GL_LINEAR, GL_CLAMP_TO_EDGE);
	_sceneFramebuffer = std::make_shared<Framebuffer>(800, 600, GL_RGBA, GL_FLOAT, GL_RGBA16F, GL_LINEAR, GL_CLAMP_TO_EDGE);
	_toneMappingFramebuffer = std::make_shared<Framebuffer>(800, 600, GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA, GL_LINEAR, GL_CLAMP_TO_EDGE);
	_fxaaFramebuffer = std::make_shared<Framebuffer>(800, 600, GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA, GL_LINEAR, GL_CLAMP_TO_EDGE);*/

	// Create directional light.
	_directionalLights.emplace_back(glm::vec3(0.0f), glm::vec3(2.0f), glm::ortho(-0.75f, 0.75f, -0.75f, 0.75f, 2.0f, 6.0f));

	// Create point lights.
	const float lI = 6.0; // Light intensity.
	std::vector<glm::vec3> colors = { glm::vec3(lI,0.0,0.0), glm::vec3(0.0,lI,0.0), glm::vec3(0.0,0.0,lI), glm::vec3(lI,lI,0.0) };

	PointLight::loadProgramAndGeometry();

	// Query the renderer identifier, and the supported OpenGL version.
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	std::cout << "Renderer: " << renderer << std::endl;
	std::cout << "OpenGL version supported: " << version << std::endl;
	checkGLError();

	// GL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);
	checkGLError();

	// Initialize objects.
	//_suzanne.init( "suzanne", {"suzanne_texture_color", "suzanne_texture_normal", "suzanne_texture_ao_specular_reflection"}, 1);

	//_dragon.init("dragon", {"dragon_texture_color", "dragon_texture_normal", "dragon_texture_ao_specular_reflection" },  1);

	_drone.init("Drone_obj", { "Flying drone_col", "Flying drone_Nor_2", "Flying drone_Ao_2" }, 1); //Drone_obj

	_tank1.init("King", { "King_color", "King_N 2","King_S" }, 1); //King

	_tank2.init("Abrams_BF3", { "M1A2", "M1A2_N 2","M1A2_S" }, 1); //Abrams_BF3

	_tank3.init("apc", { "us_apc_chass_asian", "us_apc_chass_asian_N 2","us_apc_chass_asian_S" }, 1); //apc

	_tank5.init("bradle.lwo", { "ttext", "ttext_N 2","ttext_S" }, 1); //bradle.lwo

	_tank4.init("m53-55", { "M53_55_crash", "","M53_55_crash_S" }, 1); //m53-55

	_ramp.init("Metal_Barrel", { "Metal_Barrel_UV_01", "Metal_Barrel_UV_01_NRM","Metal_Barrel_UV_01_SPC" }, 1);  // Metal_Barrel

	_ramp2.init("Metal_Barrel", { "Metal_Barrel_UV_01", "Metal_Barrel_UV_01_NRM","Metal_Barrel_UV_01_SPC" }, 1); // Metal_Barrel

	_heli1.init("3ds_file", { "Bodyheli", "Body_N","Body_S" }, 1); //3ds_file

	//_dragon.init("OBJ", { "", "", "" }, 1);

	_tanks.init("", { "8430f8ee", "8430f8ee_N 2", "8430f8ee_S" }, 1); // t-90a(Elements_of_war)

	_plane.init("plane", { "10450_Rectangular_Grass_Patch_v1_Diffuse", "10450_Rectangular_Grass_Patch_v1_Diffuse", "10450_Rectangular_Grass_Patch_v1_Diffuse" }, 2);

	_skybox.init();

	std::map<std::string, GLuint> ambientTextures = _gbuffer->textureIds({ TextureType::Albedo, TextureType::Normal, TextureType::Depth });
	ambientTextures["ssaoTexture"] = _ssaoBlurFramebuffer->textureId();
	_ambientScreen.init(ambientTextures);

	const std::vector<TextureType> includedTextures = { TextureType::Albedo, TextureType::Depth, TextureType::Normal, TextureType::Effects };
	for (auto& dirLight : _directionalLights) {
		dirLight.init(_gbuffer->textureIds(includedTextures));
	}

	for (auto& pointLight : _pointLights) {
		pointLight.init(_gbuffer->textureIds(includedTextures));
	}

	_ssaoBlurScreen.init(_ssaoFramebuffer->textureId(), "boxblur_float");
	_toneMappingScreen.init(_sceneFramebuffer->textureId(), "tonemap");
	_fxaaScreen.init(_toneMappingFramebuffer->textureId(), "fxaa");
	_finalScreen.init(_fxaaFramebuffer->textureId(), "final_screenquad");
	checkGLError();


	// Position fixed objects.
	//glm::mat4 dragonModel = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(-0.1,-0.05,-0.25)),glm::vec3(0.5f));
	//dragonModel = glm::translate(glm::mat4(1.0f), glm::vec3(-5.1, -5.05, -5.25));

	glm::mat4 tank1Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.5, 0, 0.5)), glm::vec3(0.01f));
	tank1Model = glm::rotate(tank1Model, glm::radians(0.0f), glm::vec3(1, 0, 0));

	//tank3Model = glm::rotate(tank3Model, glm::radians(90.0f), glm::vec3(0, 1, 0));

	heli1Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(-5, 5, -13)), glm::vec3(0.05f));
	/*heli1Model = glm::rotate(heli1Model, glm::radians(90.0f), glm::vec3(1, 0, 0));
	heli1Model = glm::rotate(heli1Model, glm::radians(-45.0f), glm::vec3(1, 0, 0));
	heli1Model = glm::rotate(heli1Model, glm::radians(45.0f), glm::vec3(0, 1, 0));*/

	const glm::mat4 planeModel = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, -0.5f)), glm::vec3(100.0f)); //findme: change last value to increase the size of the plane

	// _dragon.update(dragonModel);
	_drone.update(droneModel);
	_tank1.update(tank1Model);
	_tank2.update(tank2Model);
	_tank3.update(tank3Model);
	_ramp.update(rampModel);
	_ramp2.update(ramp2Model);
	_tank4.update(tank4Model);
	_tank5.update(tank5Model);
	_heli1.update(heli1Model);
	_tanks.update(tanksModel);
	_plane.update(planeModel);

}


void Renderer::draw() {

	float currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	// Compute the time elapsed since last frame
	double elapsed = glfwGetTime() - _timer;
	_timer = glfwGetTime();

	// Physics simulation
	physics(elapsed);


	glm::vec2 invRenderSize = 1.0f / pCamera->renderSize();

	// --- Light pass -------

	// Draw the scene inside the framebuffer.
	for (auto& dirLight : _directionalLights) {

		dirLight.bind();

		// Draw objects.
		// _suzanne.drawDepth(dirLight.mvp());
		// _dragon.drawDepth(dirLight.mvp());
		_drone.drawDepth(dirLight.mvp());
		_tank1.drawDepth(dirLight.mvp());
		_tank2.drawDepth(dirLight.mvp());
		_tank3.drawDepth(dirLight.mvp());
		_ramp.drawDepth(dirLight.mvp());
		_ramp2.drawDepth(dirLight.mvp());
		_tank4.drawDepth(dirLight.mvp());
		_tank5.drawDepth(dirLight.mvp());
		_heli1.drawDepth(dirLight.mvp());
		_tanks.drawDepth(dirLight.mvp());
		//_plane.drawDepth(planeModel, _light._mvp);

		dirLight.blurAndUnbind();

	}

	// ----------------------

	// --- Scene pass -------
	// Bind the full scene framebuffer.
	_gbuffer->bind();
	// Set screen viewport
	glViewport(0, 0, _gbuffer->width(), _gbuffer->height());

	// Clear the depth buffer (we know we will draw everywhere, no need to clear color.
	glClear(GL_DEPTH_BUFFER_BIT);

	// Draw objects
	//_suzanne.draw(_camera.view(), _camera.projection());
	//tanks
	tanksModel = glm::rotate(tanksModel, glm::radians(-90.0f), glm::vec3(0, 0, 1));
	_tanks.update(tanksModel);
	_tanks.draw(pCamera->GetViewMatrix(), pCamera->GetProjectionMatrix());
	tanksModel = glm::rotate(tanksModel, glm::radians(90.0f), glm::vec3(0, 0, 1));
	_tanks.update(tanksModel);
	_tanks.draw(pCamera->GetViewMatrix(), pCamera->GetProjectionMatrix());

	//_dragon.draw(_camera.view(), _camera.projection());
	_drone.draw(pCamera->GetViewMatrix(), pCamera->GetProjectionMatrix());
	droneModel = glm::translate(droneModel, glm::vec3(1, dronemovey / 3, -1));

	_drone.update(droneModel);
	_drone.draw(pCamera->GetViewMatrix(), pCamera->GetProjectionMatrix());
	droneModel = glm::translate(droneModel, glm::vec3(-2, 0 , 0));

	_drone.update(droneModel);
	_drone.draw(pCamera->GetViewMatrix(), pCamera->GetProjectionMatrix());
	droneModel = glm::translate(droneModel, glm::vec3(1, -dronemovey / 3, 1));

	_drone.update(droneModel);

	_tank3.draw(pCamera->GetViewMatrix(), pCamera->GetProjectionMatrix());
	tank3Model = glm::translate(tank3Model, glm::vec3(14, -20, 0));
	_tank3.update(tank3Model);
	_tank3.draw(pCamera->GetViewMatrix(), pCamera->GetProjectionMatrix());
	tank3Model = glm::translate(tank3Model, glm::vec3(-28, 0, 0));
	_tank3.update(tank3Model);
	_tank3.draw(pCamera->GetViewMatrix(), pCamera->GetProjectionMatrix());
	tank3Model = glm::translate(tank3Model, glm::vec3(14, 20, 0));
	_tank3.update(tank3Model);

	_tank2.draw(pCamera->GetViewMatrix(), pCamera->GetProjectionMatrix());
	tank2Model = glm::translate(tank2Model, glm::vec3(-7, 0, 15));
	_tank2.update(tank2Model);
	_tank2.draw(pCamera->GetViewMatrix(), pCamera->GetProjectionMatrix());
	tank2Model = glm::translate(tank2Model, glm::vec3(-7, 0, -15));
	_tank2.update(tank2Model);
	_tank2.draw(pCamera->GetViewMatrix(), pCamera->GetProjectionMatrix());
	tank2Model = glm::translate(tank2Model, glm::vec3(14, 0, 0));
	_tank2.update(tank2Model);

	_tank1.draw(pCamera->GetViewMatrix(), pCamera->GetProjectionMatrix());

	_tank4.draw(pCamera->GetViewMatrix(), pCamera->GetProjectionMatrix());

	_ramp2.draw(pCamera->GetViewMatrix(), pCamera->GetProjectionMatrix());
	ramp2Model = glm::translate(ramp2Model, glm::vec3(0, 8, 0));
	_ramp2.update(ramp2Model);
	_ramp2.draw(pCamera->GetViewMatrix(), pCamera->GetProjectionMatrix());
	ramp2Model = glm::translate(ramp2Model, glm::vec3(0, -8, 0));
	_ramp2.update(ramp2Model);


	_tank5.draw(pCamera->GetViewMatrix(), pCamera->GetProjectionMatrix());
	tank5Model = glm::translate(tank5Model, glm::vec3(-0.5, -0.2, 1));
	_tank5.update(tank5Model);
	_tank5.draw(pCamera->GetViewMatrix(), pCamera->GetProjectionMatrix());
	tank5Model = glm::translate(tank5Model, glm::vec3(-0.5, -0.2, 1));
	_tank5.update(tank5Model);
	_tank5.draw(pCamera->GetViewMatrix(), pCamera->GetProjectionMatrix());
	tank5Model = glm::translate(tank5Model, glm::vec3(1, 0.4, -2));
	_tank5.update(tank5Model);

	_ramp.draw(pCamera->GetViewMatrix(), pCamera->GetProjectionMatrix());
	rampModel = glm::translate(rampModel, glm::vec3(0, -12, 6));
	_ramp.update(rampModel);
	_ramp.draw(pCamera->GetViewMatrix(), pCamera->GetProjectionMatrix());
	rampModel = glm::translate(rampModel, glm::vec3(0, -12, 6));
	_ramp.update(rampModel);
	_ramp.draw(pCamera->GetViewMatrix(), pCamera->GetProjectionMatrix());
	rampModel = glm::translate(rampModel, glm::vec3(0, 24, -12));
	_ramp.update(rampModel);

	heli1Model = glm::translate(heli1Model, glm::vec3(1, dronemovey / 6, -1));
	_heli1.update(heli1Model);
	_heli1.draw(pCamera->GetViewMatrix(), pCamera->GetProjectionMatrix());

	_plane.draw(pCamera->GetViewMatrix(), pCamera->GetProjectionMatrix());


	for (auto& pointLight : _pointLights) {
		pointLight.drawDebug(pCamera->GetViewMatrix(), pCamera->GetProjectionMatrix());
	}

	_skybox.draw(pCamera->GetViewMatrix(), pCamera->GetProjectionMatrix());

	// Unbind the full scene framebuffer.
	_gbuffer->unbind();
	// ----------------------

	glDisable(GL_DEPTH_TEST);

	// --- SSAO pass
	_ssaoFramebuffer->bind();
	glViewport(0, 0, _ssaoFramebuffer->width(), _ssaoFramebuffer->height());
	_ambientScreen.drawSSAO(2.0f * invRenderSize, pCamera->GetViewMatrix(), pCamera->GetProjectionMatrix());
	_ssaoFramebuffer->unbind();

	// --- SSAO blurring pass
	_ssaoBlurFramebuffer->bind();
	glViewport(0, 0, _ssaoBlurFramebuffer->width(), _ssaoBlurFramebuffer->height());
	_ssaoBlurScreen.draw(invRenderSize);
	_ssaoBlurFramebuffer->unbind();

	// --- Gbuffer composition pass
	_sceneFramebuffer->bind();

	glViewport(0, 0, _sceneFramebuffer->width(), _sceneFramebuffer->height());

	_ambientScreen.draw(invRenderSize, pCamera->GetViewMatrix(), pCamera->GetProjectionMatrix());

	glEnable(GL_BLEND);
	for (auto& dirLight : _directionalLights) {
		dirLight.draw(invRenderSize, pCamera->GetViewMatrix(), pCamera->GetProjectionMatrix());
	}
	glCullFace(GL_FRONT);
	for (auto& pointLight : _pointLights) {
		pointLight.draw(invRenderSize, pCamera->GetViewMatrix(), pCamera->GetProjectionMatrix());
	}
	glDisable(GL_BLEND);
	glCullFace(GL_BACK);
	_sceneFramebuffer->unbind();

	_toneMappingFramebuffer->bind();
	glViewport(0, 0, _toneMappingFramebuffer->width(), _toneMappingFramebuffer->height());
	_toneMappingScreen.draw(invRenderSize);
	_toneMappingFramebuffer->unbind();

	// --- FXAA pass -------
	// Bind the post-processing framebuffer.
	_fxaaFramebuffer->bind();
	// Set screen viewport.
	glViewport(0, 0, _fxaaFramebuffer->width(), _fxaaFramebuffer->height());

	// Draw the fullscreen quad
	_fxaaScreen.draw(invRenderSize);

	_fxaaFramebuffer->unbind();
	// ----------------------


	// --- Final pass -------
	// We now render a full screen quad in the default framebuffer, using sRGB space.
	glEnable(GL_FRAMEBUFFER_SRGB);

	// Set screen viewport.
	glViewport(0, 0, GLsizei(pCamera->screenSize()[0]), GLsizei(pCamera->screenSize()[1]));

	// Draw the fullscreen quad
	_finalScreen.draw(1.0f / pCamera->screenSize());

	glDisable(GL_FRAMEBUFFER_SRGB);
	// ----------------------
	glEnable(GL_DEPTH_TEST);

	// Update timer
	_timer = glfwGetTime();
}

void Renderer::physics(double elapsedTime) {

	//_camera.update(elapsedTime);

	// Update lights.
	_directionalLights[0].update(glm::vec3(2.0f, 1.5f, 2.0f), pCamera->GetViewMatrix());

	for (size_t i = 0; i < _pointLights.size(); ++i) {
		auto& pointLight = _pointLights[i];
		glm::vec4 newPosition = glm::rotate(glm::mat4(1.0f), (float)elapsedTime, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(pointLight.local(), 1.0f);
		pointLight.update(glm::vec3(newPosition), pCamera->GetViewMatrix());
	}
	//findme: Simulate accelerated day cycle by moving sunlight as time passes
	// Update objects.

	//const glm::mat4 suzanneModel = glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.2,0.0,0.0)),float(_timer),glm::vec3(0.0f,1.0f,0.0f)),glm::vec3(0.25f));
	// const glm::mat4 dragonModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.6, 0.6, 0.6)); //findme: change vec3 to set the position
	//_suzanne.update(suzanneModel);
	// _dragon.update(dragonModel);
	if (tank1movez != 0) {
		if (tank1speedx<50 && tank1speedx>-50) {
			tank1speedx += 0.3f * tank1movex;
		}
		else {
			tank1speedx *= 0.97;
		}
		tank1posx += 0.01f * (tank1speedx / 30) * reverse;
		tank1posz += 0.03f * tank1movez;
		if (tank1movez < 0) {
			reverse = -1;
		}
		else {
			reverse = 1;
		}
	}
	glm::mat4 tank1Model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(tank1posx, -0.3, tank1posz)), glm::vec3(0.02f));
	tank1Model = glm::rotate(tank1Model, glm::radians(tank1speedx / 2), glm::vec3(0, 1, 0));
	_tank1.update(tank1Model);

	if (dronemovey > 10 || dronemovey < 5) {
		dronemoveyRev *= -1;
	}
	dronemovey += 0.01 * dronemoveyRev;
}


void Renderer::clean() const {
	// Clean objects.
	//_suzanne.clean();
	_tanks.clean();
	//_dragon.clean();
	_drone.clean();
	_tank2.clean();
	_tank3.clean();
	_tank4.clean();
	_tank5.clean();
	_ramp.clean();
	_ramp2.clean();
	_tank1.clean();
	_heli1.clean();
	_plane.clean();
	_skybox.clean();
	for (auto& dirLight : _directionalLights) {
		dirLight.clean();
	}
	_ambientScreen.clean();
	_fxaaScreen.clean();
	_ssaoBlurScreen.clean();
	_toneMappingScreen.clean();
	_finalScreen.clean();
	_gbuffer->clean();
	_ssaoFramebuffer->clean();
	_ssaoBlurFramebuffer->clean();
	_sceneFramebuffer->clean();
	_toneMappingFramebuffer->clean();
	_fxaaFramebuffer->clean();
}


void Renderer::resize(int width, int height) {
	//Update the size of the viewport.
	glViewport(0, 0, width, height);
	// Update the projection matrix.
	pCamera->screen(width, height);
	// Resize the framebuffer.
	_gbuffer->resize(pCamera->renderSize());
	_ssaoFramebuffer->resize(0.5f * pCamera->renderSize());
	_ssaoBlurFramebuffer->resize(pCamera->renderSize());
	_sceneFramebuffer->resize(pCamera->renderSize());
	_toneMappingFramebuffer->resize(pCamera->renderSize());
	_fxaaFramebuffer->resize(pCamera->renderSize());
}

void Renderer::keyPressed(int key, int action) {

	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_M) {
			tank1movex = 0;
			tank1movez = 0;
			tank1posx = 0;
			tank1posz = -5;
			tank1speedx = 0;
		}
		if (key == GLFW_KEY_UP) {
			tank1movez = 1;
		}
		else if (key == GLFW_KEY_DOWN) {
			tank1movez = -1;
		}
		if (key == GLFW_KEY_LEFT) {
			tank1movex = 1 * reverse;
		}
		else if (key == GLFW_KEY_RIGHT) {
			tank1movex = -1 * reverse;
		}

		//_camera.key(key, true);
	}
	else if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_UP || key == GLFW_KEY_DOWN) {
			tank1movez = 0;
		}
		if (key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT) {
			tank1movex = 0;
		}

		//_camera.key(key, false);
	}
}

void Renderer::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		pCamera->ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		pCamera->ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		pCamera->ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		pCamera->ProcessKeyboard(RIGHT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		pCamera->ProcessKeyboard(UP, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		pCamera->ProcessKeyboard(DOWN, (float)deltaTime);

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		pCamera->Reset(width, height);

	}

}

void Renderer::buttonPressed(int button, int action, double x, double y) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
		}
		else if (action == GLFW_RELEASE) {
		}
	}
	else {
		std::cout << "Button: " << button << ", action: " << action << std::endl;
	}
}

void Renderer::mousePosition(double x, double y, bool leftPress, bool rightPress) {

	pCamera->MouseControl((float)x, (float)y, leftPress);
	/*if (leftPress){
		_camera.mouse(MouseMode::Move, float(x), float(y));
	}*/
}



