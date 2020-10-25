# web-blocker ![C/C++ CI](https://github.com/caseyavila/web-blocker/workflows/C/C++%20CI/badge.svg)
A simple program to block websites through the host file; the project that introduced me to programming. Originally in Python 3, rewritten in C.

## Configuration
To configure the program, edit `config.h` to include the following:

- location of your hosts file
- ip that should be used for blocking domains

The default configuration is for a Unix-like system, though edits should *theoretically* make it MS Windows compatible (though I have not tested this).

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
This script simply makes and deletes entries in the hosts file as following:
```
127.0.0.1 <blocked_url>  # This ip can be edited; see Configuration
```
If you would like to use a different ip-address, simply change its occurances in the source code and rebuild.
