#include "pruned_landmark_labeling.h"
#include <iostream>
#include <vector>
#include <cstring>
#include <filesystem>
#include <random>
#include <chrono>

char* choose_centrality(int n);

int main(){

    std::vector<int> nums;
    int num;
    // get the mapname
    std::string map_name;
    std::string map_path;
    std::cout << "Enter the the name of map, e.g : graph_example.tsv"<<std::endl ;
    std::getline(std::cin, map_name);
    std::cout << "The map name you entered: " << map_name << std::endl;

    std::ofstream outfile(map_name + "_output.txt");
    if (!outfile.is_open()) {
        std::cerr << "Failed to open output file." << std::endl;
        return 1;
    }

    map_path = "../maps/"+ map_name;
    std::string source_dir = "../centrality/" + map_name + "/";
    std::string dest_file_name = "centrality.txt";


    std::cout << std::endl << "This C++ file will execute PLL based on one specific order.\n"
              << "Please Input a number or several numbers' combination to decide which centrality,\n"
              << "there are several option:\n"
              << "0——random, 1——degree, 2——BC, 3——RK, 4——GS, 5——Kadabra, 6-Close e.g: 1 2 3 a\n"
              << "Note: Input -1 or any alphabet to end input" << std::endl;
    while (std::cin >> num && num != -1) {
        nums.push_back(num);
    }
    //输出到文件
    std::streambuf* coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(outfile.rdbuf());
   
    for (auto n : nums) {
        PrunedLandmarkLabeling<> pll;
        char* centrality = choose_centrality(n);
        std::cout << map_name << "->" << centrality << std::endl;
        std::ifstream source_file(source_dir + centrality + ".txt");
        std::string content((std::istreambuf_iterator<char>(source_file)), std::istreambuf_iterator<char>());
        source_file.close();
        std::ofstream dest_file(dest_file_name);
        dest_file << content;
        dest_file.close();
        pll.ConstructIndex(const_cast<char*>(map_path.c_str()));
        int V = pll.PrintStatistics();
        std::mt19937_64 rng(std::random_device{}());
        std::uniform_int_distribution<int> dist(0, V-1);
        std::vector<int> rand_nums;
        auto start_time = std::chrono::high_resolution_clock::now();
        for (int i=0; i< 1000; i++){
            int start = dist(rng);
            int end = dist(rng);
            pll.QueryDistance(dist(rng), dist(rng));
        }
        auto end_time = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        std::cout << "1000 times query time: " << elapsed.count() << " microseconds\n" << std::endl;
    }
    std::cout.rdbuf(coutbuf);
    outfile.close();
    return 0;
}

char* choose_centrality(int n) {
    char* centrality = new char[20]; // 为指针分配内存
    switch (n) {
        case 0:
            std::strcpy(centrality, "Random");
            break; // 添加 break 语句
        case 1:
            std::strcpy(centrality, "Degree");
            break; // 添加 break 语句
        case 2:
            std::strcpy(centrality, "BC");
            break; // 添加 break 语句
        case 3:
            std::strcpy(centrality, "RK");
            break; // 添加 break 语句
        case 4:
            std::strcpy(centrality, "GS");
            break; // 添加 break 语句
        case 5:
            std::strcpy(centrality, "Kadabra");
            break; // 添加 break 语句
        case 6:
            std::strcpy(centrality, "Close");
            break; // 添加 break 语句
        default:
            std::strcpy(centrality, "degree");
    }
    return centrality;
}
