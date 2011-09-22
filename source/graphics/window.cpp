#include "window.hpp"

Window::Window ()
{
}

Window::~Window ()
{
}

void Window::create (const unsigned short width, const unsigned short height, const char* window_title, bool smooth_shading)
{
	this->width = width;
	this->height = height;

	/* SDL stuff */
	/*if(SDL_Init(SDL_INIT_TIMER) < 0 ||*/ if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{   
		std::cout << "Unable to init SDL " << SDL_GetError() << "\nPlease file a bug report on github.com/Compendium/chatter/issues\n";
		exit(1);
	}

	//SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	screen = SDL_SetVideoMode(width, height, 32, SDL_OPENGL | SDL_DOUBLEBUF);
	SDL_WM_SetCaption(window_title, NULL);
	/* // SDL stuff */
	
	/* OpenGL stuff */
	//const float ftobconst = 1.0f / 255.0f;
	//glClearColor(ftobconst * 146.f, ftobconst * 161.f, ftobconst * 207.f, 0);
	glClearColor(0, 0, 0, 0);

	glViewport(0, 0, width, height);

	glEnable(GL_VERTEX_ARRAY);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_CULL_FACE); //enabe culling of "invisible" faces
	//glFrontFace(GL_CCW);		//in which direction are the vertices of front facing faces arranged? --> CCW
	glCullFace(GL_BACK);		//cull back

	glEnable(GL_DEPTH_TEST); //enable depth testing, this prevents stuff being drawn over things that are nearer to the camera

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); //should be enabled by default on modern hardware

	if(smooth_shading)
	{
		glShadeModel(GL_SMOOTH);
	}

	//glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glewInit();
	if(!GLEW_VERSION_2_0)
	{
		std::cout << "Your graphics card doesn't support Opengl 2.x, and I need it to run properly. Therefore i will now exit in peace." << std::endl;
		exit(-1);
	}

	//projection = glm::perspective(80.0, double(width) / double(height), 0.1, 1000.0);
	projection = glm::ortho((double)-(width/2), (double)(width/2), (double)(height/2), (double)-(height/2), -1.0, 1.0);//glm::ortho(-1.0, 1.0, 1.0, -1.0, -1.0, 1.0);
	/* // OpenGL stuff */
}

void Window::resize (const unsigned short width, const unsigned short height)
{
	this->width = width;
	this->height = height;

	screen = SDL_SetVideoMode(width, height, 32, SDL_OPENGL | SDL_DOUBLEBUF);
	glViewport(0, 0, width, height);
	projection = glm::ortho((double)-(width/2), (double)(width/2), (double)(height/2), (double)-(height/2), -1.0, 1.0);
	//projection = glm::perspective(80.0, double(width) / double(height), 0.1, 1000.0);
}

void Window::clear ()
{
	glClear(GL_COLOR_BUFFER_BIT |	GL_DEPTH_BUFFER_BIT |	GL_STENCIL_BUFFER_BIT);
}

void Window::swapBuffers()
{
	SDL_GL_SwapBuffers();
}

void Window::swap()
{
	SDL_GL_SwapBuffers();
}

void Window::close ()
{
	SDL_Quit();
}
