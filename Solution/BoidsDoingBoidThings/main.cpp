#ifdef _WIN32
#include <Windows.h>
#endif
#include "Engine.h"

int main()
{
	return Engine::Instance().Go();
}

#ifdef _WIN32
int WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	return main();
}
#endif