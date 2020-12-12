//internal includes
#include "custom_include/common.h"
#include "custom_include/Object.h"
#include "custom_include/FrameBuffer.h"
#include "custom_include/ShaderProgram.h"

#if defined __linux__ || defined __APPLE__ 
#else
//use Nvidia
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	_declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 0x00000001;
}
#endif

int initGL()
{
	int res = 0;
	//грузим функции opengl через glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}

	std::cout << "Vendor: "   << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: "  << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL: "     << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	return 0;
}

int main(int argc, char** argv)
{

	if(!glfwInit())
    return -1;

	//запрашиваем контекст opengl версии 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 

	GLFWwindow*  window = glfwCreateWindow(WIDTH, HEIGHT, "MashGraph Torbakhov", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window); 
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	if(initGL() != 0) 
		return -1;
	
	//Reset any OpenGL errors which could be present for some reason
	GLenum gl_error = glGetError();
	while (gl_error != GL_NO_ERROR)
		gl_error = glGetError();

	//создание шейдерной программы из двух файлов с исходниками шейдеров
	//используется класс-обертка ShaderProgram
	std::unordered_map<GLenum, std::string> shaders;
	shaders[GL_VERTEX_SHADER]   = "vertex_normal_parallax.glsl";
	shaders[GL_FRAGMENT_SHADER] = "fragment_normal_parallax.glsl";
	ShaderProgram program(shaders); GL_CHECK_ERRORS;

	glEnable(GL_DEPTH_TEST);

	Mesh framebufferQuadMesh;
	framebufferQuadMesh.load("meshes/rect.obj");

	shaders[GL_VERTEX_SHADER] = "vertex_normal_parallax.glsl";
	shaders[GL_FRAGMENT_SHADER] = "fragment_shadows.glsl";
	ShaderProgram shadowGenShaderProgram(shaders);

	Camera shadowCamera;
	shadowCamera.set_pos(glm::vec3(8.0f, 6.0f, -5.0f));
	shadowCamera.set_dir(-shadowCamera.get_pos());

	Framebuffer shadowMapFramebuffer;
	Texture shadowMapTexture;
	shadowMapTexture.init(GL_DEPTH_COMPONENT, SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, GL_DEPTH_COMPONENT, GL_FLOAT);
	shadowMapFramebuffer.initializeTextures(&shadowMapTexture);


	//Objects
	Object cube, plane;
	Mesh cubeMesh, planeMesh;
	Texture cubeTexture, planeTexture;
	Texture cubeNormalMap, planeNormalMap;
	Texture cubeHeightMap, planeHeightMap;

	cubeMesh.load("meshes/cube.obj");
	cube.mesh = &cubeMesh;

	cubeTexture.loadTexture("textures/Metal_Grill_Base_Color.jpg");
	cube.diffuseMap = &cubeTexture;
	cubeNormalMap.loadTexture("textures/Metal_Grill_Normal.jpg");
	cube.normalMap = &cubeNormalMap;
	cubeHeightMap.loadTexture("textures/Metal_Grill_Height.bmp");
	cube.heightMap = &cubeHeightMap;
	cube.shadowMap = &shadowMapTexture;
	cube.shader = &program;
	cube.position = glm::vec3(0, -0.1f, 0);
	cube.scale = 1;

	planeMesh.load("meshes/plane.obj");
	plane.mesh = &planeMesh;

	planeTexture.loadTexture("textures/Ground_Dirt_Base_Color.jpg");
	plane.diffuseMap = &planeTexture;
	planeNormalMap.loadTexture("textures/Ground_Dirt_Normal.jpg");
	plane.normalMap = &planeNormalMap;
	planeHeightMap.loadTexture("textures/Ground_Dirt_Height.bmp");
	plane.heightMap = &planeHeightMap;
	plane.shadowMap = &shadowMapTexture;
	plane.shader = &program;
	plane.position = glm::vec3(0, -2, 0);
	plane.scale = 4;



	//Camera
	Camera cam;
	cam.set_pos(glm::vec3(7.0f, 5.0f, 7.0f));
	cam.set_dir(-cam.get_pos());
	cam.set_move_speed(5.0f);

	//night_vision
	Texture night_visionDepthTexture;
	Texture night_visionColorTexture;
	Texture night_visionNormalMap;
	Framebuffer night_visionFramebuffer;
	Object night_visionScreen;

	shaders[GL_VERTEX_SHADER] = "vertex_quad.glsl";
	shaders[GL_FRAGMENT_SHADER] = "fragment_night_vision.glsl";
	ShaderProgram night_visionShader(shaders);

	night_visionDepthTexture.init(GL_DEPTH_COMPONENT, WIDTH, HEIGHT, GL_DEPTH_COMPONENT, GL_FLOAT);
	night_visionColorTexture.init(GL_RGB, WIDTH, HEIGHT, GL_RGB, GL_FLOAT);

	night_visionFramebuffer.initializeTextures(&night_visionDepthTexture, &night_visionColorTexture);

	night_visionScreen.mesh = &framebufferQuadMesh;
	night_visionScreen.diffuseMap = &night_visionColorTexture;
	night_visionNormalMap.loadTexture("textures/noise_texture.bmp");
	night_visionScreen.normalMap = &night_visionNormalMap;
	night_visionScreen.heightMap = &night_visionDepthTexture;
	night_visionScreen.shader = &night_visionShader;

	bool night_visionMode = false;
	bool night_visionChanged = false;

	double time_prev = glfwGetTime();
	float deltaTime;

	//rendering
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		//Rendering ShadowMap
		shadowMapFramebuffer.bind();

		glViewport(0, 0, SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);               GL_CHECK_ERRORS;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;

		cube.drawObject(&shadowCamera, &shadowGenShaderProgram);
		plane.drawObject(&shadowCamera, &shadowGenShaderProgram);
		
		shadowMapFramebuffer.unbind();
		GL_CHECK_ERRORS;

		if (night_visionMode) night_visionFramebuffer.bind();
		// clear screen
		glViewport  (0, 0, WIDTH, HEIGHT);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear     (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		cube.drawObject(&cam, NULL, &shadowCamera);
		plane.drawObject(&cam, NULL, &shadowCamera);

		if (night_visionMode) {
			night_visionFramebuffer.unbind();

			glViewport(0, 0, WIDTH, HEIGHT);
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			night_visionScreen.drawObject(NULL,NULL,NULL, deltaTime);
		}

		deltaTime = glfwGetTime() - time_prev;
		time_prev = deltaTime + time_prev;

		cam.moveCam(window, deltaTime);
		
		if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
			if (!night_visionChanged) {
				night_visionMode = !night_visionMode;
				night_visionChanged = true;
			}
		}
		else {
			night_visionChanged = false;
		}

		glfwSwapBuffers(window); 
	}

	glfwTerminate();
	return 0;
}
