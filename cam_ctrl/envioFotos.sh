#!/bin/bash

#se asume que comienza desde el puntoInicial y cambia a 1 cuando este en el punto final
SECCION=0

Flag=0

while [ $Flag = 0 ]
do
    if [ $SECCION = 0 ]
    then
        echo "seccion 1"
        sleep .5
        #capturar la imagen de la webcam aqui:
        fswebcam -S 20 -q -r 640x480 --jpeg 85 --no-banner /home/boris/Desktop/casaAbierta/fotosInicio/puntoInicio.jpg
        sudo ./driver I
        scp /home/boris/Desktop/casaAbierta/fotosInicio/puntoInicio.jpg boro101094@191.232.235.107:/home/boro101094/casaAbierta/procesos/fotosInicio

        ssh -p 22 boro101094@191.232.235.107 "/home/boro101094/casaAbierta/procesos/reconocer.sh puntoInicio.jpg $SECCION;"
        echo "cambio"
        SECCION=1

    else
        echo "seccion 2"
        sleep .5
        fswebcam -S 20 -q -r 640x480 --jpeg 85 --no-banner /home/boris/Desktop/casaAbierta/fotosFinal/puntoFinal.jpg
        sudo ./driver D
        scp /home/boris/Desktop/casaAbierta/fotosFinal/puntoFinal.jpg boro101094@191.232.235.107:/home/boro101094/casaAbierta/procesos/fotosFinal
        ssh -p 22 boro101094@191.232.235.107 "/home/boro101094/casaAbierta/procesos/reconocer.sh puntoFinal.jpg $SECCION;"
        echo "cambio"
        SECCION=0
    fi
done
exit 0
