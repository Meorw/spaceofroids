#include "RenderChimp.h"

#if defined RC_WIN32 && defined RC_GLUT

#include <windows.h>
#include <tchar.h>

#include "Command.h"
#include "Platform.h"

#ifdef _DEBUG 
	#define CRTDBG_MAP_ALLOC 
	#include <crtdbg.h> 
#endif


static DeviceState_t deviceState;


static f64 timer_freq;

static i32 disp_w, disp_h;

static bool alt_pressed = false;
static bool f4_pressed = false;

static bool alt_changed = false;
static bool shift_changed = false;
static bool ctrl_changed = false;


static const unsigned char shiftmap[32] = { ' ',
	'!', '"', '#', '$', '%', '&', '\"', '(',
	')', '*', '+', '<', '_', '>', '?', ')',
	'!', 64, '#', '$', '%', '^', '&', '*',
	'(', ':', ':', '<', '+', '>', '?'};



static bool do_die = false;
	


bool *Platform::getKeyState()
{
	return deviceState.key;
}

bool *Platform::getMouseButtonState()
{
	return deviceState.button;
}

vec2f Platform::getMousePosition()
{
	return vec2f(deviceState.x, deviceState.y);
}


void *Platform::getWindowHandle()
{
#if 0
	HWND hWnd = 0;

	SDL_SysWMinfo wmInfo;

		/* Get window context */
	SDL_VERSION(&wmInfo.version);
	SDL_GetWMInfo(&wmInfo);

//	hWnd = (NativeWindowType) wmInfo.window;
	hWnd = wmInfo.window;

	return (void *) hWnd;
#endif

	return 0;
}


/* Timer functions */
f32 Platform::getFrameTime()
{
	return deviceState.time;
}

f32 Platform::getFrameTimeStep()
{
	return deviceState.timeStep;
}

f32 Platform::getCurrentTime()
{
	u64 t;
	QueryPerformanceCounter((LARGE_INTEGER *) &t);
	return (f32) (((f64) t) / timer_freq);
}


inline void Platform::update_time()
{
	u64 t;
	QueryPerformanceCounter((LARGE_INTEGER *) &t);
	f32 newTime = (f32) (((f64) t) / timer_freq);
	deviceState.timeStep = newTime - deviceState.time;
	deviceState.time = newTime;
}

u32 key_pressed = 0;
u32 mouse_pressed = 0;

static void glut_keyboard_down_spec(int key, int x, int y)
{
	i32 mod = glutGetModifiers();

	if (mod |= GLUT_ACTIVE_ALT) {
		alt_pressed = true;
		deviceState.key[RC_KEY_ALT] = true;
	}
	if (mod |= GLUT_ACTIVE_CTRL) {
		deviceState.key[RC_KEY_CTRL] = true;
	}
	if (mod |= GLUT_ACTIVE_SHIFT) {
		deviceState.key[RC_KEY_CTRL] = true;
		shift_changed = true;
	}

	switch (key) {
		case GLUT_KEY_UP:
			deviceState.key[RC_KEY_UP_ARROW] = true;
			break;
		case GLUT_KEY_DOWN:
			deviceState.key[RC_KEY_DOWN_ARROW] = true;
			break;
		case GLUT_KEY_RIGHT:
			deviceState.key[RC_KEY_RIGHT_ARROW] = true;
			break;
		case GLUT_KEY_LEFT:
			deviceState.key[RC_KEY_LEFT_ARROW] = true;
			break;
		case GLUT_KEY_PAGE_UP:
			deviceState.key[RC_KEY_PAGE_UP] = true;
			break;
		case GLUT_KEY_PAGE_DOWN:
			deviceState.key[RC_KEY_PAGE_DOWN] = true;
			break;
		case GLUT_KEY_F4:
			f4_pressed = true;
		default:
			if (key >= GLUT_KEY_F1 && key <= GLUT_KEY_F12) {
				deviceState.key[(u32) (key - GLUT_KEY_F1) + RC_KEY_F1] = true;
			}
			break;
	}

}

static void glut_keyboard_up_spec(int key, int x, int y)
{
	i32 mod = glutGetModifiers();

	if (mod |= GLUT_ACTIVE_ALT) {
		alt_pressed = false;
		deviceState.key[RC_KEY_ALT] = false;
	}
	if (mod |= GLUT_ACTIVE_CTRL) {
		deviceState.key[RC_KEY_CTRL] = false;
	}
	if (mod |= GLUT_ACTIVE_SHIFT) {
		deviceState.key[RC_KEY_CTRL] = false;
		shift_changed = true;
	}

	switch (key) {
		case GLUT_KEY_UP:
			deviceState.key[RC_KEY_UP_ARROW] = false;
			break;
		case GLUT_KEY_DOWN:
			deviceState.key[RC_KEY_DOWN_ARROW] = false;
			break;
		case GLUT_KEY_RIGHT:
			deviceState.key[RC_KEY_RIGHT_ARROW] = false;
			break;
		case GLUT_KEY_LEFT:
			deviceState.key[RC_KEY_LEFT_ARROW] = false;
			break;
		case GLUT_KEY_PAGE_UP:
			deviceState.key[RC_KEY_PAGE_UP] = false;
			break;
		case GLUT_KEY_PAGE_DOWN:
			deviceState.key[RC_KEY_PAGE_DOWN] = false;
			break;
		case GLUT_KEY_F4:
			f4_pressed = false;
		default:
			if (key >= GLUT_KEY_F1 && key <= GLUT_KEY_F12) {
				deviceState.key[(u32) (key - GLUT_KEY_F1) + RC_KEY_F1] = false;
			}
			break;
	}

}

static void glut_keyboard_down(unsigned char key, int x, int y)
{
	i32 mod = glutGetModifiers();

	if (mod |= GLUT_ACTIVE_ALT) {
		alt_pressed = true;
		deviceState.key[RC_KEY_ALT] = true;
	}
	if (mod |= GLUT_ACTIVE_CTRL) {
		deviceState.key[RC_KEY_CTRL] = true;
	}
	if (mod |= GLUT_ACTIVE_SHIFT) {
		deviceState.key[RC_KEY_CTRL] = true;
		shift_changed = true;
	}

	if (key >= 65 && key <= 90) {
		deviceState.key[key + 32] = true;
	} else if (key == 13) {
		deviceState.key[RC_KEY_RETURN] = true;
	} else if (key == 9) {
		deviceState.key[RC_KEY_TAB] = true;
	} else if (key < 128) {
		deviceState.key[key] = true;
	}
}
	
static void glut_keyboard_up(unsigned char key, int x, int y)
{
	i32 mod = glutGetModifiers();

	if (mod |= GLUT_ACTIVE_ALT) {
		alt_pressed = false;
		deviceState.key[RC_KEY_ALT] = false;
	}
	if (mod |= GLUT_ACTIVE_CTRL) {
		deviceState.key[RC_KEY_CTRL] = false;
	}
	if (mod |= GLUT_ACTIVE_SHIFT) {
		deviceState.key[RC_KEY_CTRL] = false;
		shift_changed = true;
	}


	if (key >= 65 && key <= 90) {
		deviceState.key[key + 32] = false;
	} else if (key == 13) {
		deviceState.key[RC_KEY_RETURN] = false;
	} else if (key == 9) {
		deviceState.key[RC_KEY_TAB] = false;
	} else if (key < 128) {
		deviceState.key[key] = false;
	}
}


static void glut_mouse_button(int button, int state,
                                int x, int y)
{
	bool t;

	if (state == GLUT_DOWN) {
		t = true;
	} else {
		t = false;
	}

	switch (button) {
		case GLUT_LEFT_BUTTON:
			deviceState.button[RC_MOUSE_LEFT] = t;
			break;
		case GLUT_RIGHT_BUTTON:
			deviceState.button[RC_MOUSE_RIGHT] = t;
			break;
		case GLUT_MIDDLE_BUTTON:
			deviceState.button[RC_MOUSE_MIDDLE] = t;
			break;
		default:
			break;
	}

}

static void glut_mouse_motion(int x, int y)
{
	deviceState.x = 2.0f * ((f32) x) / ((f32) disp_w) - 1.0f;
	deviceState.y = 1.0f - 2.0f * ((f32) y) / ((f32) disp_h);
}

#ifdef RC_GLUT

void glut_update()
{
	if (alt_pressed && f4_pressed || RCRun()) {
		RCEnd();
		exit(0);
	}

}

#endif /* RC_GLUT */


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
//int RCMain_()
{
	i32 a = 1;
	char *b[] = { "RenderChimp" };

	printf("Starting up...\n");

	printf("GLUT initiatiation...\n");
	glutInit(&a, b);
	printf("Done\n");


	return Platform::init();
}

i32 Platform::init()
{

	u64 tf;

	printf("Timer setup...\n");
		/* Set up timer */
	HANDLE process_handle = GetCurrentProcess();
	SetProcessAffinityMask(process_handle, 1);

	QueryPerformanceFrequency((LARGE_INTEGER *) &tf);
	timer_freq = (f64) tf;
	printf("Done\n");

	disp_w = RC_DEFAULT_DISPLAY_WIDTH;
	disp_h = RC_DEFAULT_DISPLAY_HEIGHT;

	//glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL);

	printf("GLUT init display...\n");
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL);
	printf("Done\n");

	printf("GLUT window...\n");
	glutInitWindowPosition(100, 50);
	glutInitWindowSize(disp_w, disp_h);
	glutCreateWindow("RenderChimp (GLUT)");
	printf("Done\n");

	printf("GLUT callback functions...\n");
	glutDisplayFunc(glut_update);
	glutIdleFunc(glut_update);
	glutKeyboardFunc(glut_keyboard_down);
	glutKeyboardUpFunc(glut_keyboard_up);
	glutSpecialFunc(glut_keyboard_down_spec);
	glutSpecialUpFunc(glut_keyboard_up_spec);
	glutMouseFunc(glut_mouse_button);
	glutPassiveMotionFunc(glut_mouse_motion);
	glutMotionFunc(glut_mouse_motion);
	printf("Done\n");

	memset(&deviceState, 0, sizeof(DeviceState_t));

	Platform::update_time();

	printf("Entering RC Main...\n");
	RCMain();

	#ifdef _DEBUG
		_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF);
	#endif

	return 0;

}

i32 Platform::initFrame()
{
	update_time();
	return 0;
}


i32 Platform::run()
{
	Console::log("Using GLUT");
	glutMainLoop();

	return 0;
}


/* Texture functions */

void Platform::getTexture(
		const char			*filename,
		bool				flip_y,
		u8					**pixels,
		u32					*width,
		u32					*height,
		u32					*bytes_per_pixel
	)
{
	u32 s;
	SDL_Surface *img = IMG_Load(filename);

	if (!img) {
		REPORT_WARNING("Texture image \"%s\" not found", filename);
		*pixels = 0;
		return;
	}

	*width = img->w;
	*height = img->h;
	*bytes_per_pixel = img->format->BytesPerPixel;

	s = (*bytes_per_pixel) * (*width) * (*height);

	*pixels = (u8 *) malloc(s);
	memcpy(*pixels, img->pixels, s);

	SDL_FreeSurface((SDL_Surface *) img);

	/* LOWPRIO: Better implementation... when I'm feeling less lazy */
	if (flip_y) {
		u32 x, y, i;

		for (y = 0; y < (*height) >> 1; y++) {
			for (x = 0; x < *width; x++) {

				for (i = 0; i < *bytes_per_pixel; i++) {
					u8 *p0, *p1;
					u8 t;

					p0 = &(*pixels)[(y * (*width) + x) * (*bytes_per_pixel) + i];
					p1 = &(*pixels)[((*height - y - 1) * (*width) + x) * (*bytes_per_pixel) + i];

					t = *p0;
					*p0 = *p1;
					*p1 = t;
				}

			}
		}
		
	}
}

void Platform::destroyTexture(
		unsigned char		*pixels
	)
{

	if (pixels)
		free(pixels);

}


void Platform::getDisplaySize(
		u32					*width,
		u32					*height
	)
{
	*width = disp_w;
	*height = disp_h;
}

void Platform::setDisplay(
		bool				fullscreen,
		u32					width,
		u32					height
	)
{
#if 0
	u32 full = fullscreen ? SDL_FULLSCREEN : 0;

#if defined RC_OPENGL_20 || defined RC_CUDA_TRACER
	screen = SDL_SetVideoMode(width, height, 0, SDL_OPENGL | full);
#elif defined RC_OPENGL_ES_20
	screen = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | full);
#endif

	if (!screen) {

		REPORT_WARNING("Couldn't initiate screen resolution (%d, %d) with fullscreen %s", width, height, fullscreen ? "on" : "off");

		/* Could not initialize the specified resolution. Revert to old settings */
#if defined RC_OPENGL_20 || defined RC_CUDA_TRACER
		screen = SDL_SetVideoMode(disp_w, disp_h, 0, SDL_OPENGL);
#elif defined RC_OPENGL_ES_20
		screen = SDL_SetVideoMode(disp_w, disp_h, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
#endif
	
		if (!screen) {
			REPORT_ERROR("Couldn't revert to old screen resolution (%d, %d)", disp_w, disp_h);
		}

	} else {

		disp_w = width;
		disp_h = height;

	}
#endif

}

#endif /* RC_WIN32 && RC_GLUT */


