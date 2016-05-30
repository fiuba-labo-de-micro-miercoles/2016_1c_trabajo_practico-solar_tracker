;*****************************************************
;* 	CODIGO PWM PORTD
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
#IFNDEF PWM_DATA
.DEF	PWM_DATA 	= R16
.EQU	PWM_DC_DEFAULT = 30		;Duty cicle {0,255}
#ENDIF
;***************************************************** 
.CSEG
SETUP_PWM_D:
;Se inicializan como salida los pines de PWM
	INPUT PWM_DATA,DDRD
;	ANDI PWM_DATA,(~(0x60))	;Mascara para tocar solo D5 Y D6
	ANDI PWM_DATA,(~((1<<DDD5)|(1<<DDD6)))	;Mascara para tocar solo D5 Y D6
	ORI PWM_DATA,((1<<DDD5)|(1<<DDD6))
	OUTPUT DDRD,PWM_DATA
;Se inicializan como Fast PWM y non-inverting mode
	;Fast PWM: WGM02=0 (por defecto), WGM01=1 y WGM00=1
	;Non-inverting mode: COM0A1=1 y COM0A0=0
	;Descripcion de registros en seccion 15.9 (pag 106-112)
	INPUT PWM_DATA,TCCR1A	;Timer/counter control register A
;	ANDI PWM_DATA,(~(0xF3))	;Mascara para no tocar bits 2 y 3
	ANDI PWM_DATA,(~((1<<COM1A1)|(1<<COM1B1)|(1<<COM1A0)|(1<<COM1B0)|(1<<WGM10)|(1<<WGM11)))
	ORI PWM_DATA,((1<<COM1A1)|(1<<COM1B1)|(1<<COM1A0)|(1<<COM1B0)|(1<<WGM10)|(0<<WGM11))	;fast PWM, non-inverting
	OUTPUT TCCR1A,PWM_DATA
;Se inicializa el prescaler del PWM
	INPUT PWM_DATA,TCCR1B	;Timer/counter control register B
	;hacer mascara de forma tal que los bits 0, 1, 2 queden en 0
;	ANDI PWM_DATA,(~0x1F)	;Mascara para no tocar bits 3 a 7
	ANDI PWM_DATA,(~((1<<WGM13)|(1<<WGM12)|(1<<CS10)|(1<<CS11)|(1<<CS12)))
	ORI PWM_DATA,((0<<WGM13)|(1<<WGM12)|(0<<CS10)|(1<<CS11)|(0<<CS12))	;Prescaler = 8
	OUTPUT TCCR1B,PWM_DATA
RET
;*****************************************************

;ESTO ES UNA SUB RUTINA QUE SE DEBE EJECUTAR CADA VEZ QUE SE QUIERE SETEAR EL PWM
;RECIBE EL VALOR POR PWM_DATA
PWM_D_SET:	
	OUTPUT OCR1AL,PWM_DATA	
RET

;ESTO ES UNA SUB RUTINA QUE SE DEBE EJECUTAR CADA VEZ QUE SE QUIERE RESETEAR EL PWM
PWM_D_RESET:
	CLR PWM_DATA
	OUTPUT OCR1AL,PWM_DATA
RET
