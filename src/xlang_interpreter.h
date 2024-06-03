#ifndef XLANG_INTERPRETER
#define XLANG_INTERPRETER

#include "xeus/xinterpreter.hpp"

#include "nlohmann/json.hpp"

using xeus::xinterpreter;

namespace nl = nlohmann;

namespace httplib{ class Client;};

namespace xlang
{
	class xlang_interpreter : public xinterpreter
	{
	public:

		xlang_interpreter() = default;
		virtual ~xlang_interpreter() = default;

		void set_exe_path(const std::string path){ exePath  = path; };

	private:

		void configure_impl() override;

		nl::json execute_request_impl(int execution_counter,
			const std::string& code,
			bool silent,
			bool store_history,
			nl::json user_expressions,
			bool allow_stdin) override;

		nl::json complete_request_impl(const std::string& code,
			int cursor_pos) override;

		nl::json inspect_request_impl(const std::string& code,
			int cursor_pos,
			int detail_level) override;

		nl::json is_complete_request_impl(const std::string& code) override;

		nl::json kernel_info_request_impl() override;

		void shutdown_request_impl() override;

		std::string exePath;
		std::string addr;
		bool isConnected = false;
		httplib::Client* client = nullptr;
	};
}

#endif