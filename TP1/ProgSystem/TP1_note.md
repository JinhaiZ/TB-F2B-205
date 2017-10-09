# F2B205 TP1 Programmation système Unix/Linux

## 1. Appels systèmes et fonctions

1. 声明变量的两种方式:

- pointeur. **allour le mémoir quand déclarer la valeur**
	```  c
	struct stat *buffer = malloc(sizeof(struct stat));
	```
- 堆？栈？
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
