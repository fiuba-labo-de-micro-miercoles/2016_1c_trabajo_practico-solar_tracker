/*
 * PRUEBA_ADC.asm
 *
 *  Created: 25/05/2016 04:10:33 a.m.
 *   Author: MAU
 *
 *	ESTE PROGRAMA RECIBE POR ADC0 UN LDR Y MANDA POR LOS 4 LEDS
 *	DE LA PLACA DEL CdR EL DATO.
 *
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

;		.ORG	ADCCaddr		; ADC CONVERSION COMPLETE
;		RJMP	ISR_ADC_CONVERSION_COMPLETE

.ORG 	INT_VECTORS_SIZE

RESET:
		LDI R18,0xFC
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

		RCALL	ADC_INIT

PRUEBA_CONVERSOR:
		INPUT AUX,ADCSRA
		ORI AUX,((1<<ADEN)|(1<<ADSC))
		OUTPUT ADCSRA,AUX

ESPERO:	INPUT AUX,ADCSRA
		SBRC AUX,ADSC
		RJMP ESPERO
		ANDI AUX,(~(1<<ADEN))
		OUTPUT ADCSRA,AUX
;SE INICIA LA CONVERSION [ADEN=1]Y SE ESPERA A QUE TERMINE [ADSC=1] ?? O ADIF ???
		INPUT AUX,ADCH
		CPI AUX,0x00		
		BREQ LED1
		CPI AUX,0x01		
		BREQ LED2
		CPI AUX,0x02		
		BREQ LED3
		CPI AUX,0x03		
		BREQ LED4	

SIGO:	RCALL DELAY
		RJMP PRUEBA_CONVERSOR

/*ISR_ADC_CONVERSION_COMPLETE:
		
		INPUT AUX,ADCSRA
		SBR AUX,ADIF
		OUTPUT ADCSRA,AUX

		LDI AUX,PD7
		OUTPUT PORTD,AUX
	RETI
*/
;-------------------------------------------------------------------------
;					CONVERSOR ANALOGICO-DIGITAL
;-------------------------------------------------------------------------

ADC_INIT:
;ADMUX = REFS1 REFS0 ADLAR – MUX3 MUX2 MUX1 MUX0
;INTERNAL VREF=VCC Y EL DATO AJUSTADO A DERECHA [ADCH:ADCL]. SE SELECCIONA POR DEFECTO EL CANAL ADC1 [LDR DE LA PLACA DE CdR MODIF]
	LDI AUX,((0<<REFS1)|(1<<REFS0)|(0<<ADLAR)|(0<<MUX3)|(0<<MUX2)|(0<<MUX1)|(1<<MUX0))
	OUTPUT ADMUX,AUX
;ADCSRA = ADEN ADSC ADATE ADIF ADIE ADPS2 ADPS1 ADPS0		
;SE HABILITA EL ADC, AUTO TRIGGER ON POR INT_EXT0, FLAG INTERRUPCION EN CERO, PRESCALER DIV POR 64 [trabaja en aprox 100Khz]
	LDI AUX,((1<<ADEN)|(0<<ADSC)|(0<<ADATE)|(0<<ADIF)|(0<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(0<<ADPS0))
	OUTPUT ADCSRA,AUX

;	LDI AUX,(0<<ADTS2)|(0<<ADTS1)|(0<<ADTS0)
;	OUTPUT ADCSRB,AUX

	LDI AUX,(1<<ADC1D)
	OUTPUT DIDR0,AUX
	;SE DESHABILITA LA PARTE DIGITAL INTERNA DEL PIN A UTILIZAR
RET
;-------------------------------------------------------------------------

LED1:
		CBI PORTC,3
		RCALL DELAY
		SBI PORTC,3
		RCALL DELAY
		RET
LED2:
		CBI PORTC,2
		RCALL DELAY
		SBI PORTC,2
		RCALL DELAY
		RET
LED3:
		CBI PORTD,7
		RCALL DELAY
		SBI PORTD,7
		RCALL DELAY
		RET
LED4:
		CBI PORTD,4
		RCALL DELAY
		SBI PORTD,4
		RCALL DELAY
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
