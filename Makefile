CXX = g++
CXXFLAGS = -Wall -Werror -std=c++17

TARGET = load_balancer

all: $(TARGET)

$(TARGET): main.o LoadBalancer.o Server.o Request.o RequestQueue.o
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.o LoadBalancer.o Server.o Request.o RequestQueue.o

main.o: main.cpp LoadBalancer.h Request.h
	$(CXX) $(CXXFLAGS) -c main.cpp

LoadBalancer.o: LoadBalancer.cpp LoadBalancer.h Server.h RequestQueue.h
	$(CXX) $(CXXFLAGS) -c LoadBalancer.cpp

Server.o: Server.cpp Server.h Request.h
	$(CXX) $(CXXFLAGS) -c Server.cpp

Request.o: Request.cpp Request.h
	$(CXX) $(CXXFLAGS) -c Request.cpp

RequestQueue.o: RequestQueue.cpp RequestQueue.h Request.h
	$(CXX) $(CXXFLAGS) -c RequestQueue.cpp

clean:
	rm -f $(TARGET) *.o
	rm -f simulation_log.txt