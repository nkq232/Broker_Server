# Broker_Server
Big Task




Chạy Client:

./client 127.0.0.1

Chay Server:
sudo apachectl stop
gcc -o test sqltest.c `mysql_config --cflags --libs`


gcc -pthread -o server server.c  `mysql_config --cflags --libs` -ljson-c
