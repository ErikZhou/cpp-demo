#include <stdio.h>
#include <assert.h>

#include <zmq.h>

int main()
{
    void *context = zmq_ctx_new();
    void *publisher = zmq_socket(context, ZMQ_PUB);
    int rc = zmq_bind(publisher, "tcp://127.0.0.1:5556");
    assert(rc == 0);

    while(1)
    {
        rc = zmq_send(publisher, "Hello World!", 12, 0);
        assert(rc == 12);
    }

    zmq_close(publisher);
    zmq_ctx_destroy(context);

    return 0;
}
