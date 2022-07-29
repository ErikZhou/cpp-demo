/* ipc_client test 
* a cpp client wrapper for zemomq
*
* by qqzhoucn@gmail.com July 29, 2022
*
* Usage:
* g++ -std=c++11 test_client.cpp -o test_client -lzmq
*/

#include "ipc_client.hpp"
#include <iostream>
#include <chrono>
#include <thread>

class Test{
public:
    Test(){

    }

    void run(){
        using namespace zmq_ipc;
        IpcClient client =  IpcClient("ipc:///tmp/feeds0");
        int ret = client.connect();
        if (ret ==0){
            for(int i=0;i<count_; ++i){
                std::string message="message from client No." + std::to_string(i);
                std::string reply("");
                client.send_message(message, reply);
                std::cout<<"message:"<<message<<"\n";
                std::cout<<"reply:"<<reply<<"\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
    }

private:
    long long count_ = 100;
};

int main(){
    Test test;
    test.run();
    return 0;

}

