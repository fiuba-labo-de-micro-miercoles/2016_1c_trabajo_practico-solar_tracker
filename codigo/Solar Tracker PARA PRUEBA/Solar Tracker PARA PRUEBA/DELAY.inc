;***********************************************************
;		Para crear los delays, no te hagas el crack y
;		usá el bocho de alguien que ya lo pensó:
;		http://www.bretmulvey.com/avrdelay.html
;
;		TENER EN CUENTA QUE EL MICRO TRABAJA A 8MHZ
;
;***********************************************************
.CSEG

DELAY_100us:
    ldi  AUX, 2
    ldi  AUX1, 9
L4: dec  AUX1
    brne L4
    dec  AUX
    brne L4
RET

DELAY_25ms:
    ldi  r18, 2
    ldi  r19, 4
    ldi  r20, 187
L5: dec  r20
    brne L5
    dec  r19
    brne L5
    dec  r18
    brne L5
    nop
RET

DELAY_50ms:

    ldi  AUX, 3
    ldi  AUX1, 8
    ldi  AUX2, 120
L1: dec  AUX2
    brne L1
    dec  AUX1
    brne L1
    dec  AUX
    brne L1
RET

DELAY_LO_QUE_SE_ME_ANTOJE:

;ABRI EL PROGRAMA Y LISTO.
;HACER PUSH Y POP DE LOS REGS QUE SE USEN

RET

DELAY_500ms:
    ldi  AUX, 21
    ldi  AUX1, 75
    ldi  AUX2, 191
L3: dec  AUX2
    brne L3
    dec  AUX1
    brne L3
    dec  AUX
    brne L3
    nop
RET