//Headers
#include "default.hpp"
#include <iostream>
//#include <thread>
//#include <exception>
//#include <stdexcept>
//#include <cassert>
#include "base.hpp"
#include "graphics.hpp"
#include "inputs.hpp"
#include "output.hpp"
#include "kernel.hpp"
using namespace std;

bas Base;
grh Graphics;
hid Inputs;
ntf Output;
knl Kernel;

int main(int , char** )
{
	try{
		Base.start();
		Graphics.start();
		Inputs.start();
		Output.start();
		Kernel.start();
		return EXIT_SUCCESS;
	}
	catch (const char* message)
	{
		cerr << message << endl;
		return EXIT_FAILURE;
	}
}

/*Tips
atexit -> Llamar una funcion antes de salir
glutEnterGameMode -> Mejor que el fullscreen, limita popups menu, aun no probado
glutSetWindow, glutGetWindow -> Manejar varias ventanas
glutTimerFunc -> Un timer pero no funciono
gluErrorString -> Traducir GLenum a string, usado con excepciones de gl y glu
glGetError -> Manejar errores, debe estar revisando constantemente, es inviable*/

/*Falta
- Modo debugging manejando librerias de excepciones
- Revisar lo de FullScreen*/
