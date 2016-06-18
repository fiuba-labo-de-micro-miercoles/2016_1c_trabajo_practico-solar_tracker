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
    ldi  r18, 2
    ldi  r19, 9
L4: dec  r19
    brne L4
    dec  r18
    brne L4
RET

DELAY_50ms:
	PUSH R18
	PUSH R19
	PUSH R20

    ldi  r18, 3
    ldi  r19, 8
    ldi  r20, 120
L1: dec  r20
    brne L1
    dec  r19
    brne L1
    dec  r18
    brne L1

	POP R20
	POP R19
	POP R18
RET

DELAY_LO_QUE_SE_ME_ANTOJE:

;ABRI EL PROGRAMA Y LISTO.
;HACER PUSH Y POP DE LOS REGS QUE SE USEN

RET

DELAY_500ms:
    ldi  r18, 21
    ldi  r19, 75
    ldi  r20, 191
L3: dec  r20
    brne L3
    dec  r19
    brne L3
    dec  r18
    brne L3
    nop
RET