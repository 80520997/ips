
#include "Server.h"


Server::Server(const std::string& port,const std::string& ip)
        : acceptor_(io_context_),strand_(io_context_)
{

    thread_pool_size_ = std::thread::hardware_concurrency();
    asio::ip::tcp::resolver resolver(io_context_);
    asio::ip::tcp::endpoint endpoint = *resolver.resolve(ip, port).begin();
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();
    do_accept();
}


void Server::run()
{

    std::vector<std::shared_ptr<std::thread> > threads;
    for (std::size_t i = 0; i < thread_pool_size_; ++i)
    {

        std::shared_ptr<std::thread> thread(new std::thread([this](){io_context_.run();}));

        threads.push_back(thread);
    }
    for (std::size_t i = 0; i < threads.size(); ++i)
        threads[i]->join();
}


void Server::do_accept()
{
    acceptor_.async_accept(
            [this](std::error_code ec, asio::ip::tcp::socket socket)
            {
                if (!acceptor_.is_open())
                {
                    return;
                }
                if (!ec)
                {
                    (std::make_shared<Connection>(std::move(socket)))->start();
                }

                do_accept();
            }
    );
}