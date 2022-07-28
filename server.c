//server.c
#include <stdio.h>
#include <zmq.h>
#include <pthread.h>
#include <string.h>
 
 
int main()
{
	void *zeroReqpSock = NULL;
	void *zeroReqpCtx = NULL;
	int recv_size = 0;
	int ret = -1;
	char recv_buf[1024] = {0};
	char send_buf[1024] = {0};
	char *add_reply = "proxy_reply:";
 
	zeroReqpCtx = zmq_ctx_new();
 
    //创建socket
    zeroReqpSock = zmq_socket(zeroReqpCtx, ZMQ_REP);
	ret = zmq_bind(zeroReqpSock, "ipc:///tmp/feeds0");
 
	if(ret < 0) {
		printf("=== Bining IPC failed\n");
		return 0;
	}
 
    while(1)
	{
		memset(recv_buf, 0, sizeof(recv_buf));
		recv_size = zmq_recv (zeroReqpSock, recv_buf, 1024, 0);
		printf("\n===size=%d\n",recv_size);
		printf("=== recv_buf: %s\n", recv_buf);
		if (recv_size > 0) {
			memset(send_buf, 0, sizeof(send_buf));
			memcpy(send_buf, add_reply, strlen(add_reply));
			memcpy(send_buf+strlen(add_reply), recv_buf, sizeof(recv_buf));
			printf("=== send_buf(%d):%s\n", strlen(send_buf), send_buf);
			zmq_send(zeroReqpSock, send_buf, strlen(send_buf), 0);
		}
		sleep(2);
	}
	zmq_close(zeroReqpSock);
	zmq_ctx_destroy(zeroReqpCtx);
}
