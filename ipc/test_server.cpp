/* ipc_server test
* a cpp server wrapper for zemomq
*
* by qqzhoucn@gmail.com July 29, 2022
*
* Usage:
* g++ -std=c++11 test_server.cpp -o test_server -lzmq
*/

#include "ipc_server.hpp"
#include <iostream>

class Test{
public:
    Test(){

    }

    void eventCallbackHandler(const std::string& message, std::string& reply){
        // Handle the reported event.
        std::cout<<"eventCallbackHandler message:"<< message<<"\n";
        
        //added your only code here
        reply = "replay from server";
        std::cout<<"eventCallbackHandler No[" <<count_<<"] reply:"<< reply<<"\n";
        count_++;
    }
    
    void run(){
        using namespace zmq_ipc;
        IpcServer server =  IpcServer("ipc:///tmp/feeds0");

        using FyFunction = std::function<void(const std::string&, std::string&)>;
        FyFunction fun = std::bind(&Test::eventCallbackHandler, this, std::placeholders::_1, std::placeholders::_2);
        server.setEventCallback(fun);
        
        std::cout<<"IPC name:"<<server.get_name()<<"\n";
        server.init();
        server.run();
    }

private:
    long long count_ = 0;
};

int main(){
    Test test;
    test.run();
	return 0;

}
