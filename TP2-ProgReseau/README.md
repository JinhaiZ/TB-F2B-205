## question 1

关于::1

> 在电脑网络中，localhost （意为“本地主机”，指“这台电脑”）是给回路网络接口（loopback）的一个标准主机名，相对应的IP地址为127.0.0.1（IPv4）和[::1]（IPv6）。这个名称也是一个保留域名(RFC 2606) (参见.localhost)，为了避免同狭义定义主机名混淆而单独列出。

## question 2

建立子进程
fork()
管理不同进程
```c
switch(pid) {
    case -1: //problem
    fprintf(stderr, "Error of server\n");
    exit(EXIT_FAILURE);
    case 0: //we are in the child
    communication(sfd, s, from, fromlen, host, nread, nwrite, ns, message, buf);
    if (st == -1) {
    fprintf(stderr, "Error of child process\n");
    exit(EXIT_FAILURE);
    }
    default: //we are in the father
    signal(SIGCLD, sig_handler);
}
```

注意zombie

```c
void sig_handler(int signum) {
  printf("child signal recevied\n");
  int pid = wait(NULL);
  printf("child process %d terminated\n", pid);
}
```

## question 3

why use out.flush()

> Flushes the output stream and forces any buffered output bytes to be written out. The general contract of flush is that calling it is an indication that, if any bytes previously written have been buffered by the implementation of the output stream, such bytes should immediately be written to their intended destination.

## question 4

### 调用函数时注意匹配类型

```c
int getnameinfo(const struct sockaddr *sa, socklen_t salen,
                       char *host, socklen_t hostlen,
                       char *serv, socklen_t servlen, int flags);
```

signal handler申明一次就可以，不要在循环里使用

```c
signal(SIGCLD, sig_handler);
```

### 注意子进程的生存周期

完成任务后记得要退出`exit(EXIT_SUCCESS); `

```c
switch(pid) {
  case -1: //problem
    fprintf(stderr, "Error of server\n");
    exit(EXIT_FAILURE);
  case 0: //we are in the child
    // life cycle of child
    printf("We are in the child\n");
    close(sfd);
    communication(ns, (struct sockaddr *)&from, fromlen);
    exit(EXIT_SUCCESS); 
  default: //we are in the father
    printf("we are in the father\n");
}
```

### 这个程序的安全隐患

http://localhost:8888/index.html/../../some-file-in-root-folder

如果有类似上面的http请求，这个程序会返回真实的父目录中的文件

### 如何读取文件

```c
char buf[SIZE];
fd = open("file_path");

n = read(fd, buf, SIZE);
write(soc, buf, n);


```

## question 5

### `getaddrinfo`函数的作用

````c
int getaddrinfo(const char *node, const char *service,
                const struct addrinfo *hints,
                struct addrinfo **res);
```

> Given node and service, which identify an Internet host and a service, getaddrinfo() returns one or more addrinfo structures, each of which contains an Internet address that can be specified in a call to bind(2) or connect(2).  The getaddrinfo() function  combines the functionality provided by the gethostbyname(3) and getservbyname(3) functions into a single interface, but unlike the latter functions, getaddrinfo() is reentrant and allows programs to eliminate IPv4-versus-IPv6 dependencies.

### 注意sendto和recvfrom的函数签名

尤其是addrlen参数的类型要求是不一样的

```c
ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
              const struct sockaddr *dest_addr, socklen_t addrlen);
```

```c
ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
                struct sockaddr *src_addr, socklen_t *addrlen)
```

### 问题

向不存在的地址发送消息时将得到下列错误

```bash
sendto: Network is unreachable
```