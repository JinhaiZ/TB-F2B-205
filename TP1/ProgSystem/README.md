# F2B205 TP1 Programmation système Unix/Linux

## 1. Appels systèmes et fonctions

1. 声明变量的两种方式:

- 方法一. **变量存放在堆区，程序员分配释放**

	声明时
	```c
	struct stat *buffer = malloc(sizeof(struct stat));
	```
	调用时
	```c
	stat(argv[1], buffer);
	```

- 方法二 **变量存放在栈区，由编译器自动分配释放**

	声明时
	```c
	struct stat buffer;
	```
	调用时
	```c
	stat(argv[1], &buffer);
	```

> 一个由C/C++编译的程序占用的内存分为以下几个部分：
1、栈区（stack）—由编译器自动分配释放，存放函数的参数值，局部变量的值等。其操作方式类似于数据结构中的栈。
2、堆区（heap）—一般由程序员分配释放，若程序员不释放，程序结束时可能由OS回收。注意它与数据结构中的堆是两回事，分配方式倒是类似于链表。
3、全局区（静态区）（static），全局变量和静态变量的存储是放在一块的，初始化的全局变量和静态变量在一块区域，未初始化的全局变量和未初始化的静态变量在相邻的另一块区域。  程序结束后由系统释放。
4、文字常量区—常量字符串就是放在这里的。程序结束后由系统释放
5、程序代码区—存放函数体的二进制代码。

2. getpwuid() function

> la valeur de reour de function getpwuid() peut vers une zone statique et donc être écrasé par des appels successifs.

deep copy 可以解决问题

## 2. Environnement

- 通过外部变量（全局变量） environ 可获得
```  c
	extern char **environ;
```
- 通过 getenv() function

## 3. fork() 和 exec()

- fork() est l'appel système 子进程从父进程中克隆而来
返回值为0 表示在子进程，-1 表示出错，其他表示在父进程

- exec() est l'appel système
execl(), execlp(), execv(), execvp()

## 4. Le traitement des signaux

- La définition du signal: **une sorte d'interruption** logicielle envoyée par un processus par le noyau après qu'un événement particulier soit intercenu. 

-  signal() function et sigaction() function
signal(SIGINT, sighandler)
**这个函数的返回值是之前的handeler**
