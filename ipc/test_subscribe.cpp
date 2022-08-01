/* ipc_server test
* a cpp server wrapper for zemomq
*
* by qqzhoucn@gmail.com August 1, 2022
*
* Usage:
* g++ -std=c++11 test_subscribe.cpp -o test_subscribe -lzmq
*/

#include "subscribe.hpp"
#include <iostream>

class Test{
public:
    Test(){

    }

    void eventCallbackHandler(const std::string& message){
        // Handle the reported event.
        std::cout<<"eventCallbackHandler No[" <<count_<<"]:"<< message<<"\n";
        count_++;
    }
    
    void run(){
        using namespace zmq_pub_sub;
        Subscribe sub =  Subscribe("tcp://127.0.0.1:5556");

        using MyFunction = std::function<void(const std::string&)>;
        MyFunction fun = std::bind(&Test::eventCallbackHandler, this, std::placeholders::_1);
        sub.setEventCallback(fun);
        
        std::cout<<"sub name:"<<sub.get_name()<<"\n";
        int ret = sub.init();
        std::cout<<"ret="<<ret<<"\n";
        sub.run();
    }

private:
    long long count_ = 0;
};

int main(){
    Test test;
    test.run();
	return 0;

}
