//
// Created by spl211 on 05/11/2020.
//

#include "../include/Graph.h"

using namespace std;

Graph::Graph(std::vector<std::vector<int>> matrix):edges(),infectedNode(),components(){
    edges =  matrix;
    infectedNode = vector<bool>(matrix.size(), false);
    components = findComponentsBFS();
}

void Graph::infectNode(int nodeInd) {
    infectedNode[nodeInd] = true;
}

bool Graph::isInfected(int nodeInd) const{
    return infectedNode[nodeInd];
}

std::vector<int> Graph::getNonInfNeighbors(int nodeInd,const vector<bool> &hasVirus) const{
    vector<int> neighbors;
    int nEdges = edges[nodeInd].size();
    for(int i =0;i<nEdges;i++){
        if (edges[nodeInd][i]==1 and !isInfected(i) and !hasVirus[i])
            neighbors.push_back(i);
    }
    return neighbors;
}

Tree * Graph::BFS(int nodeInd, const Session &session)const {
    Tree* currTree = Tree::createTree(session,nodeInd);
    Tree &toReturn = *currTree;
    //BFS implement
    vector<bool>visited(edges.size(), false);
    vector<Tree*>q;
    q.push_back(currTree);
    visited[nodeInd] = true;
    while (!q.empty()){
        currTree = q[0];
        int nodeInd = currTree->getNode();
        q.erase(q.cbegin());
        int nEdges = edges.size();
        for (int i = 0; i < nEdges; ++i) {
            if (edges[nodeInd][i]== 1 and (!visited[i])){
                visited[i] = true;
                Tree* nextTree = Tree::createTree(session,i);
                currTree->addChild(*nextTree);
                delete nextTree;
                q.push_back(currTree->getChildren().back());
            }
        }
    }
    return &toReturn;
}


void Graph::removeEdges(int nodeInd)  {
    int nEdges = edges[nodeInd].size();
    for (int i = 0; i < nEdges; ++i) {
        if (edges[nodeInd][i]==1) {
            edges[nodeInd][i] = 0;
            edges[i][nodeInd] = 0;
        }
    }
}
vector<vector<int>> Graph::findComponentsBFS() const{
    vector<vector<int>> componentMatrix;
    vector<bool>visited(edges.size(),false);
    int nEdges = edges.size();
    for (int i = 0; i < nEdges; ++i) {
        if (!visited[i]) {
            vector<int> currComp;
            currComp.push_back(i);
            vector<int> q;
            q.push_back(i);
            visited[i] = true;
            while (!q.empty()) {
                int k = q[0];
                q.erase(q.cbegin());
                int nEdges = edges.size();
                for (int j = 0; j < nEdges; j++) {
                    if (edges[k][j] == 1 and (!visited[j])) {
                        q.push_back(j);
                        visited[j] = true;
                        currComp.push_back(j);
                    }
                }
            }
            componentMatrix.push_back(currComp);
        }
    }
    return componentMatrix;
}

const vector<vector<int>> Graph::getEdges() const {
    return edges;
}

const vector<bool> Graph::getInfected() const {
    return infectedNode;
}

bool Graph::areCompsUniform(vector<bool> &hasVirus) {
    components = findComponentsBFS();
    bool ans = true;
    int nComponents = components.size();
    for (int i = 0; i < nComponents and ans; ++i) {
        if (!isCompValid(components[i],hasVirus))
            ans = false;
    }
    return ans;
}

bool Graph::isCompValid(vector<int> &comp,vector<bool> &hasVirus) const {
    bool ans =true;
    bool isFirstInf = isInfected(comp[0]);
    //Check for uniform infection status
    int compSize = comp.size();
    for (int i = 1; i < compSize and ans; ++i) {
        if (isInfected(comp[i]) != isFirstInf)
            ans = false;
    }
    //Check for uniform virus status
    if (ans and !isFirstInf)// if all nonInfected
        for (int i = 0; i < compSize and ans; ++i) {
            if (hasVirus[comp[i]])
                ans = false;
        }
    return ans;
}
