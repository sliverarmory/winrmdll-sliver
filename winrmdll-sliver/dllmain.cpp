#include "pch.h"

#include <cstdint>
#include <string>
#include <cwchar>
#include <cstdio>
#include <locale>
#include <vector>
#include <sstream>

#include "dllmain.h"
#include "winrm.h"

// global var for returning output to the callback
std::string msg{ "" };

// GOLD - *dynamically* allocate the buffer, clear it first,
//   and format it, before appending to the global 'msg' var
void appendFormattedMessage(const char* format, ...)
{
	va_list args;
	va_start(args, format);

	// Determine the required size of the formatted string
	va_list args_copy;
	va_copy(args_copy, args);
	int size = vsnprintf(nullptr, 0, format, args_copy);
	va_end(args_copy);

	if (size < 0) {
		// Handle error
		return;
	}

	// Allocate the buffer with the required size (+2 for null-terminator and potential overflow)
	std::unique_ptr<char[]> outBuf(new char[size + 2]);

	// Format the string into the buffer
	vsnprintf(outBuf.get(), size + 1, format, args); // Use size + 1 here

	va_end(args);

	msg.append(outBuf.get());

	// No need to deallocate the buffer explicitly (std::unique_ptr will handle it)
}

// Arguments class source: https://github.com/MrAle98/Sliver-PortBender/
class Arguments {
public:
	Arguments(const char* argument_string);
	std::string Hostname;
	std::string Command;
	std::string Username;
	std::string Password;
};

Arguments::Arguments(const char* argument_string) {
	std::istringstream iss(argument_string);
	std::string arg;
	std::string value;

	while (iss >> arg) {
		if (arg == "-i") {
			if (!(iss >> Hostname)) {
				// handle error: missing hostname value
				Hostname = ""; // Set to empty string
			}
		}
		else if (arg == "-c") {
			// lazy- reading to the end
			std::getline(iss, value);
			if (value != "") {
				Command = value.substr(1);  // Remove leading space
			}
			else {
				Command = "";
			}
		}
		else if (arg == "-u") {
			if (!(iss >> Username)) {
				// handle error: missing username value
				Username = ""; // Set to empty string
			}
		}
		else if (arg == "-p") {
			if (!(iss >> Password)) {
				// handle error: missing password value
				Password = ""; // Set to empty string
			}
		}
	}

	return;
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
	// Clear the contents of 'msg'
	msg = "";

	//appendFormattedMessage( "winrmdll-sliver v0.0.2\n");

	if (bufferSize < 1) {
		appendFormattedMessage("You must provide an argument\n");
		callback(msg.c_str(), msg.length());
		return 1;
	}

	//appendFormattedMessage("[Execute] buffer size: %u\n", bufferSize);
	//appendFormattedMessage("[Execute] argsBuffer: %s\n", argsBuffer);

	Arguments args = Arguments(argsBuffer);

	appendFormattedMessage( "[+] Arguments processed\n");
	appendFormattedMessage("       hostname: %s\n", args.Hostname.c_str());
	appendFormattedMessage("        command: %s\n", args.Command.c_str());
	appendFormattedMessage("       username: %s\n", args.Username.c_str());
	appendFormattedMessage("       password: %s\n\n", args.Password.c_str());

	// convert for winrm
	std::wstring host = ConvertToWideString(args.Hostname);
	std::wstring command = ConvertToWideString(args.Command);
	std::wstring username = ConvertToWideString(args.Username);
	std::wstring password = ConvertToWideString(args.Password);

	// begin winrm
	if (username == L"" || password == L"")
	{
		username = username.erase();
		password = password.erase();
	}
	
	//appendFormattedMessage("[Execute] (wide)username:%ls | (wide)password:%ls\n", username.c_str(), password.c_str());

	WinRM* pWinRM = new WinRM();
	
	if (pWinRM->Setup(host, username, password))
	{
		//appendFormattedMessage("[Execute] Successfully setup pWinRM\n");
		pWinRM->Execute(command);
		delete pWinRM;
	}
	else {
		appendFormattedMessage("[Execute] Failed to setup pWinRM\n");
	}

	//appendFormattedMessage("[Execute] Finished!\n");
	callback(msg.c_str(), msg.length());
	return 0;
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