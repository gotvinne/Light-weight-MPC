
from matplotlib import pyplot as plt
from SimulationData import *
import numpy as np

def PlotPrediction(sim_data):
    fig, axs = plt.subplots(nrows=sim_data.n_CV)
    t = np.arange(0, sim_data.T, dtype=int)
    for i in range(sim_data.n_CV):
        axs[i, i].set(xlabel='T', ylabel=sim_data.cv_units[i], title='Visualization of MPC prediction')
        axs.grid()
        plt.show()
    

