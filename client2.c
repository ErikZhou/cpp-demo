// client2.c
#include <zmq.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
 
int main(int argc, char *argv[])
{
	void * zeroReqpCtx = NULL;
	void * zeroReqpSock = NULL;
	char recv_buf[1024] = {0};
	char send_buf[1024] = "client2";
	int ret = -1;
	int recv_size = -1;
 
	zeroReqpCtx = zmq_ctx_new();
    //创建socket
	
    zeroReqpSock = zmq_socket(zeroReqpCtx, ZMQ_REQ);
 
 
    ret = zmq_connect(zeroReqpSock, "ipc:///tmp/feeds0");
	if(ret < 0) {
		printf("=== Connect IPC failed\n");
		return 0;
	}
 
    while(1) {
		printf("=== send_buf(%d):%s", strlen(send_buf), send_buf);
		zmq_send(zeroReqpSock, send_buf, strlen(send_buf), 0);
 
		memset(recv_buf, 0, sizeof(recv_buf));
		recv_size = zmq_recv (zeroReqpSock, recv_buf, 1024, 0);
		printf("\n===recv_size=%d\n",recv_size);
		
		if (recv_size > 0) {
			printf("=== recv_buf: %s\n", recv_buf);
		}
	}
 
	zmq_close(zeroReqpSock);
	zmq_ctx_destroy(zeroReqpCtx);
 
	return 0;
}
