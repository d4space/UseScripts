# using: CMSSW_10_2_15_patch2/external/slc7_amd64_gcc700/bin/root

#import tensorflow as tf
#from tensorflow import keras
#from tensorflow.keras import layers
from keras.models import *
from keras.layers import *
from keras.datasets import *

inputs = Input(shape=(784,), name="digits")
x       = Dense(64, activation="relu", name="dense_1")(inputs)
x       = Dense(64, activation="relu", name="dense_2")(x)
outputs = Dense(10, activation="softmax", name="predictions")(x)

model = Model(inputs=inputs, outputs=outputs)

(x_train, y_train), (x_test, y_test) = mnist.load_data()
#(x_train, y_train), (x_test, y_test) = keras.datasets.mnist.load_data()

## Preprocess the data (these are NumPy arrays)
x_train = x_train.reshape(60000, 784).astype("float32")/255
x_test  = x_test.reshape(10000, 784).astype("float32")/255

y_train = y_train.astype("float32")
y_test  = y_test.astype("float32")
#
## Reserve 10,000 samples for validation
#
x_val = x_train[-10000:]
y_val = y_train[-10000:]
x_train = x_train[:-10000]
y_train = y_train[:-10000]

#optimizer='RMSprop',
model.compile(
    optimizer='adam',
    loss = 'sparse_categorical_crossentropy',
    metrics=['sparse_categorical_accuracy'],
    )

print ("Fit model on training data")
#"We call fit(), which will train the model by slicing the data into batches of size batch_size, and repeatedly iterating over the entire dataset for a given number of epochs."

history = model.fit(
    x_train,
    y_train,
    batch_size=64,
    epochs=2,
    # We pass some validation for
    # monitoring validation loss and metrics
    # at the end of each epoch
    validation_data=(x_val, y_val),
    )
