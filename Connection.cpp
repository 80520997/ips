#include <memory>
#include "Connection.h"
#include "Ip.h"



Connection::Connection(asio::ip::tcp::socket socket)
        : socket_(std::move(socket)),strand_(socket_.get_io_context())
{
}

Connection::~Connection() {
    std::error_code ignored_ec;
    socket_.shutdown(asio::ip::tcp::socket::shutdown_both,ignored_ec);
}

void Connection::start(){
    auto self(shared_from_this());
    strand_.post([this,self](){read_len();});

}
void Connection::read_len()
{
    auto self(shared_from_this());
    asio::async_read(socket_,asio::buffer(&body_length, sizeof(body_length)),
        [this,self](std::error_code ec, std::size_t bytes_transferred)
        {
            if (!ec)
            {
                body_length = asio::detail::socket_ops::network_to_host_long(body_length);
                strand_.post([this,self](){read_body();});
            }
        }
    );


}

void Connection::read_body()
{
    auto self(shared_from_this());
    asio::async_read(socket_,asio::buffer(buffer_, body_length),
        [this,self](std::error_code ec, std::size_t bytes_transferred)
        {
            if (!ec)
            {
                buffer_[body_length] = '\0';
                body_length = asio::detail::socket_ops::network_to_host_long(body_length);
                strand_.post([this,self](){do_write();});
            } 
        }
    );


}

void Connection::do_write()
{
    auto addr = Ip::find(buffer_);
    body_length = asio::detail::socket_ops::host_to_network_long(addr.length());
    memcpy(static_cast<void *>(buffer_),static_cast<void *>(&body_length),sizeof(body_length));
    memcpy(static_cast<void *>(buffer_ + sizeof(body_length)),static_cast<const void *>(addr.c_str()),addr.length());
    auto self(shared_from_this());
    asio::async_write(socket_, asio::buffer(buffer_, sizeof(body_length) + addr.length()),
        [this, self](std::error_code ec, std::size_t)
        {
            if (!ec)
            {
                read_len();
            } 
        }
    );
}