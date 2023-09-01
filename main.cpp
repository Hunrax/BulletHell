#include "Window.h"

int main(int argc, char **argv)
{
	while (1)
	{
		Window window;
		if (!window.run())
			break;
	}
	return 0;
}