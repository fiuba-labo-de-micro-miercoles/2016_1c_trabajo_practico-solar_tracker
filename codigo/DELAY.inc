;***********************************************************
;		Para crear los delays, no te hagas el crack y
;		usá el bocho de alguien que ya lo pensó:
;		http://www.bretmulvey.com/avrdelay.html
;
;		TENER EN CUENTA QUE EL MICRO TRABAJA A 8MHZ
;
;***********************************************************

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