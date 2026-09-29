#include "core/Application.hpp"
#include "core/Constants.hpp"

int main() {

	Application app;
	if (app.exit_code != ERROR_CODE_SUCCESS)
		return app.exit_code;	

	app.Run();

    return app.exit_code;
}
