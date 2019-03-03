# memory-sort

### overview
Binary file sort utility.

### sorting algorithm
* read file by chunks
* sort each chunk 
* write each sorted chunk into temporary file
* use external sort to create new sorted file

Note: algorithm can we improved if data in file is unique

### requirements
* g++ compiler with C++17 support

### compile program
```$ make```

### run sorting algorithm
```$ memorysort file-to-sort sorted-file```

### additional utilities
Run ```generate_random_file.sh fize_size_in_bytes``` to generate random file

```$ make dump```
dump - binary file as decimals 
Example
```$ ./filedump sorted.bin | less```

```$ make test```
test - verify that file is sorted in ASC order 
Example:
```$ ./test random_file.bin```
