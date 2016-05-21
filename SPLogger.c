#include "SPLogger.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//File open mode
#define SP_LOGGER_OPEN_MODE "w"

#define DEBUG_TITLE "DEBUG"
#define WARNING_TITLE "WARNING"
#define ERROR_TITLE "ERROR"
#define DEBUG_PRINT "---%s---\n- file: %s\n- function: %s\n- line: %d\n- message: %s\n"
#define INFO_PRINT "---INFO---\n- message: %s\n"

// Global variable holding the logger
SPLogger logger = NULL;

struct sp_logger_t {
	FILE* outputChannel; //The logger file
	bool isStdOut; //Indicates if the logger is stdout
	SP_LOGGER_LEVEL level; //Indicates the level
};

SP_LOGGER_MSG spLoggerCreate(const char* filename, SP_LOGGER_LEVEL level) {
	if (logger != NULL) { //Already defined
		return SP_LOGGER_DEFINED;
	}
	logger = (SPLogger) malloc(sizeof(*logger));
	if (logger == NULL) { //Allocation failure
		return SP_LOGGER_OUT_OF_MEMORY;
	}
	logger->level = level; //Set the level of the logger
	if (filename == NULL) { //In case the filename is not set use stdout
		logger->outputChannel = stdout;
		logger->isStdOut = true;
	} else { //Otherwise open the file in write mode
		logger->outputChannel = fopen(filename, SP_LOGGER_OPEN_MODE);
		if (logger->outputChannel == NULL) { //Open failed
			free(logger);
			logger = NULL;
			return SP_LOGGER_CANNOT_OPEN_FILE;
		}
		logger->isStdOut = false;
	}
	return SP_LOGGER_SUCCESS;
}

void spLoggerDestroy() {
	if (!logger) {
		return;
	}
	if (!logger->isStdOut) {//Close file only if not stdout
		fclose(logger->outputChannel);
	}
	free(logger);//free allocation
	logger = NULL;
}




/**
 * 	Prints error message. The error message format is given below:
 * 	---ERROR---
 * 	- file: <file>
 *  - function: <function>
 *  - line: <line>
 *  - message: <msg>
 *
 * 	<file> 	   - is the string given by file, it represents the file in which
 * 		   		 the error print call occurred.
 * 	<function> - is the string given by function, it represents the function in which
 * 			   	 the error print call occurred.
 * 	<line> 	   - is the string given by line, it represents the line in which
 * 		   		 the error print call occurred
 * 	<msg> 	   - is the string given by msg, it contains the msg given by the user
 *
 * 	Error messages will be printed at levels:
 *
 * 	SP_LOGGER_ERROR_LEVEL,
 *	SP_LOGGER_WARNING_ERROR_LEVEL,
 *	SP_LOGGER_INFO_WARNING_ERROR_LEVEL,
 *	SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL
 *
 * 	A new line will be printed after the print call.
 *
 * @param msg     	- The message to printed
 * @param file    	- A string representing the filename in which spLoggerPrintError call occurred
 * @param function 	- A string representing the function name in which spLoggerPrintError call ocurred
 * @param line		- A string representing the line in which the function call occurred
 * @return
 * SP_LOGGER_UNDIFINED 			- If the logger is undefined
 * SP_LOGGER_INVAlID_ARGUMENT	- If any of msg or file or function are null or line is negative
 * SP_LOGGER_WRITE_FAIL			- If Write failure occurred
 * SP_LOGGER_SUCCESS			- otherwise
 */


SP_LOGGER_MSG spLoggerPrintError(const char* msg, const char* file, const char* function, const int line){
	if (logger == NULL) {
		return SP_LOGGER_UNDIFINED;
	}
	if ((line < 0) || (msg == NULL) || (file == NULL) || (function == NULL)) {
		return SP_LOGGER_INVAlID_ARGUMENT;
	}

	if ((logger->level == SP_LOGGER_ERROR_LEVEL) || (logger->level == SP_LOGGER_WARNING_ERROR_LEVEL) || (logger->level == SP_LOGGER_INFO_WARNING_ERROR_LEVEL) || (logger->level == SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL)) {
		if (fprintf(logger->outputChannel, DEBUG_PRINT, ERROR_TITLE, file, function, line, msg) < 0) {
			return SP_LOGGER_WRITE_FAIL;
		}
	}
	return SP_LOGGER_SUCCESS;

}


SP_LOGGER_MSG spLoggerPrintWarning(const char* msg, const char* file,
		const char* function, const int line){

	if (logger == NULL) {
		return SP_LOGGER_UNDIFINED;
	}
	if ((line < 0) || (msg == NULL) || (file == NULL) || (function == NULL)) {
		return SP_LOGGER_INVAlID_ARGUMENT;
	}

	if ((logger->level == SP_LOGGER_WARNING_ERROR_LEVEL) || (logger->level == SP_LOGGER_INFO_WARNING_ERROR_LEVEL) || (logger->level == SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL)) {
		if (fprintf(logger->outputChannel, DEBUG_PRINT, WARNING_TITLE, file, function, line, msg) < 0) {
			return SP_LOGGER_WRITE_FAIL;
		}
	}
	return SP_LOGGER_SUCCESS;
}


SP_LOGGER_MSG spLoggerPrintInfo(const char* msg){
	if (logger == NULL) {
		return SP_LOGGER_UNDIFINED;
	}
	if (msg == NULL) {
		return SP_LOGGER_INVAlID_ARGUMENT;
	}

	if ((logger->level == SP_LOGGER_INFO_WARNING_ERROR_LEVEL) || (logger->level == SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL)) {
		if (fprintf(logger->outputChannel, INFO_PRINT, msg) < 0) {
			return SP_LOGGER_WRITE_FAIL;
		}
	}
	return SP_LOGGER_SUCCESS;

}


SP_LOGGER_MSG spLoggerPrintDebug(const char* msg, const char* file,
		const char* function, const int line){
	if (logger == NULL) {
		return SP_LOGGER_UNDIFINED;
	}
	if ((line < 0) || (msg == NULL) || (file == NULL) || (function == NULL)) {
		return SP_LOGGER_INVAlID_ARGUMENT;
	}

	if (logger->level == SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL){
		if (fprintf(logger->outputChannel, DEBUG_PRINT, DEBUG_TITLE, file, function, line, msg) < 0) {
			return SP_LOGGER_WRITE_FAIL;
		}
	}
	return SP_LOGGER_SUCCESS;

}


SP_LOGGER_MSG spLoggerPrintMsg(const char* msg){
	if (logger == NULL) {
		return SP_LOGGER_UNDIFINED;
	}
	if (msg == NULL) {
		return SP_LOGGER_INVAlID_ARGUMENT;
	}

	if (fprintf(logger->outputChannel, "%s\n", msg) < 0) {
		return SP_LOGGER_WRITE_FAIL;
	}

	return SP_LOGGER_SUCCESS;

}

