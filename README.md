
# Mutithreaded Server Using C

AThe Multithreaded Web Server is a basic server application implemented in C that allows concurrent handling of incoming HTTP requests through a pool of worker threads. The server uses POSIX threads (pthread) for multithreading and provides options for configuring the number of threads, enabling debugging mode, and monitoring usage statistics.


## Commands
Compile  Commands

```bash
gcc -o server server.c -lpthread

```
Mode Commands

Start
 ```bash
./server

```
 Enable debugging mode:
 ```bash
./server --debug

```
To specify a custom number of threads (e.g., 8 threads):
```bash
./server --threads 8

```
Enable debugging mode and specify a custom number of threads:
```bash
./server --debug --threads 8

```
Enable SJF scheduling:
```bash
./server --sjf

```
Enable SJF scheduling with debugging mode and custom number of threads:
```bash
./server --debug --threads 8 --sjf

```
 Enable debugging mode and specify a custom number of threads with logging:
```bash
./server --debug --threads 8 --logfile server.log

```
Enable debugging mode and specify a custom number of threads with statistics:
```bash
./server --debug --threads 8 --stats

```
Enable statistics:
```bash
./server --stats  

```
## Acknowledgements

 - [Burinsslor.jp](https://bruinsslot.jp/post/simple-http-webserver-in-c/)
 - [README EDITOR](https://readme.so/editor)
 - [YOUTUBE TUTORIAL 1](https://www.youtube.com/watch?v=gk6NL1pZi1M)
 - [YOUTUBE TUTORIAL 2](https://www.youtube.com/watch?v=uA8X5zNOGw8&list=PL9IEJIKnBJjFZxuqyJ9JqVYmuFZHr7CFM)
 - [YOUTUBE TUTORIAL 3](https://www.youtube.com/watch?v=bdIiTxtMaKA&list=PL9IEJIKnBJjH_zM5LnovnoaKlXML5qh17)
- [RFC1945](http://ftp.ics.uci.edu/pub/ietf/http/rfc1945.html) 
