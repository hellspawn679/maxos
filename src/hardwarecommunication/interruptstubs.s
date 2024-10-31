.set IRQ_BASE, 0x20

.section .text

.extern _ZN5maxos21hardwarecommunication16InterruptManager15HandleInterruptEhj


.macro HandleExceptionErrCode num
.global _ZN5maxos21hardwarecommunication16InterruptManager19HandleException\num\()Ev
_ZN5maxos21hardwarecommunication16InterruptManager19HandleException\num\()Ev:
    movb $\num, (interruptnumber)
    jmp int_bottom
.endm

.macro HandleExceptionNoErrCode num
.global _ZN5maxos21hardwarecommunication16InterruptManager19HandleException\num\()Ev
_ZN5maxos21hardwarecommunication16InterruptManager19HandleException\num\()Ev:
    movb $\num, (interruptnumber)
    push $0                    # Push Dummy Error Code
    jmp int_bottom
.endm


.macro HandleInterruptRequest num
.global _ZN5maxos21hardwarecommunication16InterruptManager26HandleInterruptRequest\num\()Ev
_ZN5maxos21hardwarecommunication16InterruptManager26HandleInterruptRequest\num\()Ev:
    movb $\num + IRQ_BASE, (interruptnumber)
    push $0                    # Push Dummy Error Code
    jmp int_bottom
.endm

HandleExceptionNoErrCode 0x00
HandleExceptionNoErrCode 0x01
HandleExceptionNoErrCode 0x02
HandleExceptionNoErrCode 0x03
HandleExceptionNoErrCode 0x04
HandleExceptionNoErrCode 0x05
HandleExceptionNoErrCode 0x06
HandleExceptionNoErrCode 0x07
HandleExceptionErrCode   0x08
HandleExceptionNoErrCode 0x09
HandleExceptionErrCode   0x0A
HandleExceptionErrCode   0x0B
HandleExceptionErrCode   0x0C
HandleExceptionErrCode   0x0D
HandleExceptionErrCode   0x0E
HandleExceptionNoErrCode 0x0F
HandleExceptionNoErrCode 0x10
HandleExceptionErrCode   0x11
HandleExceptionNoErrCode 0x12
HandleExceptionNoErrCode 0x13
HandleExceptionNoErrCode 0x14
HandleExceptionErrCode   0x15
HandleExceptionNoErrCode 0x16
HandleExceptionNoErrCode 0x17
HandleExceptionNoErrCode 0x18
HandleExceptionNoErrCode 0x19
HandleExceptionNoErrCode 0x1A
HandleExceptionNoErrCode 0x1B
HandleExceptionNoErrCode 0x1C
HandleExceptionErrCode   0x1D
HandleExceptionErrCode   0x1E
HandleExceptionNoErrCode 0x1F

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01
HandleInterruptRequest 0x02
HandleInterruptRequest 0x03
HandleInterruptRequest 0x04
HandleInterruptRequest 0x05
HandleInterruptRequest 0x06
HandleInterruptRequest 0x07
HandleInterruptRequest 0x08
HandleInterruptRequest 0x09
HandleInterruptRequest 0x0A
HandleInterruptRequest 0x0B
HandleInterruptRequest 0x0C
HandleInterruptRequest 0x0D
HandleInterruptRequest 0x0E
HandleInterruptRequest 0x0F
HandleInterruptRequest 0x31

int_bottom:
    # register sichern
    pusha
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    # ring 0 segment register laden
    #mov $0x18, %eax
    #mov %eax, %ds
    #mov %eax, %es
    #mov %eax, %fs
    #mov %eax, %gs

    # C++ Handler aufrufen
    cld
    pushl %esp
    push (interruptnumber)
    call _ZN5maxos21hardwarecommunication16InterruptManager15HandleInterruptEhj
    mov %eax, %esp # den stack wechseln

    # register laden
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa
    add $4, %esp               # Remove error code

.global _ZN5maxos21hardwarecommunication16InterruptManager15InterruptIgnoreEv
_ZN5maxos21hardwarecommunication16InterruptManager15InterruptIgnoreEv:
    iret


.data
    interruptnumber: .long 0