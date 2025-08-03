import matplotlib.pyplot as plt
import numpy as np



# for_x = np.linspace(0,10,100)

x = []
y = []


for i in range(-20,20):
    #main function here
    #var = newx and newy
    newx = i
    newy = 5
    #main functions end here
    x.append(newx)
    y.append(newy)

fig, ax = plt.subplots()


ax.plot(x,y)


ax.set_xlabel("X-axis Label")
ax.set_ylabel("Y-axis Label")
ax.set_title("Simple Sine Wave Plot")


plt.show()