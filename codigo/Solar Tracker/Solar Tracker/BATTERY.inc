;-------------------------------------------------------------------------
;
;							BATTERY.inc
;
;-------------------------------------------------------------------------
.CSEG
READ_V_BATTERY:
		LDI ADC_DATA_L,ADC_BATTERY			;ELIJO EL PIN DE LA BATER�A
		RCALL ADC_SELECT_INPUT				;LLAMO LA FUNCION PARA SELECCIONAR LA BATERIA
		RCALL ADC_SIMPLE_CONVERSION			;LLAMO LA FUNCION PARA MEDIR
RET

CHECK_IF_BATTERY_MINIMUM:
	CLC
	CPI		ADC_DATA_H,MIN_BATTERY_VALUE	;COMPARAR PARA VER SI HAY SUFICIENTE BATERIA PARA OPERAR
	BRCC	BAT_OK							;[CARRY=1]: BATTERY LOW. [CARRY=0]: BATTERY OK
	RCALL	INDICATE_BATTERY_LOW
BAT_OK:
RET

INDICATE_BATTERY_LOW:
	INPUT	AUX,PORTD
	ANDI	AUX,(~((1<<PIN_BATTERY_LED_OK)|(1<<PIN_BATTERY_LED_LOW)))
	ORI		AUX,((1<<PIN_BATTERY_LED_OK)|(0<<PIN_BATTERY_LED_LOW))		;PRENDE POR CERO.
	OUTPUT	PORTD,AUX
	SEC										;[CARRY=1]: BATTERY LOW. [CARRY=0]: BATTERY OK
RET

INDICATE_BATTERY_OK:
	INPUT	AUX,PORTD
	ANDI	AUX,(~((1<<PIN_BATTERY_LED_OK)|(1<<PIN_BATTERY_LED_LOW)))
	ORI		AUX,((0<<PIN_BATTERY_LED_OK)|(1<<PIN_BATTERY_LED_LOW))		;PRENDE POR CERO.
	OUTPUT	PORTD,AUX
	CLC										;[CARRY=1]: BATTERY LOW. [CARRY=0]: BATTERY OK
RET