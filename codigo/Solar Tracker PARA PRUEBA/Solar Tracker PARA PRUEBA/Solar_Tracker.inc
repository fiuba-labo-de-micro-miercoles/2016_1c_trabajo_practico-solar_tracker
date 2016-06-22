/******************************************

	 Me parece mas comodo e intuitivo
	poner los registros y definiciones
		en un header general

******************************************/
.DSEG
.ORG SRAM_START

;--------------------HEADER DE LA LUZ-----------------------------------------------
.EQU	LIGHT_PIN					=	PD3
;-----------------------------------------------------------------------------------


;--------------------HEADER DEL ADC-------------------------------------------------
.DEF	ADC_DATA_L					=	R19
.DEF	ADC_DATA_H					=	R20

.EQU	ADC_BATTERY					=	6		;LA BATERIA ESTA EN EL PIN 19 [ADC6]
.EQU	ADC_SOLAR_PANEL				=	7		;EL PANEL ESTA EN EL PIN 22 [ADC7]
;-----------------------------------------------------------------------------------


;--------------------HEADER DEL PUERTO SERIE----------------------------------------
.EQU	BAUD_RATE					=	103	; 12	76.8 kbps e=0.2%	@8MHz y U2X=1
											; 25	38.4 kbps e=0.2%	@8MHz y U2X=1
											; 51	19.2 kbps e=0.2% 	@8MHz y U2X=1
											; 103	9600 bps  e=0.2% 	@8MHz y U2X=1

.EQU	BUF_SIZE					=	 64	;tamaño en bytes del buffer de transmisión

;SERIE DE COMANDOS DE RECEPCION POR BT
.EQU	BT_COMMAND_PROJECT_NAME			=	'q'
.EQU	BT_COMMAND_V_BAT				=	'w'
.EQU	BT_COMMAND_V_PANEL				=	'e'
.EQU	BT_COMMAND_LIGHT_TURN_ON		=	'r'
.EQU	BT_COMMAND_LIGHT_TURN_OFF		=	't'
.EQU	BT_COMMAND_MANUAL_LIGHT_OFF		=	'y'
.EQU	BT_COMMAND_ELEVATION_NORTH		=	'u'
.EQU	BT_COMMAND_ELEVATION_SOUTH		=	'i'
.EQU	BT_COMMAND_AZIMUT_EAST			=	'o'
.EQU	BT_COMMAND_AZIMUT_WEST			=	'p'
.EQU	BT_COMMAND_RESET				=	'z'
.EQU	BT_COMMAND_MANUAL_MOTORS_OFF	=	'l'

TX_BUF:	.BYTE	BUF_SIZE	; buffer de transmisión

.DEF	PTR_TX_L					=	R8		;puntero al buffer de datos a transmitir
.DEF	PTR_TX_H					=	R9
.DEF	BYTES_A_TX					=	R10 	;nro. de bytes a transmitir desde el buffer
;-----------------------------------------------------------------------------------

;--------------------------------HEADER DE LA BATERIA-------------------------------
.EQU	PIN_BATTERY_LED_OK			=	PD4
.EQU	PIN_BATTERY_LED_LOW			=	PD7

.EQU	MIN_BATTERY_VALUE			=	170		;{0,255} RESULTADO DEL ADC QUE HACE IR A BAJO CONSUMO EL EQUIPO POR NO TENER SUFICIENTE TENSION
										;YA ESTA AJUSTADO! 10.6v
V_BATTERY_DATA:		.BYTE 8			;[DECENA,UNIDAD,'.',DECIMAL,'V','\r','\n',0]
;-----------------------------------------------------------------------------------

;-------------------------------HEADER DEL PANEL SOLAR------------------------------
.EQU	PIN_SOLAR_PANEL_LED_OK		=	PC1
.EQU	PIN_SOLAR_PANEL_LED_LOW		=	PC0

.EQU	MIN_SOLAR_PANEL_VALUE		=	30	;{0,255} RESULTADO DEL ADC QUE DECIDE SI ES DE DIA O NOCHE.
										;FALTA AJUSTAR CON EL SOL! .... O CON LO QUE VAMOS A MOSTRAR!
V_SOLAR_PANEL_DATA: .BYTE 8			;[DECENA,UNIDAD,'.',DECIMAL,'V','\r','\n',0]
;-----------------------------------------------------------------------------------

;--------------------REGISTROS DE USO GENERAL SIN IMPORTAR QUE TENGA----------------
.DEF	AUX							=	R16
.DEF	AUX1						=	R17
.DEF	AUX2						=	R18
.DEF	AUX3						=	R22
.DEF	AUX4						=	R23

;-----------------------------------------------------------------------------------

;------------------------------HEADER DE LDRS---------------------------------------
.EQU CANT_SAMPLES = 8 ;TIENE QUE SER MULTIPLO DE 2

LDR_NO_BUFFER:						.BYTE CANT_SAMPLES
LDR_NE_BUFFER:						.BYTE CANT_SAMPLES
LDR_SE_BUFFER:						.BYTE CANT_SAMPLES
LDR_SO_BUFFER:						.BYTE CANT_SAMPLES
LDR_NO_MEAN:						.BYTE 1
LDR_NE_MEAN:						.BYTE 1
LDR_SE_MEAN:						.BYTE 1
LDR_SO_MEAN:						.BYTE 1
COUNTER:							.BYTE 2
BT_MANUAL_LIGHT:					.BYTE 1				;[FLAG=0xFF]: SE MANEJA MANUAL. [FLAG=0x00]: SE MANEJA AUTOMATICO.
BT_MANUAL_MOTORS:					.BYTE 1				;[FLAG=0xFF]: SE MANEJA MANUAL. [FLAG=0x00]: SE MANEJA AUTOMATICO.
FLAG_AT_NIGHT:						.BYTE 1

;LAS CONSTANTES SON PARA EL MUX EN EL ADC
.EQU LDR_NO							=	3	;ADC2
.EQU LDR_NE							=	2	;ADC3
.EQU LDR_SE							=	4	;ADC4
.EQU LDR_SO							=	5	;ADC5		NO USAR EL ADC5 PARA COMPARAR! FUERA DE ESCALA LA RESISTENCIA DEL LDR

.DEF LDR_NO_LOW						=	R2
.DEF LDR_NO_HIGH					=	R3
.DEF LDR_NE_LOW						=	R4
.DEF LDR_NE_HIGH					=	R5
.DEF LDR_SE_LOW						=	R6
.DEF LDR_SE_HIGH					=	R7
.DEF LDR_SO_LOW						=	R11
.DEF LDR_SO_HIGH					=	R12



;-----------------------------------------------------------------------------------

;------------------------------HEADER DEL PWM---------------------------------------
.DEF PWM_DATA					 	=	R21

.EQU PWM_AZIMUT_DEFAULT				=	190		;{0,255} PARA SETEAR EL PWM
.EQU PWM_ELEVATION_DEFAULT			=	190		;{0,255} PARA SETEAR EL PWM
										;200 ES 10V APROX.

;-----------------------------------------------------------------------------------

;------------------------------HEADER DE MOTORES------------------------------------

.EQU MOT_1							=	PD5
.EQU MOT_2							=	PD6

;-----------------------------------------------------------------------------------
