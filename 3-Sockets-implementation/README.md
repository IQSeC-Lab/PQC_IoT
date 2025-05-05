# Compile code

use this to compile the code if you have the library on desktop

```
gcc client.c -o client \
    -I~/Desktop/liboqs/include \
    -L~/Desktop/liboqs/build \
    -loqs -Wl,-rpath,~/Desktop/liboqs/build
```

```
gcc server.c -o server \
    -I~/Desktop/liboqs/include \
    -L~/Desktop/liboqs/build \
    -loqs -Wl,-rpath,~/Desktop/liboqs/build

```

This is with sockets
