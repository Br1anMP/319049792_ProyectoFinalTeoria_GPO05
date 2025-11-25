#include <iostream>
#include <cmath>
// GLEW
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// Other Libs
#include "stb_image.h"
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//Load Models
#include "SOIL2/SOIL2.h"
// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"


// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();

// Window dimensions
const GLuint WIDTH = 1920, HEIGHT = 1080;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(-2.0f, 0.0f, 18.0f)); //posicion incial de la camara 
GLfloat lastX = WIDTH / 2.0; 
GLfloat lastY = HEIGHT / 2.0; 
bool keys[1024];
bool firstMouse = true;


// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(1.6f, -1.7f, 0.67f),     // Luz 1 
	glm::vec3(-0.4f, 4.46f, 0.64f),   // Luz 2 
};

float vertices[] = {
	   -0.5f, -0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  0.0f,0.0f,//frente
		0.5f, -0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  0.0f,1.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  1.0f,1.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  1.0f,1.0f,
	   -0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  1.0f,0.0f,
	   -0.5f, -0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  0.0f,0.0f,

	   -0.5f, -0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  0.0f,0.0f,//atras
		0.5f, -0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  0.0f,1.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  1.0f,1.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  1.0f,1.0f,
	   -0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  1.0f,0.0f,
	   -0.5f, -0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  0.0f,0.0f,

	   -0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  0.0f,0.0f,//izquierda
	   -0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  0.0f,1.0f,
	   -0.5f, -0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  1.0f,1.0f,
	   -0.5f, -0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  1.0f,1.0f,
	   -0.5f, -0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  1.0f,0.0f,
	   -0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  0.0f,0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  0.0f,0.0f,//derecha
		0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  0.0f,1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  1.0f,1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  1.0f,1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  1.0f,0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  0.0f,0.0f,

	   -0.5f, -0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  0.0f,0.0f,//abajo
		0.5f, -0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  0.0f,1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  1.0f,1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  1.0f,1.0f,
	   -0.5f, -0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  1.0f,0.0f,
	   -0.5f, -0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  0.0f,0.0f,

	   -0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  0.0f,0.0f,//arriba
		0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  0.0f,1.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  1.0f,1.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  1.0f,1.0f,
	   -0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  1.0f,0.0f,
	   -0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  1.0f,  0.0f,0.0f
};

GLuint indices[] = {
	0,1,2, 2,3,0,   // frente
	4,5,6, 6,7,4,   // atrás
	8,9,10, 10,11,8, // izquierda
	12,13,14, 14,15,12, // derecha
	16,17,18, 18,19,16, // arriba
	20,21,22, 22,23,20  // abajo
};


glm::vec3 Light1 = glm::vec3(0);

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active = false; //variable para activar o desactivar el sistema de iluminacion 

//Variables de Animacion 
float rotCarro = 0.0f;
float rotPuerta = 0.0f;
float rotPuertaGarage = 0.0f;
float posCortina = 0.0f;
float tamllanta = 0.0f; 
float timer = 10.0f; // Acumulador de tiempo
float FLegs = 0.0f;
float RLegs = 0.0f;
float personajePosX = 0.0f;
float personajePosZ = -2.0f;
float personajeRot = 0.0f;
float posPuertaX = 0.0f;
float posPuertaZ = 0.0f;
const float INTERVALO_CAMBIO = 0.1f; // Segundos que deben pasar antes del cambio

// Variables para Activar o Desactivas animaciones 
bool AnimCarro = false;
bool AnimPuerta = false;
bool AnimGarage = false;
bool AnimCortina = false;
bool AnimPersonaje = false; 
bool puertaArriba = false;
bool cortinaAbierta = false;
bool step = false;


// FUNCIÓN PARA CARGA DE TEXTURAS
GLuint loadTexture(const char* path) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	int textureWidth, textureHeight, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* image;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	// Diffuse map
	image = stbi_load(path, &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Error al cargar textura: " << path << std::endl;
	}
	stbi_image_free(image);
	return textureID;
}


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame


int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");


	//modelos utilizados en el exterior de la casa
	Model Piso_Exterior((char*)"Models/piso_exterior.obj");
	Model Skybox((char*)"Models/skybox.obj");

	//modelos de la casa 
	Model Casa((char*)"Models/casa.obj");
	Model Tranparente((char*)"Models/objetos_trans.obj");
	Model Puerta((char*)"Models/puerta.obj");
	Model PuertaGarage((char*)"Models/puerta_garage.obj");
	Model Cortina_Der((char*)"Models/cortina_der.obj");
	Model Cortina_Izq((char*)"Models/cortina_izq.obj");

	//Cocina
	Model Complemento_Cocina((char*)"Modelos_Cocina/complemento_cocina.obj");
	Model Maceta((char*)"Modelos_Cocina/maceta.obj");
	Model Alcohol((char*)"Modelos_Cocina/alcohol.obj");
	Model Lata((char*)"Modelos_Cocina/latas.obj");
	Model Plato((char*)"Modelos_Cocina/platos.obj");
	Model Refrigerador((char*)"Modelos_Cocina/refrigerador.obj");
	Model Estufa((char*)"Modelos_Cocina/estufa.obj");

	//Sala
	Model Complemento_Sala((char*)"Modelos_Sala/complemento_sala.obj");
	Model Cajonera((char*)"Modelos_Sala/cajonera.obj");
	Model Revista((char*)"Modelos_Sala/revista.obj");
	Model Florero((char*)"Modelos_Sala/florero.obj");
	Model Television((char*)"Modelos_Sala/television.obj");
	Model Sala((char*)"Modelos_Sala/sala.obj");
	Model Sillon((char*)"Modelos_Sala/sillon.obj");
	Model Silla((char*)"Modelos_Sala/silla.obj");
	Model Taza((char*)"Modelos_Sala/taza.obj");


	// Modelos Animacion 1
	Model Carro((char*)"carro/carro.obj");
	Model Llanta1((char*)"carro/llanta_1.obj");
	Model Llanta2((char*)"carro/llanta_2.obj");
	Model Llanta3((char*)"carro/llanta_3.obj");
	Model Llanta4((char*)"carro/llanta_4.obj");

	//modelos Aniamcion 2
	Model Torso((char*)"personaje/torso.obj");
	Model Mano_Der((char*)"personaje/mano_der.obj");
	Model Mano_Izq((char*)"personaje/mano_izq.obj");
	Model Pie_Der((char*)"personaje/pie_der.obj");
	Model Pie_Izq((char*)"personaje/pie_izq.obj");


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);


	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);



	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	float velocidad = 0.5f; // velocidad de movimiento (unidades por segundo)
	//float deltaTime = 0.0f;
	//float lastFrame = 0.0f;

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		Animation();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);

		


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);


		// Point light 1
		glm::vec3 lightColor;
		if (active) {
			lightColor = glm::vec3(0.4f, 0.4f, 0.4f); 

		}
		else {
			lightColor = glm::vec3(0.0f); // Luces apagadas
		}

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x * 0.2f, lightColor.y * 0.2f, lightColor.z * 0.2f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.032f);

		// Point light 2 - Color diferente (rojo)
		glm::vec3 lightColor2;
		if (active) {
			lightColor2 = glm::vec3(1.0f, 1.0f, 1.0f); // Color rojo fijo
		}
		else {
			lightColor2 = glm::vec3(0.0f); // Luces apagadas
		}

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), lightColor2.x * 0.2f, lightColor2.y * 0.2f, lightColor2.z * 0.2f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), lightColor2.x, lightColor2.y, lightColor2.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 1.0f, 0.5f, 0.5f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.032f);


		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));


		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.1f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		//glm::mat4 model(1);


		//Modelando los 7 objetos
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 modelTemp = glm::mat4(1.0f);
		glm::mat4 modelTemp1 = glm::mat4(1.0f);
		glm::mat4 modelTemp2 = glm::mat4(1.0f);


		//Carga de modelo 
		view = camera.GetViewMatrix();
		
		
		// Skybox
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Skybox.Draw(lightingShader);


		// piso
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso_Exterior.Draw(lightingShader);


		// ------------------------------------------------------------
		// Animacion Compleja 1 : Personaje caminando dentro de la casa
		// ------------------------------------------------------------
		// 

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		//Torso (padre)
		modelTemp2 = model = glm::translate(model, glm::vec3(personajePosX, 0.4f, personajePosZ));
		modelTemp2 = model = glm::rotate(model, glm::radians(personajeRot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Torso.Draw(lightingShader);

		//Mano Izquierda (hijo)
		model = modelTemp2;
		model = glm::rotate(model, glm::radians(FLegs), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mano_Izq.Draw(lightingShader);

		//Mano Derecha (hijo)
		model = modelTemp2;
		model = glm::rotate(model, glm::radians(FLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mano_Der.Draw(lightingShader);
		
		//Pie Izquierdo (hijo)
		model = modelTemp2;
		model = glm::scale(model, glm::vec3(0.9f));
		model = glm::translate(model, glm::vec3(0.0f, -0.1f, 0.0f));
		model = glm::rotate(model, glm::radians(RLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Pie_Izq.Draw(lightingShader);
		
		//Pie Derecho (hijo)
		model = modelTemp2;
		model = glm::scale(model, glm::vec3(0.9f));
		model = glm::translate(model, glm::vec3(0.0f, -0.1f, 0.0f));
		model = glm::rotate(model, glm::radians(RLegs), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Pie_Der.Draw(lightingShader);



		// Animacion simple 1 
		//puerta de la casa
		model = glm::mat4(1);
		model = glm::rotate(model, glm::radians(rotPuerta), glm::vec3(0.0f, 100.0f, 0.0f));
		model = glm::translate(model, glm::vec3(posPuertaX, 0.0f, posPuertaZ));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Puerta.Draw(lightingShader);

		// Animacion simple 2
		// puerta del garage
		model = glm::mat4(1);
		//model = glm::rotate(model, glm::radians(rotPuertaGarage), glm::vec3(100.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, rotPuertaGarage, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		PuertaGarage.Draw(lightingShader);

		// Animacion simple 3
		// cortina rosa derecha
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posCortina, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Cortina_Der.Draw(lightingShader);
		// cortina rosa izquierda
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-posCortina, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Cortina_Izq.Draw(lightingShader);

		
		//Casa
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Casa.Draw(lightingShader);


		// ------------------------------------------------------------
		// COCINA
		// ------------------------------------------------------------
		// Modelos complementarios de la Cocina

		// Modelos complementarios de la cocina
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Complemento_Cocina.Draw(lightingShader);

		// Maceta Cocina
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Maceta.Draw(lightingShader);

		// botellas de alcohol 
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Alcohol.Draw(lightingShader);

		// latas de la cocina
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Lata.Draw(lightingShader);

		// platos de la cocina
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Plato.Draw(lightingShader);

		// Refrigerador
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Refrigerador.Draw(lightingShader);

		// Estufa
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Estufa.Draw(lightingShader);


		// ------------------------------------------------------------
		// SALA
		// ------------------------------------------------------------
		// Modelos complementarios de la Sala
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Complemento_Sala.Draw(lightingShader);

		//cajonera
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Cajonera.Draw(lightingShader);

		//revistas
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Revista.Draw(lightingShader);

		//florero sala
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(2.8f, -0.45f, 2.9f));
		model = glm::scale(model, glm::vec3(0.03f));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Florero.Draw(lightingShader);

		//television 
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Television.Draw(lightingShader);

		//sala
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Sala.Draw(lightingShader);

		//sillon
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Sillon.Draw(lightingShader);

		//sillas de la mesa de la sala
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Silla.Draw(lightingShader);

		//taza
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Taza.Draw(lightingShader);


		//modelos con transparencia (escaleras, barandales, ventanas)
		model = modelTemp;
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Tranparente.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glBindVertexArray(0);


		// ------------------------------------------------------------
		// Animacion compleja 2: Automvil dando vuelta alrededor de la casa
		// ------------------------------------------------------------
		// 
		// carro
		model = modelTemp; // Iniciar una matriz limpia (Identidad)
		model = glm::rotate(model, glm::radians(rotCarro/2), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1); // Este uniform DEBE estar después de modelLoc
		Carro.Draw(lightingShader);


		//llanta 1 
		model = modelTemp;
		model = glm::rotate(model, glm::radians(rotCarro / 2), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(tamllanta + 0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Llanta1.Draw(lightingShader);

		//llanta 2
		model = modelTemp;
		model = glm::rotate(model, glm::radians(rotCarro / 2), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(tamllanta + 0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Llanta2.Draw(lightingShader);

		//llanta 3 
		model = modelTemp;
		model = glm::rotate(model, glm::radians(rotCarro / 2), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(tamllanta + 0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Llanta3.Draw(lightingShader);

		//llanta 4 
		model = modelTemp;
		model = glm::rotate(model, glm::radians(rotCarro / 2), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(tamllanta + 0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Llanta4.Draw(lightingShader);



		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");


		// Luz 1 
		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[0]);
		model = glm::scale(model, glm::vec3(0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Luz 2 
		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[1]);
		model = glm::scale(model, glm::vec3(0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		
		
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//glBindVertexArray(0);


		glBindVertexArray(0);
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();

	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	
	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{

	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(1.0f, 1.0f, 1.0f);

		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
		}
	}


	//Animacion Abrir la puerta
	if (keys[GLFW_KEY_Z])
	{
		AnimPuerta = !AnimPuerta;

	}

	//Animacion Abrir puerta de Garage
	if (keys[GLFW_KEY_X])
	{
		AnimGarage = !AnimGarage;

	}

	//Animacion para abrir o cerar cortinas
	if (keys[GLFW_KEY_C])
	{
		AnimCortina = !AnimCortina;

	}

	//Animacion para mover el automovil 
	if (keys[GLFW_KEY_V])
	{
		AnimCarro = !AnimCarro;

	}

	//Animacion para mover el personaje
	if (keys[GLFW_KEY_B])
	{
		AnimPersonaje = !AnimPersonaje;

	}

}
void Animation() {
	
	// animacion para hacer caminar el personaje dentro de la casa
	if (AnimPersonaje)  
	{
		if (personajeRot == 0.0f)
		{
			if (!step) //state 1
			{
				RLegs += 0.03f;
				FLegs += 0.03f;

				if (RLegs > 5.0f) { //condition 
					step = true;
				}
				personajePosZ += 0.001;

			}
			else
			{
				RLegs -= 0.03f;
				FLegs -= 0.03f;

				if (RLegs < -5.0f) { //condition 
					step = false;
				}
				personajePosZ += 0.001;

			}
		}

		if (personajePosZ > 0.5f)
		{
			personajeRot = 90.0f;
			//AnimPuerta = true;

		}

		if (personajeRot == 90.0f) 
		{
			if (!step) //state 1
			{
				RLegs += 0.03f;
				FLegs += 0.03f;


				if (RLegs > 5.0f) { //condition 
					step = true;
				}
				personajePosX += 0.001;
			}
			else
			{
				RLegs -= 0.03f;
				FLegs -= 0.03f;

				if (RLegs < -5.0f) { //condition 
					step = false;
				}
				personajePosX += 0.001;
			}

		}

		if (personajePosX > 4.60584f)
		{
			AnimPersonaje = !AnimPersonaje;
		}


	}


	
	//Animacion Abrir la puerta de la casa
	if (AnimPuerta)
	{
		
		if (rotPuerta == 0.0f)
		{
			rotPuerta = 90.0f;
			posPuertaX = -6.0f;
			posPuertaZ = -6.25f;
			AnimPuerta = !AnimPuerta;
		}
		else
		{
			rotPuerta = 0.0f;
			posPuertaX = 0.0f;
			posPuertaZ = 0.0;
			AnimPuerta = !AnimPuerta;
		}
		
	}

	//Animacion para abrir la puerta del garage
	if (AnimGarage) 
	{
		if (rotPuertaGarage >= 0.0f and puertaArriba==false)
		{
			rotPuertaGarage += 0.01;
			if (rotPuertaGarage > 2.6f)
			{
				AnimGarage = !AnimGarage;
				puertaArriba = true;
			}
		}
		else
		{
			rotPuertaGarage -= 0.01;
			if (rotPuertaGarage < 0.1f)
			{
				AnimGarage = !AnimGarage;
				puertaArriba = false;
			}
		}
	}
	
	//Animacion para abrir o cerrar las cortinas
	if (AnimCortina)
	{
		if (posCortina >= 0.0f and cortinaAbierta == false)
		{
			posCortina += 0.01;
			if (posCortina > 0.7f)
			{
				AnimCortina = !AnimCortina;
				cortinaAbierta = true;
			}
		}
		else
		{
			posCortina -= 0.01;
			if (posCortina < 0.1f)
			{
				AnimCortina = !AnimCortina;
				cortinaAbierta = false;
			}
		}
	}
	
	// Animacion para mover el automovil
	if (AnimCarro)
	{
		rotCarro += 0.1f;
			// 1. Acumular el tiempo transcurrido desde el último fotograma
			timer += deltaTime;

			tamllanta = 0.05f;
			if (timer >= INTERVALO_CAMBIO)
			{
				tamllanta = -0.05f; 
				// Reiniciar el temporizador restando el intervalo (para manejar con precisión el exceso de tiempo)
				timer -= INTERVALO_CAMBIO;
			}
	}
		
}
void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}