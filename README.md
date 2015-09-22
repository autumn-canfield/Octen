# Octen

Octen is a low level programing language with hot-swapping. Currently hot-swapping has been implemented, but there is no language as such.

Octen is designed to allow easy interoperability with C code. The library directory contains a library that will allow C programs to interface with Octen functions and let those functions be updated while the executable is running.

## Building

On Linux or on Windows using MinGW, a simple `make` is sufficient. I was, at one point, able to build it using visual studio, but your mileage may vary.

## Running

As stated previously there isn't really a language, right now one enters x86 opcodes in hexadecimal. Running `make` generates two executables octen(.exe) and example(.exe). One can run Octen in one terminal followed by test in another terminal, at which point one can begin entering x86 opcodes into the terminal running Octen. Opcodes should be entered all on one line with each byte prefixed by a comma and separated by a space. Once a line with opcodes has been entered Octen will send the bytes to example which will place them in executable memory. Back in Octen, entering `call` will cause a message to be sent to example, which will then execute the memory that was previously sent to it. Here's an example:

```
* ,b8 ,2a ,00 ,00 ,00 ,c3
* call
```

This sends the opcodes for:
```
mov eax, 0x00000042
ret
```
Then executes them.

After running you will probably need to kill example and/or Octen with Ctrl-C.

