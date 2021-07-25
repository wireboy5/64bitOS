; Defines various macros to make code cleander

; Include definitions
%include "kernel/utility/asm/definitions.s"

; Removes offset from `a`
%define no_offset(a) a-offset

