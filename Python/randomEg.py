
import numpy

# see: https://numpy.org/doc/1.16/reference/generated/numpy.random.RandomState.choice.html

sig_m = [100, 200]
sig_portion = [1/3., 2/3.]

rs = numpy.random.RandomState(123)
total = 0
for i in range(300):
    idx = rs.choice(sig_m,1, p=sig_portion)
    #idx = rs.choice(len(sig_m),1, p=sig_portion)
    total += idx[0]
    print i, idx[0]

print 'total', total ,'check', 100*300/3 + 200*300*2/3
