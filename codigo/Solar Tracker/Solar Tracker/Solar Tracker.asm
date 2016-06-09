/*
 * Solar_Tracker.asm
 *
 *  Created: 08/06/2016 07:45:11 p.m.
 *   Author: Agustín Picard, Joaquin Ulloa, Mauro Giordano
 */ 

.include "m88def.inc"   	;Incluye los nombres de los registros del micro
.include "Solar_Tracker.inc"
.include "MESSAGES.inc"
.include "avr_macros.inc"	;Incluye los macros
.listmac					;Permite que se expandan las macros en el listado

.CSEG
.ORG 0x0000
RJMP SETUP	

.ORG	URXCaddr		; USART, Rx Complete
RJMP	ISR_RX_USART_COMPLETA
	
.ORG	UDREaddr		; USART Data Register Empty
RJMP	ISR_REG_USART_VACIO

.ORG	OVF1addr
RJMP	ISR_TIMER_1_OV

;-------------------------------SETUP--------------------------------------------
.ORG	INT_VECTORS_SIZE
SETUP:
	RCALL STACK_INIT
	RCALL ADC_INIT			;TIENE QUE ESTAR EN "ADC.inc"
	RCALL PWM_INIT			;TIENE QUE ESTAR EN "PWM.inc"
	RCALL SERIAL_PORT_INIT	;TIENE QUE ESTAR EN "SERIAL_PORT.inc"
	RCALL LDRS_INIT
	RCALL LIGHT_TURN_OFF	;TIENE QUE ESTAR EN "LIGHT.inc"
	RCALL MOTORS_INIT
	RCALL BT_DISCONNECT		;[FLAG=0xFF]: ESTA CONECTADO A BT. [FLAG=0x00]: NO ESTA CONECTADO A BT.
	SEI
	RJMP MAIN
;--------------------------------------------------------------------------------


;-------------------------------PROGRAMA_PRINCIPAL-------------------------------
MAIN:	
	;MEDIR BATERIA 
		RCALL	READ_V_BATTERY					;MIDO LA BATERIA
		RCALL	CHECK_IF_BATTERY_MINIMUM		;[CARRY=1]: BATTERY LOW. [CARRY=0]: BATTERY OK
		BRCC	SLEEP_MODE
		RCALL	INDICATE_BATTERY_OK
	;¿DIA O NOCHE?
		RCALL	READ_V_SOLAR_PANEL				;PARA VER SI ES DE DIA O NOCHE, MIDO LA TENSION DEL PANEL SOLAR.
		RCALL	CHECK_IF_SOLAR_PANEL_MINIMUM	;[CARRY=1]: ES DE DIA. [CARRY=0]: ES DE NOCHE.
		BRCC	AT_NIGHT						;Me aseguro que esta funcion no modifique SREG 
;SI ESTOY ACA YA TENGO BATERIA SUFICIENTE, ES DE DIA.
		RCALL	LIGHT_TURN_OFF
		RCALL	INDICATE_SOLAR_PANEL_OK
;PRENDER EL TIMER, LA INTERRUPCION DEL ADC MANDAR A SLEEP
		RCALL	READ_LDRS						;LEE LOS LDR'S Y LOS MANDA A RAM.
		BRCC	SLEEP_MODE						;[CARRY=1]: HIZO PROMEDIO, NO SE VA A SLEEP. [CARRY=0]: NO HIZO PROMEDIO, SE VA A SLEEP.
		RCALL	ORIENTATE_SOLAR_PANEL			;HAY QUE RESOLVER ESTO TODAVIA.
;HACE UN LOOP CON LOS VALORES MEDIDOS Y MOVER A OJO.		
SLEEP_MODE:
;HAY QUE HACER COSAS ANTES DE IR A SLEEP, COMO APAGAR EL ADC Y NO SE QUE MAS [COMO EL PWM DE LOS MOTORES]
;SALIR DE SLEEP: EN UN TIEMPO t [TIEMPO ENTRE MEDICION Y MEDICION].
		INPUT AUX,SMCR
		ANDI AUX,(~(1<<SM2)|(1<<SM1)|(1<<SM0)|(1<<SE))
		ORI AUX,((0<<SM2)|(0<<SM1)|(0<<SM0)|(1<<SE))	;SETEO EL MODO IDLE.
		OUTPUT SMCR,AUX
		SLEEP
		NOP
		NOP
		ANDI AUX,(~(1<<SE))								;CUANDO SALGO DE SLEEP, PONGO SE=0.
		OUTPUT SMCR,AUX
RJMP MAIN
;-----------------------------------------------------------------------------------

;------------------------------MAIN_FUNCTIONS---------------------------------------
AT_NIGHT:
		RCALL LIGHT_TURN_ON
		RJMP SLEEP_MODE

STACK_INIT:
	LDI AUX,LOW(RAMEND)
	OUT SPL,AUX
	LDI	AUX,HIGH(RAMEND)
	OUT SPH,AUX
RET

ISR_TIMER_1_OV:
RETI

.include "ADC.inc"
.include "PWM.inc"
.include "SERIAL_PORT.inc"
.include "DELAY.inc"
.include "LIGHT.inc"
.include "BATTERY.inc"
.include "SOLAR_PANEL.inc"
.include "LDRS.inc"
.include "MOTORS.inc"
