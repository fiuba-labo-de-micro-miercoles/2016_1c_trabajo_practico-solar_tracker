;*****************************************************
;* 	CODIGO PWM 
;*
;*  Created: 29/05/2016
;*  Autor: Mauro Giordano, Agustin Picard, Joaquín Ulloa
;*	
;*	f_outPWM = f_clkIO / (prescaler * 256)
;*	Configuracion Fast Pwm
;*	
;*****************************************************
.CSEG
PWM_INIT:
;	RCALL PWM_INIT_0
	RCALL PWM_SOLAR_PANEL_INIT
	RCALL PWM_LIGHT_INIT
;COMENTO LA FUNCION ANTERIOR PORQUE VAMOS A USAR MOTORES DE CONTINUA PUESTOS EN OC1A Y OC1B QUE SON LOS DEL PUERTO B SOLAMENTE.
RET

/*PWM_INIT_0:
;Se inicializan como salida los pines de PWM
	INPUT PWM_DATA,DDRD
	ANDI PWM_DATA,(~((1<<DDD5)|(1<<DDD6)))	;Mascara para tocar solo D5 Y D6
	ORI PWM_DATA,((1<<DDD5)|(1<<DDD6))
	OUTPUT DDRD,PWM_DATA
;Se inicializan como Fast PWM y non-inverting mode
	;Fast PWM: WGM02=0 (por defecto), WGM01=1 y WGM00=1
	;Non-inverting mode: COM0A1=1 y COM0A0=0
	INPUT PWM_DATA,TCCR0A	;Timer/counter control register A
	ANDI PWM_DATA,(~((1<<WGM01)|(1<<WGM00)|(1<<COM0A1)|(1<<COM0A0)|(1<<COM0B1)|(1<<COM0B0)))
	ORI PWM_DATA,((1<<WGM01)|(1<<WGM00)|(1<<COM0A1)|(0<<COM0A0)|(1<<COM0B1)|(0<<COM0B0))	;fast PWM, non-inverting
	OUTPUT TCCR0A,PWM_DATA
;Se inicializa el prescaler del PWM
	INPUT PWM_DATA,TCCR0B	;Timer/counter control register B
	;hacer mascara de forma tal que los bits 0, 1, 2 no se toquen
	ANDI PWM_DATA,(~((1<<CS00)|(1<<CS01)|(1<<CS02)))
	ORI PWM_DATA,((0<<CS00)|(1<<CS01)|(0<<CS02))	;Ver tabla prescalers al final del archivo
	OUTPUT TCCR0B,PWM_DATA
;Se inicializa el pwm en cero
	CLR	AUX
	OUTPUT	OCR0A,AUX
	OUTPUT	OCR0B,AUX
RET
*/
;*****************************************************

PWM_SOLAR_PANEL_INIT:
;Se inicializan como salida los pines de PWM
	INPUT PWM_DATA,DDRB
	ANDI PWM_DATA,(~((1<<DDB1)|(1<<DDB2)))	;Mascara para tocar solo B1 Y B2
	ORI PWM_DATA,((1<<DDB1)|(1<<DDB2))
	OUTPUT DDRB,PWM_DATA
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

	CLR	AUX
	OUTPUT	OCR1AL,AUX
	OUTPUT	OCR1BL,AUX
	OUTPUT	OCR1AH,AUX
	OUTPUT	OCR1BH,AUX
RET

;*****************************************************
PWM_LIGHT_INIT:
;Se inicializan como salida los pines de PWM
;Se usa uno solo de los pines (para la luz), el otro es el MOSI, lo dejamos solo para programar
	INPUT PWM_DATA,DDRD
	ANDI PWM_DATA,(~(1<<DDD3))	;Mascara para tocar solo D3
	ORI PWM_DATA,((1<<DDD3))
	OUTPUT DDRD,PWM_DATA
;Se inicializan como Fast PWM y non-inverting mode
	;Fast PWM: WGM22=0 (por defecto), WGM21=1 y WGM20=1
	;Non-inverting mode: COM2A1=1 y COM2A0=0
	INPUT PWM_DATA,TCCR2A	;Timer/counter control register A
	ANDI PWM_DATA,(~((1<<COM2B1)|(1<<COM2B0)|(1<<WGM20)|(1<<WGM21)))
	ORI PWM_DATA,((1<<COM2B1)|(0<<COM2B0)|(1<<WGM20)|(1<<WGM21))	;fast PWM, non-inverting
	OUTPUT TCCR2A,PWM_DATA
;Se inicializa el prescaler del PWM
	INPUT PWM_DATA,TCCR2B	;Timer/counter control register B
	;hacer mascara de forma tal que los bits 0, 1, 2 no se toquen
	ANDI PWM_DATA,(~((1<<CS20)|(1<<CS21)|(1<<CS22)))
	ORI PWM_DATA,((0<<CS20)|(1<<CS21)|(0<<CS22))	;Ver tabla prescalers al final del archivo
	OUTPUT TCCR2B,PWM_DATA

	CLR	AUX
	OUTPUT	OCR2B,AUX

RET



;*****************************************************

/*
;PWM DE 16 BITS, RECIBE EL VALOR POR PWM_DATA_L Y PWM_DATA_L_H
PWM_MOTOR_ELEVACION_SET:	;EL DE ELEVACION ES PD5 [MOT_1]
	OUTPUT OCR1BL,PWM_DATA
	OUTPUT OCR1BH,PWM_DATA_H	
RET

PWM_MOTOR_ELEVACION_RESET:	;EL DE ELEVACION ES PD6 [MOT_2]
	CLR PWM_DATA
	OUTPUT OCR1BL,PWM_DATA	
	OUTPUT OCR1BH,PWM_DATA_H	
RET
*/

;*****************************************************
/*
CS22	CS21	CS20	Description
0 		0 		0 		No clock source (timer/counter stopped)
0 		0 		1 		clkT2S/(no prescaling)
0 		1 		0 		clkT2S/8 (from prescaler)
0 		1 		1 		clkT2S/32 (from prescaler)
1 		0 		0 		clkT2S/64 (from prescaler)
1 		0 		1 		clkT2S/128 (from prescaler)
1 		1 		0 		clkT2S/256 (from prescaler)
1 		1 		1 		clkT2S/1024 (from prescaler)
*/
