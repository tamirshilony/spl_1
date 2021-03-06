//
// Created by spl211 on 05/11/2020.
//

#include "../include/Agent.h"

using namespace std;

//Agent
Agent::Agent(){
}

//Virus

//ContactTracer
ContactTracer::ContactTracer() {}

Agent * ContactTracer::clone() const{
    return new ContactTracer(*this);
}

void ContactTracer::act(Session& session) {
    //1. Dequeue next infected
    int nextInf = session.dequeueInfected();
    if(nextInf != -1) {
        //2. Create tree from graph
        Tree *t = session.createTreeBFS(nextInf);
        //3. Trace tree
        int toIsolate = t->traceTree();
        //4. Remove edges
        session.isolateNode(toIsolate);
        //5. Delete resources
        delete t;
    }
}


Virus::Virus(int nodeInd):nodeInd(nodeInd) {}

Agent * Virus::clone() const{
    return new Virus(*this);
}

void Virus::act(Session& session) {
    //1.Infect node
    if(!session.checkIfInfected(nodeInd))
        session.infectNode(nodeInd);
    //2. Get neighbors
    vector<int> neighbor = session.getValidNeighbors(nodeInd);
    //3. Choose next to infect
    if(neighbor.size() != 0) {
        int minNeighbor = getMin(neighbor); // need to be implemented
        //4. Create new virus
        Virus newVirus = Virus(minNeighbor);
        //5. Add to agent list
        session.addAgent(newVirus);
        session.addVirus(minNeighbor);
    }
}
int Virus::getMin(vector<int> vec){
    int min = vec.at(0);
    int vecLen = vec.size();
    for(int i = 1; i < vecLen; i++ ){
        if(min > vec.at(i)){
            min = vec.at(i);
        }
    }
    return min;
}
