1、创建消息队列
“/message”: 消息队列名字

./mq_creat /message   


2、向特定队列发送消息
“/message”: 消息队列名字
"send this message": 消息内容
“10”：优先级
./mq_send /message "send this message" 10


3、接收消息
“/message”：要接收消息的名字

./mq_receive /message