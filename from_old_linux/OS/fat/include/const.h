#ifndef _CONST_H_
#define _CONST_H_ 1

// 函数类型
#define PUBLIC
#define PRIVATE			static
#define	EXTERN			extern

// Boolean
#define	TRUE	1
#define	FALSE	0

// GDT 和 LDT 中描述符的个数
#define	GDT_SIZE		128
#define	IDT_SIZE		256

// 权限
#define	PRIVILEGE_KRNL	0
#define	PRIVILEGE_TASK	1
#define	PRIVILEGE_USER	3

// 8259A端口
#define INT_M_CTL		0x20	// 中断控制器的输入输出端口		<主>
#define INT_M_CTLMASK	0x21	// 此端口禁用中断的设置位		<主>
#define INT_S_CTL		0xA0	// 第二中断控制器的输入输出端口	<从> 
#define INT_S_CTLMASK	0xA1    // 此端口禁用中断的设置位		<从>

#endif // _CONST_H_
