# doc from: https://keras.io/guides/functional_api/
# Developer guides/ The Functional API
# Introduction: keras ftnal api to create models flexible than tf.keras.sequential
# the api handle models w. non-linear topology, shared layers, and multiple inputs or outputs

import numpy as np
import keras as kr

def get_model():
  inputs = kr.Input(shape=(128,))
  outputs = kr.layers.Dense(1)(inputs)
  return kr.Model(inputs, outputs)

model1 = get_model()
model2 = get_model()
model3 = get_model()

inputs = kr.Input(shape=(128,))
y1 = model1(inputs)
y2 = model2(inputs)
y3 = model3(inputs)
outputs = kr.layers.average([y1, y2, y3])
ensemble_model = kr.Model(inputs=inputs, outputs=outputs)

