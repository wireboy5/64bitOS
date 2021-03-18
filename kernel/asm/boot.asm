global _start
extern entry
extern kernOffset
section .text
%define offset 0xC0000000
bits 32
_start:
    ; Setup stack, Without the kernel offset
    mov esp, stack_end - offset

    
    

    ; Run checks
    call check_multiboot
    call check_cpuid
    call check_long_mode

    ; Initialize paging
    call init_paging

    ; Load GDT
    lgdt [gdt64.pointer-offset]

    ; Long jump
    jmp gdt64.code:entry-offset


; Setup Page tables
init_paging:
    mov eax, pdpt - offset
    or eax, 0b11
    mov [pml4 - offset], eax

    mov eax, pd - offset
    or eax, 0b11
    mov [pdpt - offset], eax

    mov ecx, 0
.map_pd
    mov eax, 0x200000
    mul ecx
    or eax, 0b10000011
    mov [pd - offset + ecx * 8], eax


    inc ecx
    cmp ecx, 512
    jne .map_pd

    mov eax, pml4 - offset
    mov cr3, eax

    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ret


; Here we have some checks

; First we check for multiboot
check_multiboot:
    cmp eax, 0x36d76289
    jne .no_multiboot
    ret
.no_multiboot:
    mov al, "0"
    mov bl, "M"
    jmp error



check_cpuid:
    ; Check if CPUID is supported by attempting to flip the ID bit (bit 21)
    ; in the FLAGS register. If we can flip it, CPUID is available.

    ; Copy FLAGS in to EAX via stack
    pushfd
    pop eax

    ; Copy to ECX as well for comparing later on
    mov ecx, eax

    ; Flip the ID bit
    xor eax, 1 << 21

    ; Copy EAX to FLAGS via the stack
    push eax
    popfd

    ; Copy FLAGS back to EAX (with the flipped bit if CPUID is supported)
    pushfd
    pop eax

    ; Restore FLAGS from the old version stored in ECX (i.e. flipping the
    ; ID bit back if it was ever flipped).
    push ecx
    popfd

    ; Compare EAX and ECX. If they are equal then that means the bit
    ; wasn't flipped, and CPUID isn't supported.
    cmp eax, ecx
    je .no_cpuid
    ret
.no_cpuid:
    mov al, "0"
    mov bl, "C"
    jmp error


check_long_mode:
    ; test if extended processor info in available
    mov eax, 0x80000000    ; implicit argument for cpuid
    cpuid                  ; get highest supported argument
    cmp eax, 0x80000001    ; it needs to be at least 0x80000001
    jb .no_long_mode       ; if it's less, the CPU is too old for long mode

    ; use extended info to test if long mode is available
    mov eax, 0x80000001    ; argument for extended processor info
    cpuid                  ; returns various feature bits in ecx and edx
    test edx, 1 << 29      ; test if the LM-bit is set in the D-register
    jz .no_long_mode       ; If it's not set, there is no long mode
    ret
.no_long_mode:
    mov al, "0"
    mov bl, "L"
    jmp error


error:
    ; Prints "ERR: {code}" where code is an error code
    ; finally halts
    mov dword [0xb8000], 0x4f524f45
    mov dword [0xb8004], 0x4f3a4f52
    mov dword [0xb8008], 0x4f204f20
    mov byte  [0xb800a], al
    mov byte  [0xb800c], bl
    mov word  [0xb800d], 0x4f

    ; Halt
    hlt




section .bss
pml4:
    resb 4096
pdpt:
    resb 4096
pd:
    resb 4096

align 4096
stack_begin:
    resb 4096
stack_end:
section .rodata
gdt64:
    dq 0 ; null entry
.code: equ $ - (gdt64 - offset)
    dq (1<<43) | (1<<44) | (1<<47) | (1<<53)
.pointer:
    dw $ - gdt64 - 1 - offset
    dq gdt64 - offset