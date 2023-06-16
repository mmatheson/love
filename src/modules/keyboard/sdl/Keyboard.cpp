/**
 * Copyright (c) 2006-2023 LOVE Development Team
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 **/

#include "Keyboard.h"

#include "window/Window.h"

namespace love
{
namespace keyboard
{
namespace sdl
{

Keyboard::Keyboard()
    : key_repeat(false)
{
}

const char *Keyboard::getName() const { return "love.keyboard.sdl"; }

void Keyboard::setKeyRepeat(bool enable) { key_repeat = enable; }

bool Keyboard::hasKeyRepeat() const { return key_repeat; }

bool Keyboard::isDown(const std::vector<Key> &keylist) const
{
  const Uint8 *state = SDL_GetKeyboardState(nullptr);

  for (Key key : keylist)
  {
    SDL_Scancode scancode = SDL_GetScancodeFromKey(keymap[key]);

    if (state[scancode])
      return true;
  }

  return false;
}

bool Keyboard::isScancodeDown(const std::vector<Scancode> &scancodelist) const
{
  const Uint8 *state = SDL_GetKeyboardState(nullptr);

  for (Scancode scancode : scancodelist)
  {
    SDL_Scancode sdlcode = SDL_SCANCODE_UNKNOWN;

    if (scancodes.find(scancode, sdlcode) && state[sdlcode])
      return true;
  }

  return false;
}

Keyboard::Key Keyboard::getKeyFromScancode(Scancode scancode) const
{
  SDL_Scancode sdlscancode = SDL_SCANCODE_UNKNOWN;
  scancodes.find(scancode, sdlscancode);

  SDL_Keycode sdlkey = SDL_GetKeyFromScancode(sdlscancode);

  for (int i = 0; i < KEY_MAX_ENUM; i++)
  {
    if (keymap[i] == sdlkey)
      return (Key) i;
  }

  return KEY_UNKNOWN;
}

Keyboard::Scancode Keyboard::getScancodeFromKey(Key key) const
{
  Scancode scancode = SCANCODE_UNKNOWN;

  if (key != KEY_MAX_ENUM)
  {
    SDL_Keycode sdlkey = keymap[key];

    SDL_Scancode sdlscancode = SDL_GetScancodeFromKey(sdlkey);
    scancodes.find(sdlscancode, scancode);
  }

  return scancode;
}

void Keyboard::setTextInput(bool enable)
{
  if (enable)
    SDL_StartTextInput();
  else
    SDL_StopTextInput();
}

void Keyboard::setTextInput(bool enable, double x, double y, double w, double h)
{
  // SDL_SetTextInputRect expects coordinates in window-space but setTextInput
  // takes pixels, so we should convert.
  auto window = Module::getInstance<window::Window>(M_WINDOW);
  if (window)
  {
    window->DPIToWindowCoords(&x, &y);
    window->DPIToWindowCoords(&w, &h);
  }

  SDL_Rect rect = {(int) x, (int) y, (int) w, (int) h};
  SDL_SetTextInputRect(&rect);

  setTextInput(enable);
}

bool Keyboard::hasTextInput() const { return SDL_IsTextInputActive() != SDL_FALSE; }

bool Keyboard::hasScreenKeyboard() const { return SDL_HasScreenKeyboardSupport() != SDL_FALSE; }

bool Keyboard::getConstant(Scancode in, SDL_Scancode &out) { return scancodes.find(in, out); }

bool Keyboard::getConstant(SDL_Scancode in, Scancode &out) { return scancodes.find(in, out); }

const SDL_Keycode *Keyboard::createKeyMap()
{
  // Array must be static so its lifetime continues once the function returns.
  static SDL_Keycode k[Keyboard::KEY_MAX_ENUM] = {SDLK_UNKNOWN};

  k[Keyboard::KEY_UNKNOWN] = SDLK_UNKNOWN;

  k[Keyboard::KEY_RETURN] = SDLK_RETURN;
  k[Keyboard::KEY_ESCAPE] = SDLK_ESCAPE;
  k[Keyboard::KEY_BACKSPACE] = SDLK_BACKSPACE;
  k[Keyboard::KEY_TAB] = SDLK_TAB;
  k[Keyboard::KEY_SPACE] = SDLK_SPACE;
  k[Keyboard::KEY_EXCLAIM] = SDLK_EXCLAIM;
  k[Keyboard::KEY_QUOTEDBL] = SDLK_QUOTEDBL;
  k[Keyboard::KEY_HASH] = SDLK_HASH;
  k[Keyboard::KEY_PERCENT] = SDLK_PERCENT;
  k[Keyboard::KEY_DOLLAR] = SDLK_DOLLAR;
  k[Keyboard::KEY_AMPERSAND] = SDLK_AMPERSAND;
  k[Keyboard::KEY_QUOTE] = SDLK_QUOTE;
  k[Keyboard::KEY_LEFTPAREN] = SDLK_LEFTPAREN;
  k[Keyboard::KEY_RIGHTPAREN] = SDLK_RIGHTPAREN;
  k[Keyboard::KEY_ASTERISK] = SDLK_ASTERISK;
  k[Keyboard::KEY_PLUS] = SDLK_PLUS;
  k[Keyboard::KEY_COMMA] = SDLK_COMMA;
  k[Keyboard::KEY_MINUS] = SDLK_MINUS;
  k[Keyboard::KEY_PERIOD] = SDLK_PERIOD;
  k[Keyboard::KEY_SLASH] = SDLK_SLASH;
  k[Keyboard::KEY_0] = SDLK_0;
  k[Keyboard::KEY_1] = SDLK_1;
  k[Keyboard::KEY_2] = SDLK_2;
  k[Keyboard::KEY_3] = SDLK_3;
  k[Keyboard::KEY_4] = SDLK_4;
  k[Keyboard::KEY_5] = SDLK_5;
  k[Keyboard::KEY_6] = SDLK_6;
  k[Keyboard::KEY_7] = SDLK_7;
  k[Keyboard::KEY_8] = SDLK_8;
  k[Keyboard::KEY_9] = SDLK_9;
  k[Keyboard::KEY_COLON] = SDLK_COLON;
  k[Keyboard::KEY_SEMICOLON] = SDLK_SEMICOLON;
  k[Keyboard::KEY_LESS] = SDLK_LESS;
  k[Keyboard::KEY_EQUALS] = SDLK_EQUALS;
  k[Keyboard::KEY_GREATER] = SDLK_GREATER;
  k[Keyboard::KEY_QUESTION] = SDLK_QUESTION;
  k[Keyboard::KEY_AT] = SDLK_AT;

  k[Keyboard::KEY_LEFTBRACKET] = SDLK_LEFTBRACKET;
  k[Keyboard::KEY_BACKSLASH] = SDLK_BACKSLASH;
  k[Keyboard::KEY_RIGHTBRACKET] = SDLK_RIGHTBRACKET;
  k[Keyboard::KEY_CARET] = SDLK_CARET;
  k[Keyboard::KEY_UNDERSCORE] = SDLK_UNDERSCORE;
  k[Keyboard::KEY_BACKQUOTE] = SDLK_BACKQUOTE;
  k[Keyboard::KEY_A] = SDLK_a;
  k[Keyboard::KEY_B] = SDLK_b;
  k[Keyboard::KEY_C] = SDLK_c;
  k[Keyboard::KEY_D] = SDLK_d;
  k[Keyboard::KEY_E] = SDLK_e;
  k[Keyboard::KEY_F] = SDLK_f;
  k[Keyboard::KEY_G] = SDLK_g;
  k[Keyboard::KEY_H] = SDLK_h;
  k[Keyboard::KEY_I] = SDLK_i;
  k[Keyboard::KEY_J] = SDLK_j;
  k[Keyboard::KEY_K] = SDLK_k;
  k[Keyboard::KEY_L] = SDLK_l;
  k[Keyboard::KEY_M] = SDLK_m;
  k[Keyboard::KEY_N] = SDLK_n;
  k[Keyboard::KEY_O] = SDLK_o;
  k[Keyboard::KEY_P] = SDLK_p;
  k[Keyboard::KEY_Q] = SDLK_q;
  k[Keyboard::KEY_R] = SDLK_r;
  k[Keyboard::KEY_S] = SDLK_s;
  k[Keyboard::KEY_T] = SDLK_t;
  k[Keyboard::KEY_U] = SDLK_u;
  k[Keyboard::KEY_V] = SDLK_v;
  k[Keyboard::KEY_W] = SDLK_w;
  k[Keyboard::KEY_X] = SDLK_x;
  k[Keyboard::KEY_Y] = SDLK_y;
  k[Keyboard::KEY_Z] = SDLK_z;

  k[Keyboard::KEY_CAPSLOCK] = SDLK_CAPSLOCK;

  k[Keyboard::KEY_F1] = SDLK_F1;
  k[Keyboard::KEY_F2] = SDLK_F2;
  k[Keyboard::KEY_F3] = SDLK_F3;
  k[Keyboard::KEY_F4] = SDLK_F4;
  k[Keyboard::KEY_F5] = SDLK_F5;
  k[Keyboard::KEY_F6] = SDLK_F6;
  k[Keyboard::KEY_F7] = SDLK_F7;
  k[Keyboard::KEY_F8] = SDLK_F8;
  k[Keyboard::KEY_F9] = SDLK_F9;
  k[Keyboard::KEY_F10] = SDLK_F10;
  k[Keyboard::KEY_F11] = SDLK_F11;
  k[Keyboard::KEY_F12] = SDLK_F12;

  k[Keyboard::KEY_PRINTSCREEN] = SDLK_PRINTSCREEN;
  k[Keyboard::KEY_SCROLLLOCK] = SDLK_SCROLLLOCK;
  k[Keyboard::KEY_PAUSE] = SDLK_PAUSE;
  k[Keyboard::KEY_INSERT] = SDLK_INSERT;
  k[Keyboard::KEY_HOME] = SDLK_HOME;
  k[Keyboard::KEY_PAGEUP] = SDLK_PAGEUP;
  k[Keyboard::KEY_DELETE] = SDLK_DELETE;
  k[Keyboard::KEY_END] = SDLK_END;
  k[Keyboard::KEY_PAGEDOWN] = SDLK_PAGEDOWN;
  k[Keyboard::KEY_RIGHT] = SDLK_RIGHT;
  k[Keyboard::KEY_LEFT] = SDLK_LEFT;
  k[Keyboard::KEY_DOWN] = SDLK_DOWN;
  k[Keyboard::KEY_UP] = SDLK_UP;

  k[Keyboard::KEY_NUMLOCKCLEAR] = SDLK_NUMLOCKCLEAR;
  k[Keyboard::KEY_KP_DIVIDE] = SDLK_KP_DIVIDE;
  k[Keyboard::KEY_KP_MULTIPLY] = SDLK_KP_MULTIPLY;
  k[Keyboard::KEY_KP_MINUS] = SDLK_KP_MINUS;
  k[Keyboard::KEY_KP_PLUS] = SDLK_KP_PLUS;
  k[Keyboard::KEY_KP_ENTER] = SDLK_KP_ENTER;
  k[Keyboard::KEY_KP_0] = SDLK_KP_0;
  k[Keyboard::KEY_KP_1] = SDLK_KP_1;
  k[Keyboard::KEY_KP_2] = SDLK_KP_2;
  k[Keyboard::KEY_KP_3] = SDLK_KP_3;
  k[Keyboard::KEY_KP_4] = SDLK_KP_4;
  k[Keyboard::KEY_KP_5] = SDLK_KP_5;
  k[Keyboard::KEY_KP_6] = SDLK_KP_6;
  k[Keyboard::KEY_KP_7] = SDLK_KP_7;
  k[Keyboard::KEY_KP_8] = SDLK_KP_8;
  k[Keyboard::KEY_KP_9] = SDLK_KP_9;
  k[Keyboard::KEY_KP_PERIOD] = SDLK_KP_PERIOD;
  k[Keyboard::KEY_KP_COMMA] = SDLK_KP_COMMA;
  k[Keyboard::KEY_KP_EQUALS] = SDLK_KP_EQUALS;

  k[Keyboard::KEY_APPLICATION] = SDLK_APPLICATION;
  k[Keyboard::KEY_POWER] = SDLK_POWER;
  k[Keyboard::KEY_F13] = SDLK_F13;
  k[Keyboard::KEY_F14] = SDLK_F14;
  k[Keyboard::KEY_F15] = SDLK_F15;
  k[Keyboard::KEY_F16] = SDLK_F16;
  k[Keyboard::KEY_F17] = SDLK_F17;
  k[Keyboard::KEY_F18] = SDLK_F18;
  k[Keyboard::KEY_F19] = SDLK_F19;
  k[Keyboard::KEY_F20] = SDLK_F20;
  k[Keyboard::KEY_F21] = SDLK_F21;
  k[Keyboard::KEY_F22] = SDLK_F22;
  k[Keyboard::KEY_F23] = SDLK_F23;
  k[Keyboard::KEY_F24] = SDLK_F24;
  k[Keyboard::KEY_EXECUTE] = SDLK_EXECUTE;
  k[Keyboard::KEY_HELP] = SDLK_HELP;
  k[Keyboard::KEY_MENU] = SDLK_MENU;
  k[Keyboard::KEY_SELECT] = SDLK_SELECT;
  k[Keyboard::KEY_STOP] = SDLK_STOP;
  k[Keyboard::KEY_AGAIN] = SDLK_AGAIN;
  k[Keyboard::KEY_UNDO] = SDLK_UNDO;
  k[Keyboard::KEY_CUT] = SDLK_CUT;
  k[Keyboard::KEY_COPY] = SDLK_COPY;
  k[Keyboard::KEY_PASTE] = SDLK_PASTE;
  k[Keyboard::KEY_FIND] = SDLK_FIND;
  k[Keyboard::KEY_MUTE] = SDLK_MUTE;
  k[Keyboard::KEY_VOLUMEUP] = SDLK_VOLUMEUP;
  k[Keyboard::KEY_VOLUMEDOWN] = SDLK_VOLUMEDOWN;

  k[Keyboard::KEY_ALTERASE] = SDLK_ALTERASE;
  k[Keyboard::KEY_SYSREQ] = SDLK_SYSREQ;
  k[Keyboard::KEY_CANCEL] = SDLK_CANCEL;
  k[Keyboard::KEY_CLEAR] = SDLK_CLEAR;
  k[Keyboard::KEY_PRIOR] = SDLK_PRIOR;
  k[Keyboard::KEY_RETURN2] = SDLK_RETURN2;
  k[Keyboard::KEY_SEPARATOR] = SDLK_SEPARATOR;
  k[Keyboard::KEY_OUT] = SDLK_OUT;
  k[Keyboard::KEY_OPER] = SDLK_OPER;
  k[Keyboard::KEY_CLEARAGAIN] = SDLK_CLEARAGAIN;

  k[Keyboard::KEY_THOUSANDSSEPARATOR] = SDLK_THOUSANDSSEPARATOR;
  k[Keyboard::KEY_DECIMALSEPARATOR] = SDLK_DECIMALSEPARATOR;
  k[Keyboard::KEY_CURRENCYUNIT] = SDLK_CURRENCYUNIT;
  k[Keyboard::KEY_CURRENCYSUBUNIT] = SDLK_CURRENCYSUBUNIT;

  k[Keyboard::KEY_LCTRL] = SDLK_LCTRL;
  k[Keyboard::KEY_LSHIFT] = SDLK_LSHIFT;
  k[Keyboard::KEY_LALT] = SDLK_LALT;
  k[Keyboard::KEY_LGUI] = SDLK_LGUI;
  k[Keyboard::KEY_RCTRL] = SDLK_RCTRL;
  k[Keyboard::KEY_RSHIFT] = SDLK_RSHIFT;
  k[Keyboard::KEY_RALT] = SDLK_RALT;
  k[Keyboard::KEY_RGUI] = SDLK_RGUI;

  k[Keyboard::KEY_MODE] = SDLK_MODE;

  k[Keyboard::KEY_AUDIONEXT] = SDLK_AUDIONEXT;
  k[Keyboard::KEY_AUDIOPREV] = SDLK_AUDIOPREV;
  k[Keyboard::KEY_AUDIOSTOP] = SDLK_AUDIOSTOP;
  k[Keyboard::KEY_AUDIOPLAY] = SDLK_AUDIOPLAY;
  k[Keyboard::KEY_AUDIOMUTE] = SDLK_AUDIOMUTE;
  k[Keyboard::KEY_MEDIASELECT] = SDLK_MEDIASELECT;
  k[Keyboard::KEY_WWW] = SDLK_WWW;
  k[Keyboard::KEY_MAIL] = SDLK_MAIL;
  k[Keyboard::KEY_CALCULATOR] = SDLK_CALCULATOR;
  k[Keyboard::KEY_COMPUTER] = SDLK_COMPUTER;
  k[Keyboard::KEY_APP_SEARCH] = SDLK_AC_SEARCH;
  k[Keyboard::KEY_APP_HOME] = SDLK_AC_HOME;
  k[Keyboard::KEY_APP_BACK] = SDLK_AC_BACK;
  k[Keyboard::KEY_APP_FORWARD] = SDLK_AC_FORWARD;
  k[Keyboard::KEY_APP_STOP] = SDLK_AC_STOP;
  k[Keyboard::KEY_APP_REFRESH] = SDLK_AC_REFRESH;
  k[Keyboard::KEY_APP_BOOKMARKS] = SDLK_AC_BOOKMARKS;

  k[Keyboard::KEY_BRIGHTNESSDOWN] = SDLK_BRIGHTNESSDOWN;
  k[Keyboard::KEY_BRIGHTNESSUP] = SDLK_BRIGHTNESSUP;
  k[Keyboard::KEY_DISPLAYSWITCH] = SDLK_DISPLAYSWITCH;
  k[Keyboard::KEY_KBDILLUMTOGGLE] = SDLK_KBDILLUMTOGGLE;
  k[Keyboard::KEY_KBDILLUMDOWN] = SDLK_KBDILLUMDOWN;
  k[Keyboard::KEY_KBDILLUMUP] = SDLK_KBDILLUMUP;
  k[Keyboard::KEY_EJECT] = SDLK_EJECT;
  k[Keyboard::KEY_SLEEP] = SDLK_SLEEP;

  return k;
}

const SDL_Keycode *Keyboard::keymap = Keyboard::createKeyMap();

EnumMap<Keyboard::Scancode, SDL_Scancode, SDL_NUM_SCANCODES>::Entry Keyboard::scancodeEntries[] = {
    {SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN},

    {SCANCODE_A, SDL_SCANCODE_A},
    {SCANCODE_B, SDL_SCANCODE_B},
    {SCANCODE_C, SDL_SCANCODE_C},
    {SCANCODE_D, SDL_SCANCODE_D},
    {SCANCODE_E, SDL_SCANCODE_E},
    {SCANCODE_F, SDL_SCANCODE_F},
    {SCANCODE_G, SDL_SCANCODE_G},
    {SCANCODE_H, SDL_SCANCODE_H},
    {SCANCODE_I, SDL_SCANCODE_I},
    {SCANCODE_J, SDL_SCANCODE_J},
    {SCANCODE_K, SDL_SCANCODE_K},
    {SCANCODE_L, SDL_SCANCODE_L},
    {SCANCODE_M, SDL_SCANCODE_M},
    {SCANCODE_N, SDL_SCANCODE_N},
    {SCANCODE_O, SDL_SCANCODE_O},
    {SCANCODE_P, SDL_SCANCODE_P},
    {SCANCODE_Q, SDL_SCANCODE_Q},
    {SCANCODE_R, SDL_SCANCODE_R},
    {SCANCODE_S, SDL_SCANCODE_S},
    {SCANCODE_T, SDL_SCANCODE_T},
    {SCANCODE_U, SDL_SCANCODE_U},
    {SCANCODE_V, SDL_SCANCODE_V},
    {SCANCODE_W, SDL_SCANCODE_W},
    {SCANCODE_X, SDL_SCANCODE_X},
    {SCANCODE_Y, SDL_SCANCODE_Y},
    {SCANCODE_Z, SDL_SCANCODE_Z},

    {SCANCODE_1, SDL_SCANCODE_1},
    {SCANCODE_2, SDL_SCANCODE_2},
    {SCANCODE_3, SDL_SCANCODE_3},
    {SCANCODE_4, SDL_SCANCODE_4},
    {SCANCODE_5, SDL_SCANCODE_5},
    {SCANCODE_6, SDL_SCANCODE_6},
    {SCANCODE_7, SDL_SCANCODE_7},
    {SCANCODE_8, SDL_SCANCODE_8},
    {SCANCODE_9, SDL_SCANCODE_9},
    {SCANCODE_0, SDL_SCANCODE_0},

    {SCANCODE_RETURN, SDL_SCANCODE_RETURN},
    {SCANCODE_ESCAPE, SDL_SCANCODE_ESCAPE},
    {SCANCODE_BACKSPACE, SDL_SCANCODE_BACKSPACE},
    {SCANCODE_TAB, SDL_SCANCODE_TAB},
    {SCANCODE_SPACE, SDL_SCANCODE_SPACE},

    {SCANCODE_MINUS, SDL_SCANCODE_MINUS},
    {SCANCODE_EQUALS, SDL_SCANCODE_EQUALS},
    {SCANCODE_LEFTBRACKET, SDL_SCANCODE_LEFTBRACKET},
    {SCANCODE_RIGHTBRACKET, SDL_SCANCODE_RIGHTBRACKET},
    {SCANCODE_BACKSLASH, SDL_SCANCODE_BACKSLASH},
    {SCANCODE_NONUSHASH, SDL_SCANCODE_NONUSHASH},
    {SCANCODE_SEMICOLON, SDL_SCANCODE_SEMICOLON},
    {SCANCODE_APOSTROPHE, SDL_SCANCODE_APOSTROPHE},
    {SCANCODE_GRAVE, SDL_SCANCODE_GRAVE},
    {SCANCODE_COMMA, SDL_SCANCODE_COMMA},
    {SCANCODE_PERIOD, SDL_SCANCODE_PERIOD},
    {SCANCODE_SLASH, SDL_SCANCODE_SLASH},

    {SCANCODE_CAPSLOCK, SDL_SCANCODE_CAPSLOCK},

    {SCANCODE_F1, SDL_SCANCODE_F1},
    {SCANCODE_F2, SDL_SCANCODE_F2},
    {SCANCODE_F3, SDL_SCANCODE_F3},
    {SCANCODE_F4, SDL_SCANCODE_F4},
    {SCANCODE_F5, SDL_SCANCODE_F5},
    {SCANCODE_F6, SDL_SCANCODE_F6},
    {SCANCODE_F7, SDL_SCANCODE_F7},
    {SCANCODE_F8, SDL_SCANCODE_F8},
    {SCANCODE_F9, SDL_SCANCODE_F9},
    {SCANCODE_F10, SDL_SCANCODE_F10},
    {SCANCODE_F11, SDL_SCANCODE_F11},
    {SCANCODE_F12, SDL_SCANCODE_F12},

    {SCANCODE_PRINTSCREEN, SDL_SCANCODE_PRINTSCREEN},
    {SCANCODE_SCROLLLOCK, SDL_SCANCODE_SCROLLLOCK},
    {SCANCODE_PAUSE, SDL_SCANCODE_PAUSE},
    {SCANCODE_INSERT, SDL_SCANCODE_INSERT},
    {SCANCODE_HOME, SDL_SCANCODE_HOME},
    {SCANCODE_PAGEUP, SDL_SCANCODE_PAGEUP},
    {SCANCODE_DELETE, SDL_SCANCODE_DELETE},
    {SCANCODE_END, SDL_SCANCODE_END},
    {SCANCODE_PAGEDOWN, SDL_SCANCODE_PAGEDOWN},
    {SCANCODE_RIGHT, SDL_SCANCODE_RIGHT},
    {SCANCODE_LEFT, SDL_SCANCODE_LEFT},
    {SCANCODE_DOWN, SDL_SCANCODE_DOWN},
    {SCANCODE_UP, SDL_SCANCODE_UP},

    {SCANCODE_NUMLOCKCLEAR, SDL_SCANCODE_NUMLOCKCLEAR},
    {SCANCODE_KP_DIVIDE, SDL_SCANCODE_KP_DIVIDE},
    {SCANCODE_KP_MULTIPLY, SDL_SCANCODE_KP_MULTIPLY},
    {SCANCODE_KP_MINUS, SDL_SCANCODE_KP_MINUS},
    {SCANCODE_KP_PLUS, SDL_SCANCODE_KP_PLUS},
    {SCANCODE_KP_ENTER, SDL_SCANCODE_KP_ENTER},
    {SCANCODE_KP_1, SDL_SCANCODE_KP_1},
    {SCANCODE_KP_2, SDL_SCANCODE_KP_2},
    {SCANCODE_KP_3, SDL_SCANCODE_KP_3},
    {SCANCODE_KP_4, SDL_SCANCODE_KP_4},
    {SCANCODE_KP_5, SDL_SCANCODE_KP_5},
    {SCANCODE_KP_6, SDL_SCANCODE_KP_6},
    {SCANCODE_KP_7, SDL_SCANCODE_KP_7},
    {SCANCODE_KP_8, SDL_SCANCODE_KP_8},
    {SCANCODE_KP_9, SDL_SCANCODE_KP_9},
    {SCANCODE_KP_0, SDL_SCANCODE_KP_0},
    {SCANCODE_KP_PERIOD, SDL_SCANCODE_KP_PERIOD},

    {SCANCODE_NONUSBACKSLASH, SDL_SCANCODE_NONUSBACKSLASH},
    {SCANCODE_APPLICATION, SDL_SCANCODE_APPLICATION},
    {SCANCODE_POWER, SDL_SCANCODE_POWER},
    {SCANCODE_KP_EQUALS, SDL_SCANCODE_KP_EQUALS},
    {SCANCODE_F13, SDL_SCANCODE_F13},
    {SCANCODE_F14, SDL_SCANCODE_F14},
    {SCANCODE_F15, SDL_SCANCODE_F15},
    {SCANCODE_F16, SDL_SCANCODE_F16},
    {SCANCODE_F17, SDL_SCANCODE_F17},
    {SCANCODE_F18, SDL_SCANCODE_F18},
    {SCANCODE_F19, SDL_SCANCODE_F19},
    {SCANCODE_F20, SDL_SCANCODE_F20},
    {SCANCODE_F21, SDL_SCANCODE_F21},
    {SCANCODE_F22, SDL_SCANCODE_F22},
    {SCANCODE_F23, SDL_SCANCODE_F23},
    {SCANCODE_F24, SDL_SCANCODE_F24},
    {SCANCODE_EXECUTE, SDL_SCANCODE_EXECUTE},
    {SCANCODE_HELP, SDL_SCANCODE_HELP},
    {SCANCODE_MENU, SDL_SCANCODE_MENU},
    {SCANCODE_SELECT, SDL_SCANCODE_SELECT},
    {SCANCODE_STOP, SDL_SCANCODE_STOP},
    {SCANCODE_AGAIN, SDL_SCANCODE_AGAIN},
    {SCANCODE_UNDO, SDL_SCANCODE_UNDO},
    {SCANCODE_CUT, SDL_SCANCODE_CUT},
    {SCANCODE_COPY, SDL_SCANCODE_COPY},
    {SCANCODE_PASTE, SDL_SCANCODE_PASTE},
    {SCANCODE_FIND, SDL_SCANCODE_FIND},
    {SCANCODE_MUTE, SDL_SCANCODE_MUTE},
    {SCANCODE_VOLUMEUP, SDL_SCANCODE_VOLUMEUP},
    {SCANCODE_VOLUMEDOWN, SDL_SCANCODE_VOLUMEDOWN},
    {SCANCODE_KP_COMMA, SDL_SCANCODE_KP_COMMA},
    {SCANCODE_KP_EQUALSAS400, SDL_SCANCODE_KP_EQUALSAS400},

    {SCANCODE_INTERNATIONAL1, SDL_SCANCODE_INTERNATIONAL1},
    {SCANCODE_INTERNATIONAL2, SDL_SCANCODE_INTERNATIONAL2},
    {SCANCODE_INTERNATIONAL3, SDL_SCANCODE_INTERNATIONAL3},
    {SCANCODE_INTERNATIONAL4, SDL_SCANCODE_INTERNATIONAL4},
    {SCANCODE_INTERNATIONAL5, SDL_SCANCODE_INTERNATIONAL5},
    {SCANCODE_INTERNATIONAL6, SDL_SCANCODE_INTERNATIONAL6},
    {SCANCODE_INTERNATIONAL7, SDL_SCANCODE_INTERNATIONAL7},
    {SCANCODE_INTERNATIONAL8, SDL_SCANCODE_INTERNATIONAL8},
    {SCANCODE_INTERNATIONAL9, SDL_SCANCODE_INTERNATIONAL9},
    {SCANCODE_LANG1, SDL_SCANCODE_LANG1},
    {SCANCODE_LANG2, SDL_SCANCODE_LANG2},
    {SCANCODE_LANG3, SDL_SCANCODE_LANG3},
    {SCANCODE_LANG4, SDL_SCANCODE_LANG4},
    {SCANCODE_LANG5, SDL_SCANCODE_LANG5},
    {SCANCODE_LANG6, SDL_SCANCODE_LANG6},
    {SCANCODE_LANG7, SDL_SCANCODE_LANG7},
    {SCANCODE_LANG8, SDL_SCANCODE_LANG8},
    {SCANCODE_LANG9, SDL_SCANCODE_LANG9},

    {SCANCODE_ALTERASE, SDL_SCANCODE_ALTERASE},
    {SCANCODE_SYSREQ, SDL_SCANCODE_SYSREQ},
    {SCANCODE_CANCEL, SDL_SCANCODE_CANCEL},
    {SCANCODE_CLEAR, SDL_SCANCODE_CLEAR},
    {SCANCODE_PRIOR, SDL_SCANCODE_PRIOR},
    {SCANCODE_RETURN2, SDL_SCANCODE_RETURN2},
    {SCANCODE_SEPARATOR, SDL_SCANCODE_SEPARATOR},
    {SCANCODE_OUT, SDL_SCANCODE_OUT},
    {SCANCODE_OPER, SDL_SCANCODE_OPER},
    {SCANCODE_CLEARAGAIN, SDL_SCANCODE_CLEARAGAIN},
    {SCANCODE_CRSEL, SDL_SCANCODE_CRSEL},
    {SCANCODE_EXSEL, SDL_SCANCODE_EXSEL},

    {SCANCODE_KP_00, SDL_SCANCODE_KP_00},
    {SCANCODE_KP_000, SDL_SCANCODE_KP_000},
    {SCANCODE_THOUSANDSSEPARATOR, SDL_SCANCODE_THOUSANDSSEPARATOR},
    {SCANCODE_DECIMALSEPARATOR, SDL_SCANCODE_DECIMALSEPARATOR},
    {SCANCODE_CURRENCYUNIT, SDL_SCANCODE_CURRENCYUNIT},
    {SCANCODE_CURRENCYSUBUNIT, SDL_SCANCODE_CURRENCYSUBUNIT},
    {SCANCODE_KP_LEFTPAREN, SDL_SCANCODE_KP_LEFTPAREN},
    {SCANCODE_KP_RIGHTPAREN, SDL_SCANCODE_KP_RIGHTPAREN},
    {SCANCODE_KP_LEFTBRACE, SDL_SCANCODE_KP_LEFTBRACE},
    {SCANCODE_KP_RIGHTBRACE, SDL_SCANCODE_KP_RIGHTBRACE},
    {SCANCODE_KP_TAB, SDL_SCANCODE_KP_TAB},
    {SCANCODE_KP_BACKSPACE, SDL_SCANCODE_KP_BACKSPACE},
    {SCANCODE_KP_A, SDL_SCANCODE_KP_A},
    {SCANCODE_KP_B, SDL_SCANCODE_KP_B},
    {SCANCODE_KP_C, SDL_SCANCODE_KP_C},
    {SCANCODE_KP_D, SDL_SCANCODE_KP_D},
    {SCANCODE_KP_E, SDL_SCANCODE_KP_E},
    {SCANCODE_KP_F, SDL_SCANCODE_KP_F},
    {SCANCODE_KP_XOR, SDL_SCANCODE_KP_XOR},
    {SCANCODE_KP_POWER, SDL_SCANCODE_KP_POWER},
    {SCANCODE_KP_PERCENT, SDL_SCANCODE_KP_PERCENT},
    {SCANCODE_KP_LESS, SDL_SCANCODE_KP_LESS},
    {SCANCODE_KP_GREATER, SDL_SCANCODE_KP_GREATER},
    {SCANCODE_KP_AMPERSAND, SDL_SCANCODE_KP_AMPERSAND},
    {SCANCODE_KP_DBLAMPERSAND, SDL_SCANCODE_KP_DBLAMPERSAND},
    {SCANCODE_KP_VERTICALBAR, SDL_SCANCODE_KP_VERTICALBAR},
    {SCANCODE_KP_DBLVERTICALBAR, SDL_SCANCODE_KP_DBLVERTICALBAR},
    {SCANCODE_KP_COLON, SDL_SCANCODE_KP_COLON},
    {SCANCODE_KP_HASH, SDL_SCANCODE_KP_HASH},
    {SCANCODE_KP_SPACE, SDL_SCANCODE_KP_SPACE},
    {SCANCODE_KP_AT, SDL_SCANCODE_KP_AT},
    {SCANCODE_KP_EXCLAM, SDL_SCANCODE_KP_EXCLAM},
    {SCANCODE_KP_MEMSTORE, SDL_SCANCODE_KP_MEMSTORE},
    {SCANCODE_KP_MEMRECALL, SDL_SCANCODE_KP_MEMRECALL},
    {SCANCODE_KP_MEMCLEAR, SDL_SCANCODE_KP_MEMCLEAR},
    {SCANCODE_KP_MEMADD, SDL_SCANCODE_KP_MEMADD},
    {SCANCODE_KP_MEMSUBTRACT, SDL_SCANCODE_KP_MEMSUBTRACT},
    {SCANCODE_KP_MEMMULTIPLY, SDL_SCANCODE_KP_MEMMULTIPLY},
    {SCANCODE_KP_MEMDIVIDE, SDL_SCANCODE_KP_MEMDIVIDE},
    {SCANCODE_KP_PLUSMINUS, SDL_SCANCODE_KP_PLUSMINUS},
    {SCANCODE_KP_CLEAR, SDL_SCANCODE_KP_CLEAR},
    {SCANCODE_KP_CLEARENTRY, SDL_SCANCODE_KP_CLEARENTRY},
    {SCANCODE_KP_BINARY, SDL_SCANCODE_KP_BINARY},
    {SCANCODE_KP_OCTAL, SDL_SCANCODE_KP_OCTAL},
    {SCANCODE_KP_DECIMAL, SDL_SCANCODE_KP_DECIMAL},
    {SCANCODE_KP_HEXADECIMAL, SDL_SCANCODE_KP_HEXADECIMAL},

    {SCANCODE_LCTRL, SDL_SCANCODE_LCTRL},
    {SCANCODE_LSHIFT, SDL_SCANCODE_LSHIFT},
    {SCANCODE_LALT, SDL_SCANCODE_LALT},
    {SCANCODE_LGUI, SDL_SCANCODE_LGUI},
    {SCANCODE_RCTRL, SDL_SCANCODE_RCTRL},
    {SCANCODE_RSHIFT, SDL_SCANCODE_RSHIFT},
    {SCANCODE_RALT, SDL_SCANCODE_RALT},
    {SCANCODE_RGUI, SDL_SCANCODE_RGUI},

    {SCANCODE_MODE, SDL_SCANCODE_MODE},

    {SCANCODE_AUDIONEXT, SDL_SCANCODE_AUDIONEXT},
    {SCANCODE_AUDIOPREV, SDL_SCANCODE_AUDIOPREV},
    {SCANCODE_AUDIOSTOP, SDL_SCANCODE_AUDIOSTOP},
    {SCANCODE_AUDIOPLAY, SDL_SCANCODE_AUDIOPLAY},
    {SCANCODE_AUDIOMUTE, SDL_SCANCODE_AUDIOMUTE},
    {SCANCODE_MEDIASELECT, SDL_SCANCODE_MEDIASELECT},
    {SCANCODE_WWW, SDL_SCANCODE_WWW},
    {SCANCODE_MAIL, SDL_SCANCODE_MAIL},
    {SCANCODE_CALCULATOR, SDL_SCANCODE_CALCULATOR},
    {SCANCODE_COMPUTER, SDL_SCANCODE_COMPUTER},
    {SCANCODE_AC_SEARCH, SDL_SCANCODE_AC_SEARCH},
    {SCANCODE_AC_HOME, SDL_SCANCODE_AC_HOME},
    {SCANCODE_AC_BACK, SDL_SCANCODE_AC_BACK},
    {SCANCODE_AC_FORWARD, SDL_SCANCODE_AC_FORWARD},
    {SCANCODE_AC_STOP, SDL_SCANCODE_AC_STOP},
    {SCANCODE_AC_REFRESH, SDL_SCANCODE_AC_REFRESH},
    {SCANCODE_AC_BOOKMARKS, SDL_SCANCODE_AC_BOOKMARKS},

    {SCANCODE_BRIGHTNESSDOWN, SDL_SCANCODE_BRIGHTNESSDOWN},
    {SCANCODE_BRIGHTNESSUP, SDL_SCANCODE_BRIGHTNESSUP},
    {SCANCODE_DISPLAYSWITCH, SDL_SCANCODE_DISPLAYSWITCH},
    {SCANCODE_KBDILLUMTOGGLE, SDL_SCANCODE_KBDILLUMTOGGLE},
    {SCANCODE_KBDILLUMDOWN, SDL_SCANCODE_KBDILLUMDOWN},
    {SCANCODE_KBDILLUMUP, SDL_SCANCODE_KBDILLUMUP},
    {SCANCODE_EJECT, SDL_SCANCODE_EJECT},
    {SCANCODE_SLEEP, SDL_SCANCODE_SLEEP},

    {SCANCODE_APP1, SDL_SCANCODE_APP1},
    {SCANCODE_APP2, SDL_SCANCODE_APP2},
};

EnumMap<Keyboard::Scancode, SDL_Scancode, SDL_NUM_SCANCODES> Keyboard::scancodes(
    Keyboard::scancodeEntries, sizeof(Keyboard::scancodeEntries));

}  // namespace sdl
}  // namespace keyboard
}  // namespace love
