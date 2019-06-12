.PHNOY:all
all:server client


server:udp_server.cc
	g++ udp_server.cc -o server

client:udp_client.cc
	g++ udp_client.cc -o client


.PHONY:clean
clean:
	rm server client
