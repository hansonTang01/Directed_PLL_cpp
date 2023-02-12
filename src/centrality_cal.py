import networkit as nk
import networkx as nx
import random 
import sys

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
    degree = temp.scores()
    return degree

def cal_BC(g_nk):
    temp = nk.centrality.Betweenness(g_nk,normalized=True).run()
    BC_value = temp.scores()
    return BC_value

def cal_RK_BC(g_nk):
    temp = nk.centrality.ApproxBetweenness(g_nk, epsilon=0.01,  delta=0.1).run()
    RK_BC_value = temp.scores()
    return RK_BC_value

def cal_GS_BC(g_nk):
    temp = nk.centrality.EstimateBetweenness(g_nk, nSamples = 8192).run()
    GS_BC_value = temp.scores()
    return GS_BC_value

def cal_Kadabra_BC(g_nk):
    temp = nk.centrality.KadabraBetweenness(g_nk, err = 0.01, delta = 0.1).run().scores()
    Kadabra_BC_value = temp.scores()
    return Kadabra_BC_value

def output2file(centrality_value):
    with open("./src/centrality.txt", 'w') as f:
        for index, item in enumerate(centrality_value):
            f.write(str(item) + " " + str(index) + "\n")

def prompt(map_path):
    print("**********************************************")
    print("centrality you choose has been calculated completely,\n"
        + "Use following command line to execuate PLL algorithm:\n"
        + "\"bin/construct_index " + map_path +" index\"")
map_path = sys.argv[1]
g_nx = read_graph(map_path)
g_nk = nx2nkit(g_nx)
user_input = input("This python file will calculate one specific centrality.\n"
                    +"Please Input a number to decide which centrality,"
                    +" there are several option:\n" 
                    +"0——random, 1——degree, 2——BC, 3——RK, 4——GS, 5——Kadabra\n")
option = int(user_input)
branch = {
    0 : lambda: cal_random(g_nk),
    1 : lambda: cal_degree(g_nk),
    2 : lambda: cal_BC(g_nk),
    3 : lambda: cal_RK_BC(g_nk),
    4 : lambda: cal_GS_BC(g_nk),
    5 : lambda: cal_Kadabra_BC(g_nk)
}
func = branch.get(option)
if func:
    centrality_value = func()
    output2file(centrality_value)
    prompt(map_path)
