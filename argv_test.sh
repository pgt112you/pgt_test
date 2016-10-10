#!/bin/bash

while getopts "n" arg 
do
    case $arg in
         n)
            echo -n "centos-0.60-1"
            exit 1
            ;;
         ?)  
         exit 1
         ;;
    esac
done

echo 'hello world'
