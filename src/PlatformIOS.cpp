#include "RenderChimp.h"
#include "ape.h"

#if defined RC_IPHONE




#include "Command.h"
#include "Platform.h"

#ifdef _DEBUG 
	#define CRTDBG_MAP_ALLOC 
	#include <crtdbg.h> 
#endif


//static DeviceState_t deviceState; // change from static to global so that iOS code can access
DeviceState_t deviceState;

static f64 timer_freq;

//void (*init_func)();
//unsigned int (*update_func)();
//void (*destroy_func)();

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
	return (void *) NULL;
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

float apa = 1.0f;
double GetTicks();

f32 Platform::getCurrentTime()
{
	/*
	u64 t;
	QueryPerformanceCounter((LARGE_INTEGER *) &t);
	return (f32) (((f64) t) / timer_freq);
	*/
	f32 t;

	t = GetTicks();

	//t = ((f32) t) * 0.001f;
	printf("get ticks %f\n",t);
	return t;
}

i32 Platform::run()
{
	while(!RCRun());

	RCEnd();

	return 0;
}

inline void Platform::update_time()
{
	/*
	u64 t;
	QueryPerformanceCounter((LARGE_INTEGER *) &t);
	f32 newTime = (f32) (((f64) t) / timer_freq);
	*/
	f32 f;
	i32 t;
	double ticks;

	ticks = GetTicks();
	t = ticks;
	//printf("update ticks %f, %d\n",ticks, t);
	f = ((f32) t) * 0.001f;
	
//	deviceState.timeStep = f - deviceState.time;
//	deviceState.time = f;
	
	deviceState.timeStep = ticks - deviceState.time;
	deviceState.time = ticks;

}

u32 key_pressed = 0;
u32 mouse_pressed = 0;

int Platform::update_input()
{
	return 0;
}

i32 Platform::initFrame()
{
	update_time();
	return update_input();
}


//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
//int RCMain_()
//{
//	return Platform::init();  // FIXME move me into ipad main
//}

i32 Platform::init()
{

	i32 result;
	u64 tf;


		/* Set up timer */




	timer_freq = (f64) tf;


	disp_w = 768;
	disp_h = 1024;

		/* Set up display */

	if (result < 0)
		return 1;



	memset(&deviceState, 0, sizeof(DeviceState_t));

	Platform::update_time();

	RCMain();



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

	apeGetTexture(filename, pixels, width, height, bytes_per_pixel);

	/* TODO: Better implementation... when I'm feeling less lazy */
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








}

#endif /* RC_WIN32 */


