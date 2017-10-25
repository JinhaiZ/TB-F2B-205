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

调用函数时注意匹配类型

```c
int getnameinfo(const struct sockaddr *sa, socklen_t salen,
                       char *host, socklen_t hostlen,
                       char *serv, socklen_t servlen, int flags);
```

signal handler申明一次就可以，不要在循环里使用

```c
signal(SIGCLD, sig_handler);
```

注意子进程的生存周期，完成任务后记得要退出`exit(EXIT_SUCCESS); `

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