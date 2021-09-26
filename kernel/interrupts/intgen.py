"""
    Generates interrupt handlers.
"""

noerrcode = (
"""
[GLOBAL isr{0}]
isr{0}:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push {0}
    jmp isr_common
""")

errcode = (
"""
[GLOBAL isr{0}]
isr{0}:
    pushAll ; Push all here, that way the handler does not consume it
    push {0}
    jmp isr_common
""")


errs = [
    0x8,
    0xA,
    0xB,
    0xC,
    0xE,
    0x11,
    0x1E,
]

out = """
%include "kernel/interrupts/isrh.s"
"""

for i in range(0,256):
    out += (errcode if i in errs else noerrcode).format(str(i))

with open("isr.asm", "w+") as f:
    f.write(out)