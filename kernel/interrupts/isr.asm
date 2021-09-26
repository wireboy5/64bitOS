
%include "kernel/interrupts/isrh.s"

[GLOBAL isr0]
isr0:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 0
    jmp isr_common

[GLOBAL isr1]
isr1:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 1
    jmp isr_common

[GLOBAL isr2]
isr2:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 2
    jmp isr_common

[GLOBAL isr3]
isr3:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 3
    jmp isr_common

[GLOBAL isr4]
isr4:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 4
    jmp isr_common

[GLOBAL isr5]
isr5:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 5
    jmp isr_common

[GLOBAL isr6]
isr6:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 6
    jmp isr_common

[GLOBAL isr7]
isr7:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 7
    jmp isr_common

[GLOBAL isr8]
isr8:
    pushAll ; Push all here, that way the handler does not consume it
    push 8
    jmp isr_common

[GLOBAL isr9]
isr9:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 9
    jmp isr_common

[GLOBAL isr10]
isr10:
    pushAll ; Push all here, that way the handler does not consume it
    push 10
    jmp isr_common

[GLOBAL isr11]
isr11:
    pushAll ; Push all here, that way the handler does not consume it
    push 11
    jmp isr_common

[GLOBAL isr12]
isr12:
    pushAll ; Push all here, that way the handler does not consume it
    push 12
    jmp isr_common

[GLOBAL isr13]
isr13:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 13
    jmp isr_common

[GLOBAL isr14]
isr14:
    pushAll ; Push all here, that way the handler does not consume it
    push 14
    jmp isr_common

[GLOBAL isr15]
isr15:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 15
    jmp isr_common

[GLOBAL isr16]
isr16:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 16
    jmp isr_common

[GLOBAL isr17]
isr17:
    pushAll ; Push all here, that way the handler does not consume it
    push 17
    jmp isr_common

[GLOBAL isr18]
isr18:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 18
    jmp isr_common

[GLOBAL isr19]
isr19:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 19
    jmp isr_common

[GLOBAL isr20]
isr20:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 20
    jmp isr_common

[GLOBAL isr21]
isr21:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 21
    jmp isr_common

[GLOBAL isr22]
isr22:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 22
    jmp isr_common

[GLOBAL isr23]
isr23:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 23
    jmp isr_common

[GLOBAL isr24]
isr24:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 24
    jmp isr_common

[GLOBAL isr25]
isr25:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 25
    jmp isr_common

[GLOBAL isr26]
isr26:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 26
    jmp isr_common

[GLOBAL isr27]
isr27:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 27
    jmp isr_common

[GLOBAL isr28]
isr28:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 28
    jmp isr_common

[GLOBAL isr29]
isr29:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 29
    jmp isr_common

[GLOBAL isr30]
isr30:
    pushAll ; Push all here, that way the handler does not consume it
    push 30
    jmp isr_common

[GLOBAL isr31]
isr31:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 31
    jmp isr_common

[GLOBAL isr32]
isr32:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 32
    jmp isr_common

[GLOBAL isr33]
isr33:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 33
    jmp isr_common

[GLOBAL isr34]
isr34:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 34
    jmp isr_common

[GLOBAL isr35]
isr35:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 35
    jmp isr_common

[GLOBAL isr36]
isr36:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 36
    jmp isr_common

[GLOBAL isr37]
isr37:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 37
    jmp isr_common

[GLOBAL isr38]
isr38:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 38
    jmp isr_common

[GLOBAL isr39]
isr39:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 39
    jmp isr_common

[GLOBAL isr40]
isr40:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 40
    jmp isr_common

[GLOBAL isr41]
isr41:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 41
    jmp isr_common

[GLOBAL isr42]
isr42:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 42
    jmp isr_common

[GLOBAL isr43]
isr43:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 43
    jmp isr_common

[GLOBAL isr44]
isr44:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 44
    jmp isr_common

[GLOBAL isr45]
isr45:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 45
    jmp isr_common

[GLOBAL isr46]
isr46:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 46
    jmp isr_common

[GLOBAL isr47]
isr47:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 47
    jmp isr_common

[GLOBAL isr48]
isr48:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 48
    jmp isr_common

[GLOBAL isr49]
isr49:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 49
    jmp isr_common

[GLOBAL isr50]
isr50:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 50
    jmp isr_common

[GLOBAL isr51]
isr51:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 51
    jmp isr_common

[GLOBAL isr52]
isr52:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 52
    jmp isr_common

[GLOBAL isr53]
isr53:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 53
    jmp isr_common

[GLOBAL isr54]
isr54:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 54
    jmp isr_common

[GLOBAL isr55]
isr55:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 55
    jmp isr_common

[GLOBAL isr56]
isr56:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 56
    jmp isr_common

[GLOBAL isr57]
isr57:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 57
    jmp isr_common

[GLOBAL isr58]
isr58:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 58
    jmp isr_common

[GLOBAL isr59]
isr59:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 59
    jmp isr_common

[GLOBAL isr60]
isr60:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 60
    jmp isr_common

[GLOBAL isr61]
isr61:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 61
    jmp isr_common

[GLOBAL isr62]
isr62:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 62
    jmp isr_common

[GLOBAL isr63]
isr63:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 63
    jmp isr_common

[GLOBAL isr64]
isr64:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 64
    jmp isr_common

[GLOBAL isr65]
isr65:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 65
    jmp isr_common

[GLOBAL isr66]
isr66:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 66
    jmp isr_common

[GLOBAL isr67]
isr67:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 67
    jmp isr_common

[GLOBAL isr68]
isr68:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 68
    jmp isr_common

[GLOBAL isr69]
isr69:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 69
    jmp isr_common

[GLOBAL isr70]
isr70:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 70
    jmp isr_common

[GLOBAL isr71]
isr71:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 71
    jmp isr_common

[GLOBAL isr72]
isr72:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 72
    jmp isr_common

[GLOBAL isr73]
isr73:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 73
    jmp isr_common

[GLOBAL isr74]
isr74:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 74
    jmp isr_common

[GLOBAL isr75]
isr75:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 75
    jmp isr_common

[GLOBAL isr76]
isr76:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 76
    jmp isr_common

[GLOBAL isr77]
isr77:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 77
    jmp isr_common

[GLOBAL isr78]
isr78:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 78
    jmp isr_common

[GLOBAL isr79]
isr79:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 79
    jmp isr_common

[GLOBAL isr80]
isr80:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 80
    jmp isr_common

[GLOBAL isr81]
isr81:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 81
    jmp isr_common

[GLOBAL isr82]
isr82:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 82
    jmp isr_common

[GLOBAL isr83]
isr83:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 83
    jmp isr_common

[GLOBAL isr84]
isr84:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 84
    jmp isr_common

[GLOBAL isr85]
isr85:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 85
    jmp isr_common

[GLOBAL isr86]
isr86:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 86
    jmp isr_common

[GLOBAL isr87]
isr87:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 87
    jmp isr_common

[GLOBAL isr88]
isr88:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 88
    jmp isr_common

[GLOBAL isr89]
isr89:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 89
    jmp isr_common

[GLOBAL isr90]
isr90:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 90
    jmp isr_common

[GLOBAL isr91]
isr91:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 91
    jmp isr_common

[GLOBAL isr92]
isr92:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 92
    jmp isr_common

[GLOBAL isr93]
isr93:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 93
    jmp isr_common

[GLOBAL isr94]
isr94:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 94
    jmp isr_common

[GLOBAL isr95]
isr95:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 95
    jmp isr_common

[GLOBAL isr96]
isr96:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 96
    jmp isr_common

[GLOBAL isr97]
isr97:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 97
    jmp isr_common

[GLOBAL isr98]
isr98:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 98
    jmp isr_common

[GLOBAL isr99]
isr99:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 99
    jmp isr_common

[GLOBAL isr100]
isr100:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 100
    jmp isr_common

[GLOBAL isr101]
isr101:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 101
    jmp isr_common

[GLOBAL isr102]
isr102:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 102
    jmp isr_common

[GLOBAL isr103]
isr103:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 103
    jmp isr_common

[GLOBAL isr104]
isr104:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 104
    jmp isr_common

[GLOBAL isr105]
isr105:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 105
    jmp isr_common

[GLOBAL isr106]
isr106:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 106
    jmp isr_common

[GLOBAL isr107]
isr107:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 107
    jmp isr_common

[GLOBAL isr108]
isr108:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 108
    jmp isr_common

[GLOBAL isr109]
isr109:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 109
    jmp isr_common

[GLOBAL isr110]
isr110:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 110
    jmp isr_common

[GLOBAL isr111]
isr111:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 111
    jmp isr_common

[GLOBAL isr112]
isr112:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 112
    jmp isr_common

[GLOBAL isr113]
isr113:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 113
    jmp isr_common

[GLOBAL isr114]
isr114:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 114
    jmp isr_common

[GLOBAL isr115]
isr115:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 115
    jmp isr_common

[GLOBAL isr116]
isr116:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 116
    jmp isr_common

[GLOBAL isr117]
isr117:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 117
    jmp isr_common

[GLOBAL isr118]
isr118:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 118
    jmp isr_common

[GLOBAL isr119]
isr119:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 119
    jmp isr_common

[GLOBAL isr120]
isr120:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 120
    jmp isr_common

[GLOBAL isr121]
isr121:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 121
    jmp isr_common

[GLOBAL isr122]
isr122:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 122
    jmp isr_common

[GLOBAL isr123]
isr123:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 123
    jmp isr_common

[GLOBAL isr124]
isr124:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 124
    jmp isr_common

[GLOBAL isr125]
isr125:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 125
    jmp isr_common

[GLOBAL isr126]
isr126:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 126
    jmp isr_common

[GLOBAL isr127]
isr127:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 127
    jmp isr_common

[GLOBAL isr128]
isr128:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 128
    jmp isr_common

[GLOBAL isr129]
isr129:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 129
    jmp isr_common

[GLOBAL isr130]
isr130:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 130
    jmp isr_common

[GLOBAL isr131]
isr131:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 131
    jmp isr_common

[GLOBAL isr132]
isr132:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 132
    jmp isr_common

[GLOBAL isr133]
isr133:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 133
    jmp isr_common

[GLOBAL isr134]
isr134:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 134
    jmp isr_common

[GLOBAL isr135]
isr135:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 135
    jmp isr_common

[GLOBAL isr136]
isr136:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 136
    jmp isr_common

[GLOBAL isr137]
isr137:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 137
    jmp isr_common

[GLOBAL isr138]
isr138:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 138
    jmp isr_common

[GLOBAL isr139]
isr139:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 139
    jmp isr_common

[GLOBAL isr140]
isr140:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 140
    jmp isr_common

[GLOBAL isr141]
isr141:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 141
    jmp isr_common

[GLOBAL isr142]
isr142:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 142
    jmp isr_common

[GLOBAL isr143]
isr143:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 143
    jmp isr_common

[GLOBAL isr144]
isr144:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 144
    jmp isr_common

[GLOBAL isr145]
isr145:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 145
    jmp isr_common

[GLOBAL isr146]
isr146:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 146
    jmp isr_common

[GLOBAL isr147]
isr147:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 147
    jmp isr_common

[GLOBAL isr148]
isr148:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 148
    jmp isr_common

[GLOBAL isr149]
isr149:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 149
    jmp isr_common

[GLOBAL isr150]
isr150:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 150
    jmp isr_common

[GLOBAL isr151]
isr151:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 151
    jmp isr_common

[GLOBAL isr152]
isr152:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 152
    jmp isr_common

[GLOBAL isr153]
isr153:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 153
    jmp isr_common

[GLOBAL isr154]
isr154:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 154
    jmp isr_common

[GLOBAL isr155]
isr155:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 155
    jmp isr_common

[GLOBAL isr156]
isr156:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 156
    jmp isr_common

[GLOBAL isr157]
isr157:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 157
    jmp isr_common

[GLOBAL isr158]
isr158:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 158
    jmp isr_common

[GLOBAL isr159]
isr159:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 159
    jmp isr_common

[GLOBAL isr160]
isr160:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 160
    jmp isr_common

[GLOBAL isr161]
isr161:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 161
    jmp isr_common

[GLOBAL isr162]
isr162:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 162
    jmp isr_common

[GLOBAL isr163]
isr163:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 163
    jmp isr_common

[GLOBAL isr164]
isr164:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 164
    jmp isr_common

[GLOBAL isr165]
isr165:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 165
    jmp isr_common

[GLOBAL isr166]
isr166:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 166
    jmp isr_common

[GLOBAL isr167]
isr167:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 167
    jmp isr_common

[GLOBAL isr168]
isr168:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 168
    jmp isr_common

[GLOBAL isr169]
isr169:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 169
    jmp isr_common

[GLOBAL isr170]
isr170:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 170
    jmp isr_common

[GLOBAL isr171]
isr171:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 171
    jmp isr_common

[GLOBAL isr172]
isr172:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 172
    jmp isr_common

[GLOBAL isr173]
isr173:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 173
    jmp isr_common

[GLOBAL isr174]
isr174:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 174
    jmp isr_common

[GLOBAL isr175]
isr175:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 175
    jmp isr_common

[GLOBAL isr176]
isr176:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 176
    jmp isr_common

[GLOBAL isr177]
isr177:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 177
    jmp isr_common

[GLOBAL isr178]
isr178:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 178
    jmp isr_common

[GLOBAL isr179]
isr179:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 179
    jmp isr_common

[GLOBAL isr180]
isr180:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 180
    jmp isr_common

[GLOBAL isr181]
isr181:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 181
    jmp isr_common

[GLOBAL isr182]
isr182:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 182
    jmp isr_common

[GLOBAL isr183]
isr183:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 183
    jmp isr_common

[GLOBAL isr184]
isr184:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 184
    jmp isr_common

[GLOBAL isr185]
isr185:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 185
    jmp isr_common

[GLOBAL isr186]
isr186:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 186
    jmp isr_common

[GLOBAL isr187]
isr187:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 187
    jmp isr_common

[GLOBAL isr188]
isr188:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 188
    jmp isr_common

[GLOBAL isr189]
isr189:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 189
    jmp isr_common

[GLOBAL isr190]
isr190:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 190
    jmp isr_common

[GLOBAL isr191]
isr191:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 191
    jmp isr_common

[GLOBAL isr192]
isr192:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 192
    jmp isr_common

[GLOBAL isr193]
isr193:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 193
    jmp isr_common

[GLOBAL isr194]
isr194:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 194
    jmp isr_common

[GLOBAL isr195]
isr195:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 195
    jmp isr_common

[GLOBAL isr196]
isr196:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 196
    jmp isr_common

[GLOBAL isr197]
isr197:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 197
    jmp isr_common

[GLOBAL isr198]
isr198:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 198
    jmp isr_common

[GLOBAL isr199]
isr199:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 199
    jmp isr_common

[GLOBAL isr200]
isr200:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 200
    jmp isr_common

[GLOBAL isr201]
isr201:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 201
    jmp isr_common

[GLOBAL isr202]
isr202:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 202
    jmp isr_common

[GLOBAL isr203]
isr203:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 203
    jmp isr_common

[GLOBAL isr204]
isr204:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 204
    jmp isr_common

[GLOBAL isr205]
isr205:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 205
    jmp isr_common

[GLOBAL isr206]
isr206:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 206
    jmp isr_common

[GLOBAL isr207]
isr207:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 207
    jmp isr_common

[GLOBAL isr208]
isr208:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 208
    jmp isr_common

[GLOBAL isr209]
isr209:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 209
    jmp isr_common

[GLOBAL isr210]
isr210:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 210
    jmp isr_common

[GLOBAL isr211]
isr211:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 211
    jmp isr_common

[GLOBAL isr212]
isr212:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 212
    jmp isr_common

[GLOBAL isr213]
isr213:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 213
    jmp isr_common

[GLOBAL isr214]
isr214:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 214
    jmp isr_common

[GLOBAL isr215]
isr215:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 215
    jmp isr_common

[GLOBAL isr216]
isr216:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 216
    jmp isr_common

[GLOBAL isr217]
isr217:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 217
    jmp isr_common

[GLOBAL isr218]
isr218:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 218
    jmp isr_common

[GLOBAL isr219]
isr219:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 219
    jmp isr_common

[GLOBAL isr220]
isr220:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 220
    jmp isr_common

[GLOBAL isr221]
isr221:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 221
    jmp isr_common

[GLOBAL isr222]
isr222:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 222
    jmp isr_common

[GLOBAL isr223]
isr223:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 223
    jmp isr_common

[GLOBAL isr224]
isr224:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 224
    jmp isr_common

[GLOBAL isr225]
isr225:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 225
    jmp isr_common

[GLOBAL isr226]
isr226:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 226
    jmp isr_common

[GLOBAL isr227]
isr227:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 227
    jmp isr_common

[GLOBAL isr228]
isr228:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 228
    jmp isr_common

[GLOBAL isr229]
isr229:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 229
    jmp isr_common

[GLOBAL isr230]
isr230:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 230
    jmp isr_common

[GLOBAL isr231]
isr231:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 231
    jmp isr_common

[GLOBAL isr232]
isr232:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 232
    jmp isr_common

[GLOBAL isr233]
isr233:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 233
    jmp isr_common

[GLOBAL isr234]
isr234:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 234
    jmp isr_common

[GLOBAL isr235]
isr235:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 235
    jmp isr_common

[GLOBAL isr236]
isr236:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 236
    jmp isr_common

[GLOBAL isr237]
isr237:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 237
    jmp isr_common

[GLOBAL isr238]
isr238:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 238
    jmp isr_common

[GLOBAL isr239]
isr239:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 239
    jmp isr_common

[GLOBAL isr240]
isr240:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 240
    jmp isr_common

[GLOBAL isr241]
isr241:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 241
    jmp isr_common

[GLOBAL isr242]
isr242:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 242
    jmp isr_common

[GLOBAL isr243]
isr243:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 243
    jmp isr_common

[GLOBAL isr244]
isr244:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 244
    jmp isr_common

[GLOBAL isr245]
isr245:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 245
    jmp isr_common

[GLOBAL isr246]
isr246:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 246
    jmp isr_common

[GLOBAL isr247]
isr247:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 247
    jmp isr_common

[GLOBAL isr248]
isr248:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 248
    jmp isr_common

[GLOBAL isr249]
isr249:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 249
    jmp isr_common

[GLOBAL isr250]
isr250:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 250
    jmp isr_common

[GLOBAL isr251]
isr251:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 251
    jmp isr_common

[GLOBAL isr252]
isr252:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 252
    jmp isr_common

[GLOBAL isr253]
isr253:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 253
    jmp isr_common

[GLOBAL isr254]
isr254:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 254
    jmp isr_common

[GLOBAL isr255]
isr255:
    pushAll ; Push all here, that way the handler does not consume it
    push 0
    push 255
    jmp isr_common
