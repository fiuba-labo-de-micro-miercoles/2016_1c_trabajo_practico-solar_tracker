;------------------------------------------------------------------
; colecci�n de macros para microcontroladores AVR
;------------------------------------------------------------------
; Sintaxis:
; 	.macro NOMBRE_MACRO
; 		; cuerpo de la macro
;		; los par�metros de la macro se referencian como
;		; @0 (1er par�metro), @1 (2do par�metro), etc.
; 	.endm
;------------------------------------------------------------------

;------------------------------------------------------------------
; input: resuelve si usa "in"/"lds" seg�n la direcci�n del registro
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
; output: resuelve si usa "out"/"sts" seg�n la direcci�n del registro
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
		ldi		AUX4,@1	; Usa: variable auxiliar "t0" y un puntero
		st		@0,AUX4	; [3 ciclos, 2 words] @0={X, Y, Z}
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

.macro VECTMEAN	;@0,@1,@2	;Calcula la media de un vector @0 de longitud @1 y guarda la media en @2
		PUSH		@1
		LD 		AUX3,@0+
		CLR		@2
		DEC		@1
		CLR		AUX2
loop_mean:
		CLV
		LD		AUX4,@0+
		ADD		AUX3,AUX4
		ADC		@2,AUX2
		DEC		@1
		BRNE		loop_mean
		LDI		AUX2,3
division:
		LSR		@2
		ROR		AUX3
		DEC		AUX2
		BRNE		division
		MOV		@2,AUX3
		POP		@1
.endm


.macro SPWM 
;PROTOTYPE SET_PWM: SPWM OCRnx,PWM_DATA
;RECIBE: OCRnx,PWM_DATA
;DEVUELVE: .
	OUTPUT @0,@1
.endm

.macro RPWM 
;PROTOTYPE RESET_PWM: RPWM OCRnx
;RECIBE: OCRnx
;DEVUELVE: -
	CLR AUX
	OUTPUT @0,AUX
.endm

.macro ADDI
;PROTOTYPE ADDI: REG,CTE
;RECIBE: REG,CTE
;DEVUELVE: Suma de cte al registro
	LDI AUX4,@1
	ADD @0,AUX4
.endm

.macro ADDP
;PROTOTYPE ADDP: POINTER,REG
;RECIBE: POINTER,REG
;DEVUELVE: Pointer en posici�n inicial + AUX
	ADD @0L,@1
	BRVC NO_POINTER_OV
	INC @0H
	SUB @1,@0L
	MOV @1,@0L
NO_POINTER_OV: NOP
.endm
;-----------------------------------------------------------------------------------