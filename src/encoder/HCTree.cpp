#include "HCTree.hpp"
#include <vector>
#include <iostream>
#include <string>
#include <string.h>
void deleteHelperMethod(HCNode* node){
    if(node->c0 != nullptr){
        deleteHelperMethod(node->c0);
    }
    else if(node->c1 != nullptr){
        deleteHelperMethod(node->c1);
    }
    delete node;
}
/* TODO: Delete all objects on the heap to avoid memory leaks. */
HCTree::~HCTree() {
    deleteHelperMethod(root);
}



/**
 * TODO: Build the HCTree from the given frequency vector. You can assume the
 * vector must have size 256 and each value at index i represents the frequency
 * of char with ASCII value i. Only non-zero frequency symbols should be used to
 * build the tree. The leaves vector must be updated so that it can be used in
 * encode() to improve performance.
 *
 * When building the HCTree, you should use the following tie-breaking rules to
 * match the output from reference solution in checkpoint:
 *
 *    1. HCNode with lower count should have higher priority. If count is the
 * same, then HCNode with a larger ascii value symbol should have higher
 * priority. (This should be already defined properly in the comparator in
 * HCNode.hpp)
 *    2. When popping two highest priority nodes from PQ, the higher priority
 * node will be the ‘c0’ child of the new parent HCNode.
 *    3. The symbol of any parent node should be taken from its 'c0' child.
 */
void HCTree::build(const vector<unsigned int>& freqs) {
    priority_queue<HCNode*, std::vector<HCNode*>, HCNodePtrComp>list;
    for(int i = 0; i <= 255; i++){
        if(freqs[i] != 0){
            HCNode* node = new HCNode(freqs[i], i, 0, 0, 0);
            list.push(node);
            leaves.push_back(node);
        }
    }
    while(list.size() >= 2){
        HCNode* childFirst = list.top();
        list.pop();
        HCNode* childSecond = list.top();
        list.pop();
        HCNode* parent = new HCNode(childFirst->count + childSecond->count, childFirst->symbol, childFirst, childSecond, 0);
        childFirst->p = parent;
        childSecond->p = parent;
        list.push(parent);
    }
    root = list.top();
}

/**
 * TODO: Write the encoding bits of the given symbol to the ostream. You should
 * write each encoding bit as ascii char either '0' or '1' to the ostream. You
 * must not perform a comprehensive search to find the encoding bits of the
 * given symbol, and you should use the leaves vector instead to achieve
 * efficient encoding. For this function to work, build() must be called before
 * to create the HCTree.
 */
// void HCTree::encode(byte symbol, BitOutputStream& out) const {}

/**
 * TODO: Write the encoding bits of the given symbol to ostream. You should
 * write each encoding bit as ascii char either '0' or '1' to the ostream. You
 * must not perform a comprehensive search to find the encoding bits of the
 * given symbol, and you should use the leaves vector instead to achieve
 * efficient encoding. For this function to work, build() must have been called
 * beforehand to create the HCTree.
 */
void HCTree::encode(byte symbol, ostream& out) const {
    string result = "";
    HCNode* bottomNode;
    for(int i = 0; i < leaves.size(); i++){
        if(leaves[i]->symbol == symbol){
            bottomNode = leaves[i];
        }
    }

    while(bottomNode->p != nullptr){
        HCNode* parentNode = bottomNode->p;
        if(parentNode -> c0 == bottomNode){
            result = "0" + result;
        }
        else if(parentNode -> c1 == bottomNode){
            result = "1" + result;
        }
        bottomNode = bottomNode->p;
    }


    out << result;
}

/**
 * TODO: Decode the sequence of bits (represented as a char of either '0' or
 * '1') from the istream to return the coded symbol. For this function to work,
 * build() must have been called beforehand to create the HCTree.
 */
// byte HCTree::decode(BitInputStream& in) const { return ' '; }

/**
 * TODO: Decode the sequence of bits (represented as char of either '0' or '1')
 * from istream to return the coded symbol. For this function to work, build()
 * must have been called beforehand to create the HCTree.
 */
byte HCTree::decode(istream& in) const { 
    
    HCNode* formalNode = root;
    char decodeC;
    if(root->c0 == nullptr){
        return root->symbol;
    }
    while(formalNode->c0 != nullptr || formalNode->c1 != nullptr){
        in.get(decodeC);
        if(decodeC == '0'){
            formalNode = formalNode->c0;
        }
        else{
            formalNode = formalNode->c1;
        }
    }
    return formalNode->symbol;
}
