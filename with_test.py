#coding:utf-8


with open("with_test.dat", "r") as infile:
    content = infile.readline()
    while(content):
        print content
        content = infile.readline()
