

def test_func():
    return "test-result"

def test2(a):
    return 2*a

class TestClass():
    m1 = 0
    m2 = 0

def bench():
    return 1+1+2*4+1+1+2*4

def make_list():
    return list(range(1024))

def list_range():
    return range(1024)

def the_len(s):
    return len(s)

#from numba import jit
#@jit(nopython=False)
#def bench_jit():
#    return 1+1+2*4+1+1+2*4
#
#def bench2():
#    bench_jit()

