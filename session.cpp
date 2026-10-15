#include "session.hpp"
#include "logger.hpp"
Session::Session(
    std::size_t clientId,
    tcp::socket socket,
    std::function<void(std::shared_ptr<Session>)> onDisconnect,
    std::function<void(std::shared_ptr<Session>, const std::string&)> onMessage)
    : clientId_(clientId),
      websocket_(std::move(socket)),
      onDisconnect_(std::move(onDisconnect)),
      onMessage_(std::move(onMessage))
{
}

void Session::start()
{
    auto self = shared_from_this();

    websocket_.async_accept(
        [this, self](beast::error_code ec)
        {
            if (!ec)
            {
                doRead();
            }
            else
            {
                Logger::error( "Handshake Error: " + ec.message());
            }
        });
}

void Session::doRead()
{
    auto self = shared_from_this();

    websocket_.async_read(
        buffer_,
        [this, self]
        (
            beast::error_code ec,
            std::size_t bytesTransferred
        )
        {
            (void)bytesTransferred;

            if (!ec)
            {
                std::string message =
                    beast::buffers_to_string(buffer_.data());

                buffer_.consume(buffer_.size());
                Logger::info("Received: " + message);
                if (onMessage_)
                {
                    onMessage_(self, message);
                }

                doRead();
            }
            else
            {
                if (ec != websocket::error::closed)
                {
                  Logger::error("Read Error: " + ec.message());
                }

                if (onDisconnect_)
                {
                    onDisconnect_(self);
                }
            }
        });
}

void Session::doWrite()
{
    auto self = shared_from_this();

    websocket_.async_write(
        net::buffer(writeQueue_.front()),
        [this, self]
        (
            beast::error_code ec,
            std::size_t bytesTransferred
        )
        {
            (void)bytesTransferred;

            if (!ec)
            {
                writeQueue_.pop_front();

                if (!writeQueue_.empty())
                {
                    doWrite();
                }
            }
            else
            {
                Logger::error("Write Error: " + ec.message());

                if (onDisconnect_)
                {
                    onDisconnect_(self);
                }
            }
        });
}

void Session::send(const std::string& message)
{
    bool writeInProgress = !writeQueue_.empty();

    writeQueue_.push_back(message);

    if (!writeInProgress)
    {
        doWrite();
    }
}
std::size_t Session::getClientId() const
{
    return clientId_;
}