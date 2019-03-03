# memory-sort

### requirements
* g++ compiler with C++17 support

### compile program
```$ make```

### run sorting algorithm
```$ memorysort file-to-sort sorted-file```

### additional utilities
you can run ```generate_random_file.sh fize_size``` to generate random file

```$ make dump```
dump - binary file as decimals 
Example
```$ ./filedump sorted.bin | less```

```$ make test```
test - verify that file is sorted in ASC order 
Example:
```$ ./test random_file.bin```
