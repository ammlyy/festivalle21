with open('timing_measure.txt','r') as f:
    data = [float(line.rstrip()) for line in f.readlines()]
    f.close()
mean = float(sum(data))/len(data) if len(data) > 0 else float('nan')
print(mean)