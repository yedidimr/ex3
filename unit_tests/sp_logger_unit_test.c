#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "unit_test_util.h" //SUPPORTING MACROS ASSERT_TRUE/ASSERT_FALSE etc..
#include "../SPLogger.h"


// This is a helper function which checks if two files are identical
static bool identicalFiles(const char* fname1, const char* fname2) {
	FILE *fp1, *fp2;
	fp1 = fopen(fname1, "r");
	fp2 = fopen(fname2, "r");
	char ch1 = EOF, ch2 = EOF;

	if (fp1 == NULL) {
		return false;
	} else if (fp2 == NULL) {
		fclose(fp1);
		return false;
	} else {
		ch1 = getc(fp1);
		ch2 = getc(fp2);

		while ((ch1 != EOF) && (ch2 != EOF) && (ch1 == ch2)) {
			ch1 = getc(fp1);
			ch2 = getc(fp2);
		}
		fclose(fp1);
		fclose(fp2);
	}
	if (ch1 == ch2) {
		return true;
	} else {
		return false;
	}
}

//Logger is not defined
static bool basicLoggerTest() {
	ASSERT_TRUE(spLoggerPrintError("A","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_UNDIFINED);
	spLoggerDestroy();
	return true;
}

//Only Errors should be printed
static bool basicLoggerErrorTest() {
	const char* expectedFile = "basicLoggerErrorTestExp.log";
	const char* testFile = "basicLoggerErrorTest.log";
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	spLoggerDestroy();
	ASSERT_TRUE(identicalFiles(testFile,expectedFile));
	return true;
}

//All messages should be printed in debug level
static bool basicLoggerDebugTest() {
	const char* expectedFile = "basicLoggerDebugTestExp.log";
	const char* testFile = "basicLoggerDebugTest.log";
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	spLoggerDestroy();
	ASSERT_TRUE(identicalFiles(testFile,expectedFile));
	return true;
}

//In Info level all messages EXCEPT debug messages will be printed (i.e., error, warning and info).
static bool basicLoggerInfoTest() {

	const char* expectedFile = "basicLoggerInfoTestExp.log";
	const char* testFile = "basicLoggerInfoTest.log";
	ASSERT_TRUE(spLoggerCreate(testFile, SP_LOGGER_INFO_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintMsg("MSGE") == SP_LOGGER_SUCCESS);
	spLoggerDestroy();
	ASSERT_TRUE(identicalFiles(testFile,expectedFile));
	return true;

}

//In Warning level only warning and error messages will be printed.
static bool basicLoggerWarningTest() {
	const char* expectedFile = "basicLoggerWarningTestExp.log";
	const char* testFile = "basicLoggerWarningTest.log";
	ASSERT_TRUE(spLoggerCreate(testFile, SP_LOGGER_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintMsg("MSGE") == SP_LOGGER_SUCCESS);
	spLoggerDestroy();
	ASSERT_TRUE(identicalFiles(testFile,expectedFile));
	return true;

}

// test spLoggerCreate success 
static bool spLoggerCreateSuccessTest() {
	ASSERT_TRUE(spLoggerCreate(NULL, SP_LOGGER_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	spLoggerDestroy();
	return true;
}

// test SP_LOGGER_DEFINED return value of spLoggerCreate for the same logging level
static bool spLoggerCreateDefinedTest() {
	// create logger
	ASSERT_TRUE(spLoggerCreate(NULL, SP_LOGGER_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	// try to create again with the same level
	ASSERT_TRUE(spLoggerCreate(NULL, SP_LOGGER_WARNING_ERROR_LEVEL) == SP_LOGGER_DEFINED);
	spLoggerDestroy();
	
	return true;
}



// test SP_LOGGER_DEFINED return value of spLoggerCreate for the same logging level
static bool spLoggerCreateDefinedOtherLevelTest() {
	// create logger
	ASSERT_TRUE(spLoggerCreate(NULL, SP_LOGGER_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	// try to create again with different level
	ASSERT_TRUE(spLoggerCreate(NULL, SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL) == SP_LOGGER_DEFINED);
	spLoggerDestroy();
	return true;
}

// test failue when trying to open bad filename
static bool spLoggerCreateBadFilenameTest() {
	const char * badFilename = "\n~~@#%%^s\\n/tf\n" ;// TODO move to #define BAD_UNIX_FILENAME "\n~~@#%%^s\\n/tf\n" if it's possible to change log files (line number)// filename that cannot be created under unix os
	ASSERT_TRUE(spLoggerCreate(badFilename, SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL) == SP_LOGGER_CANNOT_OPEN_FILE);	
	return true;
}

// test writing to  spLoggerPrintWarning when logger is undefined
static bool spLoggerPrintWarningUndefinedTest() {
	ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_UNDIFINED);
	spLoggerDestroy();
	return true;
}


// test writing to  spLoggerPrintInfo when logger is undefined
static bool spLoggerPrintInfoUndefinedTest() {
	ASSERT_TRUE(spLoggerPrintInfo("A") == SP_LOGGER_UNDIFINED);
	spLoggerDestroy();
	return true;
}


// test writing to  spLoggerPrintDebug when logger is undefined
static bool spLoggerPrintDebugUndefinedTest() {
	ASSERT_TRUE(spLoggerPrintDebug("A","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_UNDIFINED);
	spLoggerDestroy();
	return true;
}

// test writing to  spLoggerPrintMsg when logger is undefined
static bool spLoggerPrintMsgUndefinedTest() {
	ASSERT_TRUE(spLoggerPrintMsg("A") == SP_LOGGER_UNDIFINED);
	spLoggerDestroy();
	return true;
}


// test all invalid arguments for spLoggerPrintWarning function 
static bool spLoggerPrintWarningInvalidArgTest() {
	// test edge cases
	ASSERT_TRUE(spLoggerCreate(NULL, SP_LOGGER_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintWarning(NULL,"sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT);
	ASSERT_TRUE(spLoggerPrintWarning("MSGB",NULL,__func__,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT);
	ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",NULL,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT);
	ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,-12) == SP_LOGGER_INVAlID_ARGUMENT);
	ASSERT_TRUE(spLoggerPrintWarning("MSGB",NULL,NULL,-1) == SP_LOGGER_INVAlID_ARGUMENT);
	spLoggerDestroy();

	return true;
}



// test all invalid arguments for spLoggerPrintError function 
static bool spLoggerPrintErrorInvalidArgTest() {
 	// test edge cases
	ASSERT_TRUE(spLoggerCreate(NULL, SP_LOGGER_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintError(NULL,"sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT);
	ASSERT_TRUE(spLoggerPrintError("MSGB",NULL,__func__,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT);
	ASSERT_TRUE(spLoggerPrintError("MSGB","sp_logger_unit_test.c",NULL,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT);
	ASSERT_TRUE(spLoggerPrintError("MSGB","sp_logger_unit_test.c",__func__,-12) == SP_LOGGER_INVAlID_ARGUMENT);
	ASSERT_TRUE(spLoggerPrintError("MSGB",NULL,NULL,-1) == SP_LOGGER_INVAlID_ARGUMENT);
	spLoggerDestroy();
	return true;
}



// test all invalid arguments for spLoggerPrintInfo function 
static bool spLoggerPrintInfoInvalidArgTest(){

	// test edge cases
	ASSERT_TRUE(spLoggerCreate(NULL, SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintInfo(NULL) == SP_LOGGER_INVAlID_ARGUMENT);
	spLoggerDestroy();
	return true;

}
// test all invalid arguments for spLoggerPrintDebug function 
static bool spLoggerPrintDebugInvalidArgTest() {
	ASSERT_TRUE(spLoggerCreate(NULL, SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintDebug("MSGB",NULL,__func__,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT);
	ASSERT_TRUE(spLoggerPrintDebug(NULL,"sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT);
	ASSERT_TRUE(spLoggerPrintDebug("MSGB","sp_logger_unit_test.c",NULL,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT);
	ASSERT_TRUE(spLoggerPrintDebug("MSGB","sp_logger_unit_test.c",__func__,-12) == SP_LOGGER_INVAlID_ARGUMENT);
	ASSERT_TRUE(spLoggerPrintDebug("MSGB",NULL,NULL,-1) == SP_LOGGER_INVAlID_ARGUMENT);
	spLoggerDestroy();

	return true;
}


// test all invalid arguments for spLoggerPrintMsg function 
static bool spLoggerPrintMsgInvalidArgTest() {
	ASSERT_TRUE(spLoggerCreate(NULL, SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintMsg(NULL) == SP_LOGGER_INVAlID_ARGUMENT);
	spLoggerDestroy();
	return true;
}


int main() {
	// general logger tests
	RUN_TEST(spLoggerCreateSuccessTest);
	RUN_TEST(spLoggerCreateDefinedTest);
	RUN_TEST(spLoggerCreateDefinedOtherLevelTest);
	RUN_TEST(spLoggerCreateBadFilenameTest);

	// test writing to log at each level
	RUN_TEST(basicLoggerErrorTest);
	RUN_TEST(basicLoggerDebugTest);
	RUN_TEST(basicLoggerWarningTest);
	RUN_TEST(basicLoggerInfoTest);

	// test logger undifined for all levels
	RUN_TEST(basicLoggerTest);
	RUN_TEST(spLoggerPrintWarningUndefinedTest);
	RUN_TEST(spLoggerPrintInfoUndefinedTest);
	RUN_TEST(spLoggerPrintDebugUndefinedTest);
	RUN_TEST(spLoggerPrintMsgUndefinedTest);

	// test invalid argument for all levels
	RUN_TEST(spLoggerPrintWarningInvalidArgTest);
	RUN_TEST(spLoggerPrintErrorInvalidArgTest);
	RUN_TEST(spLoggerPrintDebugInvalidArgTest);
	RUN_TEST(spLoggerPrintInfoInvalidArgTest);
	RUN_TEST(spLoggerPrintMsgInvalidArgTest);

	return 0;
}

