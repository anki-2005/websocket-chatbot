#include "server.hpp"
#include "session.hpp"
#include <string>
#include "logger.hpp"
Server::Server(
    boost::asio::io_context& ioContext,
    unsigned short port
)
:
acceptor_(
    ioContext,
    boost::asio::ip::tcp::endpoint(
        boost::asio::ip::tcp::v4(),
        port)),
timer_(ioContext)
{
}

void Server::startAccept()
{
    doAccept();

    updateDashboard();
}

void Server::doAccept()
{
    acceptor_.async_accept(
        [this](boost::system::error_code ec,
               boost::asio::ip::tcp::socket socket)
        {
            if (!ec)
            {
               auto session =std::make_shared<Session>(
                  nextClientId_++,
                  std::move(socket),

        [this](std::shared_ptr<Session> session)
        {
            removeSession(session);
        },

        [this]
        (
            std::shared_ptr<Session> sender,
            const std::string& message
        )
        {
            broadcast(sender, message);
        }
    );
               sessions_.insert(session);
               Logger::info(
    "Client " +
    std::to_string(session->getClientId()) +
    " connected."
);
               stats_.clientConnected();
               session->start();
            }

            doAccept();
        });
}
void Server::removeSession(std::shared_ptr<Session> session)
{
    Logger::info(
    "Client " +
    std::to_string(session->getClientId()) +
    " disconnected."
);
    sessions_.erase(session);
   stats_.clientDisconnected();
}
void Server::broadcast(
    std::shared_ptr<Session> sender,
    const std::string& message
)
{
    std::string formattedMessage =
        "[Client " +
        std::to_string(sender->getClientId()) +
        "] " +
        message;

    Logger::info(
    "Broadcasting: " +
    formattedMessage
);

    stats_.messageProcessed(formattedMessage.size());

    for (auto& session : sessions_)
    {
        if (session != sender)
        {
            session->send(formattedMessage);
        }
    }
}
void Server::updateDashboard()
{
    dashboard_.display(stats_);

    timer_.expires_after(std::chrono::seconds(1));

    timer_.async_wait(
        [this](boost::system::error_code ec)
        {
            if (!ec)
            {
                updateDashboard();
            }
        });
}