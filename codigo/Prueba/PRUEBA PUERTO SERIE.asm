/*
 * PRUEBA_PUERTO_SERIE.asm
 *
 *  Created: 18/05/2016 07:33:17 p.m.
 *   Author: MAU
 */ 


;-------------------------------------------------------------------------
; AVR - Configuración y transmisión por puerto serie
;-------------------------------------------------------------------------
;-------------------------------------------------------------------------
; MCU: ATmega328P con oscilador interno a 8 MHz
;-------------------------------------------------------------------------

;-------------------------------------------------------------------------
; Versión adaptada del ATmega8 para que corra sobre el ATmega328P.
; Compila bien pero falta probarlo sobre un MCU 
;-------------------------------------------------------------------------

;-------------------------------------------------------------------------
; INCLUSIONES
;-------------------------------------------------------------------------
.include "m328Pdef.inc"

;-------------------------------------------------------------------------
; CONSTANTES y MACROS
;-------------------------------------------------------------------------
.include "avr_macros.inc"
.listmac				; permite que se expandan las macros en el listado

.equ	 BUF_SIZE	= 64	; tamaño en bytes del buffer de transmisión

;-------------------------------------------------------------------------
; variables en SRAM
;-------------------------------------------------------------------------
		.dseg 
TX_BUF:	.byte	BUF_SIZE	; buffer de transmisión

;-------------------------------------------------------------------------
; variables en registros
;-------------------------------------------------------------------------
.def	ptr_tx_L = r8		; puntero al buffer de datos a transmitir
.def	ptr_tx_H = r9
.def	bytes_a_tx = r10 	; nro. de bytes a transmitir desde el buffer

.def	t0	= r16
.def	t1	= r17
.DEF	AUX	= R18
.DEF	AUX1 = R19
.DEF	AUX2 = R20
.DEF	AUX3 = R21

;-------------------------------------------------------------------------
; codigo
;-------------------------------------------------------------------------
		.cseg
		rjmp	RESET			; interrupción del reset

		.org	URXCaddr		; USART, Rx Complete
		rjmp	ISR_RX_USART_COMPLETA
	
		.org	UDREaddr		; USART Data Register Empty
		rjmp	ISR_REG_USART_VACIO

.ORG 	INT_VECTORS_SIZE
RESET:

		LDI R18,0xFF
		OUT DDRC,R18
		LDI R18,0x90
		OUT DDRD,R18
		RCALL DELAY
		SBI PORTC,3
		SBI PORTC,2
		SBI PORTD,4
		SBI PORTD,7
		
		ldi 	r16,LOW(RAMEND)
		out 	spl,r16
		ldi 	r16,HIGH(RAMEND)
		out 	sph,r16		; inicialización del puntero a la pila

		rcall	USART_init	; Configuración del puerto serie a 76k8 bps

		sei					; habilitación global de todas las interrupciones

X_SIEMPRE:

		USART_Receive:
		; Wait for data to be received
		input r16, UCSR0A
		sbrs r16, RXC0
		rjmp USART_Receive
		; Get and return received data from buffer
		input AUX, UDR0

		CPI AUX,'1'
		BREQ CALL_TEST_TX

		CPI AUX,'2'
		BREQ CALL_V_BAT

		CPI AUX,'3'
		BREQ CALL_V_PANEL

SIGO:	RJMP X_SIEMPRE

CALL_TEST_TX:
		ldiw	Z,(MSJ_TEST_TX*2)
		RCALL TRANSMITIR_MENSAJE
		RJMP SIGO
CALL_V_BAT:
		ldiw	Z,(MSJ_V_BAT*2)
		RCALL TRANSMITIR_MENSAJE
		RJMP SIGO
CALL_V_PANEL:
		ldiw	Z,(MSJ_V_PANEL*2)
		RCALL TRANSMITIR_MENSAJE
		RJMP SIGO

;PARA CONFIGURAR EL OSCILADOR EXTERNO A 8MHZ.
;avrdude -c usbtiny -p m328p -U lfuse:r:-:i -F		
;avrdude -c usbtiny -p m328p -U lfuse:w:0xE2:m -F

VERDE4:
		CLI
		CBI PORTC,3
		RCALL DELAY
		SBI PORTC,3
		RCALL DELAY
VERDE3:
		CLI
		CBI PORTC,3
		RCALL DELAY
		SBI PORTC,3
		RCALL DELAY
VERDE2:
		CLI
		CBI PORTC,3
		RCALL DELAY
		SBI PORTC,3
		RCALL DELAY
VERDE1:
		CLI
		CBI PORTC,3
		RCALL DELAY
		SBI PORTC,3
		SEI
		rjmp	X_SIEMPRE

DELAY:	LDI AUX1,131			;1
		LDI AUX2,150			;1
		LDI AUX3,20				;1
DELAY_:	DEC AUX1				;1
		BRNE DELAY_				;1/2
		DEC AUX2				;1
		BRNE DELAY_				;1/2
		DEC AUX3				;1
		BRNE DELAY_				;1/2
		RET						;4/5

;-------------------------------------------------------------------------
;					COMUNICACION SERIE
;-------------------------------------------------------------------------
.equ	BAUD_RATE	= 103	; 12	76.8 kbps e=0.2%	@8MHz y U2X=1
							; 25	38.4 kbps e=0.2%	@8MHz y U2X=1
							; 51	19.2 kbps e=0.2% 	@8MHz y U2X=1
							; 103	9600 bps  e=0.2% 	@8MHz y U2X=1
;-------------------------------------------------------------------------
USART_init:
		push	t0
		push	t1
		pushw	X
	
		ldi		t0,high(BAUD_RATE)
		output		UBRR0H,t0	; Velocidad de transmisión
		ldi		t0,low(BAUD_RATE)
		output		UBRR0L,t0	
		
		ldi		t0,1<<U2X0		; Modo asinc., doble velocidad
		output		UCSR0A,t0	

		; Trama: 8 bits de datos, sin paridad y 1 bit de stop, 
		ldi		t0,(0<<UPM01)|(0<<UPM00)|(0<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00)
		output		UCSR0C,t0


		; Configura los terminales de TX y RX; y habilita
		; 	únicamente la int. de recepción
		ldi		t0,(1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0)|(0<<UDRIE0)
		output		UCSR0B,t0

		movi	ptr_tx_L,LOW(TX_BUF)	; inicializa puntero al 
		movi	ptr_tx_H,HIGH(TX_BUF)	; buffer de transmisión.
	
		ldiw	X,TX_BUF				; limpia BUF_SIZE posiciones 
		ldi		t1, BUF_SIZE			; del buffer de transmisión
		clr		t0
loop_limpia:
		st		X+,t0
		dec		t1
		brne	loop_limpia
					
		clr		bytes_a_tx		; nada pendiente de transmisión

		popw	X
		pop		t1
		pop		t0
		ret


;-------------------------------------------------------------------------
; RECEPCION: Interrumpe cada vez que se recibe un byte x RS232.
;
; Recibe:	UDR (byte de dato)
; Devuelve: nada
;-------------------------------------------------------------------------
ISR_RX_USART_COMPLETA:
;
; EL registro UDR tiene un dato y debería ser procesado
;
    	reti 

;------------------------------------------------------------------------
; TRANSMISION: interrumpe cada vez que puede transmitir un byte.
; Se transmiten "bytes_a_tx" comenzando desde la posición TX_BUF del
; buffer. Si "bytes_a_tx" llega a cero, se deshabilita la interrupción.
;
; Recibe: 	bytes_a_tx.
; Devuelve: ptr_tx_H:ptr_tx_L, y bytes_a_tx.
;------------------------------------------------------------------------
ISR_REG_USART_VACIO:		; UDR está vacío
		push	t0
		push	t1
		pushi	SREG
		pushw	X


		tst		bytes_a_tx	; hay datos pendientes de transmisión?
		breq	FIN_TRANSMISION

		movw	XL,ptr_tx_L	; Recupera puntero al próximo byte a tx.
		ld		t0,X+		; lee byte del buffer y apunta al
		output		UDR0,t0		; sgte. dato a transmitir (en la próxima int.)

		cpi		XL,LOW(TX_BUF+BUF_SIZE)
		brlo	SALVA_PTR_TX
		cpi		XH,HIGH(TX_BUF+BUF_SIZE)
		brlo	SALVA_PTR_TX
		ldiw	X,TX_BUF	; ptr_tx=ptr_tx+1, (módulo BUF_SIZE)

SALVA_PTR_TX:
		movw	ptr_tx_L,XL	; preserva puntero a sgte. dato

		dec		bytes_a_tx	; Descuenta el nro. de bytes a tx. en 1
		brne	SIGUE_TX	; si quedan datos que transmitir
							;	vuelve en la próxima int.
;REVISAR ESTE GRUPO DE INSTRUCCIONES
FIN_TRANSMISION:			; si no hay nada que enviar,
		input	t0,UCSR0B
		cbr		t0,(1<<UDRIE0)
		output	UCSR0B,t0
		;se deshabilita la interrupción.

sigue_tx:
		popw	X
		popi	SREG
		pop		t1
		pop		t0
		reti

;-------------------------------------------------------------------------
; TEST_TX: transmite el mensaje almacenado en memoria flash a partir
; de la dirección MSJ_TEST_TX que termina con 0x00 (el 0 no se transmite).
; Recibe: nada
; Devuelve: ptr_tx_L|H, bytes_a_tx.  
; Habilita la int. de transmisión serie con ISR en ISR_REG_USART_VACIO().
;-------------------------------------------------------------------------
TRANSMITIR_MENSAJE:
		pushw	Z
		pushw	X
		push	t0

;		ldiw	Z,(MSJ_V_BAT*2)
		movw	XL,ptr_tx_L

LOOP_TRANSMITIR_MENSAJE:
		lpm		t0,Z+
		tst		t0
		breq	FIN_TRANSMITIR_MENSAJE

		st		X+,t0
		inc		bytes_a_tx

		cpi		XL,LOW(TX_BUF+BUF_SIZE)
		brlo	LOOP_TRANSMITIR_MENSAJE
		cpi		XH,HIGH(TX_BUF+BUF_SIZE)
		brlo	LOOP_TRANSMITIR_MENSAJE
		ldiw	X,TX_BUF	; ptr_tx++ módulo BUF_SIZE

		rjmp	LOOP_TRANSMITIR_MENSAJE
;REVISAR INSTRUCCIONES
;	
FIN_TRANSMITIR_MENSAJE:
		input	t0,UCSR0B

		sbr		t0,(1<<UDRIE0)
		output	UCSR0B,t0
		;sbi		UCSR0B,UDRIE0

		pop		t0
		popw	X
		popw	Z
		ret

;LOS MENSAJES DEBEN SER EN ASCII [NO EXTENDIDO]
MSJ_TEST_TX:	.DB	"SOLAR TRACKER",'\r','\n',0
MSJ_V_BAT:		.DB	"HACIENDO EL FUTURO",'\r',0,0
MSJ_V_PANEL:	.DB "LA TENSION DEL PANEL ES: ",'\r',0,0
;-------------------------------------------------------------------------
; fin del código
;-------------------------------------------------------------------------