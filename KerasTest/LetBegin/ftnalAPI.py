# doc from: https://keras.io/guides/functional_api/
# Developer guides/ The Functional API
# Introduction: keras ftnal api to create models flexible than tf.keras.sequential
# the api handle models w. non-linear topology, shared layers, and multiple inputs or outputs

# consider model following:
# (input: 784-dimensional vectors)
# [Dense (64 units, relu activation)]
# [Dense (64 units, relu activation)]
# [Dense (10 units, softmax activation)]
# (output: logits of a probability distribution over 10 classes)

import numpy as np
import keras as kr
#from keras.layers import *

# shape of data as 784-dim. vector
inputs = kr.Input(shape=(784,))

# nodes of layers by calling a layer on this inputs obj.
dense = kr.layers.Dense(64, activation='relu')
# layer call is like drawing an arrow from inputs to this layer created, passing inputs to dense layer and out you get x
x = dense(inputs)
x = kr.layers.Dense(64, activation='relu')(x)
outputs = kr.layers.Dense(10)(x)

# create a Model
model = kr.Model(inputs=inputs, outputs=outputs, name="mnist_model")
model.summary()

# You must install pydot and graphviz for `pydotprint` to work
#kr.utils.plot_model(model, "my_first_model.png", show_shapes=True)

# load MNIST image data, reshape into vectors, fit model on data, then evaluate the model on test data

(x_train, y_train), (x_test, y_test) = kr.datasets.mnist.load_data()

x_train = x_train.reshape(60000, 784).astype("float32")/255
x_test  = x_test.reshape(10000, 784).astype("float32")/255

model.compile(
    loss='sparse_categorical_crossentropy',
    optimizer=kr.optimizers.RMSprop(),
    metrics=["accuracy"],
    )

history = model.fit(x_train, y_train, batch_size=64, epochs=2, validation_split=0.2)

test_scores = model.evaluate(x_test, y_test, verbose=2)
print("Test loss:", test_scores[0])
print("Test accuracy:", test_scores[1])

# saved file includs: model architecture, weight values(learned  during training), model training config, if any to restart  training where you left off

model.save("myTestModel.h2")
del model

# Recreate the exact same model purely from the file:
model = kr.models.load_model("myTestModel.h2")

# Use the same graph of layers to define mutiple models
# a single graph of layers can be used to generate multiple models
# Next: the same stack of layers to instantiate two models

encoder_input = kr.Input(shape=(28,28,1), name="img")
x = kr.layers.Conv2D(16, 3, activation="relu")(encoder_input)
x = kr.layers.Conv2D(32, 3, activation="relu")(x)
x = kr.layers.MaxPooling2D(3)(x)
x = kr.layers.Conv2D(32, 3, activation="relu")(x)
x = kr.layers.Conv2D(16, 3, activation="relu")(x)
encoder_output = kr.layers.GlobalMaxPooling2D()(x)

# layers after encoder_input used for this model
encoder = kr.Model(encoder_input, encoder_output, name="encoder")
encoder.summary()

x = kr.layers.Reshape((4, 4, 1))(encoder_output)
x = kr.layers.Conv2DTranspose(16, 3, activation="relu")(x)
x = kr.layers.Conv2DTranspose(32, 3, activation="relu")(x)
x = kr.layers.UpSampling2D(3)(x)
x = kr.layers.Conv2DTranspose(16, 3, activation="relu")(x)
decoder_output = kr.layers.Conv2DTranspose(1, 3, activation="relu")(x)

# layers after encoder_input used for this model again
autoencoder = kr.Model(encoder_input, decoder_output, name="autoencoder")
autoencoder.summary()

# All Models are callable
# as if it were a layer by invoking it on an Input or on the output of another layer
# not just reusing the architecture of the model, also reusing its weights

decoder_input = kr.Input(shape=(16,), name="encoded_img")
x = kr.layers.Reshape((4, 4, 1))(decoder_input)
x = kr.layers.Conv2DTranspose(16, 3, activation="relu")(x)
x = kr.layers.Conv2DTranspose(32, 3, activation="relu")(x)
x = kr.layers.UpSampling2D(3)(x)
x = kr.layers.Conv2DTranspose(16, 3, activation="relu")(x)
decoder_output = kr.layers.Conv2DTranspose(1, 3, activation="relu")(x)

decoder = kr.Model(decoder_input, decoder_output, name="decoder")
decoder.summary()

autoencoder_input = kr.Input(shape=(28, 28, 1), name="ing")
encoded_img = encoder(autoencoder_input)
decoded_img = decoder(encoded_img)
autoencoder = kr.Model(autoencoder_input, decoded_img, name="autoencoder")
autoencoder.summary()


