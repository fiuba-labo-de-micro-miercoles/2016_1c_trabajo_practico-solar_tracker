/*
 * PRUEBA_PUERTO_SERIE.asm
 *
 *  Created: 18/05/2016 07:33:17 p.m.
 *   Author: MAU
 */ 


;-------------------------------------------------------------------------
; codigo
;-------------------------------------------------------------------------
.CSEG

SERIAL_PORT_INIT:
		PUSH	AUX
		PUSH	AUX1
		PUSHW	X
	
		LDI		AUX,HIGH(BAUD_RATE)
		OUTPUT		UBRR0H,AUX	; Velocidad de transmisi�n
		LDI		AUX,LOW(BAUD_RATE)
		OUTPUT		UBRR0L,AUX	
		
		LDI		AUX,1<<U2X0		; Modo asinc., doble velocidad
		OUTPUT		UCSR0A,AUX	

		; Trama: 8 bits de datos, sin paridad y 1 bit de stop, 
		LDI		AUX,(0<<UPM01)|(0<<UPM00)|(0<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00)
		OUTPUT		UCSR0C,AUX


		; Configura los terminales de TX y RX; y habilita
		; 	�nicamente la int. de recepci�n
		LDI		AUX,(1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0)|(0<<UDRIE0)
		OUTPUT		UCSR0B,AUX

		MOVI	PTR_TX_L,LOW(TX_BUF)	; inicializa puntero al 
		MOVI	PTR_TX_H,HIGH(TX_BUF)	; buffer de transmisi�n.
	
		LDIW	X,TX_BUF				; limpia BUF_SIZE posiciones 
		LDI		AUX1, BUF_SIZE			; del buffer de transmisi�n
		CLR		AUX
loop_limpia:
		ST		X+,AUX
		DEC		AUX1
		BRNE	loop_limpia
					
		CLR		BYTES_A_TX		; nada pendiente de transmisi�n

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

		SER AUX
		OUTPUT BT_FLAG,AUX	;[FLAG=0xFF]: ESTA CONECTADO A BT. 
							;[FLAG=0x00]: NO ESTA CONECTADO A BT.
		INPUT AUX,UDR0

		CPI AUX,BT_COMMAND_PROJECT_NAME
		BREQ CALL_PROJECT_NAME

		CPI AUX,BT_COMMAND_V_BAT
		BREQ CALL_V_BAT

		CPI AUX,BT_COMMAND_V_PANEL
		BREQ CALL_V_PANEL

		CPI AUX,BT_COMMAND_LIGHT_TURN_ON
		BREQ _LIGHT_TURN_ON

		CPI AUX,BT_COMMAND_LIGHT_TURN_OFF
		BREQ _LIGHT_TURN_OFF

		CPI AUX,BT_COMMAND_DISCONNECT
		BREQ _BT_DISCONNECT

SIGO:  	RETI 

CALL_PROJECT_NAME:
		LDIW	Z,(MSJ_PROJECT_NAME*2)
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
_LIGHT_TURN_ON:
		RCALL LIGHT_TURN_ON
		RJMP SIGO
_LIGHT_TURN_OFF:
		RCALL LIGHT_TURN_OFF
		RJMP SIGO
_BT_DISCONNECT:
		RCALL BT_DISCONNECT
		RJMP SIGO
BT_DISCONNECT:
		CLR AUX
		OUTPUT BT_FLAG,AUX
RET

;------------------------------------------------------------------------
; TRANSMISION: interrumpe cada vez que puede transmitir un byte.
; Se transmiten "BYTES_A_TX" comenzando desde la posici�n TX_BUF del
; buffer. Si "BYTES_A_TX" llega a cero, se deshabilita la interrupci�n.
;
; Recibe: 	BYTES_A_TX.
; Devuelve: PTR_TX_H:PTR_TX_L, y BYTES_A_TX.
;------------------------------------------------------------------------
ISR_REG_USART_VACIO:		; UDR est� vac�o
		PUSH	AUX
		PUSH	AUX1
		PUSHI	SREG
		PUSHW	X


		TST		BYTES_A_TX	; hay datos pendientes de transmisi�n?
		BREQ	FIN_TRANSMISION

		MOVW	XL,PTR_TX_L	; Recupera puntero al pr�ximo byte a tx.
		LD		AUX,X+		; lee byte del buffer y apunta al
		OUTPUT		UDR0,AUX		; sgte. dato a transmitir (en la pr�xima int.)

		CPI		XL,LOW(TX_BUF+BUF_SIZE)
		BRLO	SALVA_PTR_TX
		CPI		XH,HIGH(TX_BUF+BUF_SIZE)
		BRLO	SALVA_PTR_TX
		LDIW	X,TX_BUF	; ptr_tx=ptr_tx+1, (m�dulo BUF_SIZE)

SALVA_PTR_TX:
		MOVW	PTR_TX_L,XL	; preserva puntero a sgte. dato

		DEC		BYTES_A_TX	; Descuenta el nro. de bytes a tx. en 1
		BRNE	SIGUE_TX	; si quedan datos que transmitir
							;	vuelve en la pr�xima int.
;REVISAR ESTE GRUPO DE INSTRUCCIONES
FIN_TRANSMISION:			; si no hay nada que enviar,
		INPUT	AUX,UCSR0B
		CBR		AUX,(1<<UDRIE0)
		OUTPUT	UCSR0B,AUX
		;se deshabilita la interrupci�n.

sigue_tx:
		POPW	X
		POPI	SREG
		POP		AUX1
		POP		AUX
		RETI

;-------------------------------------------------------------------------
; TRANSMITIR_MENSAJE: transmite el mensaje almacenado en memoria flash a partir
; de la direcci�n �APUNTADA POR Z! que termina con 0x00 (el 0 no se transmite).
; Recibe: nada
; Devuelve: PTR_TX_L|H, BYTES_A_TX.  
; Habilita la int. de transmisi�n serie con ISR en ISR_REG_USART_VACIO().
;-------------------------------------------------------------------------
TRANSMITIR_MENSAJE:
		PUSHW	Z
		PUSHW	X
		PUSH	AUX

;		LDIW	Z,(MSJ_V_BAT*2)
		MOVW	XL,PTR_TX_L

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
		LDIW	X,TX_BUF	; ptr_tx++ m�dulo BUF_SIZE

		RJMP	LOOP_TRANSMITIR_MENSAJE
;REVISAR INSTRUCCIONES
	
FIN_TRANSMITIR_MENSAJE:
		INPUT	AUX,UCSR0B

		SBR		AUX,(1<<UDRIE0)
		OUTPUT	UCSR0B,AUX

		POP		AUX
		POPW	X
		POPW	Z
		RET

;-------------------------------------------------------------------------
; fin del c�digo
;-------------------------------------------------------------------------