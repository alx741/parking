#!/bin/bash

#se asume que comienza desde el puntoInicial y cambia a 1 cuando este en el punto final
SECCION=0

Flag=0

function funky
{
    echo "########   funky"
    sleep 1
    killall cheese
    sleep 1
    cheese -f -w &> /dev/null &
    tmp_section=$SECCION
    current_section=$tmp_section
    while true
    do
        finish_state=$(curl -s 'http://104.42.176.241:3002/ready')
        if [[ "$finish_state" == "true" ]];
        then
            killall cheese

            if [[ $current_section -eq 0 ]] && [[ $tmp_section -eq 0 ]];
            then
                touch ./lock
                sudo ./driver I
                rm ./lock
                SECCION=1
                sleep 3
            elif [[ $current_section -eq 1 ]] && [[ $tmp_section -eq 1 ]];
            then
                current_section=0
                touch ./lock
                sudo ./driver D
                rm ./lock
                SECCION=0
                sleep 3
            fi

            sleep 2
            echo "########   done funky"
            return
        fi

        if [[ $current_section -eq 0 ]];
        then
            current_section=1
            touch ./lock
            sudo ./driver I
            rm ./lock
            sleep 20
        else
            current_section=0
            touch ./lock
            sudo ./driver D
            rm ./lock
            sleep 20
        fi


    done
}

while [ $Flag = 0 ]
do
    if [ $SECCION = 0 ]
    then
        echo "seccion 1"
        sleep .5
        fswebcam -S 20 -q -r 640x480 --jpeg 85 --no-banner ./puntoInicio.jpg
        scp ./puntoInicio.jpg boro101094@191.232.235.107:/home/boro101094/casaAbierta/procesos/fotosInicio
        curl -s 'http://104.42.176.241:3002/working'
        ssh -p 22 boro101094@191.232.235.107 "/home/boro101094/casaAbierta/procesos/reconocer.sh puntoInicio.jpg $SECCION;"&
        funky
        echo "cambio"
        SECCION=1

    else
        echo "seccion 2"
        sleep .5
        fswebcam -S 20 -q -r 640x480 --jpeg 85 --no-banner ./puntoFinal.jpg
        scp ./puntoFinal.jpg boro101094@191.232.235.107:/home/boro101094/casaAbierta/procesos/fotosFinal
        curl -s 'http://104.42.176.241:3002/working'
        ssh -p 22 boro101094@191.232.235.107 "/home/boro101094/casaAbierta/procesos/reconocer.sh puntoFinal.jpg $SECCION;"&
        funky
        echo "cambio"
        SECCION=0
    fi
done
exit 0
