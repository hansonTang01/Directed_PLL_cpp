#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <chrono>

using namespace std;

class PrunedLandmarkLabeling {
public:
    PrunedLandmarkLabeling(string centralityFile, string edgesFile);
    int query(int src, int dest);
    int V;
private:
    int max_length = 9999999;
    vector<int> nodes;
    vector<vector<int> > inEdges;
    vector<vector<int> > outEdges;
    vector<vector<pair<int, int>>> inIndex;
    vector<vector<pair<int, int>>> outIndex;
    vector<bool> hasProcess;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    int* nodes_id;
    void cal_index_size();
    void constructIndex();
    bool need_to_expand(int src, int dest, int dist = -1);
};

PrunedLandmarkLabeling::PrunedLandmarkLabeling(string centralityFile, string edgesFile) {
    // 把order 读入vector：nodes, 并生成nodes_id
    ifstream centralityFileStream(centralityFile);
    double num1;
    int num2;
    V = 0;
    while (centralityFileStream >> num1 >> num2) {
        nodes.push_back(num2);
        V++;
    }
     // 准备需要的vector
    inIndex.resize(V);
    outIndex.resize(V);
    hasProcess.resize(V);
    inEdges.resize(V);
    outEdges.resize(V);
    nodes_id = new int[V](); 

    int i = 0;
    for (int node : nodes) {
        nodes_id[node] = i++;
    }

    // 把图的边读入inEdges, outEdges
    ifstream edgesFileStream(edgesFile);
    
    for (int s, d; edgesFileStream >> s >> d; ) {
        outEdges[s].push_back(d);
        inEdges[d].push_back(s);
    }

    constructIndex();
    cal_index_size();
}
bool PrunedLandmarkLabeling::need_to_expand(int src, int dest, int dist) {
    int query_result = query(src, dest);
    if (query_result <= dist) {
        return false;
    }
    return true;
}

// 构建Index
void PrunedLandmarkLabeling::constructIndex(){
    auto start_time = std::chrono::steady_clock::now(); // 获取开始时间
    int count = 0;
    const int step = V/10;
    for (int cur_node: nodes){
        // calculate forward
        // 计数
        count += 1;
        if (count % step == 0) {
            int progress = (count / step) * 10;
            cout << "Progress: " << progress << "%" << endl;
        }
        pq.push(make_pair(0, cur_node));
        fill(hasProcess.begin(), hasProcess.end(), false);
        while (!pq.empty()){
            auto [cur_dist, src] = pq.top();
            pq.pop();
            if (hasProcess[src] || nodes_id[cur_node] > nodes_id[src] || !need_to_expand(cur_node, src, cur_dist)){
                hasProcess[src] = true;
                continue;
            }
            hasProcess[src] = true;
            inIndex[src].push_back(make_pair(cur_node, cur_dist));
            vector<int> edges = outEdges[src];
            for (int dest : edges){
                if(hasProcess[dest]){
                    continue;
                }
                pq.push(make_pair(cur_dist + 1, dest));
            }
        }
        // calculate backward
        pq.push(make_pair(0, cur_node));
        fill(hasProcess.begin(), hasProcess.end(), false);
        while (!pq.empty()){
            auto [cur_dist, src] = pq.top();
            pq.pop();
            if (hasProcess[src] || nodes_id[cur_node] > nodes_id[src] || !need_to_expand(src, cur_node, cur_dist)){
                hasProcess[src] = true;
                continue;
            }
            hasProcess[src] = true;
            outIndex[src].push_back(make_pair(cur_node, cur_dist));
            vector<int> edges = inEdges[src];
            for (int dest : edges){
                if(hasProcess[dest]){
                    continue;
                }
                pq.push(make_pair(cur_dist + 1, dest));
            }
        }
    }
    auto end_time = std::chrono::steady_clock::now(); // 获取结束时间
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count(); // 计算时间差，以毫秒为单位
    std::cout << "Index time: " << elapsed_time << " ms" << std::endl; // 输出时间差
}

void PrunedLandmarkLabeling::cal_index_size(){
    size_t totalSize = sizeof(vector<vector<pair<int, int>>>);
    for (vector vec : inIndex) {
    size_t elementSize = vec.capacity() * sizeof(pair<int, int>);
    totalSize += elementSize + sizeof(vector<pair<int, int>>);
    }
    for (vector vec: outIndex) {
    size_t elementSize = vec.capacity() * sizeof(pair<int, int>);
    totalSize += elementSize + sizeof(vector<pair<int, int>>);
    }
    double totalSizeMB = static_cast<double>(totalSize) / (1024 * 1024);
    cout << "Index Size：" << totalSizeMB << " MB" << endl;
}

// 查询
int PrunedLandmarkLabeling::query(int src, int dest) {
    vector<pair<int, int>> src_list = outIndex[src];
    vector<pair<int, int>> dest_list = inIndex[dest];
    int i = 0;
    int j = 0;
    int shortest_dist = max_length;
    while (i < src_list.size() && j < dest_list.size()) {
        int tmp1 = src_list[i].first;
        int tmp2 = dest_list[j].first;
        if (tmp1 == tmp2){
            int curr_dist = src_list[i].second + dest_list[j].second;
            if (curr_dist == 1 || curr_dist == 0 ){
                shortest_dist = curr_dist;
                break;
            }
            if (curr_dist < shortest_dist) {
                shortest_dist = curr_dist;
            }
            i++;
            j++;
        }   else if (nodes_id[tmp1] < nodes_id[tmp2]){
            i += 1;
        }   else{
            j += 1;
        }
        
    }
    return shortest_dist;
}
