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
    alllist = [[]]
    for line in f:
        if line[0] == '#':
            continue
        if line == '\n':
            alllist.append([])
        else:
            value = line.split()
            alllist[-1].append((value[0],value[1]))
    return alllist


data = readData('./problem1.data')


result = []

for jobs in data:
    jobnum,computernum = int(jobs[0][0]),int(jobs[0][1])
    # print(jobnum,computernum)
    network = NetworkFlow()
    network.addVertex('s')
    network.addVertex('t')

    computers = []
    for i in range(jobnum):
        vji = 'j' + str(i+1)
        network.addVertex(vji)
        network.addEdge('s',vji,1)
        vc1,vc2 = 'c'+jobs[i+1][0],'c'+jobs[i+1][1]
        if vc1 not in computers:
            network.addVertex(vc1)
            network.addEdge(vc1,'t',1)
            computers.append(vc1)
        if vc2 not in computers:
            network.addVertex(vc2)
            network.addEdge(vc2,'t',1)
            computers.append(vc2)
        network.addEdge(vji,vc1,1)
        network.addEdge(vji,vc2,1)
    
    left = 0
    right = jobnum
    while left<right:
        mid = int((left+right)/2)
        network.clear()
        network.setInCapacity('t',mid);
        maxflow = network.maxFlow('s','t')
        if maxflow == jobnum:
            right = mid
        else:
            left = mid + 1

    # print('end:',left,right)
    result.append(left)


print("result minmaxload:",result)