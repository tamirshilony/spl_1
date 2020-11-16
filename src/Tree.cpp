//
// Created by spl211 on 05/11/2020.
//

#include "../include/Tree.h"
#include "../include/Session.h"


Tree::Tree(int rootLabel):node(rootLabel),children(){}

//Rule of five
Tree::Tree(const Tree &other):node(other.node), children() {
    int otherNChildren = other.children.size();
    for (int i = 0; i < otherNChildren; ++i) {
        children.push_back(other.children[i]->clone());
    }
}
Tree & Tree::operator=(const Tree &other) {
    if (this != &other){
        node = other.node;
        int nChild = children.size();
        for (int i = 0; i < nChild; ++i) {
            delete children[i];
        }
        int otherNChild = other.children.size();
        for (int i = 0; i < otherNChild; ++i) {
            children.push_back(other.children[i]->clone());
        }
    }
    return *this;
}
Tree::~Tree() noexcept {
    if (!children.empty()) {
        int nChild = children.size();
        for (int i = 0; i < nChild; ++i) {
            delete children[i];
        }
    }
}
Tree & Tree::operator=(Tree &&other) {
    if (this != &other){
        node = other.node;
        int nChild = children.size();
        for (int i = 0; i < nChild; ++i) {
            delete children[i];
        }
        children = other.children;
        int otherNChild = other.children.size();
        for (int i = 0; i < otherNChild; ++i) {
            other.children[i] = nullptr;
        }
    }
    return *this;
}

Tree::Tree(Tree &&other):node(other.node),children(other.children) {
    int otherNChild =other.children.size();
    for (int i = 0; i < otherNChild; ++i) {
        other.children[i] = nullptr;
    }
}

void Tree::addChild(const Tree &child) {
    Tree *toAdd = child.clone();
    children.push_back(toAdd);
}
Tree* Tree::createTree(const Session& session, int rootLabel){
    TreeType treeType = session.getTreeType();
    switch (treeType) {
        case Cycle:
            return new CycleTree(rootLabel, session.getCycle());
        case Root:
            return new RootTree(rootLabel);
        default:
            return new MaxRankTree(rootLabel);
    }
}

int Tree::getNode() const {
    return node;
}
const std::vector<Tree *> Tree::getChildren() const {
    return children;
}


//CYCLE TREE
CycleTree::CycleTree(int rootLabel, int currCycle):Tree(rootLabel), currCycle(currCycle){}
CycleTree::CycleTree(const CycleTree &other): Tree(other), currCycle(other.getCycle()) {
}
Tree *CycleTree::clone() const {
    return new CycleTree(*this);
}
int CycleTree::traceTree(){
    return this->recursiveTrace(currCycle);
}

int CycleTree::recursiveTrace(int stepsLeft) {
    std::vector<Tree*> children = this->getChildren();
    if(children.empty() or stepsLeft == 0){
        return Tree::getNode();
    }
    CycleTree *leftChild = dynamic_cast<CycleTree *>(children[0]);
    return leftChild->recursiveTrace(stepsLeft-1);
}
int CycleTree::getCycle() const {
    return currCycle;
}


//MAX RANK TREE
MaxRankTree::MaxRankTree(int rootLabel): Tree(rootLabel){}
MaxRankTree::MaxRankTree(const MaxRankTree &other):Tree(other) {}
Tree *MaxRankTree::clone() const {
    return new MaxRankTree(*this);
}
int MaxRankTree::traceTree(){
    int maxRank = 0;
    return this->recursiveTrace(this->getNode(),maxRank);
}
int MaxRankTree::recursiveTrace(int maxNode ,int &maxRank) {
    std::vector<Tree*> children = this->getChildren();
    int nChild = children.size();
    if(!nChild)
        return maxNode;
    for(int i = 0; i < nChild; i++) {
        MaxRankTree *nextChild = dynamic_cast<MaxRankTree *>(children[i]);
        if (nChild > maxRank) {
            maxRank = nChild;
            maxNode = this->getNode();
        }
        maxNode = nextChild->recursiveTrace(maxNode, maxRank);
    }
    return maxNode;
}


//ROOT TREE
RootTree::RootTree(int rootLabel):Tree(rootLabel) {}
RootTree::RootTree(const RootTree &other): Tree(other) {}
Tree *RootTree::clone() const {
    return new RootTree(*this);
}
int RootTree::traceTree() {
    return this->getNode();
}