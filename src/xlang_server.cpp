
#include "xlang_server.hpp"

namespace xlang
{

xlang_messenger::xlang_messenger(xlang_server* server)
    : p_server(server)
{
}

nl::json xlang_messenger::send_to_shell_impl(const nl::json& message)
{
    return p_server->notify_internal_listener(message);
}

xlang_server::xlang_server()
    : m_messenger(this)
{
}

void xlang_server::receive_shell(xmessage message)
{
    m_shell_messages.push(std::move(message));
}

void xlang_server::receive_control(xmessage message)
{
    m_control_messages.push(std::move(message));
}

void xlang_server::receive_stdin(xmessage message)
{
    m_stdin_messages.push(std::move(message));
}

std::size_t xlang_server::shell_size() const
{
    return m_shell_messages.size();
}

xmessage xlang_server::read_shell()
{
    return read_impl(m_shell_messages);
}

std::size_t xlang_server::control_size() const
{
    return m_control_messages.size();
}

xmessage xlang_server::read_control()
{
    return read_impl(m_control_messages);
}

std::size_t xlang_server::stdin_size() const
{
    return m_stdin_messages.size();
}

xmessage xlang_server::read_stdin()
{
    return read_impl(m_stdin_messages);
}

std::size_t xlang_server::iopub_size() const
{
    return m_iopub_messages.size();
}

xpub_message xlang_server::read_iopub()
{
    xpub_message res = std::move(m_iopub_messages.back());
    m_iopub_messages.pop();
    return res;
}

xmessage xlang_server::read_impl(message_queue& q)
{
    xmessage res = std::move(q.back());
    q.pop();
    return res;
}

xcontrol_messenger& xlang_server::get_control_messenger_impl()
{
    return m_messenger;
}

void xlang_server::send_shell_impl(xmessage message)
{
    m_shell_messages.push(std::move(message));
}

void xlang_server::send_control_impl(xmessage message)
{
    m_control_messages.push(std::move(message));
}

void xlang_server::send_stdin_impl(xmessage message)
{
    m_stdin_messages.push(std::move(message));
}

void xlang_server::publish_impl(xpub_message message, channel)
{
    m_iopub_messages.push(std::move(message));
}

void xlang_server::start_impl(xpub_message message)
{
    m_iopub_messages.push(std::move(message));
}

void xlang_server::abort_queue_impl(const listener&, long)
{
}

void xlang_server::stop_impl()
{
}

void xlang_server::update_config_impl(xconfiguration&) const
{
}

std::unique_ptr<xserver> make_xlang_server(xcontext&,
                                          const xconfiguration&,
                                          nl::json::error_handler_t)
{
    return std::make_unique<xlang_server>();
}

}

