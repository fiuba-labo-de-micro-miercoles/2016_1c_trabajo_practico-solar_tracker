;*****************************************************
;* 	CODIGO PWM 
;*
;*  Created: 29/05/2016
;*  Autor: Mauro Giordano
;*	
;*	f_outPWM = f_clkIO / (prescaler * 256)
;*	Configuracion Fast Pwm
;*	
;*****************************************************
.DSEG						;Se abre un segmento de datos para definir variables
.DEF	PWM_DATA 	= R30	;USO EL REGISTRO Z PARA INTERCAMBIAR DATOS EN WORDS
.DEF	PWM_DATA_H	= R31
.EQU	PWM_DC_DEFAULT = 30		;Duty cicle {0,255}
;***************************************************** 
.CSEG
PWM_INIT:
	RCALL PWM_INIT_PORT_B
;	RCALL PWM_INIT_PORT_B
;COMENTO LA FUNCION ANTERIOR PORQUE VAMOS A USAR MOTORES DE CONTINUA PUESTOS EN OC1A Y OC1B QUE SON LOS DEL PUERTO B SOLAMENTE.
RET

PWM_INIT_PORT_B:
;Se inicializan como salida los pines de PWM
	INPUT PWM_DATA,DDRB
	ANDI PWM_DATA (~((1<<DDB1)|(1<<DDB2)))	;Mascara para tocar solo B1 Y B2
	ORI PWM_DATA,((1<<DDB1)|(1<<DDB2))
	OUTPUT DDRB,PWM_DATA
;Se inicializan como Fast PWM y non-inverting mode
	;Fast PWM: WGM02=0 (por defecto), WGM01=1 y WGM00=1
	;Non-inverting mode: COM0A1=1 y COM0A0=0
	;Descripcion de registros en seccion 15.9 (pag 106-112)
	INPUT PWM_DATA,TCCR0A	;Timer/counter control register A
	ANDI PWM_DATA,(~((1<<WGM01)|(1<<WGM00)|(1<<COM0A1)|(1<<COM0A0)|(1<<COM0B1)|(1<<COM0B0)))	;Mascara para no tocar bits 2 y 3
	ORI PWM_DATA,((1<<WGM01)|(1<<WGM00)|(1<<COM0A1)|(0<<COM0A0)|(1<<COM0B1)|(0<<COM0B0))	;fast PWM, non-inverting
	OUTPUT TCCR0A,PWM_DATA
;Se inicializa el prescaler del PWM
	INPUT PWM_DATA,TCCR0B	;Timer/counter control register B
	;hacer mascara de forma tal que los bits 0, 1, 2 no se toquen
	ANDI PWM_DATA,(~((1<<CS00)|(1<<CS01)|(1<<CS02)))	;Mascara para no tocar bits 3 a 7
	ORI PWM_DATA,((0<<CS00)|(1<<CS01)|(0<<CS02))	;Prescaler = 8
	OUTPUT TCCR0B,PWM_DATA
;Se inicializa el pwm en cero
	RCALL PWM_MOTOR_ASIMUT_RESET
	RCALL PWM_MOTOR_ASIMUT_RESET
RET

;*****************************************************
/*
PWM_INIT_PORT_D:
;Se inicializan como salida los pines de PWM
	INPUT PWM_DATA,DDRD
	ANDI PWM_DATA,(~((1<<DDD5)|(1<<DDD6)))	;Mascara para tocar solo D5 Y D6
	ORI PWM_DATA,((1<<DDD5)|(1<<DDD6))
	OUTPUT DDRD,PWM_DATA
;Se inicializan como Fast PWM y non-inverting mode
	;Fast PWM: WGM02=0 (por defecto), WGM01=1 y WGM00=1
	;Non-inverting mode: COM0A1=1 y COM0A0=0
	;Descripcion de registros en seccion 15.9 (pag 106-112)
	INPUT PWM_DATA,TCCR1A	;Timer/counter control register A
	ANDI PWM_DATA,(~((1<<COM1A1)|(1<<COM1B1)|(1<<COM1A0)|(1<<COM1B0)|(1<<WGM10)|(1<<WGM11)))
	ORI PWM_DATA,((1<<COM1A1)|(1<<COM1B1)|(1<<COM1A0)|(1<<COM1B0)|(1<<WGM10)|(0<<WGM11))	;fast PWM, non-inverting
	OUTPUT TCCR1A,PWM_DATA
;Se inicializa el prescaler del PWM
	INPUT PWM_DATA,TCCR1B	;Timer/counter control register B
	;hacer mascara de forma tal que los bits 0, 1, 2 no se toquen
	ANDI PWM_DATA,(~((1<<WGM13)|(1<<WGM12)|(1<<CS10)|(1<<CS11)|(1<<CS12)))
	ORI PWM_DATA,((0<<WGM13)|(1<<WGM12)|(0<<CS10)|(1<<CS11)|(0<<CS12))	;Prescaler = 8
	OUTPUT TCCR1B,PWM_DATA
RET
*/

;*****************************************************
;PARA SETEAR EL PWM SE HACE CON ADIW ZL,k
;COMO EL PWM ES DE 16 BITS, RECIBE EL VALOR POR PWM_DATA_L Y PWM_DATA_L_H
PWM_MOTOR_ASIMUT_SET:		;EL DE ASIMUT ES PB1 [EN_A]
	OUTPUT OCR1AL,PWM_DATA
	OUTPUT OCR1AH,PWM_DATA_H	
RET

PWM_MOTOR_ASIMUT_RESET:
	CLR PWM_DATA			;EL DE ASIMUT ES PB1 [EN_A]
	OUTPUT OCR1AL,PWM_DATA	
	OUTPUT OCR1AH,PWM_DATA	
RET

;COMO EL PWM ES DE 16 BITS, RECIBE EL VALOR POR PWM_DATA_L Y PWM_DATA_L_H
PWM_MOTOR_ELEVACION_SET:	;EL DE ELEVACION ES PB2 [EN_B]
	OUTPUT OCR1BL,PWM_DATA
	OUTPUT OCR1BH,PWM_DATA_H	
RET

PWM_MOTOR_ELEVACION_RESET:	;EL DE ELEVACION ES PB2 [EN_B]
	CLR PWM_DATA
	OUTPUT OCR1BL,PWM_DATA	
	OUTPUT OCR1BH,PWM_DATA	
RET
