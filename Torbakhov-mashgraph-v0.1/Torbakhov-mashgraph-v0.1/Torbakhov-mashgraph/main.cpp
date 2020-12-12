//internal includes
#include "custom_include/common.h"
#include "custom_include/Object.h"
#include "custom_include/FrameBuffer.h"
#include "custom_include/ShaderProgram.h"


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

	glfwSwapInterval(1); // force 60 frames per second

	Mesh framebufferQuadMesh;
	framebufferQuadMesh.load("meshes/rect.obj");

	shaders[GL_VERTEX_SHADER] = "vertex_normal_parallax.glsl";
	shaders[GL_FRAGMENT_SHADER] = "fragment_shadows.glsl";
	ShaderProgram shadowGenShaderProgram(shaders);

	Camera shadowCamera;
	shadowCamera.pos = glm::vec3(8.0f, 6.0f, -5.0f);
	shadowCamera.dir = -shadowCamera.pos;

	Framebuffer shadowMapFramebuffer;
	Texture shadowMapTexture;
	shadowMapTexture.initDepth(SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT);
	shadowMapFramebuffer.initializeTextures(&shadowMapTexture);


	//Object
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
	cam.pos = glm::vec3(7.0f, 5.0f, 7.0f);
	cam.dir = -cam.pos;
	cam.moveSpeed = 2.0f;

	double time_prev = glfwGetTime();
	float deltaTime;

	//цикл обработки сообщений и отрисовки сцены каждый кадр
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		deltaTime = glfwGetTime() - time_prev;
		time_prev = deltaTime + time_prev;

		cam.moveCam(window, deltaTime);

		//Rendering ShadowMap
		shadowMapFramebuffer.bind();

		glViewport(0, 0, SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT);
		//очищаем экран каждый кадр
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);               GL_CHECK_ERRORS;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;

		cube.drawObject(&shadowCamera, &shadowGenShaderProgram);
		plane.drawObject(&shadowCamera, &shadowGenShaderProgram);
		
		shadowMapFramebuffer.unbind();
		GL_CHECK_ERRORS;

		program.StartUseShader();                           GL_CHECK_ERRORS;


		// очистка и заполнение экрана цветом
		//
		glViewport  (0, 0, WIDTH, HEIGHT);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear     (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		
		cube.drawObject(&cam, NULL, &shadowCamera);
		plane.drawObject(&cam, NULL, &shadowCamera);


		program.StopUseShader();

		glfwSwapBuffers(window); 
	}

	glfwTerminate();
	return 0;
}
