
#pragma once
#include <iostream>


#define ASIO_STANDALONE 1
#include "asio.hpp"
class Connection
        : public std::enable_shared_from_this<Connection>
{
public:
    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;
    explicit Connection(asio::ip::tcp::socket socket);
    ~Connection();
    void start();
private:
    void read_len();
    void read_body();
    void do_write();
    asio::ip::tcp::socket socket_;
    asio::io_service::strand strand_;
    char buffer_[8192];
    int32_t body_length = 0;

};