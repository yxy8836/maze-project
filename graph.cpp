//
//  graph.cpp
//  maze
//
//  Created by Xiangyu Yu on 4/21/20.
//  Copyright © 2020 Xiangyu Yu. All rights reserved.
//

#include "graph.hpp"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <queue>
#include <tuple>
#include <deque>

//graph default constructor
graph::graph(){
    this->tcol=0;
    this->trow=0;
}
//unique index for each vertex
int graph::convert_index(int i, int j){
    return this->tcol*(i-1)+j;
}
//build 2-D vectors by looping all values
void graph::read_file_build(std::string filename){
    std::ifstream inputfile(filename);
    if(!inputfile){
        std::cout<<"No file found! \n";
        exit(EXIT_FAILURE);
    }
    inputfile>>this->trow>>this->tcol;
    int value_temp;
    //read file and build the matrix
    for(int i=1;i<=this->trow;i++){
        //create a vertex vector for current row
        std::vector<std::shared_ptr<vertex>> vertex_line;
        for(int j=1;j<=this->tcol;j++){
            //read file for current row
            inputfile>>value_temp;
            auto vertex_ij=std::make_shared<vertex>(i,j,value_temp);
            vertex_line.push_back(vertex_ij);
            int index=this->convert_index(i,j);

            this->vertex_visit_times[index]=0;
        }
        this->vertices.push_back(vertex_line);
    }
    return;
    
};
//build adjacency list
//only include potential neighbors within the matrix range
//two lists are built for each vertex, vertial/horizontal or diagonal
void graph::build_adj_list(){
    for(int i=0;i<this->trow;i++){
        for(int j=0;j<this->tcol;j++){
            //the three vertices lower than current level
            if(i+abs(this->vertices[i][j]->value)<=this->trow-1){
                this->vertices[i][j]->vh_neib.push_back
                (this->vertices[i+abs(this->vertices[i][j]->value)][j]);
                if(j+abs(this->vertices[i][j]->value)<=this->tcol-1){
                    this->vertices[i][j]->d_neib.push_back
                    (this->vertices[i+abs(this->vertices[i][j]->value)][j+abs(this->vertices[i][j]->value)]);
                }
                if(j-abs(this->vertices[i][j]->value)>=0){
                    this->vertices[i][j]->d_neib.push_back
                    (this->vertices[i+abs(this->vertices[i][j]->value)][j-abs(this->vertices[i][j]->value)]);
                }
            }
            
            //three vertices upper level
            if(i-abs(this->vertices[i][j]->value)>=0){
                this->vertices[i][j]->vh_neib.push_back
                (this->vertices[i-abs(this->vertices[i][j]->value)][j]);
                if(j+abs(this->vertices[i][j]->value)<=this->tcol-1){
                    this->vertices[i][j]->d_neib.push_back
                    (this->vertices[i-abs(this->vertices[i][j]->value)][j+abs(this->vertices[i][j]->value)]);
                }
                if(j-abs(this->vertices[i][j]->value)>=0){
                    this->vertices[i][j]->d_neib.push_back
                    (this->vertices[i-abs(this->vertices[i][j]->value)][j-abs(this->vertices[i][j]->value)]);
                }
            }
            
            //vertex on the right
            if(j+abs(this->vertices[i][j]->value)<=this->tcol-1){
                this->vertices[i][j]->vh_neib.push_back
                (this->vertices[i][j+abs(this->vertices[i][j]->value)]);
            }
            //vertex on the left
            if(j-abs(this->vertices[i][j]->value)>=0){
                this->vertices[i][j]->vh_neib.push_back
                (this->vertices[i][j-abs(this->vertices[i][j]->value)]);
            }
            
        }
    }
    return;
};

//next moving direction when visiting a vertex
//if a vertex is negative, then next direction will be different from incoming direction
//if a vertex is positive, then next direction will be same as incomming direction
int graph::move_direction(int parent_direct, std::shared_ptr<vertex> child){
    //0 is vertical/horizontal, 1 is diagonal
    if(child->value>0){
        return parent_direct;
    }
    else{
        return 1-parent_direct;
    }
}

//kowning a child_direction, we want the parent direction so that we can trace back
//similar with move_direction
int graph::income_direction(int child_direct, std::shared_ptr<vertex> parent){
    if(parent->value>0){
        return child_direct;
    }
    else{
        return 1-child_direct;
    }
};
//main searching function BFS
int graph::BFS(){
    //queue of a tuple, first element is the time of visiting a vertex, can be 0 or 1
    //second element is the vertex pointer
    std::queue<std::tuple<int,std::shared_ptr<vertex>>> q;
    
    //initialization for the starting vertex
    int start_index=this->convert_index(1,1);
    std::tuple<int,std::shared_ptr<vertex>> start_tuple=std::make_tuple(this->vertex_visit_times[start_index],this->vertices[0][0]);
    this->vertices[0][0]->direction[this->vertex_visit_times[start_index]]=this->move_direction(0, this->vertices[0][0]);
    this->vertex_visit_times[start_index]+=1;

    //push staring vertex tuple
    q.push(start_tuple);
    
    while(!q.empty()){
        //front is the current vertex at the front of the queue
        auto front=std::get<1>(q.front());
        
        //the first element of the tuple stores the times of visiting this vertex
        //with this times value, we can index which direction the next move will be
        int front_direction=front->direction[std::get<0>(q.front())];
        
        //if next direction is horizontal/vertial, add neighbors in vh_neib
        if(front_direction==0)
        {
            for(std::vector<std::shared_ptr<vertex>>::iterator it=front->vh_neib.begin();
                it!=front->vh_neib.end();it++)
            {
                //for each neighbor, get its row/column location and unique index
                int neib_row=(*it)->row;
                int neib_col=(*it)->column;
                int neib_index=this->convert_index(neib_row, neib_col);

                //based on the unique index, we know how many times this vertex has been visited
                //if it is the first time, proceed; if it is the second time and
                //next moving direction of this vertex is different from its first moving direction, then proceed;
                //this way, it can eliminate the inifinite looping of adding same neighbors
                if(vertex_visit_times[neib_index]==0||(vertex_visit_times[neib_index]==1 && this->move_direction(0,(*it))!=(*it)->direction[0])){
                    //store its parent (front)
                    (*it)->parent[0]=front;
                    if((*it)->value==0){
                        std::cout<<"found!"<<std::endl;
                        this->goal=*it;
                        return 0;
                        
                    }
                    
                    //store the moving direction of current time (0 or 1)
                    //insert into queue and increment the visit time
                    (*it)->direction[vertex_visit_times[neib_index]]=this->move_direction(0, (*it));
                    q.push(std::make_tuple(vertex_visit_times[neib_index],*it));
                    vertex_visit_times[neib_index]+=1;
                    
                }
            }
        }
        //if next direction is diagonal, add neighbors in d_neib
        else if(front_direction==1) {
            for(std::vector<std::shared_ptr<vertex>>::iterator it=front->d_neib.begin();
                it!=front->d_neib.end();it++)
            {
                int neib_row=(*it)->row;
                int neib_col=(*it)->column;
                int neib_index=this->convert_index(neib_row, neib_col);
                
                
                if(vertex_visit_times[neib_index]==0||(vertex_visit_times[neib_index]==1 && this->move_direction(1, (*it))!=(*it)->direction[0])){
                    
                    (*it)->parent[1]=front;
                    if((*it)->value==0){
                        std::cout<<"found!"<<std::endl;
                        this->goal=*it;
                        return 1;
                       
                    }
                    (*it)->direction[vertex_visit_times[neib_index]]=this->move_direction(1, (*it));
                    
                    q.push(std::make_tuple(vertex_visit_times[neib_index],*it));
                    this->vertex_visit_times[neib_index]+=1;
                    
                    
                }
            }
        }
        //std::cout<<front->row<<front->column<<std::endl;

        q.pop();
        
    }
    std::cout<<"Not found goal"<<std::endl;
    return -1;
}

void graph::find_path(std::shared_ptr<vertex> goal, int goal_income_dir){
    
    //use a deque to store path
    std::deque<std::tuple<int,int>> path;
    std::shared_ptr<vertex> temp;
    temp=goal;
    
    //store goal pointer first and find its parent
    //based on the incoming direction to the goal
    int child_dir=goal_income_dir;
    path.push_back(std::make_tuple(temp->row,temp->column));
    temp=temp->parent[child_dir];
    
    //looping to add each vertex in the path into deque until the start is reached
    //and it was the first time when start was visted during BFS
    while(true){
        //determine which direction to keep searching the parent
        int parent_dir=this->income_direction(child_dir, temp);
        path.push_back(std::make_tuple(temp->row,temp->column));
        temp=temp->parent[parent_dir];
        child_dir=parent_dir;
        
        if(temp->row==1&&temp->column==1&&child_dir==0){
            path.push_back(std::make_tuple(temp->row,temp->column));
            break;
        }
    }
    //output the result
    while(!path.empty()){
        std::cout<<"("<<std::get<0>(path.back())<<" "<<std::get<1>(path.back())<<") ";
        path.pop_back();
    }
    std::cout<<std::endl;
    return;

}

