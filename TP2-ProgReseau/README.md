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

### getaddrinfo函数的作用

```c
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

## question 6

用ipv4发送信息，信息大小限制于65507字符

0xffff - (sizeof(IP Header) + sizeof(UDP Header))
65535 - (20 + 8) = 65507

用ipv6发送信息，信息大小限制于65527字符

0xffff - sizeof(UDP Header)
65535 - 8 = 65527

```bash
./emetteur localhost 6888 65507
./emetteur ip6-localhost 6888 65527
```

## question 8

### 发送紧急数据

![tcp header](./tcp_header.svg)
source: http://intronetworks.cs.luc.edu/current/html/tcp.html


高质量博客解释如何发送接收紧急数据(带外数据OOB)
[TCP带外数据OOB](http://www.voidcn.com/article/p-exruritx-pb.html)

### 注意

1. 紧急数据在发送和接受时没有特权，紧急数据时插入到普通的数据中进行流式发送的。在TCP层面（非应用层），发送端缓冲区中，只有先把紧急数据之前的普通数据发送后才能发送紧急数据。同时，在接收端只有接收到紧急数据之前的正常数据流之后，但接接受这个字节的带外数据。
2. URG标志紧急状态，紧急数据是否正式到达需要依据紧急指针来判定（该指针指向的位置的数据如果还未到来，说明这个紧急数据还未被接收）。
3. 在想发送缓冲区放入紧急数据后，在该紧急数据还未被发送之前（原因见1），发送的正常数据段的tcp头部就被设置了URG标记和紧急指针（就如2）。所以接受端可能会连续接受到几个tcp包，他们的包头部URG被设置，并且紧急指针都指向同一个位置（原因有1）。对接受段而言，仅接收到的第一个URG（相同紧急指针）的数据包时，通过SIGURG通知应用程序进入紧急状态（做好接受准备）。另外，应用程序可以通过select函数，在exception参数设置要监听的套接字的紧急情况，从而实现紧急数据的主动监听。实例如下：

```c
rc = select( ws + 1, &rs, NULL, &xs,NULL );
if ( rc > 0&& FD_ISSET( ws, &rs ) )
  break;
if ( rc > 0&& FD_ISSET( ws, &xs ) ) {
  t = recv( ws, x, sizeof( x ), MSG_OOB );
```

4. 如果发送方多次发送紧急数据，最后一个数据的紧急指针会将前面的覆盖。比方说你发送了一个字节的紧急数据 'X'，在 'X' 尚未被 TCP 发送前，你又发送了一个紧急数据 'Y'，那么在后面的 TCP 报文中，紧急指针都是指向了 'Y' 的。

5. 很多系统的实现，包括 Linux 将紧急数据称之为带外数据（out-of-band data, OOB），意为在连接之外传送的数据，实际上这是不对的（《TCP/IP 详解》一书称此不正确的）。即使是紧急数据，仍然会随着普通数据流一起发送，并不会单独为紧急数据开辟一条新的连接通道单独发送。紧急数据并没有被优先发送出去。

## 后记

关于select函数的理解，可以参考`man select_tut`，下面这个博客文章[linux select函数详解](http://blog.csdn.net/lingfengtengfei/article/details/12392449)

参考:

1. [23-TCP 协议（紧急标志）](http://blog.csdn.net/q1007729991/article/details/70243128)
2. [TCP带外数据（URG，MSG_OOB）](http://blog.csdn.net/ordeder/article/details/43243425)