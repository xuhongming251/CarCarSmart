// TCPTurnServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <boost/timer.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/date_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <deque>

#include "json/json.h"

#include "common.h"
#include "ProtocolParser.h"
#include "BThreadPool.h"


using boost::asio::ip::tcp;

using std::vector;
using std::string;

using std::cout;
using std::endl;


typedef struct ClientInfo
{
    boost::shared_ptr<tcp::socket>  psocket;
    std::vector<char>               buf;
    string                          responce;
    std::size_t                     bytes_transferred;
    boost::shared_ptr<void>         dbconnection;
    ProtocolParser                  parser;

} ClientInfo;

class TCPTurnService
    : public boost::enable_shared_from_this<TCPTurnService>
{
public:
    TCPTurnService(boost::asio::io_service& s):
        io_service_(s),
        acceptor_(s, tcp::endpoint(tcp::v4(), SERVER_PORT))
    {
    }
    // 创建一个socket，并监听
    void start()
    {
        // 开始等待连接（非阻塞）
        boost::shared_ptr<tcp::socket> psocket(new tcp::socket(io_service_));
        boost::shared_ptr<ClientInfo> newclient = boost::shared_ptr<ClientInfo>(new ClientInfo);
        newclient->psocket = psocket;
        newclient->buf.resize(1024);
        newclient->dbconnection.reset();
        acceptor_.async_accept(*psocket,
                               boost::bind(&TCPTurnService::accept_handler,
                                           this, psocket, newclient, _1));

    }

    void accept_handler(boost::shared_ptr<tcp::socket> psocket,
                        boost::shared_ptr<ClientInfo> c,
                        boost::system::error_code ec)
    {
        // 继续异步监听
        start();

        if (ec)
        {
            return;
        }
        //cout << psocket->remote_endpoint().address() << endl;

        //client_list_.push_back(newclient);
        //
        psocket->async_receive(boost::asio::buffer(c->buf.data(), c->buf.size()),
                               boost::bind(&TCPTurnService::read_response_handler, this,
                                           boost::asio::placeholders::error,
                                           boost::asio::placeholders::bytes_transferred,
                                           c));
    }

    void write_handle()
    {
        //cout << "ok" << endl;
    }

    void process_thread(boost::shared_ptr<ClientInfo> c)
    {
        c->parser.push_new_data(c->buf.data(), c->bytes_transferred);

        Json::Reader json_reader;
        Json::Value json_value;

        string str_json;
        string str_fun;
        int nindex = 0;
        while (c->parser.can_parser(str_json))
        {
            json_reader.parse(str_json, json_value);
            //RequestProcess request_process(&c->dbconnection);

            boost::asio::async_write(*(c->psocket),
                                     boost::asio::buffer(c->responce.c_str(), c->responce.size()),
                                     boost::bind(&TCPTurnService::write_handle, this));

            json_value.clear();

        }


        //c->buf_.fill(0);
        c->psocket->async_receive(boost::asio::buffer(c->buf.data(), c->buf.size()),
                                  boost::bind(&TCPTurnService::read_response_handler, this,
                                              boost::asio::placeholders::error,
                                              boost::asio::placeholders::bytes_transferred,
                                              c));

    }

    void read_response_handler(const boost::system::error_code& error,
                               std::size_t bytes_transferred,
                               boost::shared_ptr<ClientInfo> c)
    {
        if (!error)
        {
            c->bytes_transferred = bytes_transferred;
            ThreadPoolInstance->do_schedule(boost::bind(&TCPTurnService::process_thread,this, c));
        }
    }

protected:

private:
    boost::asio::io_service&             io_service_;
    tcp::acceptor       acceptor_;

};


int _tmain(int argc, _TCHAR* argv[])
{

    ThreadPoolInstance->init(MAX_THREAD_COUNT);

    boost::asio::io_service ios;
    TCPTurnService s(ios);

    s.start();

    vector<boost::shared_ptr<boost::thread>> threads;
    int thread_numbers = MAX_THREAD_COUNT;

    for (int i = 0; i < thread_numbers; ++i)
    {
        boost::shared_ptr<boost::thread> thread(new boost::thread(boost::bind(&boost::asio::io_service::run, &ios)));
        threads.push_back(thread);
    }

    for (size_t i = 0; i < threads.size(); ++i)
    {
        threads[i]->join();
    }

    return 0;
}

