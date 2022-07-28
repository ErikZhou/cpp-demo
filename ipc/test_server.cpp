#include "ipc_server.hpp"
#include <iostream>

int main()
{
	using namespace zmq_ipc;
    IpcServer server =  IpcServer("ipc:///tmp/feeds0");
	
	std::cout<<"IPC name:"<<server.get_name()<<"\n";
    server.init();
    server.run();
	return 0;

}
