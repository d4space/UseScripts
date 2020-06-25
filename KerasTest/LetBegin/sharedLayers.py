# using: CMSSW_10_2_15_patch2/external/slc7_amd64_gcc700/bin/root
# doc from: https://keras.io/guides/functional_api/
# non-linear connectivity topologies - w. layers that are not connected sequentially
# sth Sequential API can not handle.

import keras as kr


# embedding for 1000 words mapped to 128-dim. vectr
shared_embedding = kr.layers.Embedding(1000, 128)
text_input_a = kr.Input(shape=(None,), dtype="int32")
text_input_b = kr.Input(shape=(None,), dtype="int32")

encoded_input_a = shared_embedding(text_input_a)
encoded_input_b = shared_embedding(text_input_b)

# graph of layers is static data structure
# can access the activations of intermidiate layers(nodes in the  graph) and
# reuse them elsewhere
# VGG19 model
vgg19 = kr.applications.VGG19()
# a model returns the values of the intermediate layer activations


