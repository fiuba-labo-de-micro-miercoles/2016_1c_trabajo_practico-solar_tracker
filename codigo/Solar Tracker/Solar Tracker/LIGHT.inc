/*
*	LIGHT.inc [COMO SI FUERA UN OBJETO]
*
*  Created: 01/06/2016 13:10:11 p.m.
*   Author: MAU
*/ 
.CSEG
;FUTURA MEJORA: HACER QUE LA LUZ SE MUEVA POR PWM EN FUNCION DE LA CARGA DE LA BATERIA
LIGHT_TURN_ON:

	INPUT AUX,BT_FLAG
	CPI AUX,0xFF						;[FLAG=0xFF]: ESTA CONECTADO A BT. 
	BREQ ORDEN_BT_NO_HACER_NADA			;[FLAG=0x00]: NO ESTA CONECTADO A BT.

	SER AUX
	SPWM LIGHT,AUX

/*	INPUT AUX,PORTD
	ORI AUX,(1<<LIGHT_PIN)
	OUTPUT PORTD,AUX*/

ORDEN_BT_NO_HACER_NADA:
RET

LIGHT_TURN_OFF:

	INPUT AUX,BT_FLAG
	CPI AUX,0xFF						;[FLAG=0xFF]: ESTA CONECTADO A BT. 
	BREQ ORDEN_BT_NO_HACER_NADA			;[FLAG=0x00]: NO ESTA CONECTADO A BT.

	CLR AUX
	SPWM LIGHT,AUX

/*	INPUT AUX,PORTD
	ANDI AUX,(~(1<<LIGHT_PIN))
	OUTPUT PORTD,AUX*/

RET