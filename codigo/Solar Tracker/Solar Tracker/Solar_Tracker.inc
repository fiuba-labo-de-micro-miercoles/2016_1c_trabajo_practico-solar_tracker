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

.EQU	BUF_SIZE					=	 64	;tama�o en bytes del buffer de transmisi�n

;SERIE DE COMANDOS DE RECEPCION POR BT
.EQU	BT_COMMAND_PROJECT_NAME		=	'1'
.EQU	BT_COMMAND_V_BAT			=	'2'
.EQU	BT_COMMAND_V_PANEL			=	'3'
.EQU	BT_COMMAND_LIGHT_TURN_ON	=	'4'
.EQU	BT_COMMAND_LIGHT_TURN_OFF	=	'5'
.EQU	BT_COMMAND_DISCONNECT		=	'6'

TX_BUF:	.BYTE	BUF_SIZE	; buffer de transmisi�n

.DEF	PTR_TX_L					=	R8		;puntero al buffer de datos a transmitir
.DEF	PTR_TX_H					=	R9
.DEF	BYTES_A_TX					=	R10 	;nro. de bytes a transmitir desde el buffer
;-----------------------------------------------------------------------------------

;--------------------------------HEADER DE LA BATERIA-------------------------------
.EQU	PIN_BATTERY_LED_OK			=	PD4
.EQU	PIN_BATTERY_LED_LOW			=	PD7

.EQU	MIN_BATTERY_VALUE			=	10		;{0,255} RESULTADO DEL ADC QUE HACE IR A BAJO CONSUMO EL EQUIPO POR NO TENER SUFICIENTE TENSION
;-----------------------------------------------------------------------------------

;-------------------------------HEADER DEL PANEL SOLAR------------------------------
.EQU	PIN_SOLAR_PANEL_LED_OK		=	PC0
.EQU	PIN_SOLAR_PANEL_LED_LOW		=	PC1

.EQU	MIN_SOLAR_PANEL_VALUE		=	10		;{0,255} RESULTADO DEL ADC QUE DECIDE SI ES DE DIA O NOCHE. 
;-----------------------------------------------------------------------------------

;--------------------REGISTROS DE USO GENERAL SIN IMPORTAR QUE TENGA----------------
.DEF	AUX							=	R16
.DEF	AUX1						=	R17
.DEF	AUX2						=	R18
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
BT_FLAG:							.BYTE 1				;[FLAG=0xFF]: ESTA CONECTADO A BT. [FLAG=0x00]: NO ESTA CONECTADO A BT.


;LAS CONSTANTES SON PARA EL MUX EN EL ADC
.EQU LDR_NO							=	2	;ADC2
.EQU LDR_NE							=	3	;ADC3
.EQU LDR_SE							=	4	;ADC4
.EQU LDR_SO							=	5	;ADC5

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
.DEF PWM_DATA					 	=	R21

.EQU PWM_AZIMUT_DEFAULT				=	60		;{0,255} PARA SETEAR EL PWM
.EQU PWM_ELEVATION_DEFAULT			=	70		;{0,255} PARA SETEAR EL PWM

.EQU MOTOR_AZIMUT					=	OCR0A
.EQU MOTOR_ELEVATION				=	OCR0B

.EQU LIGHT							=	OCR2B

;-----------------------------------------------------------------------------------

;------------------------------HEADER DE MOTORES------------------------------------

.EQU MOTOR_EN_A						=	PB1
.EQU MOTOR_EN_B						=	PB2

;-----------------------------------------------------------------------------------