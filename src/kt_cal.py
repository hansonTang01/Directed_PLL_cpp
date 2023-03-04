import numpy as np
from scipy.stats import kendalltau
import sys

map_name = sys.argv[1]

user_input = input("This python file will calculate kt score .\n"
                    +"Please Input a number to decide which centrality ,"
                    +" there are several option:\n" 
                    +"0——RK, 1——GS, 2——Kadabra, 3——GNN, 4——GNN_improved\n")
options = list(map(int, user_input.split()))
modes = {
    0 : "RK",
    1 : "GS",
    2 : "Kadabra",
    3 : "ori_19_model.pt",
    4 : "30_100_20_19_model.pt"
}

# 从第一个文件中读取数据
with open('../centrality/' + map_name + "/BC.txt", 'r') as file1:
    lines1 = file1.readlines()
    # 从每行中提取第一个数字
    arr1 = np.array([float(line.split()[0]) for line in lines1])

with open(map_name + "_kt_value.txt", "a") as f:
    sys.stdout = f
    for option in options:
        mode = modes.get(option)
        with open('../centrality/' + map_name + "/" + mode + ".txt", 'r') as file2:
            lines2 = file2.readlines()
            # 从每行中提取第一个数字
            arr2 = np.array([(line.split()[0]) for line in lines2])
        tau, _ = kendalltau(arr1, arr2)
        print(f"Kendall Tau of {mode}: {tau}")
    sys.stdout = sys.__stdout__