# web-blocker
A simple program to block websites through the host file; the project that introduced me to programming. Originally in Python 3, rewritten in C.

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
This script simply makes and deletes entries in the hosts file as following:
```
127.0.0.1 <blocked_url>
```
If you would like to use a different ip-address, simply change its occurances in the source code and rebuild.
