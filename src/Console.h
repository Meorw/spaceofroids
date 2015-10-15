#ifndef RC_CONSOLE_H
#define RC_CONSOLE_H

#define RC_CONSOLE_MAX_LINE_WIDTH				512
#define RC_CONSOLE_MAX_MESSAGES					2048
#define RC_CONSOLE_MAX_STACK					32

#define RC_FPS_BUF								32

typedef struct {

	char		*msg;

	union {
		u32		alive;
		f32		timestamp;
	};

} ConsoleMessage_t;

typedef union {
	f32			time;
	u32			alive;
} Key_t;

static const char default_desc[] = "No command description added";

class Console {

	public:

		static void init();

		static void destroy();

		static void toggle();

		static void log(
				const char	*str,
				...
			);

		static void keyPress();

		static void render();

		static void toggleDrawFPS(
				bool		draw
			);

		static bool drawsFPS() { return draw_fps; }

		static bool isShowing();

	private:

		static void initBackdrop();

	private:

		static Geometry			*backdrop;

		/* Command line */
		static char				cmdline[RC_CONSOLE_MAX_LINE_WIDTH];
		static u32				cmdline_pos;

		static char				cmdstack[RC_CONSOLE_MAX_STACK][RC_CONSOLE_MAX_LINE_WIDTH]; /* TODO: Fulalloc */
		static u32				cmdstack_ptr;
		static u32				cmdstack_next;

		static char				cmdbuf[RC_CONSOLE_MAX_LINE_WIDTH];


		static u32				curr_msg;
		static ConsoleMessage_t	msg_list[RC_CONSOLE_MAX_MESSAGES];
		static f32				msg_linger_time;
		static u32				msg_offset;
		static i32				n_msgs;

		static u32				off_console_draw_n_msgs;
		static u32				on_console_draw_n_msgs;

		static Text				*text;

		static bool				show;

		static f32				nextBackspace;

		static Key_t			key[RC_KEY_LAST];

		/* FPS counter */
		static bool				draw_fps;
		static f32				fps_buf[RC_FPS_BUF];
		static u32				curr_fps;


	public:
		static bool				initiated;

};

#endif /* RC_CONSOLE_H */

