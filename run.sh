#gcc mutiProc_Server.c -o server1
#gcc mutiThread_Server.c -o  server
#gcc client.c -o client
#gcc mutiProc_Client.c -o mtclient -w
gcc mutiThread_Client.c -o mtpclient -w
./mtpclient 127.0.0.1 8080
