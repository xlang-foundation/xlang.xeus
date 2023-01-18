#pragma once

#include <cstddef>
#include <memory>
#include <queue>

#include "xeus/xcontrol_messenger.hpp"
#include "xeus/xeus_context.hpp"
#include "xeus/xserver.hpp"

using namespace xeus;

namespace xlang
{
    class xlang_server;
    class xlang_messenger : public xcontrol_messenger
    {
    public:

        xlang_messenger(xlang_server* server);
        virtual ~xlang_messenger() = default;

    private:

        nl::json send_to_shell_impl(const nl::json& message) override;

        xlang_server* p_server;
    };

    class xlang_server : public xserver
    {
    public:

        xlang_server();
        virtual ~xlang_server() = default;
        
        xlang_server(const xlang_server&) = delete;
        xlang_server& operator=(const xlang_server&) = delete;
        
        xlang_server(xlang_server&&) = delete;
        xlang_server& operator=(xlang_server&&) = delete;

        void receive_shell(xmessage message);
        void receive_control(xmessage message);
        void receive_stdin(xmessage message);

        std::size_t shell_size() const;
        xmessage read_shell();

        std::size_t control_size() const;
        xmessage read_control();
        
        std::size_t stdin_size() const;
        xmessage read_stdin();

        std::size_t iopub_size() const;
        xpub_message read_iopub();

        using xserver::notify_internal_listener;

    private:

        xcontrol_messenger& get_control_messenger_impl() override;

        void send_shell_impl(xmessage message) override;
        void send_control_impl(xmessage message) override;
        void send_stdin_impl(xmessage message) override;
        void publish_impl(xpub_message message, channel c) override;

        void start_impl(xpub_message message) override;
        void abort_queue_impl(const listener& l, long polling_interval) override;
        void stop_impl() override;
        void update_config_impl(xconfiguration& config) const override;

        using message_queue = std::queue<xmessage>;
        xmessage read_impl(message_queue& q);

        xlang_messenger m_messenger;

        message_queue m_shell_messages;
        message_queue m_control_messages;
        message_queue m_stdin_messages;
        std::queue<xpub_message> m_iopub_messages;
    };

    std::unique_ptr<xserver> make_xlang_server(xcontext& context,
                                              const xconfiguration& config,
                                              nl::json::error_handler_t eh);
}

