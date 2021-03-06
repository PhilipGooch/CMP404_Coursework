#include "Globals.h"

#if VITA_MODE

#include <platform/vita/system/platform_vita.h>
//#include "ar_app.h"
#include "App.h"

unsigned int sceLibcHeapSize = 128 * 1024 * 1024;	// Sets up the heap area size as 128MiB.

#pragma comment(lib, "SceCamera_stub")
#pragma comment(lib, "SceMotion_stub")

int main(void)
{
	gef::PlatformVita platform;

	App application(platform);
	application.Run();

	/*ARApp myApp(platform);
	myApp.Run();*/

	return 0;
}

#else

#include <platform/d3d11/system/platform_d3d11.h>
//#include "ar_app.h"
#include "App.h"

unsigned int sceLibcHeapSize = 128 * 1024 * 1024;	// Sets up the heap area size as 128MiB.

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	gef::PlatformD3D11 platform(hInstance, 960, 544, false, true);

	App application(platform);
	application.Run();

	/*ARApp myApp(platform);
	myApp.Run();*/

	return 0;
}

#endif