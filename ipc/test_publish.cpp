/* ipc_client test 
* a cpp client wrapper for zemomq
*
* by qqzhoucn@gmail.com August 1, 2022
*
* Usage:
* g++ -std=c++11 test_publish.cpp -o test_publish -lzmq
*/

#include "publish.hpp"
#include <iostream>
#include <chrono>
#include <thread>

class Test{
public:
    Test(){

    }

    void run(){
        using namespace zmq_pub_sub;
        Publish pub =  Publish("tcp://127.0.0.1:5556");
        int ret = pub.connect();
        if (ret ==0){
            for(int i=0;i<count_; ++i){
                std::string message="message from publisher No." + std::to_string(i);
                pub.send_message(message);
                std::cout<<"sent:"<<message<<"\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
    }

private:
    long long count_ = 10;
};

int main(){
    Test test;
    test.run();
    return 0;

}

