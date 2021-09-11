
#include "Keys.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @005775e0>
Gods98::Key_Globs & Gods98::keyGlobs = *(Gods98::Key_Globs*)0x005775e0;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00485ce0>
void __cdecl Gods98::Keys_Initialise(void)
{
	log_firstcall();

	for (uint32 loop=0 ; loop<256 ; loop++) {
		keyGlobs.keyName[loop] = nullptr;
	}

	Key_RegisterName(KEY_ESCAPE);
	Key_RegisterName(KEY_ONE);
	Key_RegisterName(KEY_TWO);
	Key_RegisterName(KEY_THREE);
	Key_RegisterName(KEY_FOUR);
	Key_RegisterName(KEY_FIVE);
	Key_RegisterName(KEY_SIX);
	Key_RegisterName(KEY_SEVEN);
	Key_RegisterName(KEY_EIGHT);
	Key_RegisterName(KEY_NINE);
	Key_RegisterName(KEY_ZERO);
	Key_RegisterName(KEY_MINUS);
	Key_RegisterName(KEY_EQUALS);
	Key_RegisterName(KEY_BACKSPACE);
	Key_RegisterName(KEY_TAB);
	Key_RegisterName(KEY_Q);
	Key_RegisterName(KEY_W);
	Key_RegisterName(KEY_E);
	Key_RegisterName(KEY_R);
	Key_RegisterName(KEY_T);
	Key_RegisterName(KEY_Y);
	Key_RegisterName(KEY_U);
	Key_RegisterName(KEY_I);
	Key_RegisterName(KEY_O);
	Key_RegisterName(KEY_P);
	Key_RegisterName(KEY_LEFTBRACE);
	Key_RegisterName(KEY_RIGHTBRACE);
	Key_RegisterName(KEY_RETURN);
	Key_RegisterName(KEY_LEFTCTRL);
	Key_RegisterName(KEY_A);
	Key_RegisterName(KEY_S);
	Key_RegisterName(KEY_D);
	Key_RegisterName(KEY_F);
	Key_RegisterName(KEY_G);
	Key_RegisterName(KEY_H);
	Key_RegisterName(KEY_J);
	Key_RegisterName(KEY_K);
	Key_RegisterName(KEY_L);
	Key_RegisterName(KEY_SEMICOLON);
	Key_RegisterName(KEY_AT);
	Key_RegisterName(KEY_RSINGLEQUOTE);
	Key_RegisterName(KEY_LEFTSHIFT);
	Key_RegisterName(KEY_HASH);
	Key_RegisterName(KEY_Z);
	Key_RegisterName(KEY_X);
	Key_RegisterName(KEY_C);
	Key_RegisterName(KEY_V);
	Key_RegisterName(KEY_B);
	Key_RegisterName(KEY_N);
	Key_RegisterName(KEY_M);
	Key_RegisterName(KEY_LEFTARROW);
	Key_RegisterName(KEY_RIGHTARROW);
	Key_RegisterName(KEY_QUESTIONMARK);
	Key_RegisterName(KEY_RIGHTSHIFT);
	Key_RegisterName(KEYPAD_ASTERISK);
	Key_RegisterName(KEY_ALT);
	Key_RegisterName(KEY_SPACE);
	Key_RegisterName(KEY_CAPLOCK);
	Key_RegisterName(KEY_F1);
	Key_RegisterName(KEY_F2);
	Key_RegisterName(KEY_F3);
	Key_RegisterName(KEY_F4);
	Key_RegisterName(KEY_F5);
	Key_RegisterName(KEY_F6);
	Key_RegisterName(KEY_F7);
	Key_RegisterName(KEY_F8);
	Key_RegisterName(KEY_F9);
	Key_RegisterName(KEY_F10);
	Key_RegisterName(KEYPAD_NUMLOCK);
	Key_RegisterName(KEY_SCROLLLOCK);
	Key_RegisterName(KEYPAD_7);
	Key_RegisterName(KEYPAD_8);
	Key_RegisterName(KEYPAD_9);
	Key_RegisterName(KEYPAD_MINUS);
	Key_RegisterName(KEYPAD_4);
	Key_RegisterName(KEYPAD_5);
	Key_RegisterName(KEYPAD_6);
	Key_RegisterName(KEYPAD_PLUS);
	Key_RegisterName(KEYPAD_1);
	Key_RegisterName(KEYPAD_2);
	Key_RegisterName(KEYPAD_3);
	Key_RegisterName(KEYPAD_0);
	Key_RegisterName(KEYPAD_DELETE);
	Key_RegisterName(KEY_BACKSLASH);
	Key_RegisterName(KEY_F11);
	Key_RegisterName(KEY_F12);
	Key_RegisterName(KEYPAD_ENTER);
	Key_RegisterName(KEY_RIGHTCTRL);
	Key_RegisterName(KEYPAD_FORWARDSLASH);
	Key_RegisterName(KEY_PRINTSCREEN);
	Key_RegisterName(KEY_ALTGR);
	Key_RegisterName(KEY_HOME);
	Key_RegisterName(KEY_CURSORUP);
	Key_RegisterName(KEY_PGUP);
	Key_RegisterName(KEY_CURSORLEFT);
	Key_RegisterName(KEY_CURSORRIGHT);
	Key_RegisterName(KEY_END);
	Key_RegisterName(KEY_CURSORDOWN);
	Key_RegisterName(KEY_PGDN);
	Key_RegisterName(KEY_INSERT);
	Key_RegisterName(KEY_DELETE);
}

// <LegoRR.exe @004860f0>
bool32 __cdecl Gods98::Key_Find(const char* name, OUT Keys* keyID)
{
	log_firstcall();

	for (uint32 loop = 0; loop < 256; loop++) {
		if (keyGlobs.keyName[loop] && ::_stricmp(keyGlobs.keyName[loop], name) == 0) {
			*keyID = (Keys)loop;
			return true;
		}
	}

	return false;
}

// <missing>
const char* __cdecl Gods98::Key_GetName(Keys key)
{
	return keyGlobs.keyName[key];
}

#pragma endregion