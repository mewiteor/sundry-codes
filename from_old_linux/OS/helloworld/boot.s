.code16
.text
.globl c
c:
	mov %cs,%ax
	mov %ax,%ds
	mov %ax,%es
	call DispStr
	jmp .
DispStr:
	mov $BootMessage,%ax
	mov %ax,%bp
	mov $16,%cx
	mov $0x1301,%ax
	mov $0xc,%bx
	mov $0,%dl
	int $0x10
	ret
BootMessage:	.ascii	"Hello, OS world!"
.org	510
.word	0xaa55
