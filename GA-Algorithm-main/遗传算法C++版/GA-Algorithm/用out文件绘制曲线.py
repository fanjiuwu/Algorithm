import matplotlib.pyplot as plt
f=open('out.txt')
y=f.read().split('\n')
print(type(y))
x=range(len(y))
plt.plot(x,y)
plt.show()
f.close()
