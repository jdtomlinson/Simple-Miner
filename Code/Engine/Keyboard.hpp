#ifndef include_Keyboard
#define include_Keyboard
#pragma once

//-----------------------------------------------------------------------------------------------
#include <windows.h>


//-----------------------------------------------------------------------------------------------
const int NUM_VIRTUAL_KEYS = 256;


//-----------------------------------------------------------------------------------------------
enum keys {
	KEY_0 = '0',
	KEY_1 = '1',
	KEY_2 = '2',
	KEY_3 = '3',
	KEY_4 = '4',
	KEY_5 = '5',
	KEY_6 = '6',
	KEY_7 = '7',
	KEY_8 = '8',
	KEY_9 = '9',
	KEY_A = 'A',
	KEY_B = 'B',
	KEY_C = 'C',
	KEY_D = 'D',
	KEY_E = 'E',
	KEY_F = 'F',
	KEY_G = 'G',
	KEY_H = 'H',
	KEY_I = 'I',
	KEY_J = 'J',
	KEY_K = 'K',
	KEY_L = 'L',
	KEY_M = 'M',
	KEY_N = 'N',
	KEY_O = 'O',
	KEY_P = 'P',
	KEY_Q = 'Q',
	KEY_R = 'R',
	KEY_S = 'S',
	KEY_T = 'T',
	KEY_U = 'U',
	KEY_V = 'V',
	KEY_W = 'W',
	KEY_X = 'X',
	KEY_Y = 'Y',
	KEY_Z = 'Z',
	KEY_SPACE = VK_SPACE,
};


//-----------------------------------------------------------------------------------------------
class Keyboard
{
public:
	Keyboard();
	void SetKeyUp( unsigned char setKey );
	void SetKeyDown( unsigned char setKey );
	bool IsKeyPressedDown( keys checkKey );
	bool WasKeyPressedDown( keys checkKey );
	void Update();

	bool		m_isKeyDown[ NUM_VIRTUAL_KEYS ];
	bool		m_wasKeyDown[ NUM_VIRTUAL_KEYS ];
};


#endif // include_Keyboard