
from matplotlib import pyplot as plt

def PlotPrediction(n_CV):
    fig, ax = plt.subplots(nrows=n_CV)
    ax.set(xlabel='T', ylabel='System values', title='Visualization of MPC prediction')
    ax.grid()
    plt.show()
    

