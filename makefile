client:
	g++ -std=c++1y *.cpp main/client_main.cpp Fib2584/*.cpp -o ai_client

stdio:
	g++ -std=c++1y *.cpp main/main.cpp Fib2584/*.cpp -o ai_stdio
