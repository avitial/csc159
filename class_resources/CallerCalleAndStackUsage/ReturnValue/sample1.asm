	.file	"sample1.c"
	.version	"01.01"
.stabs "/gaia/home/faculty/changw/www/137/lecture/Chapter04-Supplement/sample1/",100,0,0,.Ltext0
.stabs "sample1.c",100,0,0,.Ltext0
.text
.Ltext0:
	.stabs	"gcc2_compiled.", 0x3c, 0, 0, 0
.stabs "int:t1=r1;-2147483648;2147483647;",128,0,0,0
.stabs "char:t2=r2;0;127;",128,0,0,0
.stabs "long int:t3=r1;-2147483648;2147483647;",128,0,0,0
.stabs "unsigned int:t4=r1;0;-1;",128,0,0,0
.stabs "long unsigned int:t5=r1;0;-1;",128,0,0,0
.stabs "long long int:t6=r1;01000000000000000000000;0777777777777777777777;",128,0,0,0
.stabs "long long unsigned int:t7=r1;0000000000000;01777777777777777777777;",128,0,0,0
.stabs "short int:t8=r1;-32768;32767;",128,0,0,0
.stabs "short unsigned int:t9=r1;0;65535;",128,0,0,0
.stabs "signed char:t10=r1;-128;127;",128,0,0,0
.stabs "unsigned char:t11=r1;0;255;",128,0,0,0
.stabs "float:t12=r1;4;0;",128,0,0,0
.stabs "double:t13=r1;8;0;",128,0,0,0
.stabs "long double:t14=r1;12;0;",128,0,0,0
.stabs "complex int:t15=s8real:1,0,32;imag:1,32,32;;",128,0,0,0
.stabs "complex float:t16=r16;4;0;",128,0,0,0
.stabs "complex double:t17=r17;8;0;",128,0,0,0
.stabs "complex long double:t18=r18;12;0;",128,0,0,0
.stabs "void:t19=19",128,0,0,0
.stabs "__gnuc_va_list:t20=*19",128,0,23,0
.stabs "va_list:t20",128,0,112,0
.stabs "int8:t2",128,0,26,0
.stabs "int16:t8",128,0,27,0
.stabs "int32:t1",128,0,28,0
.stabs "int64:t6",128,0,29,0
.stabs "uint8:t11",128,0,31,0
.stabs "uint16:t9",128,0,32,0
.stabs "uint32:t4",128,0,33,0
.stabs "uint64:t7",128,0,34,0
.stabs "pos_t:t5",128,0,37,0
.stabs "REGISTER:t4",128,0,38,0
.stabs "ptrdiff_t:t3",128,0,43,0
.stabs "BOOLEAN:t11",128,0,46,0
.stabs "BOOL:t4",128,0,47,0
.stabs "BYTE:t11",128,0,50,0
.stabs "WORD:t9",128,0,51,0
.stabs "DWORD:t4",128,0,52,0
.stabs "ADDRESS:t4",128,0,54,0
.stabs "caddr_t:t21=*2",128,0,56,0
.stabs "off_t:t3",128,0,57,0
.stabs "TEXT:t11",128,0,59,0
.stabs "STRPTR:t22=*11",128,0,60,0
.stabs "uchar:t11",128,0,62,0
.stabs "ushort:t9",128,0,63,0
.stabs "uint:t4",128,0,64,0
.stabs "ulong:t5",128,0,65,0
.stabs "u_char:t11",128,0,75,0
.stabs "u_short:t9",128,0,76,0
.stabs "u_int:t4",128,0,77,0
.stabs "u_int32_t:t4",128,0,78,0
.stabs "u_int16_t:t4",128,0,79,0
.stabs "int32_t:t1",128,0,81,0
.stabs "uint8_t:t11",128,0,83,0
.stabs "uint16_t:t9",128,0,84,0
.stabs "uint32_t:t4",128,0,85,0
.stabs "n_short:t9",128,0,87,0
.stabs "n_time:t4",128,0,88,0
.stabs "n_long:t4",128,0,89,0
.stabs "splx_t:t23=ar1;0;0;4",128,0,36,0
.stabs "LONGWORD:t4",128,0,40,0
.stabs "DebugSupport:T24=e_DEBUG_BEFORE:-1,_DEBUG_NOPE:0,\\",128,0,0,0
.stabs "_DEBUG_USING_GDB:1,;",128,0,0,0
.section	.rodata
.LC0:
	.string	"x is %d\n"
.LC1:
	.string	"y is %d\n"
.text
	.align 4
.stabs "DispMsg:F1",36,0,13,DispMsg
.stabs "x:p1",160,0,12,8
.stabs "y:p1",160,0,12,12
.globl DispMsg
	.type	 DispMsg,@function
DispMsg:
.stabn 68,0,13,.LM1-DispMsg
.LM1:
	pushl %ebp
	movl %esp,%ebp
	subl $4,%esp
.stabn 68,0,14,.LM2-DispMsg
.LM2:
.LBB2:
	movl $5,-4(%ebp)
.stabn 68,0,16,.LM3-DispMsg
.LM3:
	movl 8(%ebp),%eax
	pushl %eax
	pushl $.LC0
	call printf
	addl $8,%esp
.stabn 68,0,17,.LM4-DispMsg
.LM4:
	movl 12(%ebp),%eax
	pushl %eax
	pushl $.LC1
	call printf
	addl $8,%esp
.stabn 68,0,19,.LM5-DispMsg
.LM5:
	movl -4(%ebp),%eax
	jmp .L1
	.align 4
.stabn 68,0,20,.LM6-DispMsg
.LM6:
.LBE2:
.stabn 68,0,20,.LM7-DispMsg
.LM7:
.L1:
	leave
	ret
.Lfe1:
	.size	 DispMsg,.Lfe1-DispMsg
.stabs "z:1",128,0,14,-4
.stabn 192,0,0,.LBB2-DispMsg
.stabn 224,0,0,.LBE2-DispMsg
.section	.rodata
.LC2:
	.string	"d is %d\n"
.text
	.align 4
.stabs "main:F19",36,0,23,main
.globl main
	.type	 main,@function
main:
.stabn 68,0,23,.LM8-main
.LM8:
	pushl %ebp
	movl %esp,%ebp
	subl $12,%esp
.stabn 68,0,24,.LM9-main
.LM9:
.LBB3:
.stabn 68,0,26,.LM10-main
.LM10:
	movl $1,-4(%ebp)
.stabn 68,0,27,.LM11-main
.LM11:
	movl $2,-8(%ebp)
.stabn 68,0,28,.LM12-main
.LM12:
	movl -8(%ebp),%eax
	pushl %eax
	movl -4(%ebp),%eax
	pushl %eax
	call DispMsg
	addl $8,%esp
	movl %eax,%eax
	movl %eax,-12(%ebp)
.stabn 68,0,29,.LM13-main
.LM13:
	movl -12(%ebp),%eax
	pushl %eax
	pushl $.LC2
	call printf
	addl $8,%esp
.stabn 68,0,30,.LM14-main
.LM14:
.LBE3:
.stabn 68,0,30,.LM15-main
.LM15:
.L2:
	leave
	ret
.Lfe2:
	.size	 main,.Lfe2-main
.stabs "a:1",128,0,24,-4
.stabs "b:1",128,0,24,-8
.stabs "c:1",128,0,24,-12
.stabn 192,0,0,.LBB3-main
.stabn 224,0,0,.LBE3-main
	.text
	.stabs "",100,0,0,.Letext
.Letext:
	.ident	"GCC: (GNU) 2.7.2.3"
