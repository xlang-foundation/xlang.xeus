#include <string>
#include "xeus/xinterpreter.hpp"
#include "xeus/xkernel.hpp"
#include "xeus/xsystem.hpp"
#include "xeus/xeus_context.hpp"
#include "xeus/xhelper.hpp"
#include "xeus/xkernel_configuration.hpp"
#include "xlang_kernel.h"
#include "xlang_server.hpp"

std::string _extract_filename(int argc, char* argv[])
{
    std::string res = "";
    for (int i = 0; i < argc; ++i)
    {
        if ((std::string(argv[i]) == "-f") && (i + 1 < argc))
        {
            res = argv[i + 1];
            for (int j = i; j < argc - 2; ++j)
            {
                argv[j] = argv[j + 2];
            }
            argc -= 2;
            break;
        }
    }
    return res;
}

int main(int argc, char* argv[])
{
    std::string file_name = _extract_filename(argc, argv);

    auto context = xeus::make_empty_context();
    using interpreter_ptr = std::unique_ptr<xlang::xlang_kernel>;
    interpreter_ptr interpreter = interpreter_ptr(new xlang::xlang_kernel());

    if (!file_name.empty())
    {
        auto config = xeus::load_configuration(file_name);
        xeus::xkernel kernel(config,
            xeus::get_user_name(),
            std::move(context),
            std::move(interpreter),
            xlang::make_xlang_server);

        std::clog <<
            "Starting xeus-cling kernel...\n\n"
            "If you want to connect to this kernel from an other client, you can use"
            " the " + file_name + " file."
            << std::endl;

        kernel.start();
    }
    else
    {

        xeus::xkernel kernel(
            xeus::get_user_name(),
            std::move(context),
            std::move(interpreter),
            xlang::make_xlang_server);

        const auto& config = kernel.get_config();
        std::clog <<
            "Starting xeus-cling kernel...\n\n"
            "If you want to connect to this kernel from an other client, just copy"
            " and paste the following content inside of a `kernel.json` file. And then run for example:\n\n"
            "# jupyter console --existing kernel.json\n\n"
            "kernel.json\n```\n{\n"
            "    \"transport\": \"" + config.m_transport + "\",\n"
            "    \"ip\": \"" + config.m_ip + "\",\n"
            "    \"control_port\": " + config.m_control_port + ",\n"
            "    \"shell_port\": " + config.m_shell_port + ",\n"
            "    \"stdin_port\": " + config.m_stdin_port + ",\n"
            "    \"iopub_port\": " + config.m_iopub_port + ",\n"
            "    \"hb_port\": " + config.m_hb_port + ",\n"
            "    \"signature_scheme\": \"" + config.m_signature_scheme + "\",\n"
            "    \"key\": \"" + config.m_key + "\"\n"
            "}\n```\n";

        kernel.start();
    }

	return 0;
}
