memorysort: main.cc
	g++ -g -W -Werror -pthread -std=c++17 -o memorysort main.cc bfiterator.cc

dump: file_dump.cc
	g++ -g -W -Werror -std=c++17 -o filedump file_dump.cc

test: verify_sort_order.cc
	g++ -g -W -Werror -std=c++17 -o test verify_sort_order.cc

clean:
	rm -rf *.o memorysort tmp/* filedump test
