#include "overrides.h"

DM_MenuBar::DM_MenuBar(int x, int y, int w, int h, const char* label) : Fl_Menu_Bar(x, y, w, h, label) {
	this->color(fl_rgb_color(70, 70, 72));
	this->labelcolor(fl_rgb_color(200, 200, 200));
}

DM_Button::DM_Button(int x, int y, int w, int h, const char* label) : Fl_Button(x, y, w, h, label) {
	this->color(fl_rgb_color(70, 70, 72));
	this->labelcolor(fl_rgb_color(200, 200, 200));
}

DM_Check_Button::DM_Check_Button(int x, int y, int w, int h, const char* label) : Fl_Check_Button(x, y, w, h, label) {
	this->color(fl_rgb_color(70, 70, 72));
	this->labelcolor(fl_rgb_color(200, 200, 200));
}

DM_Int_Input::DM_Int_Input(int x, int y, int w, int h, const char* label) : Fl_Int_Input(x, y, w, h, label) {
	this->color(fl_rgb_color(70, 70, 72));
	this->labelcolor(fl_rgb_color(200, 200, 200));
	this->textcolor(fl_rgb_color(200, 200, 200));
	this->textfont(FL_COURIER);
	this->textsize(12);
	this->box(FL_BORDER_BOX);
	this->value("0");
	this->when(FL_WHEN_CHANGED);
	this->maximum_size(3);
}

int DM_Int_Input::handle(int event) {
	switch (event) {
		case FL_KEYBOARD: {
			// disallow negation
			if (Fl::event_key() == '-') {
				return 0;
			}

			// disallow any non-zero numbers after a zero
			if (event == FL_KEYBOARD) {
				// Check if the key pressed is a number
				if (Fl::event_key() >= '0' && Fl::event_key() <= '9') {
					const char* value = this->value();

					// Disallow entering numbers other than '0' if the current value is '0'
					if (strcmp(value, "0") == 0 && Fl::event_key() != '0') {
						return 1; // Ignore the key press
					}
				}
			}

			return Fl_Int_Input::handle(event);
		}
		case FL_UNFOCUS: {
			const char* value = this->value();

			// If the value is empty, set it to 0
			if (strcmp(value, "") == 0) {
				this->value("0");
			}

			return Fl_Int_Input::handle(event);
		}
		default:
			return Fl_Int_Input::handle(event);
	}
}

DM_Choice::DM_Choice(int x, int y, int w, int h, const char* label) : Fl_Choice(x, y, w, h, label) {
	this->color(fl_rgb_color(70, 70, 72));
	this->labelcolor(fl_rgb_color(200, 200, 200));
	this->textcolor(fl_rgb_color(200, 200, 200));
	this->textfont(FL_COURIER);
	this->textsize(12);
	this->box(FL_BORDER_BOX);
	this->down_box(FL_BORDER_BOX);
	this->value(0);
	this->when(FL_WHEN_CHANGED);
}

DM_Input::DM_Input(int x, int y, int w, int h, const char* label) : Fl_Input(x, y, w, h, label) {
	this->color(fl_rgb_color(70, 70, 72));
	this->labelcolor(fl_rgb_color(200, 200, 200));
	this->textcolor(fl_rgb_color(200, 200, 200));
	this->textfont(FL_COURIER);
	this->textsize(12);
	this->box(FL_BORDER_BOX);
	this->align(FL_ALIGN_TOP_LEFT);
	this->value("");
	this->when(FL_WHEN_CHANGED);
}