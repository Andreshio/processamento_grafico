#include "Includes.h"
#define SPRITE
#include "Sprite.cpp"

//#include "<Windows.h>"

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"


//#define EXIT_FAILURE -1
//#define EXIT_SUCCESS 0

//Definicao das varives globais
int NEW_WIDTH = 800;
int NEW_HEIGHT = 600;

int WIDTH = 800;
int HEIGHT = 600;

float xCentro = 0.0f;
float yCentro = 0.0f;

unsigned int texture1;
unsigned int texture2;

vector<Sprite *> layers;

glm::mat4 matrix_translaction = glm::mat4(1);
glm::mat4 matrix_rotation = glm::mat4(1);
glm::mat4 matrix_scala = glm::mat4(1);
glm::mat4 matrix = matrix_translaction * matrix_rotation * matrix_scala;

float value_scala = 1.2f;
float value_move = 3.0f;

//Define escala dos objetos
void window_content_scale_callback(GLFWwindow* window, float xscale, float yscale)
{
	//set_interface_scale(xscale, yscale);
}

//Define acoes do mouse
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			// pega pos click
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);

			//Realiza a proporcao do clique para projecao original
			xpos = WIDTH * xpos / NEW_WIDTH;
			ypos = HEIGHT * ypos / NEW_HEIGHT;

			matrix_translaction = glm::translate(matrix_translaction, glm::vec3((float)(xpos - xCentro), (float)(ypos - yCentro), 0.0f));
			matrix = matrix_translaction * matrix_rotation;
			xCentro = xpos;
			yCentro = ypos;


		}
	}
}

//Define acoes do redimensionamento da tela
void window_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

	NEW_WIDTH = width;
	NEW_HEIGHT = height;
}

//Define acoes do teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	if ((action == GLFW_REPEAT || action == GLFW_PRESS)) {
		if (key == GLFW_KEY_U) {
			matrix_rotation = glm::rotate(matrix_rotation, glm::radians(-20.0f), glm::vec3(0, 0, 1));
			matrix = matrix_translaction * matrix_rotation * matrix_scala;
		}
		else if (key == GLFW_KEY_R) {
			matrix_rotation = glm::rotate(matrix_rotation, glm::radians(20.0f), glm::vec3(0, 0, 1));
			matrix = matrix_translaction * matrix_rotation * matrix_scala;
		}
		else if (key == GLFW_KEY_C) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture1);
		}
		else if (key == GLFW_KEY_V) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture2);
		}
		else if (key == GLFW_KEY_RIGHT) {
			matrix_translaction = glm::translate(matrix_translaction,
				glm::vec3(value_move, 0.0f, 0.0f));
			matrix = matrix_translaction * matrix_rotation * matrix_scala;
			xCentro = xCentro + value_move;
		}
		else if (key == GLFW_KEY_LEFT) {
			matrix_translaction = glm::translate(matrix_translaction,
				glm::vec3(-value_move, 0.0f, 0.0f));
			matrix = matrix_translaction * matrix_rotation * matrix_scala;
			xCentro = xCentro - value_move;
		}
		else if (key == GLFW_KEY_DOWN) {
			matrix_translaction = glm::translate(matrix_translaction,
				glm::vec3(0.0f, value_move, 0.0f));
			matrix = matrix_translaction * matrix_rotation * matrix_scala;
			yCentro = yCentro + value_move;
		}
		else if (key == GLFW_KEY_UP) {
			matrix_translaction = glm::translate(matrix_translaction,
				glm::vec3(0.0f, -value_move, 0.0f));
			matrix = matrix_translaction * matrix_rotation * matrix_scala;
			yCentro = yCentro - value_move;
		}
		else if (key == GLFW_KEY_ESCAPE) {
			glfwSetWindowShouldClose(window, true);
		}
		else if (key == GLFW_KEY_KP_ADD)
		{
			matrix_scala = glm::scale(matrix_scala, glm::vec3(value_scala, value_scala, value_scala));
			matrix = matrix_translaction * matrix_rotation * matrix_scala;
		}
		else if (key == GLFW_KEY_KP_SUBTRACT)
		{
			matrix_scala = glm::scale(matrix_scala, glm::vec3(1.0f / value_scala, 1.0f / value_scala, 1.0f / value_scala));
			matrix = matrix_translaction * matrix_rotation * matrix_scala;
		}
	}
}

unsigned int create_textures(const char* filename) {
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);//ou GL_REPEAT
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//ou GL_LINEAR
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// load and generate the texture
	int tex_width, tex_height, nrChanel;
	unsigned char *data = SOIL_load_image(filename, &tex_width, &tex_height, 0, SOIL_LOAD_RGBA);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf("%s %s", "Failed to load texture", filename);
	}
	SOIL_free_image_data(data);
	return texture;
}

GLuint create_shader(GLenum shaderType, const char* shaderSource) {
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);
	return shader;
}

GLFWwindow* createWindow() {
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Triângulo Maluco", NULL, NULL);
	if (window == NULL) {
		printf("%s", "Houve uma falha ao criar a janela GLFW");
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);
	return window;
}

void colocarObjetoEmPosicaoAdequadaNoEspaco() {
	//Fazer a translacao no meio da tela
	double tmpxpos = WIDTH / 2;
	double tmpypos = HEIGHT / 2;
	matrix_translaction = glm::translate(matrix_translaction, glm::vec3((float)(tmpxpos), (float)(tmpypos), 0.0f));
	matrix = matrix_translaction * matrix_rotation;
	xCentro = tmpxpos;
	yCentro = tmpypos;
	// e rotaciona 180 degress
	matrix_rotation = glm::rotate(matrix_rotation, glm::radians(180.0f), glm::vec3(0, 0, 1));
	matrix = matrix_translaction * matrix_rotation;
}

int main() {
	if (!glfwInit()) {
		fprintf(stderr, "ERRO: não é possivel iniciar GLFW3\n");
		return 1;
	}

	/* Caso necess·rio, definiÁıes especÌficas para SOs, p. e. Apple OSX  Definir como 3.2 para Apple OS X */
#ifdef __APPLE__
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif // !APPLE

	GLFWwindow *window = createWindow();

	// inicia manipulador da extens„o GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	float vertices[] = {
		// positions                // colors                // texture coords
		-400.0f, -300.0f, 0.0f,     1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top left
		-400.0f, 300.0f, 0.0f,      0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom left
		400.0f, 300.0f, 0.0f,       0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom right
		400.0f, 300.0f, 0.0f,       0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom right
		400.0f, -300.0f, 0.0f,      1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // top right
		-400.0f, -300.0f, 0.0f,     1.0f, 0.0f, 1.0f,   1.0f, 1.0f, // top left
	};

	unsigned int indices[] = {
			0, 1, 2,   // first triangle
			3, 4, 5    // second triangle
	};

	unsigned int EBO;
	glGenBuffers(1, &EBO);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glBindVertexArray(VAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	/*
	const char* vertexShaderSource =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;"
		"layout (location = 1) in vec3 aColor;"
		"layout (location = 2) in vec2 aTexCoord;"
		"out vec3 outColor;"
		"out vec2 TexCoord;"
		"uniform mat4 matrix;"
		"uniform mat4 proj;"
		"void main() {"
		"   gl_Position = proj * matrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
		"   outColor = aColor;"
		"   TexCoord = aTexCoord;"
		"}";

	const char* fragmentShaderSource =
		"#version 330 core\n"
		"out vec4 frag_color;"
		"in vec3 outColor;"
		"in vec2 TexCoord;"
		"uniform sampler2D texture_a;"
		"uniform sampler2D texture_b;"
		"void main() {"
		"   frag_color = texture(texture_a, TexCoord) * vec4(outColor, 1.0);"
		"}";
	*/

	const char* vertexShaderSource =
		"#version 430\n"
		"layout (location = 0) in vec2 aPos;"
		"layout (location = 2) in vec2 aTexCoord;"
		"out vec2 TexCoord;"
		"uniform mat4 matrix;"
		"uniform mat4 proj;"
		"uniform float layer_z;"
		"void main() {"
		"   gl_Position = proj * matrix * vec4(aPos,layer_z,1.0);"  //projeção * matrix de movientos * pontos iniciais
		"   TexCoord = aTexCoord;"
		"}";


	const char* fragmentShaderSource =
		"#version 410\n"
		"out vec4 frag_color;"
		"in vec2 TexCoord;"

		"uniform sampler2D sprite;"
		"uniform float offsetX;"
		"uniform float offsetY;"

		"void main() {"
		"	vec4 texel = texture(sprite,vec2(TexCoord.x + offsetX, TexCoord.y + offsetY));"
		"	frag_color = texel;"
		"}";

	unsigned int vertexShader = create_shader(GL_VERTEX_SHADER, vertexShaderSource);
	GLuint fragmentShader = create_shader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// posições
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//texture1 = create_textures("batman.png");
	//glUniform1i(glGetUniformLocation(shaderProgram, "texture_a"), 0);

	//texture2 = create_textures("container.jpg");
	//glUniform1i(glGetUniformLocation(shaderProgram, "texture_b"), 1);


	// X e Y são entre -1 E 1
	glEnable(GL_DEPTH_TEST);

	Sprite *t1 = new Sprite("resource/backblue.png", 0.4f, 0.0f, -0.4f);
	Sprite *t0 = new Sprite("resource/batman2.png", 0.5f, 0.0f, -0.6f);

	layers.push_back(t1);
	layers.push_back(t0);
	

	//GLint location = glGetUniformLocation(shaderProgram, "inColor");
	//glUniform4f(location, 0.0f, 0.0f, 1.0f, 1.0f);



	//AO INCIAR A MASCARA ESTA EM POSICAO 0,0 SEU CENTRO
	//A TEXTURA PODE ESTAR VIRARA
	//ALGUNS COMANDOS SAO EXECUTADOS
	colocarObjetoEmPosicaoAdequadaNoEspaco();


	//--------------------------------------------------------------------------//
	//-------------------		CALLBACKS		--------------------------------//

	// esta para quando clicar com o mouse
	//glfwSetMouseButtonCallback(window, mouse_button_callback);
	
	// esta para quando clicar uma tecla
	//glfwSetKeyCallback(window, key_callback);
	// esta para quando redimensionar a tela
	//glfwSetWindowSizeCallback(window, window_size_callback);
	// esta para escalar os objetos
	//glfwSetWindowContentScaleCallback(window, window_content_scale_callback);
	glm::mat4 projection =
		glm::ortho(0.0f, (float)WIDTH, (float)HEIGHT, 0.0f, -1.0f, 1.0f);

	

	//Starta texture
	// glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture1);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Define shaderProgram como o shader a ser utilizado
		glUseProgram(shaderProgram);

		// glm projecao
		glUniformMatrix4fv(
			glGetUniformLocation(shaderProgram, "proj"), 1,
			GL_FALSE, glm::value_ptr(projection));


		glUniformMatrix4fv(
			glGetUniformLocation(shaderProgram, "matrix"), 1,
			GL_FALSE, glm::value_ptr(matrix));

		// Define vao como verte array atual
		glBindVertexArray(VAO);
		for (int i = 0; i < 2; i++) {
			glUniform1f(
				glGetUniformLocation(shaderProgram, "offsetX"), layers[i]->offsetX);
			glUniform1f(
				glGetUniformLocation(shaderProgram, "offsetY"), layers[i]->offsetY);
			glUniform1f(
				glGetUniformLocation(shaderProgram, "layer_z"), layers[i]->z);
			// bind Texture
			glActiveTexture(GL_TEXTURE0);

			//glEnable(GL_TEXTURE_2D);
			glEnable(GL_DEPTH_TEST);

			glBindTexture(GL_TEXTURE_2D, layers[i]->textureId);
			glUniform1i(glGetUniformLocation(shaderProgram, "sprite"), 0);

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		
		

		// desenha pontos a partir do p0 e 6 no total do VAO atual com o shader
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwWaitEvents();
		glfwPollEvents();
		glfwSwapBuffers(window);

	}

	delete t0;
	delete t1;

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// encerra contexto GL e outros recursos da GLFW
	glfwTerminate();

	return 0;
}
