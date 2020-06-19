# https://docs.scipy.org/doc/numpy-1.15.1/reference/generated/numpy.random.randint.html
# numpy.random.randint(low, high=None, size=None, dtype='l')
# low (inclusive) to high (exclusive)
#If high is None (the default), then results are from [0, low).

import numpy
for i in range(0,100):
    print i, numpy.random.randint(100)

#                                     ensemble, raw, column
myarray = numpy.random.randint(2, size=(2,3,4))
print myarray
