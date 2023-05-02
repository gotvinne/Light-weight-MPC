import numpy as np

def PrintNpy(filepath):
    X = np.load(filepath, mmap_mode='r')
    print(X)
    print(X.shape)

PrintNpy("vis/S11_data.npy")
PrintNpy("vis/S12_data.npy")
PrintNpy("vis/S21_data.npy")
PrintNpy("vis/S22_data.npy")
