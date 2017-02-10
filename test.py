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

l = [1,2,3,4,5,6]
def fileter_l(l):
    for i in range(0, len(l)):
        if l[i]%2 == 0:
           l[i] += 100

fileter_l(l)
print l
            
