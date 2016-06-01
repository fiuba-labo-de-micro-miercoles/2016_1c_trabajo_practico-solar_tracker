# Solar Tracker
  COMENTARIOS [MAU]:
  Poner la luz en un pin pwm [no puede ser mot1 ni mot2, puede ser bot2 o mosi].
  Tener en cuenta que si el ADC de la bateria es 0x000 => NO HAY BATERIA CONECTADA. [IDEM PANEL SOLAR].
  PARA CONFIGURAR EL OSCILADOR EXTERNO A 8MHZ.
  avrdude -c usbtiny -p m328p -U lfuse:r:-:i -F		
  avrdude -c usbtiny -p m328p -U lfuse:w:0xE2:m -F
  
PREGUNTAR SI HACER FUNCIONES COMO READ_BATTERY O ELEGIR EL CANAL, LLAMAR CONVERSION, ETC ETC
OJO QUE PARA HACER PUSHI Y POPI DE SREG SIEMPRE HAY QUE HACER PUSH Y POP DE ¡¡¡¡AUX!!!!
