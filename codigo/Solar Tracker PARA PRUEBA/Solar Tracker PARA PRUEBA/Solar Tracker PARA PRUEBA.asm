/*
 * Solar_Tracker.asm
 *
 *  Created: 08/06/2016 07:45:11 p.m.
 *   Author: Agust�n Picard, Joaquin Ulloa, Mauro Giordano
 */ 

.include "m88def.inc"   	;Incluye los nombres de los registros del micro
.include "Solar_Tracker.inc"
.include "avr_macros.inc"	;Incluye los macros
.listmac					;Permite que se expandan las macros en el listado

.CSEG
.ORG 0x0000
RJMP SETUP	

.ORG	INT0addr
RJMP	ISR_INT0

.ORG	URXCaddr		; USART, Rx Complete
RJMP	ISR_RX_USART_COMPLETA
	
.ORG	UDREaddr		; USART Data Register Empty
RJMP	ISR_REG_USART_VACIO

.ORG	OVF1addr
RJMP	ISR_TIMER_1_OV

;-------------------------------SETUP--------------------------------------------
.ORG	INT_VECTORS_SIZE
SETUP:
	LDI AUX,LOW(RAMEND)
	OUT SPL,AUX
	LDI	AUX,HIGH(RAMEND)
	OUT SPH,AUX

	/*RCALL BT_DISCONNECT		;[FLAG=0xFF]: ESTA CONECTADO A BT. [FLAG=0x00]: NO ESTA CONECTADO A BT.
	;RCALL INT_EXT_INIT
	RCALL BATTERY_INIT
	RCALL SOLAR_PANEL_INIT
	RCALL ADC_INIT			;TIENE QUE ESTAR EN "ADC.inc"

	RCALL LDRS_INIT
	RCALL SERIAL_PORT_INIT	;TIENE QUE ESTAR EN "SERIAL_PORT.inc"
	RCALL MOTORS_INIT
	SEI*/

RJMP PRUEBA

	RCALL LIGHT_TURN_OFF	;TIENE QUE ESTAR EN "LIGHT.inc"

	SEI
	RJMP PRUEBA
;--------------------------------------------------------------------------------
PRUEBA:
	;	RCALL	READ_V_BATTERY					;MIDO LA BATERIA
		LDI		ADC_DATA_H,212					;230 ES 13.5V. 34 ES 2V. 
		RCALL	VBATTERY_TO_ASCII

	;	rcall indicate_solar_panel_low
;		rcall delay_500ms

	;	LDIW	X,V_BATTERY_DATA
;		ST		X,ADC_DATA_H

		LDIW	Z,(MSJ_V_BAT*2)
		LDIW	Y,V_BATTERY_DATA
		RCALL	TRANSMITIR_TENSION	

		RCALL	DELAY_50ms 
		RCALL	DELAY_50ms 

RJMP PRUEBA
/*		RCALL	SERIAL_PORT_INIT
	
		RCALL	INDICATE_SOLAR_PANEL_LOW

		RCALL	LDRS_READ						;LEE LOS LDR'S Y LOS MANDA A RAM.
		RCALL	LDRS_MEAN

		RCALL	DELAY_500ms



		RCALL PWM_INIT			;TIENE QUE ESTAR EN "PWM.inc"
		RCALL	ORIENTATE_SOLAR_PANEL

		LDIW	Z,(MSJ_V_LDRS*2)
		LDIW	Y,LDR_NO_MEAN
		RCALL	TRANSMITIR_TENSION
	*/	
	;	RJMP SLEEP_MODE
	;	RCALL DELAY_50ms
	;	RCALL DELAY_50ms
	
RJMP PRUEBA
/*		RCALL	LDRS_READ						;LEE LOS LDR'S Y LOS MANDA A RAM.
;		RCALL	LDRS_READ						;LEE LOS LDR'S Y LOS MANDA A RAM.
;		RCALL	LDRS_READ						;LEE LOS LDR'S Y LOS MANDA A RAM.
		RCALL	LDRS_MEAN

		LDIW	Z,(MSJ_V_LDRS*2)
		LDIW	Y,LDR_NO_MEAN
		RCALL	TRANSMITIR_TENSION
*/
/*		ser aux1
		RCALL MOTOR_AZIMUT_EAST
		RCALL DELAY_500ms
		RCALL DELAY_500ms

		RCALL MOTORS_OFF
		RCALL DELAY_500ms
		RCALL DELAY_500ms
		RCALL DELAY_500ms
		RCALL DELAY_500ms

		ser aux1
		RCALL MOTOR_AZIMUT_WEST
		RCALL DELAY_500ms
		RCALL DELAY_500ms

		RCALL MOTORS_OFF
		RCALL DELAY_500ms
		RCALL DELAY_500ms
		RCALL DELAY_500ms
		RCALL DELAY_500ms*/

RJMP PRUEBA

LED1:
		CBI PORTC,0
		RCALL DELAY_50ms
		SBI PORTC,0
		RCALL DELAY_50ms
		RJMP PRUEBA
LED2:
		CBI PORTC,1
		RCALL DELAY_50ms
		SBI PORTC,1
		RCALL DELAY_50ms
		RJMP PRUEBA
LED3:
		CBI PORTD,7
		RCALL DELAY_50ms
		SBI PORTD,7
		RCALL DELAY_50ms
		RJMP PRUEBA
LED4:
		CBI PORTD,4
		RCALL DELAY_50ms
		SBI PORTD,4
		RCALL DELAY_50ms
		RJMP PRUEBA

;-------------------------------PROGRAMA_PRINCIPAL-------------------------------
/*MAIN:	
	;MEDIR BATERIA 
		RCALL	READ_V_BATTERY					;MIDO LA BATERIA
		RCALL	CHECK_IF_BATTERY_MINIMUM		;[CARRY=1]: BATTERY LOW. [CARRY=0]: BATTERY OK
		BRCS	SLEEP_MODE
		RCALL	INDICATE_BATTERY_OK
	;�DIA O NOCHE?
		RCALL	READ_V_SOLAR_PANEL				;PARA VER SI ES DE DIA O NOCHE, MIDO LA TENSION DEL PANEL SOLAR.
		RCALL	CHECK_IF_SOLAR_PANEL_MINIMUM	;[CARRY=1]: SOLAR_PANEL LOW. [CARRY=0]: SOLAR_PANEL OK
		BRCS	AT_NIGHT
;SI ESTOY ACA YA TENGO BATERIA SUFICIENTE, ES DE DIA.
		RCALL	INDICATE_BATTERY_LOW			;RCALL	LIGHT_TURN_OFF
		RCALL	INDICATE_SOLAR_PANEL_OK
;PRENDER EL TIMER, LA INTERRUPCION DEL ADC MANDAR A SLEEP.
		RCALL	LDRS_READ						;LEE LOS LDR'S Y LOS MANDA A RAM.
		RCALL	LDRS_MEAN
		RCALL	PWM_INIT						;TIENE QUE ESTAR EN "PWM.inc"	
		RCALL	ORIENTATE_SOLAR_PANEL			;HAY QUE RESOLVER ESTO TODAVIA.

*/
;HAY QUE HACER COSAS ANTES DE IR A SLEEP, COMO APAGAR EL ADC Y NO SE QUE MAS [COMO EL PWM DE LOS MOTORES]
SLEEP_MODE:

		RCALL INDICATE_SOLAR_PANEL_OK
		RCALL SLEEP_TIMER_INIT
		INPUT AUX,SMCR
		ANDI AUX,((~((1<<SM2)|(1<<SM1)|(1<<SM0)|(1<<SE))))
		ORI AUX,((0<<SM2)|(0<<SM1)|(0<<SM0)|(1<<SE))	;SETEO EL MODO IDLE.
		OUTPUT SMCR,AUX
		SLEEP
		NOP
		INPUT AUX,SMCR
		ANDI AUX,(~(1<<SE))								;CUANDO SALGO DE SLEEP, PONGO SE=0.
		ORI AUX,(0<<SE)
		OUTPUT SMCR,AUX
	;	RJMP MAIN
RJMP PRUEBA
;-----------------------------------------------------------------------------------

;------------------------------MAIN_FUNCTIONS---------------------------------------
ISR_INT0:
RETI

ISR_TIMER_1_OV:
RETI

NO_SOLAR_PANEL_CONNECTED:
	INPUT AUX,BT_FLAG
	TST AUX
	BRNE SLEEP_MODE			;SI NO HAY CONEXION BT, NO MANDA NADA
	LDIW	Z,(MSJ_DISCONNECTED_PANEL*2)
	RCALL TRANSMITIR_MENSAJE
	RJMP SLEEP_MODE

AT_NIGHT:
;	RCALL LIGHT_TURN_ON
	RCALL INDICATE_BATTERY_OK
	RCALL INDICATE_SOLAR_PANEL_LOW
	RCALL DELAY_500ms 
	RJMP SLEEP_MODE

INT_EXT_INIT:
		;SETEO EL PIN INT0 (PD2) COMO ENTRADA.
		INPUT	AUX,DDRD
		ANDI	AUX,(~(1<<PD2))
		OUTPUT	DDRD,AUX

		;SETEO PARA QUE SALTE LA INT0 SI HAY CUALQUIER CAMBIO.
		INPUT	AUX,EICRA
		ANDI	AUX,(~((1<<ISC00)|(1<<ISC01)))
		ORI		AUX,((1<<ISC00)|(0<<ISC01))		
		OUTPUT	EICRA,AUX
	
		;HABILITO LA INTERRUPCION INT0.
		INPUT	AUX,EIMSK
		ANDI	AUX,~((1<<INT0)|(1<<INT1))		
		ORI		AUX,((1<<INT0)|(0<<INT1))
		OUTPUT	EIMSK,AUX
RET

SLEEP_TIMER_INIT:
			LDI	AUX,0xEE		;Pongo como valor inicial del timer 34286 para que cuando haga overflow haya contado (8E6)/256
			OUTPUT TCNT1L,AUX
			LDI AUX,0x85
			OUTPUT TCNT1H,AUX
			LDI AUX,(1<<CS12)|(0<<CS11)|(0<<CS10)		;Seteo el prescaler a 256
			OUTPUT TCCR1B,AUX
			LDI AUX,0
			OUTPUT TCCR1A,AUX
			LDI AUX,(1<<TOIE1)
			OUTPUT TIMSK1,AUX
RET

.include "ADC.inc"
.include "PWM.inc"
.include "SERIAL_PORT.inc"
.include "DELAY.inc"
.include "LIGHT.inc"
.include "BATTERY.inc"
.include "SOLAR_PANEL.inc"
.include "LDRS.inc"
.include "MOTORS.inc"
.include "MESSAGES.inc"
