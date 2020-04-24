//
//  vertex.hpp
//  maze
//
//  Created by Xiangyu Yu on 4/21/20.
//  Copyright © 2020 Xiangyu Yu. All rights reserved.
//

#ifndef vertex_hpp
#define vertex_hpp

#include <stdio.h>
#include <tuple>
#include <vector>
#include <memory>
//vertex class
class vertex{
    public:
    //row and column index of a vertex
    int row;
    int column;
    //value (steps) of a vertex
    int value;
    //potential 2 directions,
    //vertical/diagonal or diagonal,
    //first element of array is the direction of first time visit
    //second element of array is the direction of second time visit
    //the direction of two times has to to different
    int direction[2]={-1,-1};
    //two parents, one for diagonal the other for vertical/horizontal
    //store the information for back tracing
    std::shared_ptr<vertex> parent[2]={NULL,NULL};
    //adjacency list for two directions
    std::vector<std::shared_ptr<vertex>> vh_neib;
    std::vector<std::shared_ptr<vertex>> d_neib;
    
  
    vertex(int r, int c, int v):row(r),column(c),value(v){};

};

#endif /* vertex_hpp */
