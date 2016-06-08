/******************************************

	 Me parece mas comodo e intuitivo
	poner los registros y definiciones
		en un header general

******************************************/

.IFNDEF SOLAR_TRACKER
.DSEG

;--------------------HEADER DE LA LUZ-----------------------------------------------
.EQU	LIGHT_PIN					=	PB0
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
.EQU	BT_COMMAND_PROJECT_NAME		=	'1'
.EQU	BT_COMMAND_V_BAT			=	'2'
.EQU	BT_COMMAND_V_PANEL			=	'3'
.EQU	BT_COMMAND_LIGHT_TURN_ON	=	'4'
.EQU	BT_COMMAND_LIGHT_TURN_OFF	=	'5'
.EQU	BT_COMMAND_DISCONNECT		=	'6'

TX_BUF:	.BYTE	BUF_SIZE	; buffer de transmisión

.DEF	PTR_TX_L					=	R8		;puntero al buffer de datos a transmitir
.DEF	PTR_TX_H					=	R9
.DEF	BYTES_A_TX					=	R10 	;nro. de bytes a transmitir desde el buffer
;-----------------------------------------------------------------------------------

;--------------------------------HEADER DE LA BATERIA-------------------------------
.DEF	PIN_BATTERY					=	ADC6
.DEF	PIN_BATTERY_LED_OK			=	PD4
.DEF	PIN_BATTERY_LED_LOW			=	PD7

.EQU	MIN_BATTERY_VALUE			=			;{0,255} RESULTADO DEL ADC QUE HACE IR A BAJO CONSUMO EL EQUIPO POR NO TENER SUFICIENTE TENSION
;-----------------------------------------------------------------------------------

;-------------------------------HEADER DEL PANEL SOLAR------------------------------
.DEF	PIN_SOLAR_PANEL				=	ADC7
.DEF	PIN_SOLAR_PANEL_LED_OK		=	PC0
.DEF	PIN_SOLAR_PANEL_LED_LOW		=	PC1

.EQU	MIN_SOLAR_PANEL_VALUE		=			;{0,255} RESULTADO DEL ADC QUE DECIDE SI ES DE DIA O NOCHE. 
;-----------------------------------------------------------------------------------

;--------------------REGISTROS DE USO GENERAL SIN IMPORTAR QUE TENGA----------------
.DEF	AUX							=	R16
.DEF	AUX1						=	R17
.DEF	AUX2						=	R18
;-----------------------------------------------------------------------------------

;------------------------------HEADER DE LDRS---------------------------------------
.EQU NUM_SAMPLES = 32 ;TIENE QUE SER MULTIPLO DE 2

.ORG SRAM_START
LDR_NO_VEC:							.BYTE NUM_SAMPLES
LDR_NE_VEC:							.BYTE NUM_SAMPLES
LDR_SE_VEC:							.BYTE NUM_SAMPLES
LDR_SO_VEC:							.BYTE NUM_SAMPLES
LDR_NO_MEAN:						.BYTE 1
LDR_NE_MEAN:						.BYTE 1
LDR_SE_MEAN:						.BYTE 1
LDR_SO_MEAN:						.BYTE 1
COUNTER:							.BYTE 2
BT_FLAG:							.BYTE 1			;[FLAG=0xFF]: ESTA CONECTADO A BT. [FLAG=0x00]: NO ESTA CONECTADO A BT.

;LAS CONSTANTES SON PARA EL MUX EN EL ADC
.EQU LDR_NO							=	ADC2
.EQU LDR_NE							=	ADC3
.EQU LDR_SE							=	ADC4
.EQU LDR_SO							=	ADC5

.DEF LDR_NO_LOW						=	R11
.DEF LDR_NO_HIGH					=	R12
.DEF LDR_SO_LOW						=	R2
.DEF LDR_SO_HIGH					=	R3
.DEF LDR_SE_LOW						=	R4
.DEF LDR_SE_HIGH					=	R5
.DEF LDR_NE_LOW						=	R6
.DEF LDR_NE_HIGH					=	R7

;-----------------------------------------------------------------------------------

;------------------------------HEADER DEL PWM---------------------------------------

.DEF MOTOR_AZIMUT					=	OCR0A
.DEF MOTOR_ELEVATION				=	OCR0B
.DEF LIGHT							=	OCR2B

;-----------------------------------------------------------------------------------

.ENDIF