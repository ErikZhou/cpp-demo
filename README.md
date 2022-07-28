# cpp-demo

# Build zeromq
git clone https://github.com/zeromq/libzmq.git 
cd libzmq 
git tag 
git checkout v4.3.4 
./autogen.sh 
./configure && make check 
sudo make install 
sudo ldconfig 
cd ..

# Test on MacOS 12.4

## pub/sub
clang -std=c99 -I. sub.c -lzmq -o sub

clang -std=c99 -I. pub.c -lzmq -o pub

./pub

./sub

## IPC demo 
clang -std=c99 -I. server.c -lzmq -o server

clang -std=c99 -I. client1.c -lzmq -o client1

clang -std=c99 -I. client2.c -lzmq -o client2

## IPC C++ wrapper
