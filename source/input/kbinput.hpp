#ifndef _EVENTINPUT_HPP
#define _EVENTINPUT_HPP

#include <SDL/SDL.h>

typedef enum Key;

class KbInput
{
	public:
		KbInput();
		~KbInput();

		/* Text mode stuff */
		void enableTextmode();	//keyboard layout translation
		void disableTextmode();
		char getChar();
		/* --------------- */

		/* Handle events one by one */
		bool inputAvailable();
		Key getKey();
		/* ------------------------ */

		const char* getKeyStates();

		bool isPressed(Key k);
		bool isReleased(Key k);

		bool closeReq();

	private:
		int refresh();
		bool close_requested;		
		SDL_Event event;
		char* keystate;
};

typedef enum Key
{
	null = 0,
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
	kW = SDLK_W,
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
	kReturn = SDLK_RETURN,
	kBackspace = SDLK_BACKSPACE,
	kTab = SDLK_TAB,
	kLShift = SDLK_LSHIFT,
	kRShift = SDLK_RSHIFT
} Key;

#endif
