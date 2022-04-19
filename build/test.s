.file "main.pas"
.text
.globl main
.type main, @function
main:
movl $mes, %ebx
push %ebx
call puts
add $4, %esp
push $0
pop %eax
ret
.data
mes:
.string "Hello, world!"

