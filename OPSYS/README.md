# Strace

```sh
strace ./idk.c

strace -f ./idk.c # child processses

strace -e trace=execve ./idk.c # execve

strace -c ./idk.c # summary

strace -e trace="%desc" ./idk.c # file descriptor

```

```gdb
-exec set follow-fork-mode child
```
