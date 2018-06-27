#!/bin/sh

fswebcam -S 20 -q -r 640x480 --jpeg 85 --no-banner ./face.jpg

result=$(python detect.py)

if [[ "$result" == "[]" ]];
then
    echo "no face"
    ./control_client revocar
else
    echo "face! yay!"
    ./control_client autorizar
    sleep 5
    ./control_client revocar
fi
