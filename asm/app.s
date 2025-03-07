fun draw_half_circle:
    ; x9 - x, x10 - y, x14 - r, x11 - dir, x2 - color
    muladd x15, x14, x11, x10
    putpxl x9, x15, x2
    muladd x15, x14, x11, x9
    putpxl x15, x10, x2
    mul x15, x14, x11
    sub x15, x9, x15
    putpxl x15, x10, x2
    cmplt x15, x14, 0

    mov x12, 0 ; x12 is x
    mov x13, x14 ; x13 is y

    cj x15, skip_ret
    ret
skip_ret:
    shl x15, x14, 1
    sub x15, 3, x15 
    ; x15 is d

loop:
    jnpos x15, d_not_positive
    sub x13, x13, 1
    sub x16, x12, x13
    shl x16, x16, 2
    add x16, x16, 10
    add x15, x15, x16
    jmp draw
d_not_positive:
    shl x16, x12, 2
    add x16, x16, 6
    add x15, x15, x16
draw:
    add x12, x12, 1
    mul x17, x11, x12
    add x19, x17, x9
    mul x18, x11, x13
    add x20, x18, x10
    putpxl x19, x20, x2
    sub x19, x9, x17
    putpxl x19, x20, x2
    add x19, x18, x9
    add x20, x17, x10
    putpxl x19, x20, x2
    sub x19, x9, x18
    putpxl x19, x20, x2
    cmpgte x19, x13, x12
    cj x19, loop
    ret

fun main:
    rndrng x0, 449
    rndrng x1, 193
    rnd x2
    slctev x3, x2, -1, 1
    rnd x4
    slctev x5, x4, -1, 1
    ; x0 is cur_x, x1 is cur_y, x2 is color, x3 is dir_x, x4 is bc_delta, x5 is dir_y
    ; x6 is x, x7 is y, x8 is background_color
    mov x11, 1

label22:
label27:
    putpxl x6, x7, x8
    incjneq x6, 512, label27
    incjneq x7, 256, label22

    add x9, x0, 30
    add x10, x1, 30
    mov x14, 30

    call draw_half_circle
    mov x11, -1
    call draw_half_circle
    
    mov x14, 6

    add x9, x0, 20
    add x10, x1, 20
    call draw_half_circle
    mov x11, 1
    call draw_half_circle

    add x9, x0, 40
    call draw_half_circle
    mov x11, -1
    call draw_half_circle

    mov x14, 15

    add x10, x1, 33
    call draw_half_circle
    mov x11, 1
    call draw_half_circle

    add x9, x0, 63
    cmpgt x12, x9, 511
    cmplt x13, x0, 3
    or x12, x12, x13
    sub x13, 0, x3
    select x3, x3, x13, x3
    cmplt x12, x1, 3
    add x10, x1, 63
    cmpgt x13, x10, 255
    or x12, x12, x13
    sub x13, 0, x5
    select x5, x5, x13, x5

    muladd x0, x3, 3, x0
    muladd x1, x5, 3, x1

    cmpneq x12, x8, 0
    cmpneq x13, x8, -1
    and x12, x12, x13
    cj x12, skip_negate_delta
    sub x4, 0, x4
skip_negate_delta:
    rnd x2
    add x8, x8, x4
    flush
    jmp label22
