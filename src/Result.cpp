#include "RenderChimp.h"
#include "Result.h"

#define RC_RESULT_MAX_STRING_LENGTH					16384

FILE *logfile;
char *last_report = 0;
u32 report_occured = 0;

void (* textout_func)(i32, char *);


char result_string[RC_RESULT_MAX_STRING_LENGTH] = { 0 };

/*---------------------------------------------------------------------------*/

void ResultInit()
{

#ifdef RC_LOG_TO_FILE

	logfile = fopen("log.txt", "w");

	fprintf(logfile, "\n === RenderChimp log === \n\n");

	fflush(logfile);

#endif /* RC_LOG_TO_FILE */

}

/*---------------------------------------------------------------------------*/

void ResultRelease()
{
	if (last_report)
		free(last_report);

#ifdef RC_LOG_TO_FILE

	fprintf(logfile, "\n === End of log === \n\n");

	fflush(logfile);

	fclose(logfile);

#endif /* RC_LOG_TO_FILE */

}

/*---------------------------------------------------------------------------*/

void ResultSetOutputFunc(
		void (* textout)(i32, char *)
	)
{
	textout_func = textout;
}

/*---------------------------------------------------------------------------*/

void ResultReport(
		const char			*file,
		const char			*function,
		i32					line,
		ResultType_t		type,

		const char			*str,
		...
	)
{
	u32 len;

	va_list args;
	va_start(args, str);

	vsnprintf(result_string, RC_RESULT_MAX_STRING_LENGTH - 1, str, args);

	va_end(args);

	len = strlen(result_string);

	if (len >= (RC_RESULT_MAX_STRING_LENGTH - 1)) {
		strcat(&result_string[RC_RESULT_MAX_STRING_LENGTH - 5], "...");
	}

#ifdef RC_LOG_TO_FILE

	if (logfile) {
		if (line != -1) {

			fprintf(logfile, "%s: %s (%s, %s, %d)\n",
				type == RESULT_ERROR ? "Error" : type == RESULT_WARNING ? "Warning" : "Message",
				result_string, file, function, line);

		} else {

			fprintf(logfile, "%s\n", result_string);

		}

		fflush(logfile);
	}

#endif /* RC_LOG_TO_FILE */

//	if (r.type == RESULT_ERROR || (!report_occured || strcmp(c, last_report) != 0)) {
//		printf("%s", c);

//	
	printf("%s: %s (%s, %s, %d)\n\n",
		type == RESULT_ERROR ? "Error" : type == RESULT_WARNING ? "Warning" : "",
		result_string, file, function, line);

	if (Console::initiated) {
		if (type == RESULT_WARNING) {
			Console::log("~c9639Warning: ~r%s", result_string);
		} else if (type == RESULT_ERROR) {
			Console::log("Error:~c6009 ~r%s", result_string);
		} else if (type == RESULT_ASSERT) {
			Console::log("Assert:~c6609 ~r%s", result_string);
		} else if (type == RESULT_FILE) {
			/* Do not log to console */
		} else {
			Console::log(result_string);
		}
	} else if (type != RESULT_NEUTRAL && type != RESULT_SUCCESS && type != RESULT_FILE) {
		ResultRelease();
		exit(1);
	}

	if (type == RESULT_ERROR || type == RESULT_ASSERT) {
		ResultRelease();
		exit(1);
	}

//	if (last_report)
//		free(last_report);

//	last_report = duplicateString(c);

//	report_occured = 1;

}

/*---------------------------------------------------------------------------*/



