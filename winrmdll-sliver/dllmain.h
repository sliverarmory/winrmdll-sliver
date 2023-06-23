#pragma once
#include <string>

typedef int (*goCallback)(const char*, int);

extern std::string msg;

void appendFormattedMessage(const char* format, ...);