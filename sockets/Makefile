

all:client1.cpp client2.cpp server_seq.cpp server_fork.cpp server_thread.cpp server_poll.cpp server_epoll.cpp server_select.cpp
	g++ -pthread client1.cpp -o client1
	g++ -pthread client2.cpp -o client2
	g++ -pthread server_seq.cpp -o server_seq
	g++ -pthread server_fork.cpp -o server_fork
	g++ -pthread server_select.cpp -o server_select
	g++ -pthread server_poll.cpp -o server_poll
	g++ -pthread server_epoll.cpp -o server_epoll
	g++ -pthread server_thread.cpp -o server_thread	

clean:
	@echo "Cleaning up..."
	rm -rvf *.o ${BINS}