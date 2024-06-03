
#include <iostream>
#include <algorithm>
#include <string>

#include "nlohmann/json.hpp"
#include "xeus/xhelper.hpp"
#include "xlang_interpreter.h"
#include "httplib.h"

#if (WIN32)
#include <windows.h>
#include <winsock2.h>
#else
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif
#include <thread>
#include <atomic>
#include <sstream>

namespace nl = nlohmann;

int GetLocalValidPort()
{
	int startPort = 35000;
	int endPort = 36000;

	for (int port = startPort; port <= endPort; ++port) {
#if (WIN32)
		SOCKET testSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (testSocket == INVALID_SOCKET) {
			printf("Error creating socket.\n");
			WSACleanup();
			return -1;
		}

		sockaddr_in service;
		service.sin_family = AF_INET;
		service.sin_addr.s_addr = htonl(INADDR_ANY);
		service.sin_port = htons(port);

		if (bind(testSocket, (SOCKADDR*)&service, sizeof(service)) == 0) {
			closesocket(testSocket);
			return port;
		}

		closesocket(testSocket);
#else
		int sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd == -1) {
			std::cerr << "Error creating socket." << std::endl;
			return -1;
		}

		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port = htons(port);

		if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == 0) {
			close(sockfd);
			return port;
		}

		close(sockfd);
#endif
	}
	return -1;
}

std::string ArgEncode(const std::string& str) {
	std::ostringstream escaped;
	escaped.fill('0');
	escaped << std::hex;

	for (char c : str) {
		if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
			escaped << c;
		}
		else {
			escaped << '%' << std::setw(2) << int(static_cast<unsigned char>(c));
		}
	}

	return escaped.str();
}

namespace xlang
{

	nl::json xlang_interpreter::execute_request_impl(int execution_counter, // Typically the cell number
		const std::string& code, // Code to execute
		bool /*silent*/,
		bool /*store_history*/,
		nl::json /*user_expressions*/,
		bool /*allow_stdin*/)
	{
		std::string inCode(code);
		nl::json pub_data;
		// connect to a xlang server
		std::string inCodeLower(code);
		std::transform(inCodeLower.begin(), inCodeLower.end(), inCodeLower.begin(), ::tolower);
		size_t cmd_pos = inCodeLower.find("xlang_connect");
		if ( cmd_pos != std::string::npos)
		{
			inCodeLower.erase(std::remove_if(inCodeLower.begin(), inCodeLower.end(), ::isspace), inCodeLower.end());
			size_t start_pos = inCodeLower.find('(', cmd_pos + sizeof("xlang_connect") - 1);
			if (start_pos == std::string::npos) {
				publish_execution_error("CmdError", "1", {"xlang cmd not have '('"});
				return xeus::create_error_reply();
			}

			size_t end_pos = inCodeLower.find(')', start_pos + 1);
			if (end_pos == std::string::npos) {
				publish_execution_error("CmdError", "1", { "xlang cmd not have ')'" });
				return xeus::create_error_reply();
			}

			std::string addrNew = inCodeLower.substr(start_pos + 1, end_pos - start_pos - 1);
			if (addr == addrNew && isConnected)
			{
				pub_data["text/plain"] = "xlang server has connected";
				publish_execution_result(execution_counter, pub_data, nl::json::object());
			}
			else
			{
				if (isConnected && client)
				{
					isConnected = false;
					delete client;
				}

				int tryCount = 5;
				int tryTimes = 1;
				addr = addrNew;
				if (addr.empty()) // create a local xlang server
				{
					int port = GetLocalValidPort();
					if (port < 0)
					{

					}
					STARTUPINFO startup_info;
					PROCESS_INFORMATION process_info;

					memset(&startup_info, 0, sizeof(startup_info));
					startup_info.cb = sizeof(startup_info);

					std::string cmd = exePath + "xlang.exe -event_loop -dbg -enable_python -port " + std::to_string(port);
#if (WIN32)
					if (!CreateProcess(NULL, (LPSTR)cmd.c_str(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, (LPCSTR)exePath.c_str(), &startup_info, &process_info))
					{
						//pub_data["text/plain"] = "launch local xlang server failed";
						CloseHandle(process_info.hProcess);
						CloseHandle(process_info.hThread);
						publish_execution_error("CmdError", "1", { "launch local xlang server failed" });
						return xeus::create_error_reply();
					}
					else
					{
						pub_data["text/plain"] = "local xlang server is starting";
						publish_execution_result(execution_counter, pub_data, nl::json::object());
						
					}
#else
					pid_t child_pid;
					child_pid = fork();
					if (child_pid >= 0)
					{
						if (child_pid == 0)
						{//inside child process,run the executable 
							if (execlp(cmd.c_str(), exePath.c_str(), (char*)nullptr) == -1)
							{
								pub_data["text/plain"] = "launch local xlang server failed";
								publish_execution_result(execution_counter, pub_data, nl::json::object());
								return xeus::create_error_reply();
							}
						}
						else
						{//inside parent process, child_pid is the id for child process
							processId = child_pid;
						}
					}
#endif
					client = new httplib::Client("localhost", port);
				}
				else // connect to a running xlang server
					client = new httplib::Client(addr);
			
				client->set_read_timeout(100, 0); // 100sec for a long time running code

				std::string msg = "connecting to xlang server, try ";
				while(tryTimes <= tryCount)
				{
					pub_data["text/plain"] = msg + std::to_string(tryTimes);
					publish_execution_result(execution_counter, pub_data, nl::json::object());
					if (client->Get("/devops/checkStarted"))
					{
						isConnected = true;
						pub_data["text/plain"] = "connected to xlang server";
						publish_execution_result(execution_counter, pub_data, nl::json::object());
						break;
					}
					else
					{
						pub_data["text/plain"] = "connecting to xlang server failed, try in 1 sec";
						publish_execution_result(execution_counter, pub_data, nl::json::object());
						std::this_thread::sleep_for(std::chrono::milliseconds(1000));
					}
					++tryTimes;
					if (tryTimes > tryCount)
					{
						//pub_data["text/plain"] = "can not connect to xlang server";
						publish_execution_error("ConnectError", "1", { "can not connect to xlang server" });
						return xeus::create_error_reply();
					}
				}
			}
		}
		else //
		{
			if (!isConnected)
			{
				publish_execution_error("ConnectError", "1", { "connect a xlang server first" });
				return xeus::create_error_reply();
			}

			std::stringstream ss(code);
			char c = '\n';
			std::string codeLine;
			while (getline(ss, codeLine, c)) // run cell's code line by line
			{
				// start get print in thread
				std::thread get_print_thread([this, execution_counter]() {
					client->Get("/devops/getprint",
								[&](const char* data, size_t data_length) { // received a print data
										nl::json pub_data0;
										pub_data0["text/plain"] = data;
										publish_execution_result(execution_counter, pub_data0, nl::json::object()); // publish print data to jupyter
										return true;
									});
					});
				get_print_thread.detach();
				HANDLE hPrintThread = get_print_thread.native_handle();			
				std::string argCode = ArgEncode(codeLine);
				auto res = client->Get("/devops/runcodeline?code=" + argCode + "&exe_num=" + std::to_string(execution_counter));
			
				DWORD result = WaitForSingleObject(hPrintThread, INFINITE); // waiting for get print thread finish
				if (result == WAIT_OBJECT_0) {
					//std::cout << "Thread has finished." << std::endl;
				}
				else {
					//std::cerr << "Wait for thread failed!" << std::endl;
				}
				CloseHandle(hPrintThread);
				if(res)
				{
					pub_data["text/plain"] = "return value: " + res->body;
					publish_execution_result(execution_counter, pub_data, nl::json::object());
				}
				else
				{
					publish_execution_error("RunCodeLineError", "1", { "run code line on xlang server error" });
				}
			}
		}
		return xeus::create_successful_reply();
	}

	void xlang_interpreter::configure_impl()
	{
		// Perform some operations
	}

	nl::json xlang_interpreter::complete_request_impl(const std::string& code,
		int cursor_pos)
	{
		// Code starts with 'H', it could be the following completion
		//if (code[0] == 'H')
		//{
		//	return xeus::create_complete_reply({ "Hello", "Hey", "Howdy" }, 5, cursor_pos);
		//}
		//// No completion result
		//else
		{
			return xeus::create_complete_reply({}, cursor_pos, cursor_pos);
		}
	}

	nl::json xlang_interpreter::inspect_request_impl(const std::string& code,
		int /*cursor_pos*/,
		int /*detail_level*/)
	{
		nl::json result;

		//if (code.compare("print") == 0)
		//{
		//	return xeus::create_inspect_reply(true,
		//		{ "text/plain", "Print objects to the text stream file, [...]" });
		//}
		//else
		{
			return xeus::create_inspect_reply();
		}
	}

	nl::json xlang_interpreter::is_complete_request_impl(const std::string& /*code*/)
	{
		return xeus::create_is_complete_reply("complete");
	}

	nl::json xlang_interpreter::kernel_info_request_impl()
	{
		return xeus::create_info_reply(
			"",
			"xlang_kernel",
			"0.1.0",
			"xlang",
			"0.1",
			"text/x-python",
			".x");
	}

	void xlang_interpreter::shutdown_request_impl()
	{
		std::cout << "xlang_kernel shutdown" << std::endl;
	}

}