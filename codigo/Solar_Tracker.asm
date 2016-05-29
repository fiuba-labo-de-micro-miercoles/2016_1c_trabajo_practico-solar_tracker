;*****************************************************
;* 	Programa Principal Seudo Codigo
;*
;*  Created: 29/05/2016
;*  Autor: Joaquín Ulloa
;*	
;***************************************************** 

.include "m88def.inc"   	;Incluye los nombres de los registros del micro
.include "avr_macros.inc"	;Incluye los macros
.listmac					;Permite que se expandan las macros en el listado
;***************************************************** 
.DSEG						;Se abre un segmento de datos para definir variables

.DEF

.EQU

.CSEG
;***************************************************** 
.ORG 0x0000					;se setean los registros de interrupciones
RJMP SETUP					;CONVIENE HACER ESTOY ASI??
;Interrupciones

SETUP:
;Se inicializa el stack pointer
	LDI PWM_DATA,LOW(RAMEND)
	OUT SPL,PWM_DATA
	LDI	PWM_DATA,HIGH(RAMEND)
	OUT SPH,PWM_DATA
;Inicializacion

MAIN:	
	medir bateria:
		leer adc bat
		comparar
		si menor min jump a bajo consumo	;Habria que pense si de todas formas no hay que mover el panel
	dia o noche:
		leerLDRs
		comparar
		luz:
			verificar si hay que prender luz
			verificar si la luz esta prendida
			si cumple ambos prender
		si no hay sol jump a bajo consumo
	track:
		leerLDRs
		comparar
		si no hay diferencia salgo del loop
		moverMotores	;hay que pensar si harcodiamos cuanto tiempo se mueven los motores dependiendo la diferencia entre LEDR o si hacemos otra cosa
		detenerMotores
		volver a track
	leer panel
	bajo consumo	
RJMP MAIN


;Nos falta saber como hacer una interrupcion serie y como ir a modo bajo consumo
