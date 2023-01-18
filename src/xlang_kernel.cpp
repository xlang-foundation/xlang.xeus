#include "xlang_kernel.h"

namespace xlang
{
	void xlang_kernel::configure_impl()
	{
	}

	nl::json xlang_kernel::execute_request_impl(
		int execution_counter, const std::string& code,
		bool silent, bool store_history,
		nl::json user_expressions, bool allow_stdin)
	{
		return nl::json();
	}

	nl::json xlang_kernel::complete_request_impl(
		const std::string& code, int cursor_pos)
	{
		return nl::json();
	}

	nl::json xlang_kernel::inspect_request_impl(
		const std::string& code, int cursor_pos, int detail_level)
	{
		return nl::json();
	}

	nl::json xlang_kernel::is_complete_request_impl(
		const std::string& code)
	{
		return nl::json();
	}

	nl::json xlang_kernel::kernel_info_request_impl()
	{
		return nl::json();
	}

	void xlang_kernel::shutdown_request_impl()
	{
	}
}