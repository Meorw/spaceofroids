/*
 * Error and warning handling system
 */

#ifndef RC_RESULT_H
#define RC_RESULT_H

/* Standard messages */
static const char RC_RES_OK[] = "Everything is fine";

static const char RC_RES_WRN[] = "Undefined warning";

static const char RC_RES_ERR[] = "Undefined error";

static const char RC_RES_ERR_ASSERT[] = "Assert failed";
static const char RC_RES_ERR_ARG[] = "Invalid argument";
static const char RC_RES_ERR_MEM[] = "Out of memory";

/** Result error level */
typedef enum {
	RESULT_SUCCESS			= 0x0000,
	RESULT_NEUTRAL,
	RESULT_WARNING,
	RESULT_ERROR,
	RESULT_FILE,
	RESULT_ASSERT
} ResultType_t;

class Result {

	public:

		static void init();

		static void release();


	static FILE *logfile;

};

#define SUCCESS(r)			((r) == RESULT_SUCCESS)

#define FAILURE(r)			(!SUCCESS(r))





/** Initiate the result file (if RC_LOG_TO_FILE is defined) */
void ResultInit();

/** Wraps up the result log file */
void ResultRelease();

void ResultSetOutputFunc(
		void (* textout)(i32, char *)
	);

/** Report a result to a log file and standard output */
void ResultReport(
		const char			*file,
		const char			*function,
		i32					line,
		ResultType_t		type,

		const char			*str,
		...
	);



/** Print message to screen, if possible */
void ResultUpdateMessage(
		u32					width
	);

#ifdef RC_APPLE
	#define REPORT(m, ...)				ResultReport(__FILE__, __FUNCTION__, __LINE__, RESULT_NEUTRAL, (char *) (m), ##__VA_ARGS__)
	#define REPORT_WARNING(m, ...)		ResultReport(__FILE__, __FUNCTION__, __LINE__, RESULT_WARNING, (char *) (m), ##__VA_ARGS__)
	#define REPORT_ERROR(m, ...)		ResultReport(__FILE__, __FUNCTION__, __LINE__, RESULT_ERROR, (char *) (m), ##__VA_ARGS__)
	#define REPORT_FILE(m, ...)			ResultReport(__FILE__, __FUNCTION__, __LINE__, RESULT_FILE, (char *) (m), ##__VA_ARGS__)
#else
	#define REPORT(m, ...)				ResultReport(__FILE__, __FUNCTION__, __LINE__, RESULT_NEUTRAL, (char *) (m), __VA_ARGS__)
	#define REPORT_WARNING(m, ...)		ResultReport(__FILE__, __FUNCTION__, __LINE__, RESULT_WARNING, (char *) (m), __VA_ARGS__)
	#define REPORT_ERROR(m, ...)		ResultReport(__FILE__, __FUNCTION__, __LINE__, RESULT_ERROR, (char *) (m), __VA_ARGS__)
	#define REPORT_FILE(m, ...)			ResultReport(__FILE__, __FUNCTION__, __LINE__, RESULT_FILE, (char *) (m), __VA_ARGS__)
#endif

#ifdef RC_ENABLE_ASSERT

	#define ASSERT(m)				if (!((u32) (m))) ResultReport(__FILE__, __FUNCTION__, __LINE__, RESULT_ASSERT, RC_RES_ERR_ASSERT)

#else /* RC_ENABLE_ASSERT */

	#define ASSERT(a)

#endif /* RC_ENABLE_ASSERT */



#endif /* RC_RESULT_H */
