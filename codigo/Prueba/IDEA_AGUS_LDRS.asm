

MOVING_AVERAGE: ;Subrutina que aplica el filtro moving average
			LDI COUNT,32 ;Seteo el contador de coeficientes
			;Seteo los punteros
			RCALL SET_POINTERS_MA

LOOP:		RCALL LDR_NO_READ	;Leo el LDR noroeste
			LD X+,ADC_DATA_L	;Mando la medición a RAM
			RCALL LDR_NE_READ	;Leo el LDR noreste
			LD Y+,ADC_DATA_L	;Mando la medición a RAM
			RCALL LDR_SO_READ	;Leo el LDR suroeste
			LD Z,ADC_DATA_L		;Mando la medición a RAM
			RCALL LDR_SE_READ	;Leo el LDR sureste
			STD Z+32,ADC_DATA_L	;Mando la medición a RAM
			INC R31				;Incremento el puntero Z
			BRVS 2				;Si hubo overflow, seteo el byte bajo de Z a 0 y la alta la incremento en 1
			LDI R31,0
			INC R30
			DEC COUNT			;Cuento a ver si ya tengo los 32 coeficientes
			BRNE SLEEP_MODE
			RCALL CALC_MEAN
			RJMP MOVING_AVERAGE

SET_POINTERS_MA:	;Seteo punteros con los que voy a escribir a RAM las mediciones
			LDI XL,LOW(MA_LDR_NO)
			LDI XH,HIGH(MA_LDR_NO)
			LDI YL,LOW(MA_LDR_NE)
			LDI YH,HIGH(MA_LDR_NE)
			LDI ZL,LOW(MA_LDR_SO)
			LDI ZH,HIGH(MA_LDR_SO)
			LDI AUX,LOW(MA_LDR_SE)
			MOV R2,AUX				;Uso a R2 como parte baja del puntero al vector de mediciones del LDR_SE
			LDI AUX,HIGH(MA_LDR_SE)
			MOV R3,AUX				;Uso a R3 como parte alta del puntero al vector de mediciones del LDR_SE
RET

CALC_MEAN:	;Calculo la media de las muestras que se tomaron para poder mover el panel acorde
		LDI COUNT,32
		RCALL SET_POINTERS_MA
		vectmean X,COUNT,DATA_NO
		vectmean Y,COUNT,DATA_NE
		vectmean Z,COUNT,DATA_SO
		MOV ZL,R2
		MOV ZH,R3
		vectmean Z,COUNT,DATA_SE
RET
