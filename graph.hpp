//
//  graph.hpp
//  maze
//
//  Created by Xiangyu Yu on 4/21/20.
//  Copyright © 2020 Xiangyu Yu. All rights reserved.
//

#ifndef graph_hpp
#define graph_hpp

#include <stdio.h>
#include <vector>
#include "vertex.hpp"
#include <memory>
#include <unordered_map>
//graph class
class graph{
    public:
    //total rows and columns
    int trow;
    int tcol;
    //final goal vertex
    std::shared_ptr<vertex> goal;
    //a 2-D vector to store all vertex objects give by input file
    std::vector<std::vector<std::shared_ptr<vertex>>> vertices;
    //a map mapping how many times a vertex has been visited
    //only different directions outgoing will be counted, once for each
    //key value is the index of the vertes computed by "convert_index"
    std::unordered_map<int,int> vertex_visit_times;
    
    graph();
    //convert location i,j to a unique vertex index
    int convert_index(int i, int j);
    //compute the moving direction based on a vertex (child) value (positive or negative)
    //and its parent direction
    int move_direction(int parent_direct, std::shared_ptr<vertex> child);
    //compute the incoming direction based on a vertex (parent) value
    //and its child_direction
    int income_direction(int child_direct, std::shared_ptr<vertex> parent);
    //read file and build the 2-D vectors
    void read_file_build(std::string filename);
    //build adjacency list
    void build_adj_list();
    //perform BFS search for the goal:
    //which adjacency list to be looped is based on the direction (computed by move_direction)
    //a vertex can be visited at most twice, and each time the direction is stored
    //in vertex->direction array, the parent will be stored in vertex->parent array
    int BFS();
    //find the path from start to goal based on parents and incoming direction
    void find_path(std::shared_ptr<vertex> goal,int goal_income_dir);
};



#endif /* graph_hpp */
