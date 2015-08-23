format PE console
entry _start

include 'win32a.inc'

section '.data' data readable writable

floatformat db "%f", 0ah, 0

val dq 50.45
val2 dq 22.00
res rq 1

section '.code' code readable executable

_start:
	fld qword [val]
	fld qword [val2]
	fsubp st1, st0
	fstp qword [res]
	mov eax, res
	invoke printf, floatformat, [eax], [eax + 4]
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