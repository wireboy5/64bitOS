; The kernel link offset
%define offset 0xFFFFFFFF80000000

; Removes offset from `a`
%define no_offset(a) a-offset