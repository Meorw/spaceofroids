
#ifndef RC_COMMAND_H
#define RC_COMMAND_H

#define RC_COMMAND_MAX_ARGS						32
#define RC_COMMAND_HASH_SIZE					1001

/*---------------------------------------------------------------------------*/

typedef struct {

	u32			key;
	char		*line;

} KeyBinding_t;

typedef struct Command_t Command_t;

struct Command_t {

	char		*name;
	char		*desc;
	bool		(* funcptr)(u32, char **);

	Command_t	*hash_next;
	Command_t	*list_next;

};

class Command {

	public:

		static void init();

		static void destroy();

		static void add(
				const char	*command_name,
				const char	*desc,
				bool		(* funcptr)(u32, char **)
			);

		static void remove(
				const char	*command_name
			);

		static void run(
				char		*str
			);

		static bool bindKey(
				char		*keyn,
				char		*line
			);

		static bool checkArgCount(
				u32		expected,
				u32		got,
				char	**args
			);

		static void consumeKeys();


	public:

		static Command_t	*command_hash[RC_COMMAND_HASH_SIZE];
		static Command_t	*command_head;

		static u32			n_bindings;
		static KeyBinding_t	binding[RC_KEY_LAST];

		static Key_t		key[RC_KEY_LAST];

};

#endif /* RC_COMMAND_H */

