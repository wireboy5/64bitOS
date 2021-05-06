[global _start]
[extern entry]

%define offset 0xC0000000
;%define offset 0

section .text

bits 32
_start:
    ; Setup stack, Without the kernel offset
    mov esp, stack_end - offset

    
    

    ; Run checks
    call check_multiboot
    call check_cpuid
    call check_long_mode

    ; Save the multiboot info structure
    mov [mboot_info], ebx

    mov ax, "t"
    out 0x3f8, ax
    
    ; Initialize paging
    jmp init_paging
    


; Setup Page tables
init_paging:
    ; Map first PML4 Entry
    mov eax, pdpt - offset
    or eax, 0b11
    mov [pml4 - offset], eax

    

    ; Map the higher half entry in PDPT
    mov eax, pd - offset
    or eax, 0b11
    mov [pdpt - offset + 3 * 8], eax

    ; Map the lower half entry in PDPT
    mov eax, pd - offset
    or eax, 0b11
    mov [pdpt - offset + 0 * 8], eax

    

    ; Map all page tables
    mov eax, pts - offset
    mov ebx, 0b11
    mov ecx, 512 * 512 ; This is how many entries we have in all of the tables
    mov edx, 0
    call map_pts
    
    ; Here we map the page directory
    mov eax, pts - offset ; The address of the page tables
    mov ecx, 512 ; 512 entries
    mov ebx, 0x11 ; The flags
    mov edx, pd - offset ; The address of the page directory entry
    call map_pd ; Map the page directory


    
    ; Load PML4
    mov eax, pml4 - offset
    mov cr3, eax

    ; Enable PAE
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax
    
    ; Enable long mode
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr
    
    
    
    ; Enable paging
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax
    
    ; Do not unmap lower half
    ; This current paging system is simply for
    ; convinience whilst setting up a new system
    
    
    ; Load GDT
    lgdt [gdt64_pointer]

    ; Long jump
    jmp 0x08:entry

map_pts:
    ; EAX : address of the page tables
    ; EBX : flags of entries
    ; ECX : number of tables
    ; EDX : offset address
    

    ; Clear all non flags
    and ebx, 0xFFF

    

    ; Apply the flags on EDX
    or edx, ebx

.map1
    ; Move the address into the table
    mov [eax], edx

    ; Add 4096 to the offset address
    add edx, 0x1000

    ; Move the the next table
    add eax, 8

    dec ecx ; Decrement the counter
    cmp ecx, 0 ; Compare the counter to zero
    jg .map1 ; if ecx > 0, loop


    ret

map_pd:
    ; EAX : address of the page tables
    ; EBX : flags of entries
    ; ECX : number of tables
    ; EDX : address of the page directory entry

.map_pde:

    or eax, ebx ; Apply flags to the entry

    mov [edx], eax ; Move into table

    add eax, 0x1000 ; Next table
    add edx, 8 ; Next entry

    dec ecx
    cmp ecx, 0
    jg .map_pde

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
    push edx
    cpuid                  ; returns various feature bits in ecx and edx
    test edx, 1 << 29      ; test if the LM-bit is set in the D-register
    pop edx
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
align 4096
; We want a single PML4
pml4:
    resb 4096
; A single pdpt
pdpt:
    resb 4096
; And a single page directory
pd:
    resb 4096

; we only use one of each of those because we are only mapping a 
; relatively small quantity of memory, enough to get us functioning in long mode

; However, we want 512 page tables, for entry in the page directory
; We treat these almost like an array
pts:
    resb 4096 * 512

; Now for the stack
stack_begin:
    resb 4096
stack_end:

; This is the address of mboot info.
; This will be set at the beginning of the _start label
[global mboot_info]
mboot_info:
    resq 1

section .rodata

align 4096

gdt64_start:
gdt64_null:
    dd 0x00000000
    dd 0x00000000

gdt64_code:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0b10011010
    db 0b10101111
    db 0x00

gdt64_data:
    dw 0x0000
    dw 0xFFFF
    db 0x00
    db 0b10010010
    db 0b10101111
    db 0x00

gdt64_end:

gdt64_pointer:
    dw (gdt64_end) - (gdt64_start) - 1
    dd gdt64_start