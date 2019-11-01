# network-flow maxflow
# problem2 matrix problem9
# 201728015029060
# Wang Xingang


class Edge(object):
    def __init__(self, u, v, c, forward):
        self.u = u
        self.v = v
        self.capacity = c
        self.forward = forward

    def __repr__(self):
        return str(self.u) + "->" + str(self.v) + ":" + str(self.capacity)


class NetworkFlow(object):
    def __init__(self):
        self.adjacent = {}
        self.flow = {}
        self.height = {}
        self.excess = {}

    def addVertex(self, v):
        self.adjacent[v] = []
        self.height[v] = 0
        self.excess[v] = 0

    def addEdge(self, u, v, w):
        if u == v:
            raise ValueError("u == v")
        e = Edge(u, v, w, True)
        re = Edge(v, u, 0, False)
        e.redge = re
        re.redge = e
        self.adjacent[u].append(e)
        self.adjacent[v].append(re)
        self.flow[e] = 0
        self.flow[re] = 0

    def getEdge(self, u,v):
        for e in self.adjacent[u]:
            if e.v == v:
                return e

    def maxFlow(self, s, t):
        self.height[s] = len(self.adjacent)
        for e in self.adjacent[s]:
            self.flow[e] = e.capacity
            self.flow[e.redge] = e.capacity
            self.excess[e.v] = e.capacity
        self.excess[s] = 0
        
        while True:
            exc = [x for x in self.excess if self.excess[x] > 0 and x != t]
            judge = False
            flags = False
            if len(exc) == 0:
                break
            elif len(exc) == 1 and exc[0] == s:
                flags = True
                
            for x in exc:
                flag = False
                for e in self.adjacent[x]:
                    if self.height[x] > self.height[e.v]:
                        if e.forward and e.capacity - self.flow[e] > 0:
                            bottleneck = min(self.excess[x], e.capacity - self.flow[e])
                            self.flow[e] += bottleneck
                            self.flow[e.redge] += bottleneck
                            self.excess[x] -= bottleneck
                            self.excess[e.v] += bottleneck
                            flag = True
                        elif not e.forward and self.flow[e] > 0:
                            bottleneck = min(self.excess[x], self.flow[e])
                            self.flow[e] -= bottleneck
                            self.flow[e.redge] -= bottleneck
                            self.excess[x] -= bottleneck
                            self.excess[e.v] += bottleneck
                            flag = True
                
                if flag == False:
                    self.height[x] += 1
                    judge = True
            if judge and flags:
                break
        return self.excess[t]


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

def verify(rows, cols, matrix):
    m = len(rows)
    n = len(cols)

    mrows = []
    mcols = []
    for i in range(m):
        mrows.append(sum(matrix[i]))
        
    for j in range(n):
        sumj = 0
        for i in range(m):
            sumj += matrix[i][j]
        mcols.append(sumj)

    if rows == mrows and cols == mcols:
        print('verify ok')
    else:
        print('verify failure')
data = readData('./problem2.data')

for mat in data:
    m,n = mat[0]
    network = NetworkFlow()
    vertexr = ['r'+str(i+1) for i in range(m)]
    vertexc = ['c'+str(i+1) for i in range(n)]
    total = sum(mat[1])
    print('------------------------')
    print('matrix total:',total)

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
    

    print('maxflow:',network.maxFlow('s','t'))

    matrix = []
    count = -1
    for vr in vertexr:
        count += 1
        matrix.append([])
        for vc in vertexc:
            e = network.getEdge(vr,vc)
            matrix[count].append(network.flow[e])

    # print('matrix:',matrix)
    verify(mat[1], mat[2], matrix)
