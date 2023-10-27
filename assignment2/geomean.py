data = [
        [0.339681, 0.51132, 0.615946, 0.201646, 0.946511, 0.151025], # lru
        [0.311224, 0.505894, 0.492507, 0.208528, 1.00417, 0.152128], # lip
        [0.315178, 0.512692, 0.601574, 0.202711, 0.99008, 0.150813], # bip
        [0.342739, 0.458382, 0.621765, 0.202352, 0.989906, 0.151193], # dip
        [0.341923, 0.509502, 0.612429, 0.201868, 0.950422, 0.15244] # ptlru
    ]

result = []
geomean = 1

for i in range (1, 5):
    geomean = 1
    for j in range(len(data[0])):
        geomean = data[i][j]/data[0][j] * geomean
    geomean = geomean**(1/6)
    result.append(geomean)
print(result)