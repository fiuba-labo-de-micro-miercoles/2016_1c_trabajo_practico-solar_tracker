;****************************************************
;*	Filtro moving average de 32 coeficientes
;*
;*  Created: 05/06/2016
;*  Autor: Agustín Picard
;*	
;*	
;***************************************************** 

.include "m328Pdef.inc"   	;Incluye los nombres de los registros del micro
.include "ADC.inc"
.include "DELAY.inc"
.include "avr_macros.inc"	;Incluye los macros
.listmac					;Permite que se expandan las macros en el listado
;***************************************************** 
.DSEG						;Se abre un segmento de datos para definir variables
.DEF	PWM_DATA 	= R16
.DEF	AUX		= R17
.DEF	AUX1	= R18
.DEF	AUX2	= R19
.DEF	AUX3	= R20

.ORG 0X100
MA_LDR_NO:	.BYTE	32
MA_LDR_NE:	.BYTE	32
MA_LDR_SO:	.BYTE	32
MA_LDR_SE:	.BYTE	32

.CSEG
.ORG 0x0000
	RJMP MAIN_MA ;Seteo para la interrupción de reset

.ORG 0x00D
	RJMP INTTIMER1OVF

.ORG INT_VECTORS_SIZE

MAIN_MA:	;Seteo el stack pointer
INIT_CONFIG_MA:
			LDI AUX,LOW(RAMEND)
			OUT SPL,AUX
			LDI	AUX,HIGH(RAMEND)
			OUT SPH,AUX	
			SEI ;Habilito interrupciones globales

MOVING_AVERAGE: ;Subrutina que aplica el filtro moving average
			LDI COUNT,32 ;Seteo el contador de coeficientes
			;Seteo los punteros
			RCALL SET_POINTERS_MA

LOOP:		RCALL LDR_NO_READ	;Leo el LDR noroeste
			LD X+,ADC_DATA_L	;Mando la medición a RAM
			RCALL LDR_NE_READ	;Leo el LDR noreste
			LD Y+,ADC_DATA_L	;Mando la medición a RAM
			RCALL LDR_SO_READ	;Leo el LDR suroeste
			LD Z,ADC_DATA_L		;Mando la medición a RAM
			RCALL LDR_SE_READ	;Leo el LDR sureste
			STD Z+32,ADC_DATA_L	;Mando la medición a RAM
			INC R31				;Incremento el puntero Z
			BRVS 2				;Si hubo overflow, seteo el byte bajo de Z a 0 y la alta la incremento en 1
			LDI R31,0
			INC R30
			DEC COUNT			;Cuento a ver si ya tengo los 32 coeficientes
			BRNE SLEEP_MODE
			RCALL CALC_MEAN
			RJMP MOVING_AVERAGE


LDR_NO_READ:
	LDI ADC_DATA_L,LDR_NO
	RCALL ADC_INPUT_SELECT
	RCALL ADC_SIMPLE_CONVERSION
RET

LDR_SO_READ:
	LDI ADC_DATA_L,LDR_SO
	RCALL ADC_INPUT_SELECT
	RCALL ADC_SIMPLE_CONVERSION
RET

LDR_SE_READ:
	LDI ADC_DATA_L,LDR_SE
	RCALL ADC_INPUT_SELECT
	RCALL ADC_SIMPLE_CONVERSION
RET

LDR_NE_READ:
	LDI ADC_DATA_L,LDR_NE
	RCALL ADC_INPUT_SELECT
	RCALL ADC_SIMPLE_CONVERSION
RET

SET_POINTERS_MA:	;Seteo punteros con los que voy a escribir a RAM las mediciones
			ldiw X,MA_LDR_NO
			ldiw Y,MA_LDR_NE
			ldiw Z,MA_LDR_SO
			LDI AUX,LOW(MA_LDR_SE)
			MOV R2,AUX				;Uso a R2 como parte baja del puntero al vector de mediciones del LDR_SE
			LDI AUX,HIGH(MA_LDR_SE)
			MOV R3,AUX				;Uso a R3 como parte alta del puntero al vector de mediciones del LDR_SE
RET

CALC_MEAN:	;Calculo la media de las muestras que se tomaron para poder mover el panel acorde
		LDI COUNT,32
		RCALL SET_POINTERS_MA
		vectmean X,COUNT,DATA_NO
		vectmean Y,COUNT,DATA_NE
		vectmean Z,COUNT,DATA_SO
		MOV ZL,R2
		MOV ZH,R3
		vectmean Z,COUNT,DATA_SE
RET


SLEEP_MODE:
;HAY QUE HACER COSAS ANTES DE IR A SLEEP, COMO APAGAR EL ADC Y NO SE QUE MAS
;SALIR DE SLEEP: EN UN TIEMPO t [TIEMPO ENTRE MEDICION Y MEDICION].
		INPUT AUX,SMCR
		ANDI AUX,(~(1<<SM2)|(1<<SM1)|(1<<SM0)|(1<<SE))
		ORI AUX,((0<<SM2)|(0<<SM1)|(0<<SM0)|(1<<SE))	;SETEO EL MODO IDLE.
		OUTPUT SMCR,AUX
		RCALL INIT_TIMER	;Ver bien ESTO!!
		SLEEP
		NOP
		ANDI AUX,(~(1<<SE))								;CUANDO SALGO DE SLEEP, PONGO SE=0.
		OUTPUT SMCR,AUX
		RJMP LOOP

INIT_TIMER:
			LDI	AUX,0xEE		;Pongo como valor inicial del timer 34286 para que cuando haga overflow haya contado (8E6)/256
			output TCNT1L,AUX
			LDI AUX,0x85
			output TCNT1H,AUX
			LDI AUX,0b00000100	;Seteo el prescaler a 256
			output TCCR1B,AUX
			LDI AUX,0x01
			output TIMSK1,AUX
RET

INTTIMER1OVF:
		RETI
