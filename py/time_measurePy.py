import tensorflow as tf
import numpy as np
from tensorflow.keras.models import load_model
import time

pred_model = load_model("./Festivalle21/Source/models/best_model.hdf5", compile=False)
opt = tf.keras.optimizers.Adam(learning_rate=0.001)
pred_model.compile(optimizer=opt, loss="mse", metrics=[ tf.keras.metrics.RootMeanSquaredError() ])

X_test = np.load("./py/test_song/X.npy")

print(X_test.shape)

with open('timing_measurePython.txt', 'w') as f:
    for i in range(60):
        buffer = X_test[i]
        buffer = np.expand_dims(buffer, axis=0)
        start_time = time.perf_counter()
        pred_model.predict(buffer)

        f.write((str(time.perf_counter() - start_time) + '\n'))