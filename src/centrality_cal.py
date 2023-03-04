import networkit as nk
import networkx as nx
import random 
import sys
import os 
import time

def read_graph(map_path):
    import networkx as nx
    G = nx.Graph()
    f = open(map_path, 'r')
    data = f.readlines()
    f.close()
    for idx, lines in enumerate(data):
        src, dest = lines.split(" ")
        G.add_edge(int(src), int(dest))

    print(f"map {map_path} has {len(G.nodes())} nodes and {len(G.edges())} edges.")
    # print(G.number_of_nodes)
    return G

def nx2nkit(g_nx):
    node_num = g_nx.number_of_nodes()
    g_nkit = nk.Graph(directed=False)

    for i in range(node_num):
        g_nkit.addNode()

    for e1,e2 in g_nx.edges():
        g_nkit.addEdge(e1,e2)   
    return g_nkit

def cal_random(g_nk):
    random_value = [round(random.uniform(-10, 10), 2) for i in range (g_nk.numberOfNodes())]
    return random_value

def cal_degree(g_nk):
    temp = nk.centrality.DegreeCentrality(g_nk).run()
    degree = temp.ranking()
    return degree

def cal_BC(g_nk):
    temp = nk.centrality.Betweenness(g_nk, normalized=True).run()
    BC_value = temp.ranking()
    return BC_value

def cal_RK_BC(g_nk):
    temp = nk.centrality.ApproxBetweenness(g_nk, epsilon=0.01,  delta=0.1).run()
    RK_BC_value = temp.ranking()
    return RK_BC_value

def cal_GS_BC(g_nk):
    temp = nk.centrality.EstimateBetweenness(g_nk, nSamples = 8192).run()
    GS_BC_value = temp.ranking()
    return GS_BC_value

def cal_Kadabra_BC(g_nk):
    temp = nk.centrality.KadabraBetweenness(g_nk, err = 0.01, delta = 0.1).run()
    Kadabra_BC_value = temp.ranking()
    return Kadabra_BC_value

def cal_close(g_nk):
    temp = nk.centrality.Closeness(g_nk, False, nk.centrality.ClosenessVariant.Standard)
    temp.run()
    Close_value = temp.ranking()
    return Close_value

def output2file(centrality_value,  map_name, mode):
    directory = "../centrality/" + map_name
    if not os.path.exists(directory):
        os.makedirs(directory)
    file_path = os.path.join(directory, mode+".txt")
    with open(file_path, 'w') as f:
        for item in centrality_value:
            f.write(str(item[1]) + " " + str(item[0]) + "\n")

def prompt():
    print("**********************************************")
    print("centrality you choose has been calculated completely,\n")
# 从路径解析文件名
map_path = sys.argv[1]
import os
map_name = os.path.basename(map_path)

# Read map
g_nx = read_graph(map_path)
g_nk = nx2nkit(g_nx)
user_input = input("This python file will calculate one specific centrality.\n"
                    +"Please Input a number to decide which centrality,"
                    +" there are several option:\n" 
                    +"0——random, 1——degree, 2——BC, 3——RK, 4——GS, 5——Kadabra, 6-Close\n")
options = list(map(int, user_input.split()))
print(options)
branch = {
    0 : lambda: cal_random(g_nk),
    1 : lambda: cal_degree(g_nk),
    2 : lambda: cal_BC(g_nk),
    3 : lambda: cal_RK_BC(g_nk),
    4 : lambda: cal_GS_BC(g_nk),
    5 : lambda: cal_Kadabra_BC(g_nk),
    6 : lambda: cal_close(g_nk)
}
modes = {
    0 : "Random",
    1 : "Degree",
    2 : "BC",
    3 : 'RK',
    4 : 'GS',
    5 : 'Kadabra',
    6 : 'Close'
}
with open(map_name + "_centrality_cal_time.txt", "a") as f:
    sys.stdout = f

    # calculate centrality
    for option in options:
        func = branch.get(option)
        mode = modes.get(option)
        if func:
            start = time.perf_counter()
            centrality_value = func()
            end = time.perf_counter()
            print(f"time cost of calculate {mode}: {end-start:.4f}")
            output2file(centrality_value, map_name, mode)
            prompt()
    sys.stdout = sys.__stdout__
