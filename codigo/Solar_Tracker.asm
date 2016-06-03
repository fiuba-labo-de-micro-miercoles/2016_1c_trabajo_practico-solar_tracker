;*************************************************************
;*				Programa Principal Seudo Codigo
;*
;*					Created: 29/05/2016
;*
;*  Autors: Joaquín Ulloa, Agustin Picard, Mauro Giordano
;*	
;*************************************************************

.include "m88def.inc"   	;Incluye los nombres de los registros del micro
.include "avr_macros.inc"	;Incluye los macros
.listmac					;Permite que se expandan las macros en el listado

.include "Solar_Tracker.inc"
.include "ADC.inc"
.include "PWM.inc"
.include "SERIAL_PORT.inc"
.include "DELAY.inc"
.include "LIGHT.inc"
.include "BATTERY.inc"
.include "SOLAR_PANEL.inc"

;-------------------------------------------------------------------------------- 
.CSEG
.ORG	0x0000
RJMP SETUP	

.ORG	URXCaddr		; USART, Rx Complete
RJMP	ISR_RX_USART_COMPLETA
	
.ORG	UDREaddr		; USART Data Register Empty
RJMP	ISR_REG_USART_VACIO
	
;-------------------------------SETUP--------------------------------------------
SETUP:
	LDI PWM_DATA,LOW(RAMEND)
	OUT SPL,PWM_DATA
	LDI	PWM_DATA,HIGH(RAMEND)
	OUT SPH,PWM_DATA

	RCALL ADC_INIT			;TIENE QUE ESTAR EN "ADC.inc"
	RCALL PWM_INIT			;TIENE QUE ESTAR EN "PWM.inc"
	RCALL SERIAL_PORT_INIT	;TIENE QUE ESTAR EN "SERIAL_PORT.inc"
	RCALL LIGHT_TURN_OFF	;TIENE QUE ESTAR EN "LIGHT.inc"

	CLT	;[T=1]: ESTA CONECTADO A BT. [T=0]: NO ESTA CONECTADO A BT.
	SEI
;--------------------------------------------------------------------------------

;-------------------------------PROGRAMA_PRINCIPAL-------------------------------
MAIN:	
	;MEDIR BATERIA 
		RCALL	READ_V_BATTERY					;MIDO LA BATERIA
		RCALL	CHECK_IF_BATTERY_MINIMUM		;[CARRY=1]: BATTERY LOW. [CARRY=0]: BATTERY OK
		BRCC	SLEEP_MODE
		RCALL	INDICATE_BATTERY_OK

;Mauro: "Creo que para ver si es de dia o noche leemos los 4 LDRs y nos quedamos con el minimo"
;	¿DIA O NOCHE?
		RCALL	READ_V_SOLAR_PANEL				;PARA VER SI ES DE DIA O NOCHE, MIDO LA TENSION DEL PANEL SOLAR.
		RCALL	CHECK_IF_SOLAR_PANEL_MINIMUM	;[CARRY=1]: ES DE DIA. [CARRY=0]: ES DE NOCHE.
		BRCC	AT_NIGHT						;Me aseguro que esta funcion no modifique SREG 
;SI ESTOY ACA YA TENGO BATERIA SUFICIENTE, ES DE DIA.
		RCALL	LIGHT_TURN_OFF
		RCALL	ORIENTATE_SOLAR_PANEL			;HAY QUE RESOLVER ESTO TODAVIA.
;	track:
;			leerLDRs
;			comparar
;			si no hay diferencia salgo del loop
;			moverMotores
;			volver a track
;		detenerMotores

;Joaco: "hay que pensar si harcodiamos cuanto tiempo se mueven los motores dependiendo la diferencia entre LDR o si hacemos otra cosa"
;;Mauro: "como leemos de 0-5v y el adc es de 10bits [1023 valores = 4,8mV por muestra]; shifteamos 2 lugares el adc y perdemos 15mV y listo.
;;		En síntesis: comparamos hasta que sea igual [control por lazo cerrado] y listo." 
;Mauro: "MEJOR AUN: AJUSTAMOS A LA IZQUIERDA LA INFO [CON ADLAR=1] Y AGARRO SOLO ADCH, YA TIRANDO LOS 15mV.
		BRTS MAIN

SLEEP_MODE:
;HAY QUE HACER COSAS ANTES DE IR A SLEEP, COMO APAGAR EL ADC Y NO SE QUE MAS
;SALIR DE SLEEP: EN UN TIEMPO t [TIEMPO ENTRE MEDICION Y MEDICION].
		INPUT AUX,SMCR
		ANDI AUX,(~(1<<SM2)|(1<<SM1)|(1<<SM0)|(1<<SE))
		ORI AUX,((0<<SM2)|(0<<SM1)|(0<<SM0)|(1<<SE))	;SETEO EL MODO IDLE.
		OUTPUT SMCR,AUX
		SLEEP
		NOP
;CUANDO SALGO DE SLEEP, PONGO SE=0.
		ANDI AUX,(~(1<<SE))
		OUTPUT SMCR,AUX
		RJMP MAIN

;------------------------------MAIN_FUNCTIONS-----------------------------------
AT_NIGHT:
		RCALL LIGHT_TURN_ON
		RCALL INDICATE_SOLAR_PANEL_LOW
		RJMP SLEEP_MODE

.include "MESSAGES.inc"