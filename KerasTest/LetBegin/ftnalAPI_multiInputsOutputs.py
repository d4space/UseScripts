# doc from: https://keras.io/guides/functional_api/
# Developer guides/ The Functional API
# API easy to manipulate mutiple inputs and outputs, cannot be handled w. Sequential API

# a system for ranking custom issue tickets by priority and routing them to the correct department

# the title of the ticket (text input),
# text body of the ticket(text input), and
# any tags added by the user (categorical input)
# outputs:
# priority score btw 0 and 1 (scalar sigmoid output), and
# department that should handle the ticket(softmax output over the set of departments)


import numpy as np
import keras as kr


num_tags = 12 # unique issue tags
num_words = 10000 # size of vocabulary obtained processing text data
num_departments = 4 # departments for predictions

title_input = kr.Input(
    shape=(None,), name="title"
    )
body_input = kr.Input(shape=(None,), name="body")
tags_input = kr.Input(
    shape=(num_tags,), name="tags"
    )

# Embed each word in the title into 64-dim. vector
title_features = kr.layers.Embedding(num_words, 64)(title_input)
# Embed each word in the text into 64-dim vector
body_features = kr.layers.Embedding(num_words, 64)(body_input)

# Reduce sequence of embedded words in the title into a single 128-dim
title_features = kr.layers.LSTM(128)(title_features)
# reduce sq of embedded words in the body into 32-dim
body_features = kr.layers.LSTM(32)(body_features)

# Merge via concatenation
x = kr.layers.concatenate([title_features, body_features, tags_input])

# stick a logistic regression for priority predection
priority_pred = kr.layers.Dense(1, name="priority")(x)
# stick a department classifier 
department_pred = kr.layers.Dense(num_departments, name="department")(x)

model = kr.Model(
    inputs=[title_input, body_input, tags_input],
    outputs=[priority_pred, department_pred],
    )

# assign different losses to each output
# different weights each loss
"""
model.compile(
    optimizer=kr.optimizers.RMSprop(1e-3),
    loss=[
      kr.losses.BinaryCrossentropy(from_logits=True),
      kr.losses.CategoricalCrossentropy(from_logits=True),
      ],
    loss_weights=[1.0, 0.2],
    )
"""
# specify the loss for each output layers
model.compile(
    optimizer = kr.optimizers.RMSprop(1e-3),
    loss={
      "priority":"binary_crossentropy",
      "department": "categorical_crossentropy",
      },
    loss_weights=[1.0, 0.2],
    )

# Dummy data
title_data = np.random.randint(num_words, size=(1280, 10))
body_data  = np.random.randint(num_words, size=(1280, 10))
tags_data  = np.random.randint(2, size=(1280, num_tags)).astype("float32")

# Dummy target data
priority_targets = np.random.randint(2, size=(1280, 1))
dept_targets = np.random.randint(2, size=(1280, num_departments))

model.fit(
    {"title": title_data, "body":body_data, "tags": tags_data},
    {"priority": priority_targets, "department": dept_targets},
    epochs=2,
    batch_size=32,
    )

