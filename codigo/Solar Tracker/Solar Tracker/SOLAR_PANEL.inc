;-------------------------------------------------------------------------
;
;							SOLAR_PANEL.inc
;
;-------------------------------------------------------------------------
.CSEG
READ_V_SOLAR_PANEL:
;RECIBE: NADA
;DEVUELVE: TENSION DEL PANEL EN ADC_DATA_H
	LDI ADC_DATA_L,ADC_SOLAR_PANEL			;ELIJO EL PIN DEL PANEL SOLAR
	RCALL ADC_SELECT_INPUT					;LLAMO LA FUNCION PARA SELECCIONAR EL PANEL SOLAR
	RCALL ADC_SIMPLE_CONVERSION				;LLAMO LA FUNCION PARA MEDIR
RET

CHECK_IF_SOLAR_PANEL_CONNECTED:

	SEC
	TST ADC_DATA_H
	BREQ HAY_PANEL_CONECTADO
	CLC
HAY_PANEL_CONECTADO:
RET

CHECK_IF_SOLAR_PANEL_MINIMUM:
;RECIBE: TENSION DEL PANEL EN ADC_DATA_H
;DEVUELVE: [CARRY=1]: SOLAR_PANEL LOW. [CARRY=0]: SOLAR_PANEL OK
	CLC
	CPI ADC_DATA_H,MIN_SOLAR_PANEL_VALUE	;COMPARAR PARA VER SI ES DE DIA O NOCHE
	BRCS _INDICATE_SOLAR_PANEL_LOW			;[CARRY=1]: SOLAR_PANEL LOW. [CARRY=0]: SOLAR_PANEL OK
	RCALL INDICATE_SOLAR_PANEL_OK

RETURN_INDICATE_SOLAR_PANEL_LOW:
RET

INDICATE_SOLAR_PANEL_OK:
	INPUT	AUX,PORTC
	ANDI	AUX,(~((1<<PIN_SOLAR_PANEL_LED_OK)|(1<<PIN_SOLAR_PANEL_LED_LOW)))
	ORI		AUX,((0<<PIN_SOLAR_PANEL_LED_OK)|(1<<PIN_SOLAR_PANEL_LED_LOW))		;PRENDE POR CERO.
	OUTPUT	PORTC,AUX
	SEC										;[CARRY=1]: ES DE DIA. [CARRY=0]: ES DE NOCHE.
RET

_INDICATE_SOLAR_PANEL_LOW:
	RCALL INDICATE_SOLAR_PANEL_LOW
	RJMP RETURN_INDICATE_SOLAR_PANEL_LOW
INDICATE_SOLAR_PANEL_LOW:
	INPUT	AUX,PORTC
	ANDI	AUX,(~((1<<PIN_SOLAR_PANEL_LED_OK)|(1<<PIN_SOLAR_PANEL_LED_LOW)))
	ORI		AUX,((1<<PIN_SOLAR_PANEL_LED_OK)|(0<<PIN_SOLAR_PANEL_LED_LOW))		;PRENDE POR CERO.
	OUTPUT	PORTC,AUX
	CLC										;[CARRY=1]: ES DE DIA. [CARRY=0]: ES DE NOCHE.
RET

ORIENTATE_SOLAR_PANEL:
;YA ESTAN LOS PROMEDIOS DE LOS LDR Y HAY QUE COMPARAR Y MOVER EL PANEL.
;PRIMERO EN ASIMUT, LUEGO EN ELEVACION.
	CLI	;DESHABILITO LAS INTERRUPCIONES GLOBALES

	;COMPARAR_NO_NE:
		INPUT AUX,LDR_NO_MEAN
		INPUT AUX1,LDR_NE_MEAN
		CP AUX,AUX1
;MEJORAR: VER LA RESTA Y MOVER EN FUNCION DE ESO
		BRLO _MOTOR_AZIMUT_LEFT
	RETURN_MOTOR_AZIMUT_LEFT:

		INPUT AUX,LDR_NO_MEAN
		INPUT AUX1,LDR_NE_MEAN
		CP AUX1,AUX
		BRLO _MOTOR_AZIMUT_RIGHT
	RETURN_MOTOR_AZIMUT_RIGHT:

		INPUT AUX,LDR_NO_MEAN
		INPUT AUX1,LDR_NE_MEAN
		CP AUX,AUX1
		BREQ _MOTOR_AZIMUT_OFF
	RETURN_MOTOR_AZIMUT_OFF:

	;COMPARAR_NO_SO:
		INPUT AUX,LDR_NO_MEAN
		INPUT AUX1,LDR_SO_MEAN
		CP AUX,AUX1
;MEJORAR: VER LA RESTA Y MOVER EN FUNCION DE ESO
		BRLO _MOTOR_ELEVATION_UP
	RETURN_MOTOR_ELEVATION_UP:

		INPUT AUX,LDR_NO_MEAN
		INPUT AUX1,LDR_SO_MEAN
		CP AUX1,AUX
		BRLO _MOTOR_ELEVATION_DOWN
	RETURN_MOTOR_ELEVATION_DOWN:

		INPUT AUX,LDR_NO_MEAN
		INPUT AUX1,LDR_SO_MEAN
		CP AUX,AUX1
		BREQ _MOTOR_ELEVATION_OFF
	RETURN_MOTOR_ELEVATION_OFF:

	SEI	;HABILITO LAS INTERRUPCIONES GLOBALES
RET

_MOTOR_AZIMUT_LEFT:
RCALL MOTOR_AZIMUT_LEFT
RJMP RETURN_MOTOR_AZIMUT_LEFT

_MOTOR_AZIMUT_RIGHT:
RCALL MOTOR_AZIMUT_RIGHT
RJMP RETURN_MOTOR_AZIMUT_RIGHT

_MOTOR_AZIMUT_OFF:
RCALL MOTOR_AZIMUT_OFF
RJMP RETURN_MOTOR_AZIMUT_OFF

_MOTOR_ELEVATION_UP:
RCALL MOTOR_ELEVATION_UP
RJMP RETURN_MOTOR_ELEVATION_UP

_MOTOR_ELEVATION_DOWN:
RCALL MOTOR_ELEVATION_DOWN
RJMP RETURN_MOTOR_ELEVATION_DOWN

_MOTOR_ELEVATION_OFF:
RCALL MOTOR_ELEVATION_OFF
RJMP RETURN_MOTOR_ELEVATION_OFF
