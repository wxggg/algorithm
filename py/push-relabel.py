# network-flow maxflow
# push-relabel algorithm


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


network = NetworkFlow()
for i in range(4):
    network.addVertex(i)


# network.addEdge(0,1,3)
# network.addEdge(1,2,1)
# network.addEdge(2,3,2)

network.addEdge(0,1,64)
network.addEdge(0,2,32)
network.addEdge(1,2,1)
network.addEdge(1,3,64)
network.addEdge(2,3,32)
print(network.adjacent)

print(network.maxFlow(0, 3))
