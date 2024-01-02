#include "application.h"

int main() {
	Application& app = Application::GetInstance();
	if (!app.InitializeWindow()) {
		_SEND_ALERT("Failed to initialize main window!");
		return 1;
	}
	
	return app.eventloop();
}