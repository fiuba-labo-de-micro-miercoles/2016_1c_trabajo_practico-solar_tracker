;*****************************************************
;* 	Programa PWM
;*
;*  Created: 27/05/2016
;*  Autor: Joaquín Ulloa
;*	
;*	f_outPWM = f_clkIO / (prescaler * 256)
;*	Configuracion Fast Pwm
;*	
;***************************************************** 

.include "m88pdef.inc"   	;Incluye los nombres de los registros del micro
.include "avr_macros.inc"	;Incluye los macros
.listmac					;Permite que se expandan las macros en el listado
;***************************************************** 
.DSEG						;Se abre un segmento de datos para definir variables

.DEF	PWM_DATA 	= R16
.DEF	PWM_AUX 	= R17

.EQU	PWM_DC	= 128		;Duty cicle {0,255}

.CSEG
;***************************************************** 
.ORG 0x0000					;se setean los registros de interrupciones
RJMP MAIN_PWM


MAIN_PWM:
;Se inicializa el stack pointer
	LDI PWM_DATA,LOW(RAMEND)
	OUT SPL,PWM_DATA
	LDI	PWM_DATA,HIGH(RAMEND)
	OUT SPH,PWM_DATA

;Se inicializan como salida los pines de PWM
	SBI DDRB,1				;(PCINT1/OC1A) PB1
	SBI DDRB,2				;(PCINT2/OC1B/SS) PB2
	SBI DDRD,5				;(PCINT21/OC0B/T1) PD5
	SBI DDRD,6				;(PCINT22/OC0A/AIN0) PD6

;Se inicializan como Fast PWM y non-inverting mode
	;Fast PWM: WGM02=0 (por defecto), WGM01=1 y WGM00=1
	;Non-inverting mode: COM0A1=1 y COM0A0=0
	;Descripcion de registros en seccion 15.9 (pag 106-112)
	LDI PWM_DATA,PWM_DC		;Se define el duty cycle
	output OCR0A,PWM_DATA		;Se carga el duty cycle
	input PWM_DATA,TCCR0A	;Timer/counter control register A
	;hacer mascara de forma tal que todos los bits queden en 0, salvo el 3 y el 2
	ORI PWM_DATA,((1<<WGM01)|(1<<WGM00)|(1<<COM0A1)|(0<<COM0A0)|(1<<COM0B1)|(0<<COM0B0))
	OUT TCCR0A,PWM_DATA

	input PWM_DATA,TCCR0A	;Timer/counter control register B
	;hacer mascara de forma tal que los bits 0, 1, 2 queden en 0
	ORI PWM_DATA,((0<<CS00)|(1<<CS01)|(0<<CS02))	;Se configura el prescaler en 8
	output TCCR0B,PWM_DATA

	;hasta aca se configuro el pwm para el timer de 8 bits, falta el de 16
	

LOOP:
	RJMP LOOP
