#coding:utf-8

############### 复杂对象，类或者字典，list什么的，作为函数的参数都是浅拷贝，相当于是传的指针#############
class A(object):
    def __init__(self):
        self.a = 5
        self.b = 6

def fun(Aobj):
    Aobj.a = 10


aobj = A()
print aobj.a
fun(aobj)
print aobj.a

##############################################################################
