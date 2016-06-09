# Solar Tracker
COMENTARIOS [MAU]:

10/06 SE MONTA EL PANEL.

11/06-12/06 FIN DE SEMANA RE-LOCO DE PRUEBAS.

FALTA TERMINAR UN EJE DE LA MECANICA.

Tener en cuenta que si el ADC de la bateria es 0x000 => NO HAY BATERIA CONECTADA. [IDEM PANEL SOLAR].

Para configurar el oscilador externo a 8MHz:

  avrdude -c usbtiny -p m88p -U lfuse:r:-:i -F		
  avrdude -c usbtiny -p m888p -U lfuse:w:0xE2:m -F
  
OJO QUE PARA HACER PUSHI Y POPI DE SREG SIEMPRE HAY QUE HACER PUSH Y POP DE ¡¡¡¡AUX!!!!

HAY QUE HACER QUE CUANDO SE VAYA A DORMIR, SE GIRE A LA POSICION INICIAL!

Hacer que se pueda mover manual 
