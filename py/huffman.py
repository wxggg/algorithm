
class Node(object):

    def get_weight(self):
        pass

    def isleaf(self):
        pass


class LeafNode(Node):
    def __init__(self, value=0, freq=0):
        super(LeafNode, self).__init__()
        self.value = value
        self.weight = freq

    def isleaf(self):
        return True

    def get_weight(self):
        return self.weight

    def get_value(self):
        return self.value


class InterNode(Node):
    def __init__(self, lchild=None, rchild=None):
        super(InterNode, self).__init__()

        self.weight = lchild.get_weight() + rchild.get_weight()
        self.lchild = lchild
        self.rchild = rchild

    def isleaf(self):
        return False

    def get_weight(self):
        return self.weight

    def left(self):
        return self.lchild

    def right(self):
        return self.rchild


class HuffmanTree(object):
    def __init__(self, flag, value=0, freq=0, ltree=None, rtree=None):
        super(HuffmanTree, self).__init__()

        if flag == 0:
            self.root = LeafNode(value, freq)
        else:
            self.root = InterNode(ltree.get_root(), rtree.get_root())

    def get_root(self):
        return self.root

    def get_weight(self):
        return self.root.get_weight()

    def traverse_huffman_tree(self, root, code, freqs):
        if root.isleaf():
            freqs[root.get_value()] = code
        else:
            self.traverse_huffman_tree(root.left(), code+'0', freqs)
            self.traverse_huffman_tree(root.right(), code+'1', freqs)

    def traverse(self, root):
        if root.isleaf():
            pass
        else:
            self.traverse(root.left())
            self.traverse(root.right())


def build_huffman_tree(lists):
    while len(lists) > 1:
        lists.sort(key=lambda x: x.get_weight())

        v1 = lists[0]
        v2 = lists[1]
        lists = lists[2:]

        new_huffman_tree = HuffmanTree(1, 0, 0, v1, v2)
        lists.append(new_huffman_tree)

    return lists[0]


def compress(ifilename, ofilename):
    ifile = open(ifilename, 'rb')
    ifdata = ifile.read()
    ifsize = ifile.tell()

    freqs = {}
    for i in range(ifsize):
        term = int(ifdata[i])
        if term in freqs.keys():
            freqs[term] = freqs[term] + 1
        else:
            freqs[term] = 1

    lists = []
    for x in freqs.keys():
        lists.append(HuffmanTree(0, x, freqs[x], None, None))

    length = len(freqs.keys())
    ofile = open(ofilename, 'wb')
    ofile.write(bytes(((length) & 255,)))
    ofile.write(bytes(((length >> 8) & 255,)))
    ofile.write(bytes(((length >> 16) & 255,)))
    ofile.write(bytes(((length >> 24) & 255,)))

    for x in freqs.keys():
        ofile.write(bytes((x,)))
        freq = freqs[x]
        ofile.write(bytes(((freq) & 255,)))
        ofile.write(bytes(((freq >> 8) & 255,)))
        ofile.write(bytes(((freq >> 16) & 255,)))
        ofile.write(bytes(((freq >> 24) & 255,)))

    hufftree = build_huffman_tree(lists)
    hufftree.traverse_huffman_tree(hufftree.get_root(), '', freqs)

    hufftree.traverse(hufftree.get_root())

    code = ''
    for i in range(ifsize):
        key = int(ifdata[i])
        code = code + freqs[key]
        out = 0
        while len(code) > 8:
            for x in range(8):
                out = out << 1
                if code[x] == '1':
                    out = out | 1
            code = code[8:]
            ofile.write(bytes((out,)))
            out = 0

    ofile.write(bytes((len(code),)))
    out = 0
    for i in range(len(code)):
        out = out << 1
        if code[i] == '1':
            out = out | 1

    for i in range(8-len(code)):
        out = out << 1

    ofile.write(bytes((out,)))

    ofile.close()


def decompress(ifilename, ofilename):

    ifile = open(ifilename, 'rb')
    ifdata = ifile.read()
    ifsize = ifile.tell()

    length = 0
    length = (length | int(ifdata[3])) << 8
    length = (length | int(ifdata[2])) << 8
    length = (length | int(ifdata[1])) << 8
    length = (length | int(ifdata[0]))

    freqs = {}
    for i in range(length):
        x = int(ifdata[4+5*i+0])
        freq = 0
        freq = (freq | int(ifdata[4+5*i+4])) << 8
        freq = (freq | int(ifdata[4+5*i+3])) << 8
        freq = (freq | int(ifdata[4+5*i+2])) << 8
        freq = (freq | int(ifdata[4+5*i+1]))
        freqs[x] = freq

    lists = []
    for x in freqs.keys():
        lists.append(HuffmanTree(0, x, freqs[x], None, None))

    hufftree = build_huffman_tree(lists)
    hufftree.traverse_huffman_tree(hufftree.get_root(), '', freqs)

    hufftree.traverse(hufftree.get_root())

    ofile = open(ofilename, 'wb')
    code = ''
    node = hufftree.get_root()

    for x in range(length*5+4, ifsize):
        c = int(ifdata[x])
        for i in range(8):
            if c & 128:
                code = code + '1'
            else:
                code = code + '0'
            c = c << 1

        while len(code) > 24:
            if node.isleaf():
                ofile.write(bytes((node.get_value(),)))
                node = hufftree.get_root()
            if code[0] == '1':
                node = node.right()
            else:
                node = node.left()
            code = code[1:]

    sub = code[-16:-8]
    last_length = 0
    for i in range(8):
        last_length = last_length << 1
        if sub[i] == '1':
            last_length = last_length | 1

    code = code[:-16] + code[-8:-8+last_length]

    while len(code) > 0:
        if node.isleaf():
            ofile.write(bytes((node.get_value(),)))
            node = hufftree.get_root()
        if code[0] == '1':
            node = node.right()
        else:
            node = node.left()

        code = code[1:]

    if node.isleaf():
        ofile.write(bytes((node.get_value(),)))
        node = hufftree.get_root()

    ofile.close()


compress("./graph.txt", "./graph_compress")
decompress("./graph_compress", "./graph_uncompress.txt")

compress("./Aesop_Fables.txt", "./Aesop_Fables_compress")
decompress("./Aesop_Fables_compress", "./Aesop_Fables_compress.txt")
