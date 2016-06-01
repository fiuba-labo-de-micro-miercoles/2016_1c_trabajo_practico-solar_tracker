/*
 * PRUEBA_PUERTO_SERIE.asm
 *
 *  Created: 18/05/2016 07:33:17 p.m.
 *   Author: MAU
 */ 

;-------------------------------------------------------------------------
; CONSTANTES y MACROS
;-------------------------------------------------------------------------
.IFNDEF MAURO
.EQU	 BUF_SIZE	= 64	; tamaño en bytes del buffer de transmisión
.ENDIF
;-------------------------------------------------------------------------
; variables en SRAM
;-------------------------------------------------------------------------
.DSEG
TX_BUF:	.BYTE	BUF_SIZE	; buffer de transmisión

;-------------------------------------------------------------------------
; variables en registros
;-------------------------------------------------------------------------
.DEF	PTR_TX_L = r8		; puntero al buffer de datos a transmitir
.DEF	PTR_TX_H = r9
.DEF	BYTES_A_TX = r10 	; nro. de bytes a transmitir desde el buffer

.DEF	AUX	= r16
.DEF	AUX1= r17

;-------------------------------------------------------------------------
; codigo
;-------------------------------------------------------------------------
.CSEG

.ORG	URXCaddr		; USART, Rx Complete
RJMP	ISR_RX_USART_COMPLETA
	
.ORG	UDREaddr		; USART Data Register Empty
RJMP	ISR_REG_USART_VACIO

SERIAL_PORT_INIT:

		RCALL	USART_INIT	; Configuración del puerto serie a 76k8 bps
RET

;-------------------------------------------------------------------------
;					COMUNICACION SERIE
;-------------------------------------------------------------------------
.equ	BAUD_RATE	= 103	; 12	76.8 kbps e=0.2%	@8MHz y U2X=1
							; 25	38.4 kbps e=0.2%	@8MHz y U2X=1
							; 51	19.2 kbps e=0.2% 	@8MHz y U2X=1
							; 103	9600 bps  e=0.2% 	@8MHz y U2X=1
;-------------------------------------------------------------------------
USART_INIT:
		PUSH	AUX
		PUSH	AUX1
		PUSHW	X
	
		LDI		AUX,HIGH(BAUD_RATE)
		OUTPUT		UBRR0H,AUX	; Velocidad de transmisión
		LDI		AUX,LOW(BAUD_RATE)
		OUTPUT		UBRR0L,AUX	
		
		LDI		AUX,1<<U2X0		; Modo asinc., doble velocidad
		OUTPUT		UCSR0A,AUX	

		; Trama: 8 bits de datos, sin paridad y 1 bit de stop, 
		LDI		AUX,(0<<UPM01)|(0<<UPM00)|(0<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00)
		OUTPUT		UCSR0C,AUX


		; Configura los terminales de TX y RX; y habilita
		; 	únicamente la int. de recepción
		LDI		AUX,(1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0)|(0<<UDRIE0)
		OUTPUT		UCSR0B,AUX

		MOVI	PTR_TX_L,LOW(TX_BUF)	; inicializa puntero al 
		MOVI	PTR_TX_H,HIGH(TX_BUF)	; buffer de transmisión.
	
		LDIW	X,TX_BUF				; limpia BUF_SIZE posiciones 
		LDI		AUX1, BUF_SIZE			; del buffer de transmisión
		CLR		AUX
loop_limpia:
		ST		X+,AUX
		DEC		AUX1
		BRNE	loop_limpia
					
		CLR		BYTES_A_TX		; nada pendiente de transmisión

		POPW	X
		POP		AUX1
		POP		AUX
RET

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
		input AUX,UDR0

		CPI AUX,'1'
		BREQ CALL_TEST_TX

		CPI AUX,'2'
		BREQ CALL_V_BAT

		CPI AUX,'3'
		BREQ CALL_V_PANEL

SIGO:  	RETI 

CALL_TEST_TX:
		LDIW	Z,(MSJ_TEST_TX*2)
		RCALL TRANSMITIR_MENSAJE
		RJMP SIGO
CALL_V_BAT:
		LDIW	Z,(MSJ_V_BAT*2)
		RCALL TRANSMITIR_MENSAJE
		RJMP SIGO
CALL_V_PANEL:
		LDIW	Z,(MSJ_V_PANEL*2)
		RCALL TRANSMITIR_MENSAJE
		RJMP SIGO

;------------------------------------------------------------------------
; TRANSMISION: interrumpe cada vez que puede transmitir un byte.
; Se transmiten "BYTES_A_TX" comenzando desde la posición TX_BUF del
; buffer. Si "BYTES_A_TX" llega a cero, se deshabilita la interrupción.
;
; Recibe: 	BYTES_A_TX.
; Devuelve: PTR_TX_H:PTR_TX_L, y BYTES_A_TX.
;------------------------------------------------------------------------
ISR_REG_USART_VACIO:		; UDR está vacío
		PUSH	AUX
		PUSH	AUX1
		PUSHI	SREG
		PUSHW	X


		TST		BYTES_A_TX	; hay datos pendientes de transmisión?
		BREQ	FIN_TRANSMISION

		MOVW	XL,PTR_TX_L	; Recupera puntero al próximo byte a tx.
		LD		AUX,X+		; lee byte del buffer y apunta al
		OUTPUT		UDR0,AUX		; sgte. dato a transmitir (en la próxima int.)

		CPI		XL,LOW(TX_BUF+BUF_SIZE)
		BRLO	SALVA_PTR_TX
		CPI		XH,HIGH(TX_BUF+BUF_SIZE)
		BRLO	SALVA_PTR_TX
		LDIW	X,TX_BUF	; ptr_tx=ptr_tx+1, (módulo BUF_SIZE)

SALVA_PTR_TX:
		MOVW	PTR_TX_L,XL	; preserva puntero a sgte. dato

		DEC		BYTES_A_TX	; Descuenta el nro. de bytes a tx. en 1
		BRNE	SIGUE_TX	; si quedan datos que transmitir
							;	vuelve en la próxima int.
;REVISAR ESTE GRUPO DE INSTRUCCIONES
FIN_TRANSMISION:			; si no hay nada que enviar,
		INPUT	AUX,UCSR0B
		CBR		AUX,(1<<UDRIE0)
		OUTPUT	UCSR0B,AUX
		;se deshabilita la interrupción.

sigue_tx:
		POPW	X
		POPI	SREG
		POP		AUX1
		POP		AUX
		RETI

;-------------------------------------------------------------------------
; TEST_TX: transmite el mensaje almacenado en memoria flash a partir
; de la dirección MSJ_TEST_TX que termina con 0x00 (el 0 no se transmite).
; Recibe: nada
; Devuelve: PTR_TX_L|H, BYTES_A_TX.  
; Habilita la int. de transmisión serie con ISR en ISR_REG_USART_VACIO().
;-------------------------------------------------------------------------
TRANSMITIR_MENSAJE:
		PUSH	Z
		PUSHW	X
		PUSH	AUX

;		LDIW	Z,(MSJ_V_BAT*2)
		MOV2	XL,PTR_TX_L

LOOP_TRANSMITIR_MENSAJE:
		LPM		AUX,Z+
		TST		AUX
		BREQ	FIN_TRANSMITIR_MENSAJE

		ST		X+,AUX
		INC		BYTES_A_TX

		CPI		XL,LOW(TX_BUF+BUF_SIZE)
		BRLO	LOOP_TRANSMITIR_MENSAJE
		CPI		XH,HIGH(TX_BUF+BUF_SIZE)
		BRLO	LOOP_TRANSMITIR_MENSAJE
		LDIW	X,TX_BUF	; ptr_tx++ módulo BUF_SIZE

		RJMP	LOOP_TRANSMITIR_MENSAJE
;REVISAR INSTRUCCIONES
;	
FIN_TRANSMITIR_MENSAJE:
		INPUT	AUX,UCSR0B

		SBR		AUX,(1<<UDRIE0)
		OUTPUT	UCSR0B,AUX

		POP		AUX
		POPW	X
		POPW	Z
		RET

;LOS MENSAJES DEBEN SER EN ASCII [NO EXTENDIDO]
MSJ_TEST_TX:	.DB	"SOLAR TRACKER",'\r','\n',0
MSJ_V_BAT:		.DB	"HACIENDO EL FUTURO",'\r',0,0
MSJ_V_PANEL:	.DB "LA TENSION DEL PANEL ES: ",'\r',0,0
;-------------------------------------------------------------------------
; fin del código
;-------------------------------------------------------------------------