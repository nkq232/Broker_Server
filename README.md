# Broker_Server
Big Task


Update token:
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

Chạy Client:

./client 127.0.0.1

Chay Server:
sudo apachectl stop
gcc -o test sqltest.c `mysql_config --cflags --libs`


gcc -ljson-c -pthread -o test server.c sqltest.c `mysql_config --cflags --libs`
