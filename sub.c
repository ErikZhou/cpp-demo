#include <stdio.h>
#include <assert.h>

#include <zmq.h>

int main()
{
    void *context = zmq_ctx_new();
    void *subscriber = zmq_socket(context, ZMQ_SUB);
    int rc = zmq_connect(subscriber, "tcp://127.0.0.1:5556");
    assert(rc == 0);
    rc = zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0);
    assert(rc == 0);

    char message[12];

    while(1)
    {
        rc = zmq_recv(subscriber, message, 12, 0);
        assert(rc != -1);
        printf("%s\n", message);
    }

    zmq_close(subscriber);
    zmq_ctx_destroy(context);

    return 0;
}
