.text
.global _start @ 全局标号

_start:
    ldr pc, =Reset_Handler      @ 复位中断
    ldr pc, =Undefined_Handler  @ 未定义指令中断
    ldr pc, =SVC_Handler        @ SVC(Supervisor)中断
    ldr pc, =PrefAbort_Handler  @ 预取终止中断
    ldr pc, =DataAbort_Handler  @ 数据终止中断
    ldr pc, =NotUsed_Handler    @ 未使用中断
    ldr pc, =IRQ_Handler        @ IRQ中断
    ldr pc, =FIQ_Handler        @ FIQ(快速)中断

    @ 复位中断服务函数
    Reset_Handler:
        @ 关闭全局中断
        cpsid i

        @ 采用读-改-写的方式
        mrc p15, 0, r0, c1, c0, 0   @ 读取 CP15 的 C1 寄存器到 R0 中
        bic r0, r0, #(0x1 << 12)    @ 清除 C1 的 I 位，关闭 I Cache
        bic r0, r0, #(0x1 << 2)     @ 清除 C1 的 C 位，关闭 D Cache
        bic r0, r0, #0x2            @ 清除 C1 的 A 位，关闭对齐检查
        bic r0, r0, #(0x1 << 11)    @ 清除 C1 的 Z 位，关闭分支预测
        bic r0, r0, #0x1            @ 清除 C1 的 M 位，关闭 MMU
        mcr p15, 0, r0, c1, c0, 0   @ 将 r0 的值写入到 CP15 的 C1 中

#if 0
        @ 汇编版本设置中断向量表偏移
        ldr r0, =0x87800000
        dsb
        isb
        mcr p15, 0, r0, c12, c0, 0
        dsb
        isb
#endif

#if 0
    .global _bss_start
    _bss_start :
        .word __bss_start

    .global _bss_end
    _bss_end :
        .word __bss_end

        @ 清除BSS段
        ldr r0, _bss_start  @ 读取BSS段起始地址
        ldr r1, _bss_end    @ 读取BSS段终止地址
        mov r2, #0          @ r2寄存器写0
    bss_loop:               @ 循环
        stmia r0!, {r2}     @ 将r2保存的值写入r0保存的地址且地址自增
        cmp r0, r1          @ 比较r0和r1的值
        ble bss_loop        @ b:跳转 le:小于等于，不满足继续清除
#endif

        @ 进入IRQ模式
        mrs r0, cpsr        @ 读取cpsr到r0
        bic r0, r0, #0x1f   @ 将r0低5位清零
        orr r0, r0, #0x12   @ r0或上0x12，表示使用IRQ模式
        msr cpsr, r0        @ 将r0的数据写入cpsr
        ldr sp, =0x80600000 @ IRQ 模式栈首地址为 0x80600000,大小为 2MB

        @ 进入SYS模式
        mrs r0, cpsr        @ 读取cpsr到r0
        bic r0, r0, #0x1f   @ 将r0低5位清零
        orr r0, r0, #0x1f   @ r0或上0x1f，表示使用SYS模式
        msr cpsr, r0        @ 将r0的数据写入cpsr
        ldr sp, =0x80400000 @ SYS 模式栈首地址为 0x80400000,大小为 2MB

        @ 进入SVC模式
        mrs r0, cpsr        @ 读取cpsr到r0
        bic r0, r0, #0x1f   @ 将r0低5位清零
        orr r0, r0, #0x13   @ r0或上0x13，表示使用SVC模式
        msr cpsr, r0        @ 将r0的数据写入cpsr
        ldr sp, =0x80200000 @ SYS 模式栈首地址为 0x80200000,大小为 2MB

        @ 打开全局中断
        cpsie i 

        @ 跳转到main函数
        b main

    @ 未定义指令中断服务函数
    Undefined_Handler:
        ldr r0, =Undefined_Handler
        bx r0

    @ SVC中断服务函数
    SVC_Handler:
        ldr r0, =SVC_Handler
        bx r0

    @ 预取终止中断服务函数
    PrefAbort_Handler:
        ldr r0, =PrefAbort_Handler
        bx r0

    @ 数据终止中断服务函数
    DataAbort_Handler:
        ldr r0, =DataAbort_Handler
        bx r0

    @ 未使用中断服务函数
    NotUsed_Handler:
        ldr r0, =NotUsed_Handler
        bx r0

    @ IRQ中断服务函数
    IRQ_Handler:

        push {lr}                       @ 保存 lr 地址 
        push {r0-r3, r12}               @ 保存 r0-r3， r12 寄存器 

        mrs r0, spsr                    @ 读取 spsr 寄存器 
        push {r0}                       @ 保存 spsr 寄存器 
        
        mrc p15, 4, r1, c15, c0, 0      @ 将 CP15 的 C0 内的值到 R1 寄存器中
                                        @ 参考文档 ARM Cortex-A(armV7)编程手册 V4.0.pdf P49
                                        @ Cortex-A7 Technical ReferenceManua.pdf P68 P138
                                   

        add r1, r1, #0x2000             @ GIC 基地址加 0X2000， 得到 CPU 接口端基地址 
        ldr r0, [r1, #0xC]              @ CPU 接口端基地址加 0X0C 就是 GICC_IAR 寄存器，
                                        @ GICC_IAR 保存着当前发生中断的中断号，我们要根据
                                        @ 这个中断号来绝对调用哪个中断服务函数
 
        push {r0, r1}                   @ 保存 r0,r1 

        cps #0x13                       @ 进入 SVC 模式，允许其他中断再次进去 

        push {lr}                       @ 保存 SVC 模式的 lr 寄存器 
        ldr r2, =IMX6ULL_IRQ_Handler    @ 加载 C 语言中断处理函数到 r2 寄存器中
        blx r2                          @ 运行 C 语言中断处理函数，带有一个参数 

        pop {lr}                        @ 执行完 C 语言中断服务函数， lr 出栈 
        cps #0x12                       @ 进入 IRQ 模式 
        pop {r0, r1}
        str r0, [r1, #0x10]             @ 中断执行完成，写 EOIR 

        pop {r0}
        msr spsr_cxsf, r0               @ 恢复 spsr 

        pop {r0-r3, r12}                @ r0-r3,r12 出栈 
        pop {lr}                        @ lr 出栈 
        subs pc, lr, #4                 @ 将 lr-4 赋给 pc 

    @ FIQ中断服务函数
    FIQ_Handler:
        ldr r0, =FIQ_Handler
        bx r0


