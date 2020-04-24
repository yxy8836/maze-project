//
//  main.cpp
//  maze
//
//  Created by Xiangyu Yu on 4/21/20.
//  Copyright © 2020 Xiangyu Yu. All rights reserved.
//

#include <iostream>
#include "vertex.hpp"
#include "graph.hpp"
#include <memory>
#include <string>
int main() {
    
    auto g=std::make_shared<graph>();
    std::string file_name="input.txt";
    g->read_file_build(file_name);
    g->build_adj_list();
    int last_dir=g->BFS();
    g->find_path(g->goal,last_dir);
    std::cout<<"completed!\n";
    return 0;
}
