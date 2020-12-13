[bits 32]
[global setupPaging]
setupPaging:
    mov eax, [esp+4]
    mov cr3, eax

    pop eax
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax
    ret