#include "window.hpp"

motor::Window::Window ()
{
	block = true;
	alpha = 0.1111;
	last = 0;
}

motor::Window::~Window ()
{
}

void motor::Window::create (const unsigned short width, const unsigned short height, const char* window_title, bool smooth_shading)
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
	SDL_WM_SetCaption(window_title, NULL);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	/*SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1 ) ;
		SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 2 ) ;*/

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	//screen = SDL_SetVideoMode(width, height, 0, SDL_RESIZABLE | SDL_OPENGL);
	screen = SDL_SetVideoMode(width, height, 0, SDL_OPENGL);	

	//glEnable(GL_MULTISAMPLE);

	//glEnable(GL_MULTISAMPLE);
	//glEnable(GL_MULTISAMPLE_ARB);

	if(smooth_shading)
	{
		glShadeModel(GL_SMOOTH);
		//forget smooth shading, USE ALL THE ANTI ALIASING
		//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
		//glEnable(GL_MULTISAMPLE);
		///glEnable(GL_MULTISAMPLE_ARB);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST );
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST );
	}

	/* OpenGL stuff */
	//const float ftobconst = 1.0f / 255.0f;
	//glClearColor(ftobconst * 146.f, ftobconst * 161.f, ftobconst * 207.f, 0);
	//glClearColor(255, 255, 255, 255);
	glClearColor(0, 0, 0, 0);

	glViewport(0, 0, width, height);

	glEnable(GL_VERTEX_ARRAY);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_CULL_FACE); //enabe culling of "invisible" faces
	glFrontFace(GL_CCW);		//in which direction are the vertices of front facing faces arranged? --> CCW
	glCullFace(GL_BACK);		//cull back

	glEnable(GL_DEPTH_TEST);

	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); //should be enabled by default on modern hardware

	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);

	glewInit();
	if(!GLEW_VERSION_2_0)
	{
		std::cout << "Couldn't get a OpenGL 2.0 (or better) context" << std::endl;
		exit(-1);
	}

	/*
#if defined linux || __GNUC__
if(glx::glxewIsSupported("GLX_MESA_swap_control")) //linux - X11 maybe also MacOSX?
{
glx::glXSwapIntervalMESA(1);
}
else if(glx::glxewIsSupported("GLX_SGI_swap_control")) //linux - X11 / SGI - but doesn't work on my platform. maybe MacOSX?
{
glx::glXSwapIntervalSGI(1);
}

#elif defined _WIN32 || WIN32
if(wgl::glewIsSupported("WGL_EXT_swap_control")) //windows
{
wgl::wglSwapIntervalEXT(0);
}
#endif
else
{
cout << "Can't disable V-Sync, so you may experience some FPS spikes" << endl;
clog << "(supported OpenGL extensions are:\n";
clog << glGetString(GL_EXTENSIONS);
clog << "\n---\n";
	//clog << glXQueryExtensionsString(glXGetCurrentDisplay(), 0);
	clog << ")\n";
	}*/

//projection = glm::perspective(80.0, double(width) / double(height), 0.1, 1000.0);
ortho_projection = glm::ortho(0.0f, float(width), float(height), 0.0f, -10.0f, 1.0f);
//ortho_projection = glm::ortho(0.0f, float(width), 0.0f, float(height), -1.0f, 1.0f);
//ortho_projection = glm::ortho(0.0f, float(width), -float(height), 0.0f, -1.0f, 1.0f);
perspective_projection = glm::perspective(70.0, double(width) / double(height), 0.0, 1000.0);
/* // OpenGL stuff */
}

void motor::Window::resize (const unsigned short width, const unsigned short height)
{
	this->width = width;
	this->height = height;

	screen = SDL_SetVideoMode(width, height, 32, SDL_OPENGL | SDL_DOUBLEBUF);
	glViewport(0, 0, width, height);
	//ortho_projection = glm::ortho(-((double)width/2), ((double)width/2), ((double)height/2), -((double)height/2), -1.0, 1.0);
	ortho_projection = glm::ortho(0.0f, float(width), float(height), 0.0f, -100.0f, 1.0f);
	//ortho_projection = glm::ortho(0.0f, float(width), 0.0f, float(height), -1.0f, 1.0f);
	//ortho_projection = glm::ortho(0.0f, float(width), -float(height), 0.0f, -1.0f, 1.0f);
	perspective_projection = glm::perspective(70.0, double(width) / double(height), 0.1, 1000.0);
}

void motor::Window::clear ()
{
	glClear(GL_COLOR_BUFFER_BIT |	GL_DEPTH_BUFFER_BIT |	GL_STENCIL_BUFFER_BIT);
	//frametimelast = SDL_GetTicks();
}

void motor::Window::swap()
{
	sw.stop();
	frametime = sw.get();
	//cout << "frametime is " << frametime << '\n';
	sw.start();

	SDL_GL_SwapBuffers();
}

void motor::Window::close ()
{
	SDL_Quit();
}
