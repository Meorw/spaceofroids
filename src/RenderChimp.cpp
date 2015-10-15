#include "RenderChimp.h"

#include "Command.h"
#include "Platform.h"

RenderState rs;

#ifdef WIN32
#else
#include <unistd.h>
#endif


/*---------------------------------------------------------------------------*/

i32 RCRun()
{
//	for (;;) {

#ifndef RC_IPHONE
	Renderer::setRenderTarget(0);
#endif
	Renderer::setRenderState(&rs);
//	Renderer::setClearDepth(1.0f);
//	Renderer::setClearColor(vec4f(0.0f, 0.0f, 0.0f, 1.0f));
//	Renderer::clearDepth();
//	Renderer::clearColor();

	if (Platform::initFrame())
		return 1;

	Command::consumeKeys();

	if (RCUpdate())
		return 1;

	Renderer::ensure();
	Renderer::setRenderTarget(0);

	/* MEGAHACK */
	DefaultObjects::drawHelpers();

	Renderer::ensure();

	Console::render();
	Renderer::swapBuffers();

	Renderer::ensure();

	/* LOWPRIO: Hack! */
	Renderer::endFrame();
//	Renderer::swapBuffers();

	Renderer::ensure();

//	}
	return 0;
}

/*---------------------------------------------------------------------------*/

i32 RCMain()
{
	i32 bail = 0;
#ifdef WIN32
	while (!SetCurrentDirectoryA(RC_RESOURCE_PATH) && SetCurrentDirectoryA("..")) {
		bail++;
		if (bail >= 32) {
			printf("Can't find resource folder. Build directory must be located further down in the hierarchy than /res/\n");
			exit(0);
		}
	}
	SetCurrentDirectoryA("..");
#else
    char *fbuf = __FILE__;
    char fpath [8192];
    char *epos = strrchr(fbuf, '/');
    strncpy(fpath, fbuf, (epos - fbuf));
    fpath[(epos - fbuf)] = 0;
    chdir(fpath);
    while (chdir(RC_RESOURCE_PATH) != 0 && chdir("..") == 0) {
		bail++;
		if (bail >= 32) {
			printf("Can't find resource folder. Build directory must be located further down in the hierarchy than /res/\n");
			exit(0);
		}
	}
	chdir("..");
#endif

	char buf[4096];
	
	printf("Result setup...\n");
	ResultInit();
	printf("Done\n");

	printf("Display setup...\n");
	Platform::setDisplay(false, RC_DEFAULT_DISPLAY_WIDTH, RC_DEFAULT_DISPLAY_HEIGHT);
	printf("Done\n");

	printf("Renderer init...\n");
	Renderer::init();
	printf("Done\n");
	
	printf("SceneGraph setup...\n");
	SceneGraph::init();
	printf("Done\n");

	printf("Console setup...\n");
	Console::init();
	printf("Done\n");

	printf("Command setup...\n");
	Command::init();
	printf("Done\n");

	printf("Frame init...\n");
	Platform::initFrame();
	printf("Done\n");

	Console::log("~c9995=============================");
	Console::log("~c3939~g3939   _");
	Console::log("~c3939~g6969  c \"     ~r-[ RenderChimp ]-");
	Console::log("~c6969~g9999   (-       ~r-[ v 0.3.2a ]-\n ");
	Console::log("~c9995=============================\n \n ");
	//Console::log("~c9995Press ~c6966F1~c9995 to bring up the console.");
	Console::log("~c9995Press F1 to bring up the console.");

	sprintf(buf, "/config %sconfig/config.cfg", RC_RESOURCE_PATH);
	Command::run(buf);

#if defined RC_WIN32
	Console::log("Running on Win32 platform");
#else
	Console::log("Running on some platform");
#endif

	printf("Running RCInit...\n");
	RCInit();
	printf("Done\n");

	Console::log("Initiated...");

	rs.enableDepthTest(CMP_LESS);
	rs.enableDepthWrite();

	printf("Running main loop...\n");
	Platform::run();

	return 0;
}

/*---------------------------------------------------------------------------*/

void RCEnd()
{
	RCDestroy();

	Console::destroy();

	Command::destroy();

	SceneGraph::destroy();
	
	Renderer::destroy();

	if (getAllocationSize() != 0 || getCurrentAllocationCount() != 0) {
		REPORT_FILE("\n\nWarning: There are memory leaks! Check mem.txt for more info\n\n");
		ResultRelease();

		logAllocations();
	} else {
		ResultRelease();
	}
}

/*---------------------------------------------------------------------------*/


