import tensorflow as tf
import numpy as np
from tensorflow.keras.models import load_model
import time

pred_model = load_model("./Festivalle21/Source/models/best_model.hdf5", compile=False)
opt = tf.keras.optimizers.Adam(learning_rate=0.001)
pred_model.compile(optimizer=opt, loss="mse", metrics=[ tf.keras.metrics.RootMeanSquaredError() ])

print('HERE')
X_test = np.load("./py/test_song/X.npy")
start_time = time.perf_counter()
pred_model.predict(X_test[0])
print(time.perf_counter() - start_time)
