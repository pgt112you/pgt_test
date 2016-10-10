#!/bin/bash
g++ -g -lpthread -lcrypto sync_file.cpp tool.cpp -I/usr/local/include -o test
