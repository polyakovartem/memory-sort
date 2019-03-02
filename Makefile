memorysort: main.cc
	g++ -g -W -Werror -std=c++17 -o memorysort main.cc

clean:
	rm -rf *.o memorysort tmp/*
