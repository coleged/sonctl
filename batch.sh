#!/bin/bash
echo "******************************" >> /tmp/sonos.log 2>&1
date >> /tmp/sonos.log 2>&1
cat << EOF | /usr/local/bin/sonctl -i >> /tmp/sonos.log 2>&1
disconnect Ed's Office
disconnect Workshop1
disconnect Workshop2
disconnect Barn
connect Ed's Office
add Workshop1
add Workshop2
add Barn
playfv x-sonosapi-radio:catalog%2fstations%2fA7KDSQK34D7FV%2f%23chunk-_gEd2coZRwGNGGCNEiB0eg?sid=201&flags=8300&sn=4
volume Ed's Office 40
volume Workshop1 60
volume Workshop2 60
exit
EOF
echo "" >> /tmp/sonos.log 2>&1
echo "" | mail -s "Mott Radio on Sonos" colege@gmail.com
