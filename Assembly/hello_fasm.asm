format PE console
entry _start

include 'win32a.inc'

section '.data' data readable writable

hello db "Hello World", 0ah, 0
stringformat db "%s", 0ah, 0

section '.code' code readable executable

_start:
	; mov eax, 5ah
	invoke printf, stringformat, hello
	invoke getchar
	invoke ExitProcess, 0

section '.imports' import data readable

library kernel, 'kernel32.dll',\
	msvcrt, 'msvcrt.dll'

import kernel, \
       ExitProcess, 'ExitProcess'

import msvcrt, \
       printf, 'printf',\
       getchar, '_fgetchar'