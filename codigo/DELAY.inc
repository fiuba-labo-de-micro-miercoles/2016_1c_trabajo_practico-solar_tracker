;***********************************************************
;		Para crear los delays, no te hagas el crack y
;		usá el bocho de alguien que ya lo pensó:
;		http://www.bretmulvey.com/avrdelay.html
;
;		TENER EN CUENTA QUE EL MICRO TRABAJA A 8MHZ
;
;***********************************************************

DELAY_50ms:

    ldi  r18, 3
    ldi  r19, 8
    ldi  r20, 120
L1: dec  r20
    brne L1
    dec  r19
    brne L1
    dec  r18
    brne L1

RET

DELAY_LO_QUE_SE_ME_ANTOJE:

;ABRI EL PROGRAMA Y LISTO.

RET