

def changed(d):
    d["b"] += 1


d={"a":3, "b":4, "c":5}
for i in range(0, 10):
    changed(d)
    print d
