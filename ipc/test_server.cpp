#include "ipc_server.hpp"
#include <iostream>

class Test{
public:
    Test(){

    }

    void eventCallbackHandler(const std::string& message){
        // Handle the reported event.
        std::cout<<"eventCallbackHandler message:"<< message<<"\n";
    }
    
    void run(){
        using namespace zmq_ipc;
        IpcServer server =  IpcServer("ipc:///tmp/feeds0");
        server.setEventCallback(
            std::bind(&Test::eventCallbackHandler, this, std::placeholders::_1));
        std::cout<<"IPC name:"<<server.get_name()<<"\n";
        server.init();
        server.run();
    }

private:

};

int main(){
    Test test;
    test.run();
	return 0;

}
