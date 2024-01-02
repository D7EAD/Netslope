#pragma once

#include "FL/fl.h"
#include "FL/Fl_Window.h"
#include "FL/Fl_Check_Button.h"
#include "FL/Fl_Menu_Bar.h"
#include "FL/Fl_Int_Input.h"
#include "FL/Fl_Choice.h"
#include "FL/Fl_Box.h"
#include "FL/fl_ask.h"

#define WINDOW_HEIGHT 250
#define WINDOW_WIDTH  600

class DM_MenuBar : public Fl_Menu_Bar {
	public:
		DM_MenuBar(int x, int y, int w, int h, const char* label = nullptr);
};

class DM_Button : public Fl_Button {
	public:
		DM_Button(int x, int y, int w, int h, const char* label = nullptr);
};

class DM_Check_Button : public Fl_Check_Button {
	public:
		DM_Check_Button(int x, int y, int w, int h, const char* label = nullptr);
};

class DM_Int_Input : public Fl_Int_Input {
	public:
		DM_Int_Input(int x, int y, int w, int h, const char* label = nullptr);
		int handle(int event) override;
};

class DM_Choice : public Fl_Choice {
	public:
		DM_Choice(int x, int y, int w, int h, const char* label = nullptr);
};

class DM_Input : public Fl_Input {
	public:
		DM_Input(int x, int y, int w, int h, const char* label = nullptr);
};