#ifndef _INPUT_HPP
#define _INPUT_HPP

#include <iostream>
#include <SDL/SDL.h>
#include "graphics/window.hpp"
#include <bitset>
#include <map>
#include <fstream>
#include <string>
#include <sstream>
#include <functional>

class Input
{
	public:
		enum Key
		{
			//alphabet
			kA = SDLK_a,
			kB = SDLK_b,
			kC = SDLK_c,
			kD = SDLK_d,
			kE = SDLK_e,
			kF = SDLK_f,
			kG = SDLK_g,
			kH = SDLK_h,
			kI = SDLK_i,
			kJ = SDLK_j,
			kK = SDLK_k,
			kL = SDLK_l,
			kM = SDLK_m,
			kN = SDLK_n,
			kO = SDLK_o,
			kP = SDLK_p,
			kQ = SDLK_q,
			kR = SDLK_r,
			kS = SDLK_s,
			kT = SDLK_t,
			kU = SDLK_u,
			kV = SDLK_v,
			kW = SDLK_w,
			kX = SDLK_x,
			kY = SDLK_y,
			kZ = SDLK_z,

			//numeric
			k0 = SDLK_0,
			k1 = SDLK_1,
			k2 = SDLK_2,
			k3 = SDLK_3,
			k4 = SDLK_4,
			k5 = SDLK_5,
			k6 = SDLK_6,
			k7 = SDLK_7,
			k8 = SDLK_8,
			k9 = SDLK_9,

			//command keys
			kEnter = SDLK_RETURN,
			kBackspace = SDLK_BACKSPACE,
			kTab = SDLK_TAB,
			kLShift = SDLK_LSHIFT,
			kRShift = SDLK_RSHIFT,
			kArrowLeft = SDLK_LEFT,
			kArrowRight = SDLK_RIGHT,
			kArrowUp = SDLK_UP,
			kArrowDown = SDLK_DOWN
		};
	public:
		static Input * getInstance ()
		{
			static Input instance;
			return &instance;
		}

		/* Text mode stuff */
		void enableTextmode();	//keyboard layout translation for example from standard QWERTY to DVORAK, so the user can type in his favourite layout
		void disableTextmode();
		unsigned char getChar();
		void enableKeyRepeat();
		void disableKeyRepeat();
		/* --------------- */

		int refresh();

		bool isPressed (Key k);
		bool isPressed (const std::string &);
		bool isPressedSym (Key k);
		bool isPressedSym (const std::string &);

		bool isReleased (Key k);
		bool isReleasedSym (Key k);

		void requestClose();
		bool closeRequested();

		void addMapping (std::string name, Input::Key k);
		void addMapping (std::string name, SDLKey k);
		Input::Key getMapping (std::string name);

		void loadKeymapping (std::string path);
		void saveKeymapping (std::string path);

		void setMouseCallback(void*, void(*)(void*,int,int,bool));

		unsigned short getMouseX ();
		unsigned short getMouseY ();

	private:
		Input();
		~Input();
		Input(Input const&);
		void operator=(Input const &);

		std::map<std::string, unsigned int> keymap;
		//std::bitset<sizeof(char16_t) * 8> keystate;
		bool close_requested;
		bool textmode;

		//#ifdef USE_SDL
		SDL_Event event;
		//unsigned char* keystate;
		//#endif
		//void (*mousecallback)(int, int, bool); //meh..
		std::function< void (void*, int, int, bool) > mousecallback;
		void * inst;
};


#endif
