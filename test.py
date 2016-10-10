#coding:utf-8
def testb():
    a = testa()
    print a
    print d['b']

def testa():
    return 3

d = {'a':3,
     'b':4}

testb()
