#pragma once
#define INIT_LOG_H

FILE* logptr;

enum level {
	INF,
	WRN,
	ERR,
	SVR
};

int logtofile(const char* string, int type, const char* caller);