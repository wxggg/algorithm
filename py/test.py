# python3

# problem1 & problem8
# ford-fulkerson algorithm
# use delta-scaling and bi-search

class Edge(object):
    def __init__(self, u, v, c):
        self.u = u
        self.v = v
        self.capacity = c

    def __repr__(self):
        return str(self.u) + "->" + str(self.v) + ":" + str(self.capacity)


class NetworkFlow(object):
    def __init__(self):
        self.adjacent = {}
        self.flow = {}

    def addVertex(self, v):
        self.adjacent[v] = []

    def addEdge(self, u, v, w):
        if u == v:
            raise ValueError("u == v")
        e = Edge(u, v, w)
        re = Edge(v, u, 0)
        e.redge = re
        re.redge = e
        self.adjacent[u].append(e)
        self.adjacent[v].append(re)
        self.flow[e] = 0
        self.flow[re] = 0

    def getEdge(self, v):
        return self.adjacent[v]

    def findPath(self, u, v, found,  delta):
        if u == v:
            return found
        for e in self.adjacent[u]:
            residual = e.capacity - self.flow[e]
            if e.capacity >= delta and residual > 0 and (e, residual) not in found:
                ret = self.findPath(e.v, v, found + [(e, residual)], delta)
                if ret != None:
                    return ret
    
    def maxFlow(self, u, v):
        delta = sum(e.capacity for e in self.adjacent[u]) # set delta
        while delta >= 1:
            path = self.findPath(u,v,[], delta)
            # print('delta:',delta)
            # print('path:',path)
            while path != None:
                flow = min(res for e,res in path)
                for e,res in path:
                    self.flow[e] += flow
                    self.flow[e.redge] -= flow
                path = self.findPath(u,v,[],delta)
            delta = int(delta/2) # scale delta
        return sum(self.flow[e] for e in self.adjacent[u])
    
    def clear(self):
        for x in self.flow:
            self.flow[x] = 0

    def setInCapacity(self, v, c):
        for e in self.adjacent[v]:
            re = e.redge
            re.capacity = c



def readData(filepath):
    f = open(filepath, 'r')
    alllist = []
    line = f.readline()
    line = f.readline()
    line = f.readline()
    count = 0
    k = -1
    for line in f:
        if count % 3 == 0:
            alllist.append([])
            k += 1
        alllist[k].append([int(v) for v in line.split()])
        count += 1
    return alllist


data = readData('./problem2.data')

# print(data)

for mat in data[-1:]:
    m,n = mat[0]
    network = NetworkFlow()
    vertexr = ['r'+str(i+1) for i in range(m)]
    vertexc = ['c'+str(i+1) for i in range(n)]
    total = sum(mat[1])
    # print('------------------------')
    print(total)
    # print(sum(mat[2]))

    network.addVertex('s')
    network.addVertex('t')

    for i in range(m):
        network.addVertex(vertexr[i])
        network.addEdge('s',vertexr[i],mat[1][i])
        
    for i in range(n):
        network.addVertex(vertexc[i])
        network.addEdge(vertexc[i],'t',mat[2][i])

    for vr in vertexr:
        for vc in vertexc:
            network.addEdge(vr,vc,1)
    
    # print(network.adjacent)
    print(network.maxFlow('s','t'))
        
