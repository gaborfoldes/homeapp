#!/bin/bash

if [ "$#" -ne 1 ]; then
  echo "Upper bound in seconds to wait before switching the light?"
  exit
fi

onOrOff="on"

while true; do
  (( number = ( $RANDOM % $1 ) + 1 ))
  echo "Switching light $onOrOff"
  ./light "$onOrOff"
  if [[ "$onOrOff" == "on" ]]; then
    onOrOff="off"
  else
    onOrOff="on"
  fi
  echo "Waiting for $number seconds before turning light $onOrOff."
  sleep $number
done
