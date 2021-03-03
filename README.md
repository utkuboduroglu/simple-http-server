# simple-http-server
This is a simple http server written in C++. The main purpose of this project is to practice the following
programming topics in an applied manner:

* Network programming
* Multithreading and thread safety
* Optimization, algorithms and data structures
* async io

## Resources followed
1. For network programming (in C): [beej's guide to network programming](https://beej.us/guide/bgnet/)
2. For multithreading (in C): [multithreading in C, POSIX style](https://softpixel.com/~cwright/programming/threads/threads.c.php)
## TODO
1. Replace `max_connections` with an `std::vector<pthread_t>` to stop after every active connection is closed (with the help of a do-while).
2. Allow for `GET` requests to fetch files.
3. Use `realloc()` instead of `MAX_RECV_BUFFER_SIZE` to allow for more than 128 characters at a time.

Furthermore, after we have a minimal functioning 'http server', we additionaly will want to do:

* Replace all C-style programming paradigms with the corresponding C++ ones.
