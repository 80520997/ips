#pragma once
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#define ASIO_STANDALONE 1
#include "asio.hpp"
#include "Connection.h"

class Server
{
public:
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;
    explicit Server(const std::string& port,const std::string& ip);
    void run();

private:
    void do_accept();
    std::size_t thread_pool_size_;
    asio::io_service io_context_;
    asio::ip::tcp::acceptor acceptor_;
    asio::io_service::strand strand_;

};