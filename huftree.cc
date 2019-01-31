#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <assert.h>

using namespace std;
struct treeNode{
    treeNode* left;
    treeNode* right;
    int freq;
    int ch;
};

void printChar( int ch ) {
    if( ch < 16 ) {
        std::cout << "x0" << std::hex << ch;
    } else if( ch < 32 || ch > 126 ) {
        std::cout << "x" << std::hex << ch;
    } else {
        std::cout << "\"" << (char)ch << "\"";
    }
}



void printTreeHelper(treeNode* r, std::string pre){
    if(r==NULL){return;}
    if(r->right == NULL){
        //implies r->left ==NULL
        printChar(r->ch);
        std::cout<<std::endl;
    }else{
        std::cout<< "."<< std::endl;
        std::cout<< pre<< "|`1-";
        printTreeHelper(r->right,pre+"|   ");
        std::cout<< pre+"|   " << std::endl;
        std::cout<< pre << "`-0-";
        printTreeHelper(r->left,pre + "   ");
    }
}

void printCodeHelper(treeNode* r, std::string pre){
    if (r==NULL){return;}
    if (r->right==NULL){
        printChar(r->ch);
        std::cout<<": "<< pre<<std::endl;
    }
    else{
        printCodeHelper(r->left, pre+"0");
        printCodeHelper( r->right, pre+"1");
        
    }
}


struct CodeTree{
    treeNode* root;
    void printTree(){
        printTreeHelper(root,"");
    }
    
    void printCode(){
        printCodeHelper(root, "");
    }
};

//PRE: subtrees rooted at leftChild and rightChild of i are heaps
//Post: subtree rooted at is a heap

void swapDown(vector<treeNode*> &heap, int i){
    int leftChild = 2*i + 1;
    int rightChild = 2*i + 2;
    int min = i;
    // find who holds smallest element of i and its two children
    if (leftChild<heap.size()&&heap[leftChild]->freq <heap[min]->freq){
        min = leftChild;
    }
    if (rightChild<heap.size()&&heap[rightChild]->freq <heap[min]-> freq){
        min = rightChild;
    }
    //if a child holds smallest element, swap i's element to that child and recurse
    if (min != i){
        swap(heap[i],heap[min]);
        swapDown(heap,min);
    }
}

//PRE: The capacity of the array pointed to by heap is at least size
//POST: The first size elements of heap are a heap
void heapify (vector<treeNode*> &heap){
    for (int i = (heap.size()- 2)/2; i>= 0; i--){
        swapDown(heap,i);
    }
}

void swapUp(vector<treeNode*> &heap, int i){
    if (i==0) return;
    int parent= (i-1)/2;
    
    if (heap[parent]->freq > heap[i]->freq){
        treeNode* temp = heap[i];
        heap[i] = heap[parent];
        heap[parent] = temp;
        swapUp(heap,parent);
    }
}

vector<treeNode*> pq (int freq[]){
    vector<treeNode*> heap;
    for(int i=0; i<256; i++){
        if (freq[i] != 0){
            treeNode* node = new treeNode;
            node->left = NULL;
            node->right = NULL;
            node ->freq = freq[i];
            node ->ch = i;
            heap.push_back(node);
            node =NULL;
            delete node;
        }
    }
    heapify(heap);
    return heap;
}

treeNode* deleteMin(vector<treeNode*> &heap){
    assert(!heap.empty());
    treeNode* node = heap[0];
    heap[0] = heap[heap.size()-1];
    heap.resize(heap.size()-1);
    swapDown(heap,0);
    return node;
}

void insert(vector<treeNode*> &heap, treeNode* x){
    heap.push_back(x);
    swapUp(heap, heap.size()-1);
}

CodeTree CodeTree(int freq[]){
    vector<treeNode*> heap = pq(freq);
    while (heap.size()>1){
        treeNode* parent_node = new treeNode;
        treeNode* node1= new treeNode;
        treeNode* node2= new treeNode;
        node1 = deleteMin(heap);
        node2 = deleteMin(heap);
        parent_node->left = node1;
        parent_node->right= node2;
        parent_node->freq = node1->freq + node2->freq;
        parent_node->ch = 0;
        insert(heap,parent_node);
        parent_node = NULL;
        node1= NULL;
        node2 = NULL;
        delete parent_node;
        delete node1;
        delete node2;
        
    }
    struct CodeTree ct;
    ct.root = heap[0];
    return ct;
}















// Print the Huffman code tree and list of codes for a given text file.
 
void usage() {
  std::cerr << "Usage: huftree text.txt" << std::endl;
  throw std::exception();
}

int main( int argc, char *argv[] ) {
  if( argc != 2 ) {
    usage();
  }

  int freq[256] = { 0 };	// array initialized to 0
  
  std::ifstream fin( argv[1] );
  if( fin.is_open() ) {
    char ch;
    while (fin >> std::noskipws >> ch) {	// don't skip whitespace
      freq[(int)ch]++;
    }

    fin.close();
  }
  
struct  CodeTree ct = CodeTree( freq );

  ct.printTree();
  ct.printCode();

  return 0;
}
