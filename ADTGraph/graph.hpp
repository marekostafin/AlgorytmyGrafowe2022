#include <iostream>

class Graph{
    class vertex{
    public:
        std::string label;
    };

    class edge{
    public:
        double weight;
    };


    class edgeNode{
    public:
        vertex v;
        edge e;
        edgeNode *ptr;
    };

    class vertexNode{
    public:
        edgeNode *ptr;
        vertex v;
    };

    int vertex_num;

public:
    class vertex_iter;
    class edge_iter;
    class outedge_iter;
    class inedge_iter;

    vertexNode * listArray;

    Graph(int vertex_count);
    ~Graph();
    int vertex_count();
    int edge_count();
    edge_iter connect(vertex_iter src, vertex_iter dst);
    void disconnect(vertex_iter src, vertex_iter dst);
    bool are_connected(vertex_iter src, vertex_iter dst);
    vertex_iter vertices_begin();
    vertex_iter vertices_end();
    edge_iter edges_begin();
    edge_iter edges_end();
};

class Graph::vertex_iter {
public:
    Graph & parentGraph;
    int current = 0;
    vertex_iter(Graph& x);
    bool operator==(vertex_iter other);
    bool operator!=(vertex_iter other);
    vertex_iter & operator++();
    vertex & operator*();
    vertex * operator->();
    outedge_iter outedges_begin();
    outedge_iter outedges_end();
    inedge_iter inedges_begin();
    inedge_iter inedges_end();
};

class Graph::edge_iter {
public:
    int currentVertex;
    Graph & parent;
    edgeNode * current;
    edge_iter(Graph& x);
    bool operator==(edge_iter other);
    bool operator!=(edge_iter other);
    edge_iter & operator++();
    edge * operator->();
    vertex_iter source();
    vertex_iter destination();
};

class Graph::outedge_iter {
public:
    vertex_iter & parent;
    edgeNode * current;
    outedge_iter(vertex_iter & x);
    bool operator==(outedge_iter other);
    bool operator!=(outedge_iter other);
    outedge_iter & operator++();
    edge * operator->();
    vertex_iter source();
    vertex_iter destination();
};

class Graph::inedge_iter {
public:
    Graph & myGraph;
    int currentVertex;
    vertex_iter & myVertexIter;
    edgeNode * current;
    inedge_iter(Graph& x, vertex_iter& y);
    bool operator==(inedge_iter other);
    bool operator!=(inedge_iter other);
    inedge_iter & operator++();
    edge * operator->();
    vertex_iter source();
    vertex_iter destination();
};

/****************************************/


//GRAPH
Graph::Graph(int vertex_count){
    vertex_num = vertex_count;
    listArray = new vertexNode[vertex_count];
    for(int i = 0; i < vertex_count; i++){
        listArray[i].ptr = nullptr;
    }
}

Graph::~Graph(){
    delete[] listArray;
}

int Graph::vertex_count(){
    return vertex_num;
}

int Graph::edge_count(){
    int edges = 0;
    auto i = edges_begin();
    auto e = edges_end();
    while (i != e) {
        edges++;
        ++i;
    }
    return edges;
}


//VERTEX ITERATOR
Graph::vertex_iter::vertex_iter(Graph& x): parentGraph(x){}

bool Graph::vertex_iter::operator==(vertex_iter other){
    return current == other.current;
}

bool Graph::vertex_iter::operator!=(vertex_iter other){
    return current != other.current;
}

Graph::vertex_iter & Graph::vertex_iter::operator++(){
    current = current + 1;
    return *this;
}

Graph::vertex & Graph::vertex_iter::operator*(){
    return parentGraph.listArray[current].v;
}

Graph::vertex * Graph::vertex_iter::operator->(){
    return & parentGraph.listArray[current].v;
}

Graph::outedge_iter Graph::vertex_iter::outedges_begin() {
    outedge_iter outeges_begin_iter(*this);
    return outeges_begin_iter;
}

Graph::outedge_iter Graph::vertex_iter::outedges_end(){
    outedge_iter end(*this);
    end.current = nullptr;
    return end;
}

Graph::inedge_iter Graph::vertex_iter::inedges_begin(){
    inedge_iter inedges_begin_iter(parentGraph, *this);
    return inedges_begin_iter;
}

Graph::inedge_iter Graph::vertex_iter::inedges_end(){
    inedge_iter inedges_end_iter(parentGraph, *this);
    inedges_end_iter.current = nullptr;
    return inedges_end_iter;
}


//OUTEDGE ITERATOR
Graph::outedge_iter::outedge_iter(vertex_iter& x): parent(x), current((x.parentGraph.listArray[x.current].ptr)){}

bool Graph::outedge_iter::operator==(outedge_iter other){
    if(current == nullptr && other.current == nullptr){
        return true;
    }else if(current == nullptr && other.current != nullptr){
        return false;
    }else if(current != nullptr && other.current == nullptr) {
        return false;
    }else return current->ptr == other.current->ptr;
}

bool Graph::outedge_iter::operator!=(outedge_iter other){
    if(current == nullptr && other.current == nullptr){
        return false;
    }else if(current == nullptr && other.current != nullptr){
        return true;
    }else if(current != nullptr && other.current == nullptr){
        return true;
    }else return current->ptr != other.current->ptr;
}

Graph::outedge_iter & Graph::outedge_iter::operator++(){
    current = current->ptr;
    return *this;
}

Graph::edge * Graph::outedge_iter::operator->(){
    return & current->e;
}

Graph::vertex_iter Graph::outedge_iter::source(){
    return parent;
}

Graph::vertex_iter Graph::outedge_iter::destination(){
    vertex_iter destination_iter = vertex_iter(parent.parentGraph);
    while(destination_iter.parentGraph.listArray[destination_iter.current].v.label != current->v.label){
        ++destination_iter;
    }
    return destination_iter;
}


//INEDGE ITERATOR
Graph::inedge_iter::inedge_iter(Graph& x, vertex_iter& y): myGraph(x), myVertexIter(y), current(nullptr){
    edge_iter find = myGraph.edges_begin();
    while(find.current->v.label != myGraph.listArray[myVertexIter.current].v.label && find!=myGraph.edges_end()){
        ++find;
    }
    if(find!=myGraph.edges_end()){
        current = find.current;
        currentVertex = find.currentVertex;
    }
}

bool Graph::inedge_iter::operator==(inedge_iter other){
    if(current == nullptr && other.current == nullptr) return true;
    return current->v.label == other.current->v.label && currentVertex == other.currentVertex;
}

bool Graph::inedge_iter::operator!=(inedge_iter other){
    if((current == nullptr && other.current != nullptr) || (current != nullptr && other.current == nullptr)) {
        return true;
    }
    if(current == nullptr && other.current == nullptr) return false;
    return current->v.label != other.current->v.label || currentVertex == other.currentVertex;
}

Graph::inedge_iter & Graph::inedge_iter::operator++(){
    edge_iter find = edge_iter(myGraph);
    find.current = current;
    find.currentVertex = currentVertex;
    ++find;
    while(myGraph.edges_end() != find
          && find.current->v.label != myGraph.listArray[myVertexIter.current].v.label){
        ++find;
    }
    if(find!=myGraph.edges_end()){
        current = find.current;
        currentVertex = find.currentVertex;
        return *this;
    }else{
        current = nullptr;
        return *this;
    }

}

Graph::edge * Graph::inedge_iter::operator->(){
    return & current->e;
}

Graph::vertex_iter Graph::inedge_iter::source(){
    vertex_iter source_iter = vertex_iter(this->myGraph);
    source_iter.current = currentVertex;
    return source_iter;
}

Graph::vertex_iter Graph::inedge_iter::destination(){
    return myVertexIter;
}


//EDGE ITERATOR
Graph::edge_iter::edge_iter(Graph& x):currentVertex(0), parent(x), current(parent.listArray[0].ptr){
    auto i = parent.vertices_begin();
    auto e = parent.vertices_end();

    while(i != e) {
        if(parent.listArray[i.current].ptr != nullptr) {
            current = x.listArray[i.current].ptr;
            return;
        }
        ++i;
    }
}

bool Graph::edge_iter::operator==(edge_iter other){
    if(current == nullptr && other.current == nullptr){
        return true;
    }else if(current == nullptr && other.current != nullptr){
        return false;
    }else if(current != nullptr && other.current == nullptr) {
        return false;
    }else return current->v.label == other.current->v.label;
}

bool Graph::edge_iter::operator!=(edge_iter other){
    if(current == nullptr && other.current == nullptr){
        return false;
    }else if(current == nullptr && other.current != nullptr){
        return true;
    }else if(current != nullptr && other.current == nullptr){
        return true;
    }else return current->v.label != other.current->v.label;
}

Graph::edge_iter & Graph::edge_iter::operator++() {
    if(current != nullptr){
        if (current->ptr == nullptr && currentVertex != parent.vertex_num - 1) {
            currentVertex++;
            current = parent.listArray[currentVertex].ptr;
        } else if (current->ptr != nullptr) {
            current = current->ptr;
        } else if (current->ptr == nullptr && currentVertex == parent.vertex_num - 1) {
            current = nullptr;
            return *this;
        }
    }
    return *this;
}

Graph::edge * Graph::edge_iter::operator->(){
    return &current->e;
}

Graph::vertex_iter Graph::edge_iter::source(){
    vertex_iter source_iter = vertex_iter(parent);
    source_iter.current = currentVertex;
    return source_iter;
}

Graph::vertex_iter Graph::edge_iter::destination(){
    auto i = parent.vertices_begin();
    auto e = parent.vertices_end();
    while(i != e){
        if(parent.listArray[i.current].v.label == current->v.label){
            return i;
        }
        ++i;
    }
    return vertex_iter(parent);
}

//GRAPH
Graph::edge_iter Graph::connect(vertex_iter src, vertex_iter dst){
    edgeNode* newNode = new edgeNode();
    newNode->ptr = nullptr;
    newNode->v = this->listArray[dst.current].v;
    edge_iter newIter(*this);
    newIter.current = newNode;
    if(this->listArray[src.current].ptr == nullptr){
        this->listArray[src.current].ptr = newNode;
        return newIter;
    }else{
        if(!are_connected(src, dst)){
            newNode->ptr = this->listArray[src.current].ptr;
            this->listArray[src.current].ptr = newNode;
            return newIter;
        }else{
            auto i = src.outedges_begin();
            while(i.current->v.label != dst.parentGraph.listArray[dst.current].v.label){
                ++i;
            }
            newIter.current = i.current;
            newIter.currentVertex = i.parent.current;
            return newIter;
        }
    }
}

void Graph::disconnect(vertex_iter src, vertex_iter dst){
    if(!are_connected(src, dst)){
        return;
    }else{
        auto i = src.outedges_begin();
        auto e = src.outedges_end();
        outedge_iter previous = src.outedges_begin();

        if(i.current->v.label == dst.parentGraph.listArray[dst.current].v.label){
            src.parentGraph.listArray[src.current].ptr = nullptr;
        }else{
            ++i;
            while(i != e){
                if(i.current->v.label == dst.parentGraph.listArray[dst.current].v.label){
                    previous.current->ptr = i.current->ptr;
                    return;
                }
                ++i;
                ++previous;
            }
        }
    }
}

bool Graph::are_connected(vertex_iter src, vertex_iter dst){
    auto i = src.outedges_begin();
    auto e = src.outedges_end();
    while (i != e) {
        if(i.current->v.label == dst->label){
            return true;
        }
        ++i;
    }
    return false;
}

Graph::vertex_iter Graph::vertices_begin(){
    vertex_iter newIter(*this);
    newIter.current = 0;
    return newIter;
}

Graph::vertex_iter Graph::vertices_end(){
    vertex_iter newIter(*this);
    newIter.current = vertex_num;
    return newIter;
}

Graph::edge_iter Graph::edges_begin(){
    edge_iter newIter(*this);
    return newIter;
}

Graph::edge_iter Graph::edges_end(){
    edge_iter newIter(*this);
    newIter.currentVertex = vertex_num;
    newIter.current = nullptr;
    return newIter;
}