// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

/*
#include <cstdint>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <locale>
#include <codecvt>
*/
#include <cstdint>
#include <string>
#include <cwchar>
#include <cstdio>
#include <locale>
#include <vector>
#include <sstream>

#include "output.h"
#include "winrm.h"

// pseudo 'printf' buffer
Output* output;

// Sliver likes ANSI
char* w2a(WCHAR* orig);

// string manipulation utility function
char* w2a(WCHAR* wstr)
{
	const size_t n = (wcslen(wstr) + 1) * 2;
	char* astr = new char[n];

	size_t dummy = 0;
	wcstombs_s(&dummy, astr, n, wstr, _TRUNCATE);

	return astr;
}

class Arguments {
public:
	Arguments(const char* argument_string);
	std::string Hostname;
	std::string Command;
	std::string Username;
	std::string Password;
};

Arguments::Arguments(const char* argument_string) {
	std::string new_argstr(argument_string);
	std::string arg;
	std::stringstream ss(new_argstr);
	append(output, "[Arguments] argument_string size: %u\n", strlen(argument_string));
	append(output, "[Arguments] new_argstr size: %u\n", new_argstr.size());
	std::vector<std::string> args;
	while (ss >> arg) {
		args.push_back(arg);
	}

	this->Hostname = args[0];
	//this->Hostname = "dest2.vic.local";
	this->Command = args[1];
	this->Username = args[2];
	this->Password = args[3];

	return;
}

std::wstring get_wstring(const std::string& s)
{
	std::wstring ws(s.begin(), s.end());
	return ws;
}

std::wstring ConvertToWideString(const std::string& narrowString) {
	// Determine the required size for the wide string
	size_t wideStringSize = 0;
	mbstowcs_s(&wideStringSize, nullptr, 0, narrowString.c_str(), 0);

	// Convert narrow string to wide string
	std::wstring wideString;
	wideString.resize(wideStringSize - 1); // excluding null terminator
	mbstowcs_s(nullptr, &wideString[0], wideStringSize, narrowString.c_str(), wideStringSize - 1);

	return wideString;

}

extern "C" {
	__declspec(dllexport) int __cdecl Execute(char* argsBuffer, uint32_t bufferSize, goCallback callback);
}
int Execute(char* argsBuffer, uint32_t bufferSize, goCallback callback)
{
	// global variable to hold all output returned to Sliver
	output = NewOutput(2049, callback);

	append(output, "Executing winrmdll-sliver v0.0.8\n");

	if (bufferSize < 1) {
		append(output, "You must provide an argument\n");
		return failure(output);
	}

	append(output, "[Execute] buffer size: %u\n", bufferSize);
	append(output, "[Execute] argsBuffer: %s\n", argsBuffer);
	append(output, "[Arguments] argsBuffer size2: %u\n", strlen(argsBuffer));

	//try {
	Arguments args = Arguments(argsBuffer);

	append(output, "[Execute] Arguments processed...\n");
	append(output, "          hostname: %s\n", args.Hostname.c_str());
	append(output, "           command: %s\n", args.Command.c_str());
	append(output, "          username: %s\n", args.Username.c_str());
	append(output, "          password: %s\n", args.Password.c_str());

	/*
	catch (const std::exception&) {
		append(output, "[Execute] Error processing arguments!\n");
		failure(output);
	}
	*/

	// convert for winrm
	std::wstring host = ConvertToWideString(args.Hostname);
	std::wstring command = ConvertToWideString(args.Command);
	std::wstring username = ConvertToWideString(args.Username);
	std::wstring password = ConvertToWideString(args.Password);


	// begin winrm
	if (username == L"NULL" || password == L"NULL")
	{
		username = username.erase();
		password = password.erase();
	}
	
	append(output, "[Execute] (wide)username:%ls | (wide)password:%ls\n", username.c_str(), password.c_str());

	WinRM* pWinRM = new WinRM();

	//pWinRM->Setup(host, username, password);

	/*
	if (pWinRM->Setup(host, username, password))
	{
		pWinRM->Execute(command);
	}
	*/

	//delete pWinRM;
	
	append(output, "[Execute] Finished!\n");

	return success(output);
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}