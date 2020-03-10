#ifndef Renderer_h
#define Renderer_h
#include <gl3w/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>

#include "helpers/GenerationUtilities.h"

#include "Framebuffer.h"
#include "Gbuffer.h"
#include "AmbientQuad.h"
#include "camera/Camera.h"
#include "Camera_lab.h"
#include "Object.h"
#include "Skybox.h"
#include "ScreenQuad.h"
#include "lights/DirectionalLight.h"
#include "lights/PointLight.h"

class Renderer {

public:


	~Renderer();

	/// Init function
	Renderer(int width, int height);

	/// Draw function
	void draw();

	void physics(double elapsedTime);

	/// Clean function
	void clean() const;

	/// Handle screen resizing
	void resize(int width, int height);

	/// Handle keyboard inputs
	void keyPressed(int key, int action);

	void processInput(GLFWwindow* window);

	/// Handle mouse inputs
	void buttonPressed(int button, int action, double x, double y);

	void mousePosition(double x, double y, bool leftPress, bool rightPress);

	void initTanksModel();

	void initTank2Model();

	void initTank3Model();

	void initTank4Model();

	void initTank5Model();

	void initRampModel();

	void initRamp2Model();

	void initDroneModel();

	
private:
	
	double _timer;

	Camera* pCamera = nullptr;
	double deltaTime = 0.0f;    // time between current frame and last frame
	double lastFrame = 0.0f;

	//Object _suzanne;
	//Object _dragon;
	Object _drone;
	Object _tanks;
	Skybox _skybox;
	Object _plane;
	Object _tank1;
	float tank1posx = 0.0f;
	float tank1posz = -5.0f;
	float tank1speedx = 0.0f;
	float dronemovey = 7.0f;
	float dronemoveyRev = 1.0f;
	int tank1movex = 0;
	int tank1movez = 0;
	int reverse = 1;
	Object _tank2;
	Object _tank3;
	Object _heli1;
	Object _tank4;
	Object _tank5;
	Object _ramp;
	Object _ramp2;

	glm::mat4 tanksModel = glm::mat4(1.0f);
	glm::mat4 tank2Model = glm::mat4(1.0f);
	glm::mat4 tank3Model = glm::mat4(1.0f);
	glm::mat4 tank4Model = glm::mat4(1.0f);
	glm::mat4 tank5Model = glm::mat4(1.0f);
	glm::mat4 rampModel = glm::mat4(1.0f);
	glm::mat4 ramp2Model = glm::mat4(1.0f);
	glm::mat4 droneModel = glm::mat4(1.0f);


	std::shared_ptr<Gbuffer> _gbuffer;
	std::shared_ptr<Framebuffer> _ssaoFramebuffer;
	std::shared_ptr<Framebuffer> _ssaoBlurFramebuffer;
	std::shared_ptr<Framebuffer> _sceneFramebuffer;
	std::shared_ptr<Framebuffer> _toneMappingFramebuffer;
	std::shared_ptr<Framebuffer> _fxaaFramebuffer;

	AmbientQuad _ambientScreen;
	ScreenQuad _ssaoBlurScreen;
	ScreenQuad _toneMappingScreen;
	ScreenQuad _fxaaScreen;
	ScreenQuad _finalScreen;

	std::vector<DirectionalLight> _directionalLights;
	std::vector<PointLight> _pointLights;

};

#endif
