/*
 * Platform
 *
 */

#ifndef RC_PLATFORM_H
#define RC_PLATFORM_H

typedef struct {

	/* Mouse/Touch screen */
	bool			button[5];
	f32				x;
	f32				y;

	/* Keypress */
	bool			key[RC_KEY_LAST];

	/* Time */
	f64				time;
	f64				timeStep;

} DeviceState_t;


/** This is called at the beginning of the execution */
void RCInit();

/** This is called every frame */
u32 RCUpdate();

/** This is called when the application is exited */
void RCDestroy();

class Platform {

	public:

		static i32 init();
		static i32 run();

		static bool *getKeyState();
		static bool *getMouseButtonState();
		static vec2f getMousePosition();

		static f32 getCurrentTime();

		/* Time at the beginning of the frame */
		static f32 getFrameTime();

		/* Delta time from last frame */
		static f32 getFrameTimeStep();

		static i32 initFrame();


		/* Display functions */

		static void *getWindowHandle();

		static void getDisplaySize(
				u32					*width,
				u32					*height
			);

		static void setDisplay(
				bool				fullscreen,
				u32					width,
				u32					height
			);


		/* Texture functions */

		static void getTexture(
				const char			*filename,
				bool				flip_y,
				u8					**pixels,
				u32					*width,
				u32					*height,
				u32					*bytes_per_pixel
			);

		static void destroyTexture(
				unsigned char		*pixels
			);
	
#ifdef RC_OPENGL_20
		void initOpenGL20();
#endif /* RC_OPENGL_20 */
	
	private:

		static void update_time();

		static int update_input();

};

/* File reading */

#ifdef RC_WIN32

#elif defined RC_IPHONE
	extern FILE* iphone_fopen(const char* filename, const char* mode);
	#define fopen iphone_fopen

	#ifndef min
		#define min(a, b) ((a) < (b) ? (a) : (b))
	#endif

	#ifndef max
		#define max(a, b) ((a) > (b) ? (a) : (b))
	#endif

	// remap ARB to no ARB calls for iphone OGLES2
	#define glBindBufferARB glBindBuffer
	#define glGenBuffersARB glGenBuffers
	#define glBufferDataARB glBufferData

#endif

#endif /* RC_PLATFORM_H */


