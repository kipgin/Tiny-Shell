#!/bin/bash

total_seconds=${1:-20}  

while [ $total_seconds -gt 0 ]; do
  minutes=$((total_seconds / 60))
  seconds=$((total_seconds % 60))


  printf "\r%02d:%02d" $minutes $seconds

  sleep 1
  total_seconds=$((total_seconds - 1))
done

printf "\rTime's up!            \n"
