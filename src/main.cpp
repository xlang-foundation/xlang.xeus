#include <memory>

#include <iostream>

#include "xeus/xkernel.hpp"
#include "xeus/xkernel_configuration.hpp"
#include "xeus-zmq/xserver_zmq.hpp"

#include "xlang_interpreter.h"

std::string extract_filename(int* argc, char* argv[])
{
	std::string res = "";
	for (int i = 0; i < *argc; ++i)
	{
		if ((std::string(argv[i]) == "-f") && (i + 1 < *argc))
		{
			res = argv[i + 1];
			for (int j = i; j < *argc - 2; ++j)
			{
				argv[j] = argv[j + 2];
			}
			*argc -= 2;
			break;
		}
	}
	return res;
}

//inline std::string GetLastErrorMessage() {
//	DWORD errorCode = GetLastError();
//	LPSTR errorMessage = nullptr;
//	DWORD result = FormatMessageA(
//		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
//		NULL,
//		errorCode,
//		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//		(LPSTR)&errorMessage,
//		0,
//		NULL
//	);
//
//	if (result == 0) {
//		return "Failed to get error message";
//	}
//
//	std::string errorMessageStr(errorMessage);
//	LocalFree(errorMessage);
//	return errorMessageStr;
//}

int main(int argc, char* argv[])
{
	// Load configuration file
	std::string file_name = extract_filename(&argc, argv);
	xeus::xconfiguration config = xeus::load_configuration(file_name);

	auto context = xeus::make_context<zmq::context_t>();

	// Create interpreter instance
	using interpreter_ptr = std::unique_ptr<xlang::xlang_interpreter>;
	interpreter_ptr interpreter = interpreter_ptr(new xlang::xlang_interpreter());

	std::string path = argv[0];
	for (std::string::size_type pos = 0; (pos = path.find("\\", pos)) != std::string::npos;)
	{
		path.replace(pos, 1, "/");
		pos += 1;
	}
	interpreter->set_exe_path(path.substr(0, path.find_last_of("/") + 1));
	
	
	

	// Create kernel instance and start it
	xeus::xkernel kernel(config, xeus::get_user_name(), std::move(context), std::move(interpreter), xeus::make_xserver_zmq);
	kernel.start();

	return 0;
}