# From : https://machinelearningmastery.com/tutorial-first-neural-network-python-keras/
import os
from numpy import loadtxt, reshape
import keras as kr
from keras.models import Sequential, load_model
from keras.layers import Dense
#from keras.initializers import GlorotNormal, GlorotUniform
#from keras.initializers import GlorotNormal

# download file: https://raw.githubusercontent.com/jbrownlee/Datasets/master/pima-indians-diabetes.data.csv
#cmd = 'wget https://raw.githubusercontent.com/jbrownlee/Datasets/master/pima-indians-diabetes.data.csv'
#os.system(cmd)

dataset = loadtxt('pima-indians-diabetes.data.csv', delimiter=',')
x = dataset[:, 0:8]
y = dataset[:, 8] # diabet 1, 0
'''
#model = kr.models.Sequential()
#model.add(kr.layers.Dense(12, input_dim=8, activation='relu'))
#model.add(kr.layers.Dense(8, activation='relu'))
#model.add(kr.layers.Dense(1, activation='sigmoid'))
initializer = kr.initializers.RandomUniform(minval=-0.05, maxval=0.05, seed=1)
#initializer = kr.initializers.GlorotNormal()
#initializer = kr.initializers.GlorotUniform()
model = Sequential()
model.add(Dense(12, input_dim=8, activation='relu', kernel_initializer=initializer))
model.add(Dense(8, activation='relu', kernel_initializer=initializer))
model.add(Dense(1, activation='sigmoid', kernel_initializer=initializer))
model.summary()

#                                  optimizer: adam, rmsprop
model.compile(loss='binary_crossentropy', optimizer='rmsprop', metrics=['accuracy'])
model.fit(x,y, epochs=150, batch_size=100)
model.save('binaryClassEg.h1')


A, accuracy = model.evaluate(x,y)
print('Accuracy: %.2f' %(accuracy*100),A)

predictions = model.predict_classes(x) # giving 0 or 1
for i in range(5):
  print('%s => %.2f (truth %d)' % (x[i].tolist(), predictions[i], y[i]))

for i in range(5):
  predict = model.predict(reshape(x[i],(1,8)))
  print('%s => %.2f (truth %d)' % (x[i].tolist(), predict, y[i]))

'''
new_model = load_model('binaryClassEg.h1')
for i in range(5):
  predict = new_model.predict(reshape(x[i],(1,8)))
  print('%s => %.2f (truth %d)' % (x[i].tolist(), predict, y[i]))

