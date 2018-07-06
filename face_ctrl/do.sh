#!/bin/sh

killall cheese
sleep 1
fswebcam -S 20 -q -r 640x480 --jpeg 85 --no-banner ./face.jpg
cheese -w -f&

result=$(python detect.py)

if [[ "$result" == "[]" ]];
then
    echo "no face"
    ./control_client revocar
else
    echo "face! yay!"
    ./control_client autorizar
    sleep 10
    ./control_client revocar
fi
