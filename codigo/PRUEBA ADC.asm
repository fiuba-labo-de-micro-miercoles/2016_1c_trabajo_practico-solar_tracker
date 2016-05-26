/*
 * PRUEBA_ADC.asm
 *
 *  Created: 25/05/2016 04:10:33 a.m.
 *   Author: MAU
 */ 

 
;-------------------------------------------------------------------------
; INCLUSIONES
;-------------------------------------------------------------------------
.include "m328Pdef.inc"

;-------------------------------------------------------------------------
; CONSTANTES y MACROS
;-------------------------------------------------------------------------
.include "avr_macros.inc"
.listmac				; permite que se expandan las macros en el listado

.DEF	AUX	= R18
.DEF	AUX1 = R19
.DEF	AUX2 = R20
.DEF	AUX3 = R21

; codigo
;-------------------------------------------------------------------------
		.cseg
		rjmp	RESET			; interrupción del reset

		.ORG	ADCCaddr		; ADC CONVERSION COMPLETE
		RJMP	ISR_ADC_CONVERSION_COMPLETE

.ORG 	INT_VECTORS_SIZE
RESET:
		CLI

		LDI R18,0xFE
		OUT DDRC,R18
		LDI R18,0x90
		OUT DDRD,R18
		RCALL DELAY
		SBI PORTC,3
		SBI PORTC,2
		SBI PORTD,4
		SBI PORTD,7
		RCALL DELAY
				
		ldi 	r16,LOW(RAMEND)
		out 	spl,r16
		ldi 	r16,HIGH(RAMEND)
		out 	sph,r16		; inicialización del puntero a la pila

		rcall	ADC_INIT

PRUEBA_CONVERSOR:
		INPUT AUX,ADCSRA
		SBR AUX,ADEN
		SBR AUX,ADSC
		OUTPUT ADCSRA,AUX
		SEI
ESPERO:	INPUT AUX,ADCSRA
		SBRC AUX,ADIF
		RJMP ESPERO
		RCALL DELAY
;		RCALL TEST;
;SE INICIA LA CONVERSION [ADEN=1]Y SE ESPERA A QUE TERMINE [ADSC=1] ?? O ADIF ???
		INPUT AUX2,ADCH
		CLI
		CPI AUX2,0x00		
		BREQ ROJO1
		CPI AUX2,0xF0		
		BRSH VERDE1
		CPI AUX2,0xF0		
		BRLO VERDE2
		CPI AUX2,0x03		
		BREQ VERDE3		

SIGO:
		RCALL DELAY
		RCALL DELAY
		RJMP PRUEBA_CONVERSOR

ISR_ADC_CONVERSION_COMPLETE:
		
		INPUT AUX,ADCSRA
		SBR AUX,ADIF
		OUTPUT ADCSRA,AUX

		LDI AUX,PD7
		OUTPUT PORTD,AUX
	RETI

;-------------------------------------------------------------------------
;					CONVERSOR ANALOGICO-DIGITAL
;-------------------------------------------------------------------------

ADC_INIT:
;ADMUX = REFS1 REFS0 ADLAR – MUX3 MUX2 MUX1 MUX0
;INTERNAL VREF=VCC Y EL DATO AJUSTADO A DERECHA [ADCH:ADCL]. SE SELECCIONA POR DEFECTO EL CANAL ADC0 [LDR DE LA PLACA DE CDR]
	LDI AUX,(0<<REFS1)|(1<<REFS0)|(0<<ADLAR)|(0<<MUX3)|(0<<MUX2)|(0<<MUX1)|(0<<MUX0)
	OUTPUT ADMUX,AUX
;ADCSRA = ADEN ADSC ADATE ADIF ADIE ADPS2 ADPS1 ADPS0		
;SE HABILITA EL ADC, AUTO TRIGGER ON POR INT_EXT0, FLAG INTERRUPCION EN CERO, PRESCALER DIV POR 4	
	LDI AUX,(0<<ADEN)|(0<<ADSC)|(0<<ADATE)|(0<<ADIF)|(1<<ADIE)|(0<<ADPS2)|(1<<ADPS1)|(0<<ADPS0)
	OUTPUT ADCSRA,AUX

	LDI AUX,(0<<ADTS2)|(0<<ADTS1)|(0<<ADTS0)
	OUTPUT ADCSRB,AUX

	LDI AUX,(1<<ADC0D)
	OUTPUT DIDR0,AUX
	;SE DESHABILITA LA PARTE DIGITAL INTERNA DEL PIN A UTILIZAR
	RCALL TEST;

RET
;-------------------------------------------------------------------------

VERDE4:
	RCALL TEST
VERDE3:
	RCALL TEST
VERDE2:
	RCALL TEST
VERDE1:
	RCALL TEST
	RJMP SIGO

ROJO1:
	RCALL TESTR
	RJMP SIGO


TEST:
		CLI
		CBI PORTC,3
		RCALL DELAY
		SBI PORTC,3
		RCALL DELAY
		SEI
		RET
TESTR:
		CLI
		CBI PORTC,2
		RCALL DELAY
		SBI PORTC,2
		RCALL DELAY
		SEI
		RET

DELAY:	LDI AUX1,131			
		LDI AUX2,150		
		LDI AUX3,20		
DELAY_:	DEC AUX1			
		BRNE DELAY_			
		DEC AUX2			
		BRNE DELAY_			
		DEC AUX3		
		BRNE DELAY_			
		RET				
