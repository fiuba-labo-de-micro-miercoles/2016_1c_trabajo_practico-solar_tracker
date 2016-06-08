;*****************
;*	Filtro moving average de 8 coeficientes
;*
;*  Created: 05/06/2016
;*  Autor: Agustín Picard
;*	
;*	COMENTARIOS: NO ENTENDI POR QUÉ LAS FUNCIONES "SAVE_POINTERS_MA"
;*				 Y "RESTORE_POINTERS_MA" EN LDR_POINTERS_BACKUP. LLDR
;*				 EN LA FUNCION DEL PROMEDIO NO TOCA LOS REGISTROS BAJOS.
;****************** 

.include "m328Pdef.inc"   	;Incluye los nombres de los registros del micro
.include "ADC.inc"
.include "avr_macros.inc"	;Incluye los macros
.listmac					;Permite que se expandan las macros en el listado
;****************** 
.DSEG						;Se abre un segmento de datos para definir variables
.DEF	LDR_NOL	= R2
.DEF	LDR_NOH	= R3
.DEF	LDR_NEL	= R4
.DEF	LDR_NEH	= R5
.DEF	LDR_SOL	= R6
.DEF	LDR_SOH	= R7
.DEF	LDR_SEL	= R11	;R8 y R9 estaban usadas?? SI, POR PTR_TX_L,PTR_TX_H Y BYTES_A_TX.
.DEF	LDR_SEH	= R12
.DEF	AUX		= R16
.DEF	AUX1	= R17

.ORG 0X100
MA_LDR_NO:	.BYTE	8
MA_LDR_NE:	.BYTE	8
MA_LDR_SO:	.BYTE	8
MA_LDR_SE:	.BYTE	8
LDR_POINTERS_BACKUP:	.BYTE	8

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
			RCALL INIT_TIMER

MOVING_AVERAGE:
			LDI COUNT,8				;Cantidad de coeficientes
			RCALL SET_POINTERS_MA	;Seteo punteros
;
LOOP:		RLDR LDR_NO					;Leo el LDR noroeste
			RLDR LDR_NE					;Leo el LDR noreste
			RLDR LDR_SO					;Leo el LDR suroeste
			RLDR LDR_SE					;Leo el LDR sureste
			SLDR LDR_NOL,LDR_NOH,LDR_NO			
			SLDR LDR_NEL,LDR_NEH,LDR_NE
			SLDR LDR_SOL,LDR_SOH,LDR_SO
			SLDR LDR_SEL,LDR_SEH,LDR_SE
;ESTAS FUNCIONES HAY QUE HACERLAS INTERCALADAS Y LDR_XX ES UN .DEF; RLDR DEJA LA INFO EN ADC_DATA_H
;A SLDR HAY QUE PASARLE ADC_DATA_H EN EL @2.
			RCALL SAVE_POINTERS_MA
			PUSH COUNT
			RCALL CALC_MEAN
			POP COUNT
			RCALL RESTORE_POINTERS_MA
			RJMP SLEEP_MODE				;Por si después cambiamos de lugar las cosas en el código

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
			DEC COUNT
			BRNE LOOP
			RJMP MOVING_AVERAGE

SET_POINTERS_MA:	;Seteo punteros con los que voy a escribir a RAM las mediciones
			MOVI LDR_NOL,LOW(MA_LDR_NO)
			MOVI LDR_NOH,HIGH(MA_LDR_NO)
			MOVI LDR_NEL,LOW(MA_LDR_NE)
			MOVI LDR_NEH,HIGH(MA_LDR_NE)
			MOVI LDR_SOL,LOW(MA_LDR_SO)
			MOVI LDR_SOH,HIGH(MA_LDR_SO)
			MOVI LDR_SEL,LOW(MA_LDR_SE)
			MOVI LDR_SEH,HIGH(MA_LDR_SE)
RET

SAVE_POINTERS_MA:	;Guardo los punteros para no perderlos al hacer el promedio
			LDIW X,LDR_POINTERS_BACKUP
			ST X+,LDR_NOL
			ST X+,LDR_NOH
			ST X+,LDR_NEL
			ST X+,LDR_NEH
			ST X+,LDR_SOL
			ST X+,LDR_SOH
			ST X+,LDR_SEL
			ST X,LDR_SEH
RET

RESTORE_POINTERS_MA:	;Recupero la posición de los punteros como los dejé antes de promediar
			LDIW X,LDR_POINTERS_BACKUP
			LD LDR_NOL,X+
			LD LDR_NOH,X+
			LD LDR_NEL,X+
			LD LDR_NEH,X+
			LD LDR_SOL,X+
			LD LDR_SOH,X+
			LD LDR_SEL,X+
			LD LDR_SEH,X+
RET

CALC_MEAN:	;Calculo la media de las muestras que se tomaron para poder mover el panel acorde
		LDI COUNT,8
		;Calculo la media para cada LDR
		RCALL SET_POINTERS_MA		;Seteo los punteros al comienzo de cada vector
		LLDR Y,LDR_NOL,LDR_NOH		;Cargo el puntero Y con el vector del LDR noroeste
		vectmean Y,COUNT,DATA_NO	;Calculo la media de los valores del LDR noroeste
		LLDR Y,LDR_NEL,LDR_NEH		;Cargo el puntero Y con el vector del LDR noreste
		vectmean Y,COUNT,DATA_NE	;Calculo la media de los valores del LDR noreste
		LLDR Y,LDR_SOL,LDR_SOH		;Cargo el puntero Y con el vector del LDR suroeste
		vectmean Y,COUNT,DATA_SO	;Calculo la media de los valores del LDR suroeste
		LLDR Y,LDR_SEL,LDR_SEH		;Cargo el puntero Y con los valores del LDR sureste
		vectmean Z,COUNT,DATA_SE	;Calculo la media de los valores del LDR sureste
RET


INIT_TIMER:
			LDI	AUX,0xEE		;Pongo como valor inicial del timer 34286 para que cuando haga overflow haya contado (8E6)/256
			output TCNT1L,AUX
			LDI AUX,0x85
			output TCNT1H,AUX
			LDI AUX,0x04	;Seteo el prescaler a 256
			output TCCR1B,AUX
			LDI AUX,0x01
			output TIMSK1,AUX
RET

INTTIMER1OVF:
		RET