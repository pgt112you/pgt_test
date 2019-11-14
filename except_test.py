for i in range(0, 10):
    try:
        if i == 5:
            raise Exception("hello world")
        a = "aaa"
    except Exception, e:
        print ">>>>>>", e
        a = "aaaa"
        continue
    finally:
        print i
        print a
