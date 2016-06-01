/******************************************

	 Me parece mas comodo e intuitivo
	poner los registros y definiciones
		en un header general

******************************************/

.IFNDEF SOLAR_TRACKER
.DSEG

;--------------------HEADER DE LA LUZ-----------------------------------------------
.EQU	LIGHT_PIN	=	PB0
;-----------------------------------------------------------------------------------


;--------------------HEADER DEL ADC-------------------------------------------------
.DEF	ADC_DATA_L = R19
.DEF	ADC_DATA_H = R20

.EQU	ADC_BATTERY = 6			;LA BATERIA ESTA EN EL PIN 19 [ADC6]
.EQU	ADC_SOLAR_PANEL = 7		;EL PANEL ESTA EN EL PIN 22 [ADC7]
.EQU	MIN_BATTERY_VALUE =		;RESULTADO DEL ADC QUE HACE IR A BAJO CONSUMO EL EQUIPO POR NO TENER SUFICIENTE TENSION {0,255}
;-----------------------------------------------------------------------------------


;--------------------HEADER DEL PUERTO SERIE----------------------------------------
.EQU	BAUD_RATE	= 103	; 12	76.8 kbps e=0.2%	@8MHz y U2X=1
							; 25	38.4 kbps e=0.2%	@8MHz y U2X=1
							; 51	19.2 kbps e=0.2% 	@8MHz y U2X=1
							; 103	9600 bps  e=0.2% 	@8MHz y U2X=1

.EQU	 BUF_SIZE	= 64	; tamaño en bytes del buffer de transmisión

;SERIE DE COMANDOS DE RECEPCION POR BT
.EQU	BT_COMMAND_PROJECT_NAME		=	'1'
.EQU	BT_COMMAND_V_BAT			=	'2'
.EQU	BT_COMMAND_V_PANEL			=	'3'
.EQU	BT_COMMAND_LIGHT_TURN_ON	=	'4'
.EQU	BT_COMMAND_LIGHT_TURN_OFF	=	'5'
.EQU	BT_COMMAND_DISCONNECT		=	'6'

TX_BUF:	.BYTE	BUF_SIZE	; buffer de transmisión

.DEF	PTR_TX_L = r8		; puntero al buffer de datos a transmitir
.DEF	PTR_TX_H = r9
.DEF	BYTES_A_TX = r10 	; nro. de bytes a transmitir desde el buffer
;-----------------------------------------------------------------------------------

;--------------------REGISTROS DE USO GENERAL SIN IMPORTAR QUE TENGA----------------------------------------
.DEF	AUX	= R16
.DEF	AUX1= R17
;-----------------------------------------------------------------------------------

.ENDIF