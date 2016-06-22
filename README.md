# Solar Tracker

# ¡GRACIAS A LOS QUE HICIERON POSIBLE ESTE PROYECTO!

Mejoras: tomar promedio en los ldr's para luego mover.

Para configurar el oscilador externo a 8MHz:

  avrdude -c usbtiny -p m88 -U lfuse:r:-:i -F		
  avrdude -c usbtiny -p m88 -U lfuse:w:0xE2:m -F
