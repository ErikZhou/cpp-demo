/* ipc_server.hpp 
* a cpp wrapper for zemomq
*
* by qqzhoucn@gmail.com July 28, 2022
* 
* Usage:
* g++ test_server.cpp -o test_server -lzmq
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
            : name_(name), dictionary() ,is_exit_(false){}

        ~IpcServer()
        {
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
        
        /** run
         * @return none
         */
        inline void run() const{
            long long count = 0;
            while(!is_exit_ && init_){
                const char *add_reply = "proxy_reply:";
                char recv_buf[1024] = {0};
                char send_buf[1024] = {0};
                memset(recv_buf, 0, sizeof(recv_buf));
                int recv_size = zmq_recv (zeroReqpSock_, recv_buf, 1024, 0);
                //printf("\n===size=%d\n",recv_size);
                printf("=== recv_buf: %s\n", recv_buf);
                if (recv_size > 0) {
                    memset(send_buf, 0, sizeof(send_buf));
                    memcpy(send_buf, add_reply, strlen(add_reply));
                    memcpy(send_buf+strlen(add_reply), recv_buf, sizeof(recv_buf));
                    printf("=== send_buf(%d):%s\n", strlen(send_buf), send_buf);
                    printf("count=%ld\n",count++);
                    zmq_send(zeroReqpSock_, send_buf, strlen(send_buf), 0);
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }

        /** Get the list's name.
         * @return the list's name.
         */
        inline void exit()
        {
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
        std::string dictionary;
        bool is_exit_ = false;
        bool init_ = false;
        void *zeroReqpSock_ = nullptr;
        void *zeroReqpCtx_ = nullptr;
    };
}
#endif //INCLUDED_IPC_SERVER_HPP_

