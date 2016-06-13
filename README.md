# Solar Tracker
COMENTARIOS [MAU]:

11/06-12/06 FIN DE SEMANA RE-LOCO DE PRUEBAS.

Tener en cuenta que si el ADC del panel es 0x000 => NO HAY PANEL SOLAR CONECTADO.

Para configurar el oscilador externo a 8MHz:

  avrdude -c usbtiny -p m88 -U lfuse:r:-:i -F		
  avrdude -c usbtiny -p m88 -U lfuse:w:0xE2:m -F
  
OJO QUE PARA HACER PUSHI Y POPI DE SREG SIEMPRE HAY QUE HACER PUSH Y POP DE ¡¡¡¡AUX!!!!

HAY QUE HACER QUE CUANDO SE VAYA A DORMIR, SE GIRE A LA POSICION INICIAL!

Hacer que se pueda mover manual [calibracion]

# Hacer funciones de conversión del adc al valor de tension del panel y bat

# Hacer que se pueda resetear por bt
# Configurar la int_ext 
