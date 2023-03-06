import networkx as nx
import sys
import random

# read mapname
map_name = sys.argv[1]
print(f"The map name you entered is {map_name}")

# read modes
print("This python file will shuffle the centrality you have calculated.\n")


# read K%
user_input2 = input("Input a number to decide the percentage you wanna shuffle,"
                    +"eg:50(this stands for shuffling 50% of all nodes)\n")
topK = int(user_input2)/100

# top or rest
user_input3 = input("whether topk% you wanna shuffle or the rest,"
                    +"0——topk%, 1——rest\n")
choose2 = int(user_input3)

    # mode = modes.get(choose)
mode = 'Degree'
with open ('../centrality/' + map_name + '/' + mode + '_ranking.txt', 'r') as f:
    lines = f.readlines()
shuffle_lines = int(len(lines) * topK)
if choose2 == 0:
    header = lines[:shuffle_lines]
    random.shuffle(header)
    with open('../centrality/' + map_name + '/' + mode + '_shuffle_ranking' + ".txt", 'w') as f:
        f.writelines(header + lines[shuffle_lines:])
elif choose2 == 1:
    footer = lines[-shuffle_lines:]
    random.shuffle(footer)
    with open('../centrality/' + map_name + '/' + mode + '_shuffle_ranking' +  ".txt", 'w') as f:
        f.writelines(lines[:-shuffle_lines] + footer)
else: 
    print('u input error number!')
   



        
    
