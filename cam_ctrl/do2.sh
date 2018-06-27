#!/bin/sh

sudo ./driver r

while true
do
    while [ -f ./lock ]
    do
        echo "lock"
    done

    result_driver=$(sudo ./driver s)
    result_ws=$(./face_client)
    if [[ "$result_driver" == "true" ]];
    then
        if [[ "$result_ws" == "true" ]];
        then
            sudo ./driver F
        else
            sleep 3
            sudo ./driver f
        fi
    else
        sleep 3
        sudo ./driver f
    fi

done
