#!/bin/bash

for i in ./programs/*
do
   echo "Compiling " $i 
   ./compiler $i
done
