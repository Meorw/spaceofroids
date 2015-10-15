#include "RenderChimp.h"

#if defined RC_MACOSX && defined RC_SDL

#include "Command.h"
#include "Platform.h"

#include "SDL_image/SDL_image.h"

#ifdef _DEBUG 
	#define CRTDBG_MAP_ALLOC 
	#include <crtdbg.h> 
#endif


static DeviceState_t deviceState;

static SDL_Surface *screen;

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
	i32 t;

	t = SDL_GetTicks();

	return ((f32) t) * 0.001f;
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

	t = SDL_GetTicks();

	f = ((f32) t) * 0.001f;

	deviceState.timeStep = f - deviceState.time;
	deviceState.time = f;
}

u32 key_pressed = 0;
u32 mouse_pressed = 0;

int Platform::update_input()
{
	SDL_Event event;
	i32 more;

	more = SDL_PollEvent(&event);

	while (more) {
		SDLKey key = event.key.keysym.sym;

		if (event.key.type == SDL_QUIT) {

			return 1;

		} else if (event.key.type == SDL_KEYDOWN) {

			switch (key) {
				case SDLK_RALT:
				case SDLK_LALT:
					alt_pressed = true;
					deviceState.key[RC_KEY_ALT] = true;
					break;
				case SDLK_RSHIFT:
				case SDLK_LSHIFT:
					deviceState.key[RC_KEY_SHIFT] = true;
					shift_changed = true;
					break;
				case SDLK_RCTRL:
				case SDLK_LCTRL:
					deviceState.key[RC_KEY_CTRL] = true;
					break;
				case SDLK_KP_ENTER:
				case SDLK_RETURN:
					deviceState.key[RC_KEY_RETURN] = true;
					break;
				case SDLK_TAB:
					deviceState.key[RC_KEY_TAB] = true;
					break;
				case SDLK_UP:
					deviceState.key[RC_KEY_UP_ARROW] = true;
					break;
				case SDLK_DOWN:
					deviceState.key[RC_KEY_DOWN_ARROW] = true;
					break;
				case SDLK_RIGHT:
					deviceState.key[RC_KEY_RIGHT_ARROW] = true;
					break;
				case SDLK_LEFT:
					deviceState.key[RC_KEY_LEFT_ARROW] = true;
					break;
				case SDLK_F4:
					f4_pressed = true;
					break;
				default:
					break;
			}
			if (key < 128) {
				deviceState.key[key] = true;
			} else {

				if (key >= SDLK_UP && key <= SDLK_LEFT) {
					deviceState.key[(u32) (key - SDLK_UP) + RC_KEY_UP_ARROW] = true;
				} else if (key >= SDLK_PAGEUP && key <= SDLK_PAGEDOWN) {
					deviceState.key[(u32) (key - SDLK_PAGEUP) + RC_KEY_PAGE_UP] = true;
				} else if (key >= SDLK_F1 && key <= SDLK_F12) {
					deviceState.key[(u32) (key - SDLK_F1) + RC_KEY_F1] = true;
				}
			}

		} else if (event.key.type == SDL_KEYUP) {

			switch (key) {
				case SDLK_RALT:
				case SDLK_LALT:
					alt_pressed = true;
					deviceState.key[RC_KEY_ALT] = false;
					break;
				case SDLK_RSHIFT:
				case SDLK_LSHIFT:
					deviceState.key[RC_KEY_SHIFT] = false;
					shift_changed = true;
					break;
				case SDLK_RCTRL:
				case SDLK_LCTRL:
					deviceState.key[RC_KEY_CTRL] = false;
					break;
				case SDLK_KP_ENTER:
				case SDLK_RETURN:
					deviceState.key[RC_KEY_RETURN] = false;
					break;
				case SDLK_TAB:
					deviceState.key[RC_KEY_TAB] = false;
					break;
				case SDLK_UP:
					deviceState.key[RC_KEY_UP_ARROW] = false;
					break;
				case SDLK_DOWN:
					deviceState.key[RC_KEY_DOWN_ARROW] = false;
					break;
				case SDLK_RIGHT:
					deviceState.key[RC_KEY_RIGHT_ARROW] = false;
					break;
				case SDLK_LEFT:
					deviceState.key[RC_KEY_LEFT_ARROW] = false;
					break;
				case SDLK_F4:
					f4_pressed = false;
					break;
				default:
					break;
			}

			if (key < 128) {
				deviceState.key[key] = false;
			} else {
				if (key >= SDLK_UP && key <= SDLK_LEFT) {
					deviceState.key[(u32) (key - SDLK_UP) + RC_KEY_UP_ARROW] = false;
				} else if (key >= SDLK_PAGEUP && key <= SDLK_PAGEDOWN) {
					deviceState.key[(u32) (key - SDLK_PAGEUP) + RC_KEY_PAGE_UP] = false;
				} else if (key >= SDLK_F1 && key <= SDLK_F12) {
					deviceState.key[(u32) (key - SDLK_F1) + RC_KEY_F1] = false;
				}
			}

		} else if (event.key.type == SDL_MOUSEBUTTONDOWN) {
			switch (event.button.button) {
				case SDL_BUTTON_LEFT:
					deviceState.button[RC_MOUSE_LEFT] = true;
					break;
				case SDL_BUTTON_RIGHT:
					deviceState.button[RC_MOUSE_RIGHT] = true;
					break;
				case SDL_BUTTON_MIDDLE:
					deviceState.button[RC_MOUSE_MIDDLE] = true;
					break;
				case SDL_BUTTON_WHEELUP:
					deviceState.button[RC_MOUSE_SCROLL_UP] = true;
					break;
				case SDL_BUTTON_WHEELDOWN:
					deviceState.button[RC_MOUSE_SCROLL_DOWN] = true;
					break;
				default:
					break;
			}
		} else if (event.key.type == SDL_MOUSEBUTTONUP) {
			switch (event.button.button) {
				case SDL_BUTTON_LEFT:
					deviceState.button[RC_MOUSE_LEFT] = false;
					break;
				case SDL_BUTTON_RIGHT:
					deviceState.button[RC_MOUSE_RIGHT] = false;
					break;
				case SDL_BUTTON_MIDDLE:
					deviceState.button[RC_MOUSE_MIDDLE] = false;
					break;
				case SDL_BUTTON_WHEELUP:
					deviceState.button[RC_MOUSE_SCROLL_UP] = false;
					break;
				case SDL_BUTTON_WHEELDOWN:
					deviceState.button[RC_MOUSE_SCROLL_DOWN] = false;
					break;
				default:
					break;
			}
		} else if (event.key.type == SDL_MOUSEMOTION) {
			deviceState.x = 2.0f * ((f32) event.motion.x) / ((f32) disp_w) - 1.0f;
			deviceState.y = 1.0f - 2.0f * ((f32) event.motion.y) / ((f32) disp_h);
		}
		
		/*
		else if (event.key.type == SDL_KEYDOWN && !key_pressed) {
			key_pressed = 1;

			switch (event.key.keysym.sym) {
				case SDLK_TAB:
					toggle_orientation();
					break;
				case SDLK_ESCAPE:
					esc_pressed = 1;
					break;
				default:
					break;
			}

		} else if (event.key.type == SDL_KEYUP) {
			key_pressed = 0;
		} else if (event.key.type == SDL_MOUSEBUTTONDOWN || (event.key.type == SDL_MOUSEMOTION && mouse_pressed)) {
			if (event.motion.x >= vp_x && event.motion.x <= vp_x + vp_w &&
				(skin->h - event.motion.y) >= vp_y && (skin->h - event.motion.y) <= vp_y + vp_h) {

				ds->x = 2.0f * ((((f32) event.motion.x) - vp_x) / vp_w - 0.5f);
				ds->y = 2.0f * ((skin->h - ((f32) event.motion.y) - vp_y) / vp_h - 0.5f);

				mouse_pressed = 1;
			}
		} else if (event.key.type == SDL_MOUSEBUTTONUP) {
			mouse_pressed = 0;
		}
		*/

		more = SDL_PollEvent(&event);
	}

	//ds->touch = mouse_pressed ? RC_TOUCH_SINGLE : RC_TOUCH_NONE;

	if (alt_pressed && f4_pressed)
		return 1;

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
//	return Platform::init();
//}

i32 Platform::init()
{
	i32 result;
//	u64 tf;


		/* Set up timer */
	// TODO


	disp_w = RC_DEFAULT_DISPLAY_WIDTH;
	disp_h = RC_DEFAULT_DISPLAY_HEIGHT;

		/* Set up display */
	result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
	if (result < 0)
		return 1;

    	
	SDL_WM_SetCaption("RenderChimp", 0);

	memset(&deviceState, 0, sizeof(DeviceState_t));

	Platform::update_time();

	RCMain();

	#ifdef _DEBUG
		_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF);
	#endif

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

	/* LOWPRIO: Hack! */
	if (img->format->Rmask != 255 && *bytes_per_pixel != 1) {
		u32 x, y;

		for (y = 0; y < *height; y++) {
			for (x = 0; x < *width; x++) {

				u8 *p0, *p1;
				u8 t;

				p0 = &(*pixels)[(y * (*width) + x) * (*bytes_per_pixel) + 0];
				p1 = &(*pixels)[(y * (*width) + x) * (*bytes_per_pixel) + 2];

				t = *p0;
				*p0 = *p1;
				*p1 = t;

			}
		}
	}
	
	SDL_FreeSurface((SDL_Surface *) img);
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

}

/*---------------------------------------------------------------------------*/

#ifdef RC_OPENGL_20
void Platform::initOpenGL20()
{
	
}
#endif /* RC_OPENGL_20 */

/*---------------------------------------------------------------------------*/


#endif /* RC_MACOSX */


