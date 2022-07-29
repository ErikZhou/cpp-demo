/* ipc_client.hpp
* a cpp client wrapper for zemomq
*
* by qqzhoucn@gmail.com July 29, 2022
*
* Usage:
* g++ -std=c++11 test_client.cpp -o test_client -lzmq
*/

#ifndef INCLUDED_IPC_CLIENT_HPP_
#define INCLUDED_IPC_CLIENT_HPP_

#include <string>
#include <stdio.h>
#include <zmq.h>
#include <pthread.h>
#include <string.h>
#include <chrono>
#include <thread>
#include <functional>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

namespace zmq_ipc{
    /** Implementing of IpcClient
     * using an inline implementation.
     */
    class IpcClient
    {
    public:
        /** Create a telephone list.
         * @param name The name of the list.
         */
        IpcClient(const std::string& name)
            : name_(name){}

        ~IpcClient(){
            zmq_close(zeroReqpSock_);
            zmq_ctx_destroy(zeroReqpCtx_);
        }
        
        /** init
         * @return 0 -- OK, others -- errors.
         */
        inline int connect(){
            int ret = -1;
            int recv_size = -1;
         
            zeroReqpCtx_ = zmq_ctx_new();
            zeroReqpSock_ = zmq_socket(zeroReqpCtx_, ZMQ_REQ);
            ret = zmq_connect(zeroReqpSock_, "ipc:///tmp/feeds0");
            if(ret < 0) {
                init_ = false;
                printf("=== Connect IPC failed\n");
            }else{
                ret = 0;
                init_ = true;
            }
            return ret;
        }
         
        /** run
         * @return none
         */
        inline void send_message(const std::string& message, std::string& reply){
            zmq_send(zeroReqpSock_, message.c_str(), strlen(message.c_str()), 0);
     
            int recv_size = -1;
            char recv_buf[1024] = {0};
            memset(recv_buf, 0, sizeof(recv_buf));
            recv_size = zmq_recv (zeroReqpSock_, recv_buf, 1024, 0);
            if (recv_size > 0) {
                reply.assign(recv_buf);
            }
        }

        /** Get the IPC's name.
         * @return IPC's name.
         */
        inline std::string get_name() const{
            return name_;
        }

    private:
        std::string name_ = "ipc:///tmp/feeds0";
        bool init_ = false;
        void *zeroReqpSock_ = nullptr;
        void *zeroReqpCtx_ = nullptr;
    };
}
#endif //INCLUDED_IPC_CLIENT_HPP_
