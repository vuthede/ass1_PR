all:
	g++ -std=c++11 schedule.cpp sequence.cpp -o sequence.o
       
clean:
	rm -rf *.o
