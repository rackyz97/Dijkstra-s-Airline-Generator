#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>

#define INT_MAX 10000000

using namespace std;

class Node;
class Edge;

//A Node object in a graph which is used to represent a airport
class Node {
    public:
        string id;                          //airport's name which serves as an unique identifier of the Node
        int total_dist;                   //shortest distance to reach the Node from the starting point
        bool visited=false;         //indicator of whether the Node has been visited
        Node* prev;                     //pointer to the previous Node, which is the Node visited to before the current Node itself in a shortest path. i.e. for shortest path A->B->C, C's prev pointer will be B
        vector<Edge*> edge;     //a vector of pointers pf Edge objects. it is a directed graph problem so the vector only contains edge that starts from the particular Node itself. i.e. A->B will be in A's vector but not in B's.

        Node(string id){
            this->id=id;
            this->total_dist=INT_MAX;
            this->visited=false;
            this->prev=NULL;
        }
};
//An Edge object in a graph which is used to represent an airline
class Edge {
    public:
        Node* src;                      //the pointer of the Node of the source airport in a edge
        Node* dest;                    //the pointer of the Node of the destination airport in a edge
        int dist;                           //the distance of the particular edge, which represents the flight time

        Edge(Node* src, Node* dest, int dist){
            this->src=src;
            this->dest=dest;
            this->dist=dist;
            src->edge.push_back(this);
        }

};

// A comparator used to construct a priority queue for the Dijkstra's algorithm.
struct comp{
    bool operator()(const Node* lhs, const Node* rhs) const {
        return lhs->total_dist > rhs->total_dist;
    }
};

/*
    The dijkstra's algorithm that generate the shortest route of airlines.
    s: id of source airport
    d: id of destination airport
    nodes: vector that contains all the existing nodes, it is used to reset all the nodes's total distance and visited status through an iteration before each execution
    hm: an hash map that takes an airport's id(string) as a key and return the Node object representing the airport as the value. In this way the user can easily and quickly find a Node with the airport's id
*/
void dijkstra(string s, string d, vector<Node*> nodes, unordered_map<string, Node*> hm)
{
    Node* src=hm[s];
    Node* dest=hm[d];
    //reset all the nodes in the graph
    for(Node* n: nodes){
        n->total_dist=INT_MAX;
        n->visited=false;
    }
    //set the starting point, create the priority queue called min_heap, and push the starting point into the queue
    src->total_dist = 0;
    src->visited= true;
    priority_queue<Node*, vector<Node*>, comp> min_heap;
    min_heap.push(src);

/*
    while the priority queue is not empty, keep executing the following code:
    1. take away a Node from the priority queue and name it curr
    2. iterate through curr's vector of pointer of Edges that begin from curr. Inside each iteration:
        2a. we name the destination Node in an edge as next
        2b. we compare next's current total_dist(the shortest distance to reach next from the starting point)
                with the sum of curr's total distance and the distance of the edge (which is the shortest distance to reach next from the starting point but passes through curr)
    3. set the visited statues of curr to be true, so that we know it has been visited by the algorithm and we don't add it to the priority queue again
*/
    while (!min_heap.empty())
    {
        Node* curr = min_heap.top();
        min_heap.pop();

        for (Edge* e: curr->edge)
        {
            Node* next = e->dest;
            int dist = e->dist;

            if (!next->visited && (curr->total_dist + dist) < next->total_dist)
            {
                next->total_dist = curr->total_dist + dist;
                next->prev=curr;
                min_heap.push(next);
            }
        }
        curr->visited=true;
    }

/*
    The following lines of code are used to display the shortest route of airlines to reach a destination point from the starting point.
*/
    Node* temp = dest;
    vector<string> route;
    while(temp!=NULL){
        route.push_back(temp->id);
        temp=temp->prev;
    }
    cout<<"Shortest route from "<<src->id<<" to "<<dest->id<<":"<<endl;
    for(int i=route.size()-1; i>0; i--) cout<<route[i]<<"->";
    cout<<route[0]<<endl;
    cout<<"Total distance from "<<src->id<<" to "<<dest->id<<":"<<endl;
    cout<<dest->total_dist<<endl;

}

/*
    function that creates and adds edges to a graph given airport ids, distance, and a hash map
    s: id of source airport
    d: id of destination airport
    dist: distance of the flight takes to travel
    hm: an hash map that takes an airport's id(string) as a key and return the Node object representing the airport as the value. In this way the user can easily and quickly find a Node with the airport's id
*/
void add_edge(string s, string d, int dist, unordered_map<string, Node*> hm){
    Edge* e = new Edge(hm[s], hm[d], dist);
}

/*
    a example test I created to test the algorithm
    in this example, I created a graph that contains nodes with names from a-g
    then I try to generate the shortest route from a to f at first
    then generate the shortest route from b to g
*/
void dijtest(){
    vector<string> node_names = {"a", "b", "c", "d", "e", "f", "g"};
    vector<Node*> nodes;
    unordered_map<string, Node*> hm;

    for(string name: node_names){
        Node* n = new Node(name);
        nodes.push_back(n);
        hm[name]=n;
    }

    add_edge("a","c", 1, hm);
    add_edge("a","d", 2, hm);
    add_edge("b","c", 2, hm);
    add_edge("c","d", 1, hm);
    add_edge("b","f", 3, hm);
    add_edge("c","e", 3, hm);
    add_edge("e","f", 2, hm);
    add_edge("d","g", 1, hm);
    add_edge("g","f", 1, hm);

    dijkstra("a","f", nodes, hm);
    dijkstra("b","g", nodes, hm);
}
int main() {
    dijtest();
    return 0;
}

