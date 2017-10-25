# TP3 Unix domain Socket

> Unix domain socket 或者 IPC socket是一种终端，可以使同一台操作系统上的两个或多个进程进行数据通信。与管道相比，Unix domain sockets 既可以使用字节流，又可以使用数据队列，而管道通信则只能使用字节流。Unix domain sockets的接口和Internet socket很像，但它不使用网络底层协议来通信。Unix domain socket 的功能是POSIX操作系统里的一种组件。
Unix domain sockets 使用系统文件的地址来作为自己的身份。它可以被系统进程引用。所以两个进程可以同时打开一个Unix domain sockets来进行通信。不过这种通信方式是发生在系统内核里而不会在网络里传播。

中文维基写的不好，参见[英文版本](https://en.wikipedia.org/wiki/Unix_domain_socket)

A Unix domain socket or IPC socket (inter-process communication socket) is a data communications endpoint for exchanging data between processes executing on the same host operating system. Like named pipes, Unix domain sockets support transmission of a reliable stream of bytes (SOCK_STREAM, compare to TCP). In addition, they support ordered and reliable transmission of datagrams (SOCK_SEQPACKET, compare to SCTP), or unordered and unreliable transmission of datagrams (SOCK_DGRAM, compare to UDP). The Unix domain socket facility is a standard component of POSIX operating systems.

The API for Unix domain sockets is similar to that of an Internet socket, but rather than using an underlying network protocol, all communication occurs entirely within the operating system kernel. Unix domain sockets use the file system as their address name space. Processes reference Unix domain sockets as file system inodes, so two processes can communicate by opening the same socket.

In addition to sending data, processes may send file descriptors across a Unix domain socket connection using the sendmsg() and recvmsg() system calls. This allows the sending processes to grant the receiving process access to a file descriptor for which the receiving process otherwise does not have access.[1] This can be used to implement a rudimentary form of capability-based security.[2] For example, this allows the Clam AntiVirus scanner to run as an unprivileged daemon on Linux and BSD, yet still read any file sent to the daemon's Unix domain socket.


## Hypothese

1. we are capable of reading message just in one time
    1. we use a large enough buffer
    2. read operation will not be interupted
2. we are always capbale of writting just in one time

refaire le boucle de read
