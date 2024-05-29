#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

//g++ -o csv_reader csv_reader.cpp 


// ノード構造体の定義
struct Node {
    std::string type;
    int id;
    std::string parent_type;
    int parent_id;
    float length;
    float radius;
    float dir_x;
    float dir_y;
    float dir_z;
    float distance;
};

// CSVファイルを読み取る関数
std::vector<Node> readCSV(const std::string& filename) {
    std::vector<Node> nodes;
    std::ifstream file(filename);
    std::string line, word;

    // ヘッダ行をスキップ
    std::getline(file, line);

    // 各行を読み取る
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Node node;
        std::getline(ss, node.type, ',');
        ss >> node.id;
        ss.ignore(1); // カンマをスキップ
        std::getline(ss, node.parent_type, ',');
        ss >> node.parent_id;
        ss.ignore(1); // カンマをスキップ
        ss >> node.length;
        ss.ignore(1); // カンマをスキップ
        ss >> node.radius;
        ss.ignore(1); // カンマをスキップ
        ss >> node.dir_x;
        ss.ignore(1); // カンマをスキップ
        ss >> node.dir_y;
        ss.ignore(1); // カンマをスキップ
        ss >> node.dir_z;
        ss.ignore(1); // カンマをスキップ
        ss >> node.distance;

        nodes.push_back(node);
    }

    return nodes;
}

int main() {
    // CSVファイルを読み取る
    std::string filename = "sample.csv";
    std::vector<Node> nodes = readCSV(filename);

    // ノードの情報を表示する
    for (const auto& node : nodes) {
        std::cout << "Type: " << node.type << ", ID: " << node.id 
                  << ", Parent Type: " << node.parent_type << ", Parent ID: " << node.parent_id 
                  << ", Length: " << node.length << ", Radius: " << node.radius 
                  << ", Direction: (" << node.dir_x << ", " << node.dir_y << ", " << node.dir_z << ")"
                  << ", Distance: " << node.distance << std::endl;
    }

    return 0;
}
