import numpy as np

def PrintNpy(filepath):
    X = np.load(filepath, mmap_mode='r')
    print(X)
    print(X.shape)

PrintNpy("vis/S_11_data.npy")
PrintNpy("vis/S_12_data.npy")
PrintNpy("vis/S_21_data.npy")
PrintNpy("vis/S_22_data.npy")
