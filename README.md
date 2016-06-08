# Solar Tracker
COMENTARIOS [MAU]:

  08/06 SE IMPRIMEN LOS ENGRANAJES.

  10/06 SE MONTA EL PANEL.

  11/06-12/06 FIN DE SEMANA RE-LOCO DE PRUEBAS.



  
FALTA TERMINAR UN EJE DE LA MECANICA.

FALTA TERMINAR PWM.inc

Poner la luz en un pin pwm [no puede ser mot1 ni mot2, puede ser bot2 o mosi].

Tener en cuenta que si el ADC de la bateria es 0x000 => NO HAY BATERIA CONECTADA. [IDEM PANEL SOLAR].

Para configurar el oscilador externo a 8MHz:

  avrdude -c usbtiny -p m328p -U lfuse:r:-:i -F		
  avrdude -c usbtiny -p m328p -U lfuse:w:0xE2:m -F
  
OJO QUE PARA HACER PUSHI Y POPI DE SREG SIEMPRE HAY QUE HACER PUSH Y POP DE ¡¡¡¡AUX!!!!

HAY QUE HACER QUE CUANDO SE VAYA A DORMIR, SE GIRE A LA POSICION INICIAL!

PREGUNTAS: VER ARCHIVO LDRS.inc
