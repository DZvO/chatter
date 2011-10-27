#include "window.hpp"

motor::Window::Window ()
{
	block = true;
	alpha = 0.06;
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
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 0); // has no effect on my platform
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	/*SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1 ) ;
	SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 2 ) ;*/

	screen = SDL_SetVideoMode(width, height, 0, SDL_OPENGL);
	//glEnable(GL_MULTISAMPLE);
	//glEnable(GL_MULTISAMPLE_ARB);

	if(smooth_shading)
	{
		//glShadeModel(GL_SMOOTH);
		//forget smooth shading, USE ALL THE ANTI ALIASING
		//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
		//glEnable(GL_MULTISAMPLE);
		///glEnable(GL_MULTISAMPLE_ARB);
		//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST );
		//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST );

		//glEnable(GL_LINE_SMOOTH);
		//glEnable(GL_POLYGON_SMOOTH);
	}



	/* OpenGL stuff */
	//const float ftobconst = 1.0f / 255.0f;
	//glClearColor(ftobconst * 146.f, ftobconst * 161.f, ftobconst * 207.f, 0);
	glClearColor(0, 0, 0, 0);

	glViewport(0, 0, width, height);

	glEnable(GL_VERTEX_ARRAY);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_CULL_FACE); //enabe culling of "invisible" faces
	glFrontFace(GL_CCW);		//in which direction are the vertices of front facing faces arranged? --> CCW
	glCullFace(GL_BACK);		//cull back

	glEnable(GL_DEPTH_TEST); //enable depth testing, this prevents stuff being drawn over things that are nearer to the camera

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); //should be enabled by default on modern hardware


	//glEnable(GL_ALPHA_TEST);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glewInit();
	if(!GLEW_VERSION_2_0)
	{
		std::cout << "Your graphics card doesn't support Opengl 2.x, and I need it to run properly. Therefore i will now exit in peace." << std::endl;
		std::cout << "(Or there was an error initializing glew, which is equally bad!)" << std::endl;
		exit(-1);
	}


#if defined linux || __GNUC__
	if(glx::glxewIsSupported("GLX_MESA_swap_control")) //linux - X11 maybe also MacOSX?
	{
		glx::glXSwapIntervalMESA(0);
	}
	else if(glx::glxewIsSupported("GLX_SGI_swap_control")) //linux - X11 / SGI - but doesn't work on my platform. maybe MacOSX?
	{
		glx::glXSwapIntervalSGI(0);
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
	}

	//projection = glm::perspective(80.0, double(width) / double(height), 0.1, 1000.0);
	ortho_projection = glm::ortho((double)-(width/2), (double)(width/2), (double)(height/2), (double)-(height/2), -1.0, 1.0);//glm::ortho(-1.0, 1.0, 1.0, -1.0, -1.0, 1.0);
	perspective_projection = glm::perspective(70.0, double(width) / double(height), 1.0, 1000.0);
	/* // OpenGL stuff */
}

void motor::Window::resize (const unsigned short width, const unsigned short height)
{
	this->width = width;
	this->height = height;

	screen = SDL_SetVideoMode(width, height, 32, SDL_OPENGL | SDL_DOUBLEBUF);
	glViewport(0, 0, width, height);
	ortho_projection = glm::ortho(-((double)width/2), ((double)width/2), ((double)height/2), -((double)height/2), -1.0, 1.0);
	perspective_projection = glm::perspective(70.0, double(width) / double(height), 1.0, 1000.0);
}

void motor::Window::clear ()
{
	glClear(GL_COLOR_BUFFER_BIT |	GL_DEPTH_BUFFER_BIT |	GL_STENCIL_BUFFER_BIT);
}

void motor::Window::swap()
{
	getticks = SDL_GetTicks();
	frametimedelta = getticks - frametimelast;
	frametimelast = getticks;
	frametime = alpha * frametimedelta + (1.0 - alpha) * frametime;
	framespersecond = 1000.0 / frametime;
	cout << frametime << '\n';
	cout << framespersecond << '\n';
	cout << endl;
	if((int)framespersecond >= 120-1)
		SDL_Delay(8);

	SDL_GL_SwapBuffers();
}

void motor::Window::close ()
{
	SDL_Quit();
}
