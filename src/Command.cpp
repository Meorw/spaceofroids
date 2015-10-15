#include "RenderChimp.h"

#include "Command.h"

/*---------------------------------------------------------------------------*/

Command_t		*Command::command_hash[RC_COMMAND_HASH_SIZE];
Command_t		*Command::command_head;

u32				Command::n_bindings;
KeyBinding_t	Command::binding[RC_KEY_LAST];

Key_t			Command::key[RC_KEY_LAST];


static char *key_name[RC_KEY_LAST] = {0};

void setup_key_names()
{
	key_name[RC_KEY_UNDEF]			= "undefined";
	key_name[RC_KEY_BACKSPACE]		= "backspace";
	key_name[RC_KEY_TAB]			= "tab";
	key_name[RC_KEY_RETURN]			= "return";
	key_name[RC_KEY_ESCAPE]			= "escape";

	key_name[RC_KEY_SINGLE_QUOTE]	= "'";
	key_name[RC_KEY_COMMA]			= ",";
	key_name[RC_KEY_MINUS]			= "-";
	key_name[RC_KEY_PERIOD]			= ".";
	key_name[RC_KEY_SLASH]			= "/";

	key_name[RC_KEY_0]				= "0";
	key_name[RC_KEY_1]				= "1";
	key_name[RC_KEY_2]				= "2";
	key_name[RC_KEY_3]				= "3";
	key_name[RC_KEY_4]				= "4";
	key_name[RC_KEY_5]				= "5";
	key_name[RC_KEY_6]				= "6";
	key_name[RC_KEY_7]				= "7";
	key_name[RC_KEY_8]				= "8";
	key_name[RC_KEY_9]				= "9";

	key_name[RC_KEY_SEMI_COLON]		= ";";
	key_name[RC_KEY_EQUALS]			= "=";
	key_name[RC_KEY_LEFT_BRACKET]	= "[";
	key_name[RC_KEY_BACKSLASH]		= "\\";
	key_name[RC_KEY_RIGHT_BRACKET]	= "]";
	key_name[RC_KEY_GRAVE_ACCENT]	= "`";

	key_name[RC_KEY_A]				= "a";
	key_name[RC_KEY_B]				= "b";
	key_name[RC_KEY_C]				= "c";
	key_name[RC_KEY_D]				= "d";
	key_name[RC_KEY_E]				= "e";
	key_name[RC_KEY_F]				= "f";
	key_name[RC_KEY_G]				= "g";
	key_name[RC_KEY_H]				= "h";
	key_name[RC_KEY_I]				= "i";
	key_name[RC_KEY_J]				= "j";
	key_name[RC_KEY_K]				= "k";
	key_name[RC_KEY_L]				= "l";
	key_name[RC_KEY_M]				= "m";
	key_name[RC_KEY_N]				= "n";
	key_name[RC_KEY_O]				= "o";
	key_name[RC_KEY_P]				= "p";
	key_name[RC_KEY_Q]				= "q";
	key_name[RC_KEY_R]				= "r";
	key_name[RC_KEY_S]				= "s";
	key_name[RC_KEY_T]				= "t";
	key_name[RC_KEY_U]				= "u";
	key_name[RC_KEY_V]				= "v";
	key_name[RC_KEY_W]				= "w";
	key_name[RC_KEY_X]				= "x";
	key_name[RC_KEY_Y]				= "y";
	key_name[RC_KEY_Z]				= "z";

	key_name[RC_KEY_SHIFT]			= "shift";
	key_name[RC_KEY_CTRL]			= "ctrl";
	key_name[RC_KEY_ALT]			= "alt";

	key_name[RC_KEY_UP_ARROW]		= "up";
	key_name[RC_KEY_DOWN_ARROW]		= "down";
	key_name[RC_KEY_LEFT_ARROW]		= "left";
	key_name[RC_KEY_RIGHT_ARROW]	= "right";

	key_name[RC_KEY_PAGE_UP]		= "pgup";
	key_name[RC_KEY_PAGE_DOWN]		= "pgdn";

	key_name[RC_KEY_F1]				= "f1";
	key_name[RC_KEY_F2]				= "f2";
	key_name[RC_KEY_F3]				= "f3";
	key_name[RC_KEY_F4]				= "f4";
	key_name[RC_KEY_F5]				= "f5";
	key_name[RC_KEY_F6]				= "f6";
	key_name[RC_KEY_F7]				= "f7";
	key_name[RC_KEY_F8]				= "f8";
	key_name[RC_KEY_F9]				= "f9";
	key_name[RC_KEY_F10]			= "f10";
	key_name[RC_KEY_F11]			= "f11";
	key_name[RC_KEY_F12]			= "f12";
}

/*---------------------------------------------------------------------------*/

static const char *CMDHELP_bind = {
	"/bind <key> <string>"
};

bool CMD_bind(
		u32		n_args,
		char	**args
	)
{
	if (!Command::checkArgCount(2, n_args, args))
		return false;

	return Command::bindKey(args[0], args[1]);
}

/*---------------------------------------------------------------------------*/

static const char *CMDHELP_list_commands = {
	"/list_commands"
};

bool CMD_list_commands(
		u32		n_args,
		char	**args
	)
{
	Command_t *cmd = Command::command_head;
	
	Console::log("  List of commands:");

	while (cmd) {
		Console::log(cmd->name);

		cmd = cmd->list_next;
	}

	return true;
}

/*---------------------------------------------------------------------------*/

static const char *CMDHELP_screenshot = {
	"/screenshot <filename>"
};

bool CMD_screenshot(
		u32		n_args,
		char	**args
	)
{
	if (!Command::checkArgCount(1, n_args, args))
		return false;

	Renderer::produceScreenshot(args[0]);

	return true;
}

/*---------------------------------------------------------------------------*/

static const char *CMDHELP_help = {
	"/help"
};

bool CMD_help(
		u32		n_args,
		char	**args
	)
{
	if (n_args > 0) {
		Command_t *cmd = Command::command_head;

		while (cmd) {
			if (strcmp(cmd->name, args[0]) == 0) {
				Console::log("  Help for \"%s\":", cmd->name);
				Console::log(cmd->desc);

				return true;
			}

			cmd = cmd->list_next;
		}

		REPORT_WARNING("Command not found");

	} else {
		Console::log("Type \"/help <Command>\" for more info on a particular command");
		Console::log("Type \"/list_commands\" to list all available commands");
		Console::log("Use F1 to toggle console");
	}

	return true;
}

/*---------------------------------------------------------------------------*/

static const char *CMDHELP_config = {
	"/config <filename>"
};

bool CMD_config(
		u32		n_args,
		char	**args
	)
{
	char buf[RC_CONSOLE_MAX_LINE_WIDTH];
	FILE *f;

	if (!Command::checkArgCount(1, n_args, args))
		return false;

	Console::log("Loading config file \"%s\"", args[0]);

	f = fopen(args[0], "r");

	if (!f) {
		REPORT_WARNING("Couldn't read config file");
		return false;
	}

	while (fgets(buf, RC_CONSOLE_MAX_LINE_WIDTH, f)) {

		if (strlen(buf) > 0)
			Command::run(buf);

	}


	fclose(f);

	return true;
}

/*---------------------------------------------------------------------------*/

static const char *CMDHELP_draw_fps = {
	"/draw_fps <0/1>"
};

bool CMD_draw_fps(
		u32		n_args,
		char	**args
	)
{
	if (n_args == 0) {
		Console::toggleDrawFPS(!Console::drawsFPS());
	} else {
		Console::toggleDrawFPS(!(args[0][0] == '0' && args[0][1] == 0));
	}

	return true;
}

/*---------------------------------------------------------------------------*/

void Command::init()
{
	setup_key_names();

	n_bindings = 0;

	command_head = 0;
	memset(command_hash, 0, sizeof(Command_t *) * RC_COMMAND_HASH_SIZE);

	Command::add("bind", CMDHELP_bind, CMD_bind);
	Command::add("list_commands", CMDHELP_list_commands, CMD_list_commands);
	Command::add("help", CMDHELP_help, CMD_help);
	Command::add("config", CMDHELP_config, CMD_config);
	Command::add("screenshot", CMDHELP_screenshot, CMD_screenshot);
	Command::add("draw_fps", CMDHELP_draw_fps, CMD_draw_fps);

	/* System */
	MemoryRegisterCommands();

	/* Nodes */
	SceneGraph::registerCommands();
	Camera::registerCommands();
	Light::registerCommands();
	Transformable::registerCommands();

	/* Resources */
	ShaderProgram::registerCommands();

}

/*---------------------------------------------------------------------------*/

void Command::destroy()
{
	Command_t *e, *c;
	u32 i;

	for (i = 0; i < RC_COMMAND_HASH_SIZE; i++) {

		e = command_hash[i];

		while (e) {
			c = e;

			e = e->hash_next;

			free(c->name);

			if (c->desc)
				free(c->desc);

			delete c;
		}

	}

	for (i = 0; i < n_bindings; i++) {
		free(binding[i].line);
	}

}

/*---------------------------------------------------------------------------*/

void Command::add(
		const char	*command_name,
		const char	*desc,
		bool		(* func_ptr)(u32, char **)
	)
{
	Command_t *e, *p, *n;
	u32 h;

	h = calculateStringHash(command_name) % RC_COMMAND_HASH_SIZE;

	e = command_hash[h];

	while (e) {

		if (strcmp(e->name, command_name) == 0) {
			remove(command_name);
			break;
		}

		e = (Command_t *) e->hash_next;
	}

	n = new Command_t;

	n->name = duplicateString(command_name);
	n->desc = duplicateString(desc);
	n->funcptr = func_ptr;
	n->hash_next = command_hash[h];
	n->list_next = 0;
	command_hash[h] = n;

	e = command_head;
	p = 0;

	while (e) {

		if (strcmp(e->name, command_name) > 0) {
			n->list_next = e;

			if (p) {
				p->list_next = n;
			} else {
				command_head = n;
			}

			return;
		}

		p = e;
		e = (Command_t *) e->list_next;
	}

	if (p) {
		p->list_next = n;
	} else {
		command_head = n;
	}

}

/*---------------------------------------------------------------------------*/

void Command::remove(
		const char	*command_name
	)
{
	Command_t *e, *p;
	u32 h = calculateStringHash(command_name) % RC_COMMAND_HASH_SIZE;

	e = command_hash[h];
	p = 0;

	while (e) {

		if (strcmp(e->name, command_name) == 0) {

			if (p) {
				p->hash_next = e->hash_next;
			} else {
				command_hash[h] = e->hash_next;
			}

			break;
		}

		p = e;
		e = e->hash_next;
	}

	e = command_head;
	p = 0;

	while (e) {

		if (strcmp(e->name, command_name) == 0) {

			if (p) {
				p->list_next = e->list_next;
			} else {
				command_head = e->list_next;
			}

			free(e->name);
			delete e;

			return;
		}

		p = e;
		e = e->list_next;
	}

}

/*---------------------------------------------------------------------------*/

void Command::run(
		char		*str
	)
{
	char *p;
	char *orig = str;

//	static const char delim[] = " \t";
	u32 i, k, n_args;
//	char buf[RC_CONSOLE_MAX_LINE_WIDTH];
	char *cmd;
	char *argbuf[RC_COMMAND_MAX_ARGS]; /* Command + RC_CONSOLE_MAX_ARGS args */

	bool success = false;
	bool in_quotes = false;

	Command_t *e;
	u32 h, len;

#ifdef RC_LOG_FRENZY
	printf("Command: %s\n", str);
#endif

	while (*str && (*str == ' ' || *str == '\t'))
		str++;

	if (*str == '/') {
		str++;
	} else {
		Console::log(orig);
		return;
	}

	p = str;

	len = strlen(str);

	if (len == 0) {

		REPORT_WARNING("No command given");
		return;
	}

	Console::log(orig);

	k = 0;

	while (*p) {
		if (*p == ' ' || *p == '\t') {
			break;
		}

		p++;
		k++;
	}

	cmd = new char[k + 1];
	strncpy(cmd, str, k);
	cmd[k] = 0;

	/* Search for command */
	h = calculateStringHash(cmd) % RC_COMMAND_HASH_SIZE;

	e = command_hash[h];

	while (e) {
		if (strcmp(e->name, cmd) == 0) {
			break;
		}

		e = e->hash_next;
	}

	if (!e) {
		REPORT_WARNING("Command %s not found", cmd);
		return;
	}

	/* Parse arguments */
	str += k;

	i = 0;

	while (*str && *str != '\n' && *str != '\r' && i < RC_COMMAND_MAX_ARGS) {
		k = 0;
		in_quotes = false;

		while (*str && (*str == ' ' || *str == '\t'))
			str++;

		if (*str == '\"') {
			in_quotes = true;
			str++;
		}

		p = str;

		while (*p) {
			if ((*p == ' ' || *p == '\t') && !in_quotes) {
				break;
			} else if (*p == '\"' && in_quotes) {
				break;
			} else if (*p == 0) {
				break;
			}

			p++;
			k++;
		}

		argbuf[i] = new char[k + 1];
		strncpy(argbuf[i], str, k);
		argbuf[i][k] = 0;

		if (in_quotes)
			k++;

		str += k;
		i++;
	}

	n_args = i;

	success	= e->funcptr(n_args, argbuf);

	if (!success) {
		REPORT_WARNING("Command %s failed", cmd);
		Console::log("Usage: %s", e->desc);
	}

	delete cmd;

	for (i = 0; i < n_args; i++)
		delete argbuf[i];

}

/*---------------------------------------------------------------------------*/

void Command::consumeKeys()
{
	bool *key_list;
	u32 i;

	key_list = Platform::getKeyState();

	if (key_list[RC_KEY_F1]) {
		if (!key[RC_KEY_F1].alive) {
			
			Console::toggle();
			key[RC_KEY_F1].time = Platform::getFrameTime();
			
		}
	} else {
		key[RC_KEY_F1].alive = 0;
	}

	if (Console::isShowing()) {
		Console::keyPress();
	} else {

		for (i = 0; i < n_bindings; i++) {
			u32 k = binding[i].key;

			if (key_list[k]) {
				if (!key[k].alive) {
					
					Command::run(binding[i].line);
					key[k].time = Platform::getFrameTime();
					
				}
			} else {
				key[k].alive = 0;
			}
		}

	}


}

/*---------------------------------------------------------------------------*/

bool Command::bindKey(
		char		*keyn,
		char		*line
	)
{
	u32 i, j;

	if (!line || *line == 0) {
		REPORT_WARNING("No string given");
		return false;
	}

	for (i = 0; i < RC_KEY_LAST; i++) {
		if (key_name[i] && strcmp(keyn, key_name[i]) == 0) {
			break;
		}
	}

	if (i == RC_KEY_LAST) {
		REPORT_WARNING("Couldn't bind key - key name \"%s\"not found", keyn);
		return false;
	}

	for (j = 0; j < n_bindings; j++) {
		if (binding[j].key == i) {
			free(binding[j].line);

			binding[j].line = duplicateString(line);
			return true;
		}
	}

	binding[n_bindings].line = duplicateString(line);
	binding[n_bindings].key = i;
	n_bindings++;
	return true;
}

/*---------------------------------------------------------------------------*/

bool Command::checkArgCount(
		u32		expected,
		u32		got,
		char	**args
	)
{
	u32 i;

	if (expected != got) {
		REPORT_WARNING("Wrong number of arguments given - got %d but expected %d.", got, expected);

		for (i = 0; i < got; i++)
			Console::log("Arg %d: \"%s\"", i, args[i]);

		return false;
	}

	return true;
}

