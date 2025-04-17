#!/bin/bash
#kitöröl minden üzenetsort
#csak akkor használjuk, ha ellenőriztük, hogy csak a saját soraink léteznek
ipcs -q| tail -n +4| cut -f2 -d " " | while read queue
do
    if [ -n "$queue" ];then
        if ipcrm -q $queue ;then
            echo $queue sor törölve
        else
            echo $queue törlése nem sikerült
        fi
    fi
done
