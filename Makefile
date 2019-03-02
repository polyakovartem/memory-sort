memorysort: main.cc
	g++ -g -W -Werror -std=c++17 -o memorysort main.cc bfiterator.cc

dump: file_dump.cc
	g++ -g -W -Werror -std=c++17 -o filedump file_dump.cc

clean:
	rm -rf *.o memorysort tmp/*
