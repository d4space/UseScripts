import keras as kr

(X_train, y_train), (X_test, y_test) = kr.datasets.mnist.load_data()
print X_train.shape
