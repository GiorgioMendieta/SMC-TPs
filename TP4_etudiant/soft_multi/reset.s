    #################################################################################
    #	File : reset.s
    #	Author : Alain Greiner
    #	Date : 28/11/2013
    #################################################################################
    #	- It initializes the Interrupt vector
    #	- It initializes the Status Register (SR)
    #	- It defines the stack size  and initializes the stack pointer ($29)
    #	- It initializes the EPC register, and jump to the user code
    #################################################################################

    .section    .reset, "ax", @progbits

    .extern     seg_stack_base
    .extern     seg_data_base
    .extern     seg_icu_base
    .extern     _interrupt_vector
    .extern     _isr_timer
    .extern     _isr_tty_get_task0
    .extern     _isr_dma
    .extern     _isr_ioc

    .globl      reset                           # makes reset an external symbol
    .ent        reset
    .align      2

reset:
    .set        noreorder

    # Get the processor id
    mfc0    $27,    $15,                1       # Get the proc_id
    andi    $27,    0x3                         # Mask to 2 bits (max 4 procs)

    # Jump to initialize interrupt vector, ICU, main,
    li      $26,    0x0
    beq     $27,    $26,                proc0
    li      $26,    0x1
    beq     $27,    $26,                proc1
    li      $26,    0x2
    beq     $27,    $26,                proc2
    li      $26,    0x3
    beq     $27,    $26,                proc3


proc0:
    # initializes stack pointer
    la      $27,    seg_stack_base
    li      $26,    0x10000                     # $26 <= 64K
    addu    $29,    $26,                $27     # $29 <= seg_stack_base + nprocs*64K

    # initializes ICU MASK[0]
    la      $26,    seg_icu_base
    li      $27,    0x1111                      # IRQ_TTY[12] IRQ_TIM[8] IRQ_DMA[4] IRQ_IOC[0]
    sw      $27,    8($26)                      # ICU_MASK_SET

    # initializes interrupt vector
    la      $26,    _interrupt_vector           # interrupt vector address
    la      $27,    _isr_ioc
    sw      $27,    0($26)                      # interrupt_vector[0] <= _isr_ioc
    la      $27,    _isr_tty_get_task0
    sw      $27,    48($26)                     # interrupt_vector[4] <= _isr_tty_get_task0
    la      $27,    _isr_timer
    sw      $27,    32($26)                     # interrupt_vector[8] <= _isr_timer
    la      $27,    _isr_dma
    sw      $27,    16($26)                     # interrupt_vector[12] <= _isr_dma

    # initializes SR register
    li      $26,    0x0000FF13
    mtc0    $26,    $12                         # SR <= user mode / IRQ enable (after eret)

    # jumps to main proc 0
    la      $26,    seg_data_base
    lw      $26,    0($26)                      # $26 <= main[0]
    mtc0    $26,    $14                         # write it in EPC register

    eret

proc1:
    # initializes stack pointer
    la      $27,    seg_stack_base
    li      $26,    0x20000                     # $26 <= 64K
    addu    $29,    $26,                $27     # $29 <= seg_stack_base + 2*64K

    # initializes ICU MASK[1]
    la      $26,    seg_icu_base
    li      $27,    0x2220                      # IRQ_TTY[13] IRQ_TIM[9] IRQ_DMA[5]
    sw      $27,    40($26)                     # ICU_MASK_SET

    # initializes interrupt vector
    la      $26,    _interrupt_vector           # interrupt vector address
    la      $27,    _isr_tty_get_task0
    sw      $27,    52($26)                     # interrupt_vector[5] <= _isr_tty_get_task0
    la      $27,    _isr_timer
    sw      $27,    36($26)                     # interrupt_vector[9] <= _isr_timer
    la      $27,    _isr_dma
    sw      $27,    20($26)                     # interrupt_vector[13] <= _isr_dma

    # initializes SR register
    li      $26,    0x0000FF13
    mtc0    $26,    $12                         # SR <= user mode / IRQ enable (after eret)

    # jumps to main proc 1
    la      $26,    seg_data_base
    lw      $26,    4($26)                      # $26 <= main[1]
    mtc0    $26,    $14                         # write it in EPC register
    eret


proc2:
    # initializes stack pointer
    la      $27,    seg_stack_base
    li      $26,    0x30000                     # $26 <= 64K
    addu    $29,    $26,                $27     # $29 <= seg_stack_base + nprocs*64K
    # initializes ICU MASK[2]
    la      $26,    seg_icu_base
    li      $27,    0x4440                      # IRQ_TTY[14] IRQ_TIM[10] IRQ_DMA[6]
    sw      $27,    72($26)                     # ICU_MASK_SET
    # initializes interrupt vector
    la      $26,    _interrupt_vector           # interrupt vector address
    la      $27,    _isr_tty_get_task0
    sw      $27,    56($26)                     # interrupt_vector[6] <= _isr_tty_get_task0
    la      $27,    _isr_timer
    sw      $27,    40($26)                     # interrupt_vector[10] <= _isr_timer
    la      $27,    _isr_dma
    sw      $27,    24($26)                     # interrupt_vector[14] <= _isr_dma

    # initializes SR register
    li      $26,    0x0000FF13
    mtc0    $26,    $12                         # SR <= user mode / IRQ enable (after eret)

    # jumps to main proc 2
    la      $26,    seg_data_base
    lw      $26,    8($26)                      # $26 <= main[2]
    mtc0    $26,    $14                         # write it in EPC register

    eret

proc3:
    # initializes stack pointer
    la      $27,    seg_stack_base
    li      $26,    0x40000                     # $26 <= 64K
    addu    $29,    $26,                $27     # $29 <= seg_stack_base + nprocs*64K
    # initializes ICU MASK[3]
    la      $26,    seg_icu_base
    li      $27,    0x8880                      # IRQ_TTY[15] IRQ_TIM[11] IRQ_DMA[7]
    sw      $27,    104($26)                    # ICU_MASK_SET
    # initializes interrupt vector
    la      $26,    _interrupt_vector           # interrupt vector address
    la      $27,    _isr_tty_get_task0
    sw      $27,    60($26)                     # interrupt_vector[7] <= _isr_tty_get_task0
    la      $27,    _isr_timer
    sw      $27,    44($26)                     # interrupt_vector[11] <= _isr_timer
    la      $27,    _isr_dma
    sw      $27,    28($26)                     # interrupt_vector[15] <= _isr_dma

    # initializes SR register
    li      $26,    0x0000FF13
    mtc0    $26,    $12                         # SR <= user mode / IRQ enable (after eret)

    # jumps to main proc 3
    la      $26,    seg_data_base
    lw      $26,    12($26)                     # $26 <= main[3]
    mtc0    $26,    $14                         # write it in EPC register

    eret

    .end        reset
    .size       reset, .-reset

    .set        reorder
