# using: CMSSW_10_2_15_patch2/external/slc7_amd64_gcc700/bin/root
# doc from: https://keras.io/guides/functional_api/
# non-linear connectivity topologies - w. layers that are not connected sequentially
# sth Sequential API can not handle.

import keras as kr
inputs = kr.Input(shape=(32,32,3), name="img")
x = kr.layers.Conv2D(32, 3, activation='relu')(inputs)
x = kr.layers.Conv2D(64, 3, activation='relu')(x)
block_1_output = kr.layers.MaxPooling2D(3)(x)

x = kr.layers.Conv2D(64, 3, activation='relu', padding="same")(block_1_output)
x = kr.layers.Conv2D(64, 3, activation='relu', padding="same")(x)
block_2_output = kr.layers.add([x, block_1_output])

x = kr.layers.Conv2D(64, 3, activation='relu', padding="same")(block_2_output)
x = kr.layers.Conv2D(64, 3, activation='relu', padding="same")(x)
block_3_output = kr.layers.add([x, block_2_output])

x = kr.layers.Conv2D(64, 3, activation='relu')(block_3_output)
x = kr.layers.GlobalAveragePooling2D()(x)
x = kr.layers.Dense(254, activation='relu')(x)
x = kr.layers.Dropout(0.5)(x)
outputs = kr.layers.Dense(10)(x)

model = kr.Model(inputs, outputs, name="toy_resnet")
model.summary()
# keras.utils.plot_model(model, "mini_resnet.png", show_shapes=True)

(x_train, y_train), (x_test, y_test) = kr.datasets.cifar10.load_data()
x_train = x_train.astype("float32")/255.
x_test  = x_test.astype("float32")/255.
y_train = kr.utils.to_categorical(y_train,10)
y_test  = kr.utils.to_categorical(y_test,10)


#loss=kr.losses.CategoricalCrossentropy(from_logits=True) nowork
model.compile(
    optimizer = kr.optimizers.RMSprop(1e-3),
    loss="categorical_crossentropy",
    metrics=["acc"],
    )

# restrict data to the first 1000 samples
model.fit(x_train[:1000], y_train[:1000], batch_size=64, epochs=1, validation_split=0.2)

