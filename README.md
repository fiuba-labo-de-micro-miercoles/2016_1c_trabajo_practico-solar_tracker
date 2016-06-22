# Solar Tracker

# ¡GRACIAS A LOS QUE HICIERON POSIBLE ESTE PROYECTO!

¡IMPORTANTE!: CHEQUEAR QUE HAGA BIEN EL PROMEDIO DE LAS MUESTRAS EN LA FUNCION ORIENTATE_SOLAR_PANEL

Para configurar el oscilador externo a 8MHz:

  avrdude -c usbtiny -p m88 -U lfuse:r:-:i -F		
  avrdude -c usbtiny -p m88 -U lfuse:w:0xE2:m -F
