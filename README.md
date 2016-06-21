# Solar Tracker
COMENTARIOS [MAU]:

# CHEQUEAR QUE ANDE LA CALIBRACION, LA LUZ Y EL TIEMPO EN ROTAR CUANDO SE VAYA A DORMIR

Para configurar el oscilador externo a 8MHz:

  avrdude -c usbtiny -p m88 -U lfuse:r:-:i -F		
  avrdude -c usbtiny -p m88 -U lfuse:w:0xE2:m -F
  
## ¡GRACIAS A LOS QUE HICIERON POSIBLE ESTE PROYECTO!
