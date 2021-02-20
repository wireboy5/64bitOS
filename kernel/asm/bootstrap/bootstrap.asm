

[EXTERN bootstrap_complete]

[GLOBAL bootstrap_kernel]


[EXTERN enable_a20]
[EXTERN load_gdt]

bootstrap_kernel:
    ; Enable a20
    call enable_a20

    ; Enable GDT
    jmp load_gdt

    jmp bootstrap_complete