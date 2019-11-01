# network-flow maxflow
# edmonds-karp algorithm

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

    def findPath(self, u, v, found):
        if u == v:
            return found
        for e in self.adjacent[u]:
            residual = e.capacity - self.flow[e]
            if residual > 0 and (e, residual) not in found:
                ret = self.findPath(e.v, v, found + [(e, residual)])
                if ret != None:
                    return ret
    
    def findShortestPath(self, u, v): # BFS
        visited = []
        level = [(u,[])]
        while level != []:
            nextlevel = []
            for x,path in level:
                visited.append(x)
                for e in self.adjacent[x]:
                    residual = e.capacity - self.flow[e]
                    if residual > 0 and e.v not in visited:
                        nextlevel.append((e.v,path+[(e,residual)]))
                    if e.v == v:
                        return path+[(e,residual)]
            level = nextlevel


    def maxFlow(self, u, v):
        path = self.findShortestPath(u,v)
        while path != None:
            print(path)
            flow = min(res for e,res in path)
            for e,res in path:
                self.flow[e] += flow
                self.flow[e.redge] -= flow
            path = self.findShortestPath(u,v)
        return sum(self.flow[e] for e in self.adjacent[u])

network = NetworkFlow()
for i in range(6):
    network.addVertex(i)

network.addEdge(0,1,64)
network.addEdge(0,2,32)
network.addEdge(1,2,1)
network.addEdge(1,3,64)
network.addEdge(2,3,32)

print(network.adjacent)

# print(network.findShortestPath(0,3))
print(network.maxFlow(0,3))

