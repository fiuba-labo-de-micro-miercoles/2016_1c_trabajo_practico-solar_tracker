;*****************************************************
;* 	CODIGO PWM PORTB
;*
;*  Created: 29/05/2016
;*  Autor: Joaquín Ulloa
;*	Checked by: Mauro Giordano
;*	
;*	f_outPWM = f_clkIO / (prescaler * 256)
;*	Configuracion Fast Pwm
;*	
;***************************************************** 
; habria que hace un include de delays y borrar delays!!
;.include "m328Pdef.inc"   	;Incluye los nombres de los registros del micro
;.include "avr_macros.inc"	;Incluye los macros
;.listmac					;Permite que se expandan las macros en el listado
;***************************************************** 
;La rutina usa un solo registro + los de delay
.DSEG						;Se abre un segmento de datos para definir variables
.DEF	PWM_DATA 	= R16
.EQU	PWM_DC_DEFAULT = 30		;Duty cicle {0,255}
;***************************************************** 
.CSEG
SETUP_PWM_B:
;Se inicializan como salida los pines de PWM
	INPUT PWM_DATA,DDRB
;	ANDI PWM_DATA (~(0x06))	;Mascara para tocar solo B1 Y B2
	ANDI PWM_DATA (~((1<<DDB1)|(1<<DDB2)))	;Mascara para tocar solo B1 Y B2
	ORI PWM_DATA,((1<<DDB1)|(1<<DDB2))
	OUTPUT DDRB,PWM_DATA
;Se inicializan como Fast PWM y non-inverting mode
	;Fast PWM: WGM02=0 (por defecto), WGM01=1 y WGM00=1
	;Non-inverting mode: COM0A1=1 y COM0A0=0
	;Descripcion de registros en seccion 15.9 (pag 106-112)
	INPUT PWM_DATA,TCCR0A	;Timer/counter control register A
;	ANDI PWM_DATA,(~(0xF3))	;Mascara para no tocar bits 2 y 3
	ANDI PWM_DATA,(~((1<<WGM01)|(1<<WGM00)|(1<<COM0A1)|(1<<COM0A0)|(1<<COM0B1)|(1<<COM0B0)))	;Mascara para no tocar bits 2 y 3
	ORI PWM_DATA,((1<<WGM01)|(1<<WGM00)|(1<<COM0A1)|(0<<COM0A0)|(1<<COM0B1)|(0<<COM0B0))	;fast PWM, non-inverting
	OUTPUT TCCR0A,PWM_DATA
;Se inicializa el prescaler del PWM
	INPUT PWM_DATA,TCCR0B	;Timer/counter control register B
	;hacer mascara de forma tal que los bits 0, 1, 2 queden en 0
;	ANDI PWM_DATA,(~0x07)	;Mascara para no tocar bits 3 a 7
	ANDI PWM_DATA,(~((1<<CS00)|(1<<CS01)|(1<<CS02)))	;Mascara para no tocar bits 3 a 7
	ORI PWM_DATA,((0<<CS00)|(1<<CS01)|(0<<CS02))	;Prescaler = 8
	OUTPUT TCCR0B,PWM_DATA
RET
;*****************************************************

;ESTO ES UNA SUB RUTINA QUE SE DEBE EJECUTAR CADA VEZ QUE SE QUIERE SETEAR EL PWM
;RECIBE EL VALOR POR PWM_DATA
PWM_B_SET:	;ESTO ES UNA SUB RUTINA QUE SE DEBE EJECUTAR CADA VEZ QUE SE QUIERE CAMBIAR EL PWM
	OUTPUT OCR0A,PWM_DATA	
RET

;ESTO ES UNA SUB RUTINA QUE SE DEBE EJECUTAR CADA VEZ QUE SE QUIERE RESETEAR EL PWM
PWM_B_RESET:
	CLR PWM_DATA
	OUTPUT OCR0A,PWM_DATA	
RET
;*****************************************************