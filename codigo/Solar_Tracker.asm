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

.include "ADC.inc"
.include "PWM.inc"
.include "SERIAL_PORT.inc"
.include "DELAY.inc"

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

	RCALL ADC_INIT ;TIENE QUE ESTAR EN "ADC.inc"
	RCALL PWM_INIT ;TIENE QUE ESTAR EN "PWM.inc"
	RCALL SP_INIT ;TIENE QUE ESTAR EN "SERIAL_PORT.inc"

;Inicializacion

MAIN:	
	medir bateria:
		leer adc bat
		comparar
		si menor min jump a bajo consumo
;Joaco: "Habria que pense si de todas formas no hay que mover el panel"
;Mauro:	"Para mi no. Si no tiene energía, que no gaste. "
	dia o noche:
		leerLDRs
		comparar
		luz:
			verificar si hay que prender luz
			verificar si la luz esta prendida
			si cumple ambos prender
;Mauro: "AL TENER UNA OPCION DE PRENDER LA LUZ CUANDO SE MANDA POR BT, CREO QUE SE CHEQUEA SI PRENDER O NO AHI; OSEA QUE NO IRÍA ACA ESO CREO."
		si no hay sol jump a bajo consumo
	track:
		leerLDRs
		comparar
		si no hay diferencia salgo del loop
		moverMotores
;Joaco: "hay que pensar si harcodiamos cuanto tiempo se mueven los motores dependiendo la diferencia entre LDR o si hacemos otra cosa"
;Mauro: "como leemos de 0-5v y el adc es de 10bits [1023 valores = 4,8mV por muestra]; shifteamos 2 lugares el adc y perdemos 15mV y listo.
;		En síntesis: comparamos hasta que sea igual [control por lazo cerrado] y listo." 
		detenerMotores
		volver a track
	leer panel
	bajo consumo	
RJMP MAIN


;Nos falta saber como hacer una interrupcion serie y como ir a modo bajo consumo
;Tener en cuenta que si el ADC de la bateria es 0x000 => NO HAY BATERIA CONECTADA. [IDEM PANEL SOLAR]