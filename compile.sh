# gcc -I unpv13e/lib -g -o2 -Wall -c -o unpClient.o unpClient.c
# gcc -I unpv13e/lib -g -o2 -Wall -o unpClient unpClient.o unpv13e/libunp.a -lpthread
#
# gcc -I unpv13e/lib -g -o2 -Wall -c -o unpServer.o unpServer.c
# gcc -I unpv13e/lib -g -o2 -Wall -o unpServer unpServer.o unpv13e/libunp.a -lpthread

# gcc -I unpv13e/lib -g -o2 -Wall -c -o unpClient.o mutiprocUNPServer.c
# gcc -I unpv13e/lib -g -o2 -Wall -o unpClient unpClient.o unpv13e/libunp.a -lpthread

# gcc -I unpv13e/lib -g -o2 -Wall -c -o unpServer.o mutiprocUNPServer.c
# gcc -I unpv13e/lib -g -o2 -Wall -o mutiprocUNPServer unpServer.o unpv13e/libunp.a -lpthread

gcc -I unpv13e/lib -g -o2 -Wall -c -o unpServer.o mutiThreadUNPServer.c
gcc -I unpv13e/lib -g -o2 -Wall -o mutiThreadUNPServer unpServer.o unpv13e/libunp.a -lpthread

rm -f *.o
