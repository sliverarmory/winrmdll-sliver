// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include <cstdint>
#include <cstdlib>
#include <string>
//#include <iostream>
#include <sstream>
#include <vector>
#include <locale>
#include <codecvt>

#include "output.h"
#include "winrm.h"

// Sliver likes ANSI
char* w2a(WCHAR* orig);

// pseudo 'printf' buffer
Output* output;

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
	Arguments(const wchar_t* argument_string);
	std::wstring Hostname;
	std::wstring Command;
	std::wstring Username;
	std::wstring Password;
};

Arguments::Arguments(const wchar_t* argument_string) {
	std::wstring new_argstr(argument_string);
	std::wstring arg;
	std::wstringstream ss(new_argstr);

	std::vector<std::wstring> args;
	while (ss >> arg) {
		args.push_back(arg);
	}

	this->Hostname = args[0];
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

std::wstring stringToWideString(const std::string& str) {
	std::wstring s;
	append(output, "[stringToWideString] Converting: %s\n", str.c_str());

	//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	//std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	//std::wstring wide = converter.from_bytes(str);

	return s;
}

extern "C" {
	__declspec(dllexport) int __cdecl Execute(char* argsBuffer, uint32_t bufferSize, goCallback callback);
}
int Execute(char* argsBuffer, uint32_t bufferSize, goCallback callback)
{
	// global variable to hold all output returned to Sliver
	output = NewOutput(2049, callback);

	if (bufferSize < 1) {
		append(output, "You must provide an argument\n");
		return failure(output);
	}

	append(output, "[Execute] buffer size: %u\n", bufferSize);
	append(output, "[Execute] argsBuffer: %s\n", argsBuffer);

	//try {
	//Arguments args = Arguments(argsBuffer);
	Arguments args = Arguments(reinterpret_cast<wchar_t*>(argsBuffer));
	append(output, "Executing winrmdll-sliver v4");
	append(output, "[Execute] Arguments processed...\n");
	append(output, "          hostname: %ws\n", args.Hostname.c_str());
	append(output, "           command: %ws\n", args.Command.c_str());
	append(output, "          username: %ws\n", args.Username.c_str());
	append(output, "          password: %ws\n", args.Password.c_str());

	/*
	catch (const std::exception&) {
		append(output, "[Execute] Error processing arguments!\n");
		failure(output);
	}
	*/

	// convert for winrm
	//std::wstring host;
	//std::wstring command;
	//std::wstring username;
	//std::wstring password;
	
	//host = get_wstring(args.Hostname);
	//command = get_wstring(args.Command);
	//username = get_wstring(args.Username);
	//password = get_wstring(args.Password);
	
	//host = stringToWideString(args.Hostname);
	
	//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	//std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>> ansiConverter;
	//std::wstring wide;
	//wide = ansiConverter.from_bytes(args.Hostname);

	//command = stringToWideString(args.Command);
	//username = stringToWideString(args.Username);
	//password = stringToWideString(args.Password);

	// begin winrm
	/*
	if (username == L"NULL" || password == L"NULL")
	{
		username = username.erase();
		password = password.erase();
	}
	*/
	//WinRM* pWinRM = new WinRM();

	//append(output, "Username:%ws | Password:%ws\n", username.c_str(), password.c_str());
	/*
	if (pWinRM->Setup(host, username, password))
	{
		pWinRM->Execute(command);
	}

	delete pWinRM;
	*/
	
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