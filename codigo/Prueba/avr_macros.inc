;------------------------------------------------------------------
; colección de macros para microcontroladores AVR
;------------------------------------------------------------------
; Sintaxis:
; 	.macro NOMBRE_MACRO
; 		; cuerpo de la macro
;		; los parámetros de la macro se referencian como
;		; @0 (1er parámetro), @1 (2do parámetro), etc.
; 	.endm
;------------------------------------------------------------------

;------------------------------------------------------------------
; input: resuelve si usa "in"/"lds" según la dirección del registro
;        de E/S que se lee.
;------------------------------------------------------------------
.macro	input ; @0= destino {r0, ... , r31}
              ; @1= fuente I/0 ($0000-$FFFF)
.if	@1<0x40
	in	@0,@1	; si dir del reg de E/S <0x40 uso "in"
.else
	lds	@0,@1	; sino uso "lds"
.endif
.endm

;------------------------------------------------------------------
; output: resuelve si usa "out"/"sts" según la dirección del registro
;         de E/S que se escribe.
;------------------------------------------------------------------
.macro	output		; @0= destino I/O ($0000-$FFFF)
			; @1= fuente, cte o r0..r31
.if	@0<0x40
		out	@0,@1	; si dir del reg de E/S <0x40 uso "out"
.else
		sts	@0,@1	; sino uso "sts"
.endif
.endm

.macro	ldiw		; carga puntero
		ldi	@0L, LOW(@1)
		ldi	@0H, HIGH(@1)
.endm

.macro	movi	; carga registro con constante
		ldi	AUX,@1
		mov	@0,AUX
.endm

.macro	outi
		ldi		AUX,@1
		output	@0,AUX  ;FIGURABA R16!!!!! OJO!
.endm


.macro	pushw	;@0		; Pone el puntero @0 de 16 bits en la pila
		push	@0L
		push	@0H
.endm


.macro	popw	;@0		; Saca el puntero @0 de 16 bits de la pila
		pop		@0H
		pop		@0L
.endm


.macro	pushi	;@0			; Pone en pila un registro de I/O
			in		AUX,@0	; usa la variable auxiliar t0
			push	AUX
.endm


.macro	popi	;@0			; Saca de pila un registro de I/O
			pop		AUX		; usa la variable auxiliar t0
			out		@0,AUX
.endm

.macro	sti		;@0,@1	; Guarda una constante de modo indirecto
		ldi		AUX,@1	; Usa: variable auxiliar "t0" y un puntero
		st		@0,AUX	; [3 ciclos, 2 words] @0={X, Y, Z}
.endm

.macro	stsi	;@0,@1	; Guarda una constante en SRAM
		ldi		AUX,@1	; Usa: variable auxiliar "t0".
		sts		@0,AUX	; [3 ciclos, 2 words] @0={any SRAM ADDRESS}
.endm

;---------------------MACROS PROPIAS-----------------------------------------------
.macro SLDR
;STORE_LDR
;PROTOTYPE: SLDR LDR_XX_LOW,LDR_XX_HIGH,ADC_DATA_H
;RECIBE: ADC_DATA_H EL VALOR DEL LDR
;DEVUELVE: -
	MOV	ZL,@0
	MOV	ZH,@1	
	ST	Z+,@2
	MOV	@0,ZL
	MOV	@1,ZH
.endm

.macro LLDR
;LOAD_LDR_TO_POINTER
;PROTOTYPE: LLDR Z,LDR_XX_LOW,LDR_XX_HIGH
	LDI @0L,@1
	LDI @0H,@2
.endm

.macro RLDR
;READ_LDR
;RECIBE: LDR_XX
;DEVUELVE: EL RESULTADO DE LA CONVERSION EN ADC_DATA_H
	LDI ADC_DATA_L,@0
	RCALL ADC_INPUT_SELECT
	RCALL ADC_SIMPLE_CONVERSION
.endm

.macro	VECTMEAN	;@0,@1,@2	;Calcula la media de un vector @0 de longitud @1 y guarda la media en @2
		push		@1
		ld 		AUX,@0+
		clr		@2
loop_mean:
		ld		AUX1,@0+
		add		AUX,AUX1
		clr		AUX2
		adc		@2,AUX2
		dec		@1
		brne		loop_mean
		ldi		AUX2,3
division:
		lsl		AUX
		rol		@2
		dec		AUX2
		brne		division
		pop		@1
.endm
;-----------------------------------------------------------------------------------