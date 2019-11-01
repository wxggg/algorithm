# network-flow maxflow
# dinic algorithm


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
        self.residual = {}

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

    def addE(self, e, w):
        self.adjacent[e.u].append(e)
        self.residual[e] = w

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

    def findShortestPath(self, u, v):  # BFS
        visited = []
        level = [(u, [])]
        while level != []:
            nextlevel = []
            for x, path in level:
                visited.append(x)
                for e in self.adjacent[x]:
                    residual = e.capacity - self.flow[e]
                    if residual > 0 and e.v not in visited:
                        nextlevel.append((e.v, path + [(e, residual)]))
                    if e.v == v:
                        return path + [(e, residual)]
            level = nextlevel

    def getLayeredNetwork(self, u, v):
        layered = NetworkFlow()
        layered.addVertex(u)

        visited = []
        level = [u]
        count = 1
        while v not in level and level != []:
            count += 1
            visited += level
            nextlevel = []
            for x in level:
                for e in self.adjacent[x]:
                    residual = e.capacity - self.flow[e]
                    if residual > 0 and e.v not in visited:
                        nextlevel.append(e.v)
                        layered.addVertex(e.v)
                        layered.addE(e, residual)
            level = list(set(nextlevel))

        layered.count = count
        if v in level:
            return layered

    def maxFlow(self, u, v):
        layered = self.getLayeredNetwork(u, v)
        while layered != None:
            level = [u]
            totalpaths = {}
            totalpaths[u] = ([([], sum(e.capacity for e in self.adjacent[u]))])
            for i in range(layered.count):
                nextlevel = []
                nextpaths = {}
                for x in totalpaths:
                    for path, total in totalpaths[x]:
                        if x in level:
                            for e in layered.adjacent[x]:
                                if total == 0:
                                    break
                                if e.v not in nextpaths:
                                    nextpaths[e.v] = []
                                nextlevel.append(e.v)
                                if layered.residual[e] >= total:
                                    nextpaths[e.v].append((path + [e], total))
                                    break
                                else:
                                    nextpaths[e.v].append(
                                        (path + [e], layered.residual[e]))
                                    total -= layered.residual[e]
                level = list(set(nextlevel))
                if len(nextpaths) != 0:
                    totalpaths = nextpaths

            for path, total in totalpaths[v]:
                for e in path:
                    self.flow[e] += total
                    self.flow[e.redge] -= total
            layered = self.getLayeredNetwork(u, v)
        return sum(self.flow[e] for e in self.adjacent[u])


network = NetworkFlow()
for i in range(6):
    network.addVertex(i)


network.addEdge(0,1,64)
network.addEdge(0,2,32)
network.addEdge(1,2,1)
network.addEdge(1,3,64)
network.addEdge(2,3,32)

# print(network.adjacent)

# layered = network.getLayeredNetwork(0,5)
# print(layered.adjacent)
print(network.maxFlow(0, 3))

# print(network.findShortestPath(0,3))
# print(network.maxFlow(0,3))
