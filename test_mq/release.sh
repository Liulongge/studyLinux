echo "build queue project"
g++  mq_creat.cpp -o mq_creat  -lpthread -lrt
g++  mq_send.cpp -o mq_send -lpthread -lrt
g++  mq_receive.cpp -o mq_receive -lpthread -lrt