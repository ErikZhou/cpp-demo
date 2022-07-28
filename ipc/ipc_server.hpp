/* ipc_server.hpp 
* a cpp wrapper for zemomq
*
* by qqzhoucn@gmail.com July 28, 2022
* 
* Usage:
* g++ -std=c++11 test_server.cpp -o test_server -lzmq
*/

#ifndef INCLUDED_IPC_SERVER_HPP_
#define INCLUDED_IPC_SERVER_HPP_

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
    /** Implementing of IpcServer
     * using an inline implementation.
     */
    class IpcServer
    {
    public:
        /** Create a telephone list.
         * @param name The name of the list.
         */
        IpcServer(const std::string& name)
            : name_(name), is_exit_(false){}

        ~IpcServer(){
            zmq_close(zeroReqpSock_);
            zmq_ctx_destroy(zeroReqpCtx_);
        }
        
        /** init
         * @return 0 -- OK, others -- errors.
         */
        inline int init(){
            int recv_size = 0;
            int ret = -1;
            zeroReqpCtx_ = zmq_ctx_new();
            zeroReqpSock_ = zmq_socket(zeroReqpCtx_, ZMQ_REP);
            ret = zmq_bind(zeroReqpSock_, name_.c_str());
            if(ret < 0) {
                init_ = false;
                printf("=== Bining IPC failed with error code:%d\n", ret);
            }else{
                ret = 0;
                init_ = true;
            }
            return ret;
        }
        
        template <typename TFunc>
           void setEventCallback(TFunc&& func){
               eventCallback_ = std::forward<TFunc>(func);
           }

           void eventHandler(const std::string& message, std::string& reply){
               if (eventCallback_) {
                   // invoke registered callback object
                   eventCallback_(message, reply);
               }
           }
        
        /** run
         * @return none
         */
        inline void run(){
            while(!is_exit_ && init_){
                char recv_buf[1024] = {0};
                memset(recv_buf, 0, sizeof(recv_buf));
                int recv_size = zmq_recv (zeroReqpSock_, recv_buf, 1024, 0);
                if (recv_size > 0) {
                    std::string message;
                    // convertion from char string to c++ string
                    // using assign function
                    message.assign(recv_buf);
                    std::string reply;
                    
                    eventHandler(message, reply);
                    
                    //reply to client
                    zmq_send(zeroReqpSock_, reply.c_str(), strlen(reply.c_str()), 0);
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }

        /** Get the list's name.
         * @return the list's name.
         */
        inline void exit(){
            is_exit_ = true;
        }

        /** Get the IPC's name.
         * @return IPC's name.
         */
        inline std::string get_name() const
        {
            return name_;
        }


    private:
        std::string name_ = "ipc:///tmp/feeds0";
        bool is_exit_ = false;
        bool init_ = false;
        void *zeroReqpSock_ = nullptr;
        void *zeroReqpCtx_ = nullptr;
        std::function<void(const std::string&, std::string&)> eventCallback_;
    };
}
#endif //INCLUDED_IPC_SERVER_HPP_

