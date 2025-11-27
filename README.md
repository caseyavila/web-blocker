# web-blocker
A simple program to block websites through the hosts file; the project that introduced me to programming. Originally in Python 3, rewritten in C.

## Configuration
To configure the program, edit `main.c` to include the following:

- location of your hosts file
- ip that should be used for blocking domains

The default configuration is for a Unix-like system, though simply editing the location of the hosts file should *theoretically* make it MS Windows compatible (though I have not tested this).

After configuring, rebuild/reinstall.

## Building
Simply run `make` in the repository directory.

### Install
```
# make install
```

### Uninstall
```
# make uninstall
```

## Notes
This script simply comments and uncomments already-present entries in the hosts file:
```
127.0.0.1   <blocked domain>
#127.0.0.1  <unblocked domain>
```
