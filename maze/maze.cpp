#include <iostream>
#include <queue>
#include <stack>
#include <set>
#include "graph.hpp"

void drawRight(Graph::vertex_iter vertex, Graph &g){
    // Graph::vertex_iter right = Graph::vertex_iter(g);
    // right.current = i.current + 1;
    // if(!g.are_connected(i,right)){
    //     std::cout << " ";
    // }else{
    //     std::cout << "|";
    // }

    Graph::outedge_iter right = vertex.outedges_begin();
    while(right.current->v.label != std::to_string(vertex.current + 1)){
        ++right;
    }
    if(right.current->e.wall == false){
        std::cout << " ";
    }else{
        std::cout << "|";
    }
}

void drawDown(Graph::vertex_iter vertex, Graph &g, std::queue<char> &buf, int cols){
    // Graph::vertex_iter down = Graph::vertex_iter(g);
    // down.current = i.current + cols;
    // if(!g.are_connected(i,down)){
    //     buf.push(' ');
    // }else{
    //     buf.push('-');
    // }
    Graph::outedge_iter down = vertex.outedges_begin();
    while(down.current->v.label != std::to_string(vertex.current + cols)){
        ++down;
    }
    if(down.current->e.wall == false){
        buf.push(' ');
    }else{
        buf.push('-');
    }
}

void draw(int rows, int cols, Graph g){
    //  Wypisuję pierwszą od góry ścianę labiryntu, 
    //  która zależy jedynie od rozmiaru labiryntu
    for(int i = 0; i < cols; i++){
        std::cout << "+-";
    }
    std::cout << "+" << std::endl;

    //  Dzielę "pokoje" labiryntu (wierzchołki grafu) 
    //  na te znajdujące się w pierwszym oraz w pozostałych wierszach
    std::queue<char> buf;
    Graph::vertex_iter i = g.vertices_begin();
    Graph::vertex_iter e = g.vertices_end();
    while(i!=e){
        if(i.current % cols == 0){
            if(!buf.empty()){
                while(!buf.empty()){
                    std::cout << buf.front();
                    buf.pop();
                }
                std::cout << std::endl;
            }
        }
        if(i.current < cols){   //  Pierwsza linijka wierzchołków    
            if(i.current % cols == 0){
                //  Pokój najbardziej na lewo
                std::cout << "| ";
                drawRight(i, g);
                buf.push('+');
                drawDown(i, g, buf, cols);

            }else if(i.current % cols == cols - 1){
                //  Pokój najbardziej na prawo
                std::cout << " |" << std::endl;
                buf.push('+');
                drawDown(i, g, buf, cols);
                buf.push('+');
            }else{
                //  Pozostałe pokoje
                std::cout << " ";
                drawRight(i, g);
                buf.push('+');
                drawDown(i, g, buf, cols);
            }
            ++i;
            
        }else if(i.current > (cols * rows) - (cols + 1)){   //  Ostatnia linijka
            if(i.current % cols == 0){
                //  Pokój najbardziej na lewo
                std::cout << "| ";
                drawRight(i, g);
            }else if(i.current % cols == cols - 1){
                //  Pokój najbardziej na prawo
                std::cout << " |";
            }else{
                //  Pozostałe pokoje
                std::cout << " ";
                drawRight(i, g);
            }
            ++i;
            if(i.current % cols == 0){
                std::cout << std::endl;
                for(int t = 0; t < cols; t++){
                    std::cout << "+-";
                }
                std::cout << "+" << std::endl;
            }
        }else{  //  Każda następna linijka
            if(i.current % cols == 0){
                //  Pokój najbardziej na lewo
                std::cout << "| ";
                drawRight(i, g);
                buf.push('+');
                drawDown(i, g, buf, cols);
                buf.push('+');
            }else if(i.current % cols == cols - 1){
                //  Pokój najbardziej na prawo
                std::cout << " |";
                drawDown(i, g, buf, cols);
                buf.push('+');
            }else{
                //  Pozostałe pokoje
                std::cout << " ";
                drawRight(i, g);
                drawDown(i, g, buf, cols);
                buf.push('+');
            }
            ++i;
            if(i.current % cols == 0){
                std::cout << std::endl;
            }
        }
    }
    if(!buf.empty()){
        while(!buf.empty()){
            std::cout << buf.front();
            buf.pop();
        }
        std::cout << std::endl;
    }
}

template<typename S>
auto select_random(const S &s, size_t n) {
  auto it = std::begin(s);
  std::advance(it,n);
  return it;
}

int main(int argc, char *argv[]){
    if(argc < 2){
        std::cerr << "Nie wprowadzono wymaganych argumentow wejscia." << std::endl;
        return 1;
    }
    srand (time(NULL));
    int rows = std::stoi(argv[1]);
    int cols = std::stoi(argv[2]);

    //  Tworzę graf, który reprezentował będzie labirynt
    Graph maze(rows * cols);
    for(int i = 0; i < rows*cols; i++){
        maze.listArray[i].v.label = std::to_string(i);
        maze.listArray[i].v.visited = "white";
    }

    //  Ściany labiryntu odpowiadają krawędziom grafu
    //  Tzn. jeśli A->B oraz B<-A to w labiryncie fragmenty
    //  oznaczone tymi literami rozdzielone będą ścianą.
    {
        Graph::vertex_iter i = maze.vertices_begin();
        Graph::vertex_iter e = maze.vertices_end();
        Graph::vertex_iter src(maze);
        Graph::vertex_iter dst(maze);
        while(i!=e){
            if(i.current < cols){
                //  Pierwszy rząd
                if(i.current % cols == 0){
                    //  Pierwsza komórka z lewej
                    src.current = i.current;
                    dst.current = i.current + 1;
                    maze.connect(src, dst);
                    dst.current = i.current + cols;
                    maze.connect(src, dst);
                }else if(i.current % cols == (cols - 1)){
                    //  Ostatnia komórka z lewej
                    src.current = i.current;
                    dst.current = i.current - 1;
                    maze.connect(src, dst);
                    dst.current = i.current + cols;
                    maze.connect(src, dst);
                }else{
                    //  Pozostałe komórki
                    src.current = i.current;
                    dst.current = i.current + 1;
                    maze.connect(src, dst);
                    dst.current = i.current - 1;
                    maze.connect(src, dst);
                    dst.current = i.current + cols;
                    maze.connect(src, dst);
                }
            }else if(i.current > (cols * rows) - (cols + 1)){
                //  Ostatni rząd
                if(i.current % cols == 0){
                    //  Pierwsza komórka z lewej
                    src.current = i.current;
                    dst.current = i.current + 1;
                    maze.connect(src, dst);
                    dst.current = i.current - cols;
                    maze.connect(src, dst);
                }else if(i.current % cols == (cols - 1)){
                    //  Ostatnia komórka z lewej
                    src.current = i.current;
                    dst.current = i.current - 1;
                    maze.connect(src, dst);
                    dst.current = i.current - cols;
                    maze.connect(src, dst);
                }else{
                    //  Pozostałe komórki
                    src.current = i.current;
                    dst.current = i.current + 1;
                    maze.connect(src, dst);
                    dst.current = i.current - 1;
                    maze.connect(src, dst);
                    dst.current = i.current - cols;
                    maze.connect(src, dst);
                }
            }else{
                //  Pozostałe rzędy
                if(i.current % cols == 0){
                    //  Pierwsza komórka z lewej
                    src.current = i.current;
                    dst.current = i.current + 1;
                    maze.connect(src, dst);
                    dst.current = i.current + cols;
                    maze.connect(src, dst);
                    dst.current = i.current - cols;
                    maze.connect(src, dst);
                }else if(i.current % cols == (cols - 1)){
                    //  Ostatnia komórka z lewej
                    src.current = i.current;
                    dst.current = i.current - 1;
                    maze.connect(src, dst);
                    dst.current = i.current + cols;
                    maze.connect(src, dst);
                    dst.current = i.current - cols;
                    maze.connect(src, dst);
                }else{
                    //  Pozostałe komórki
                    src.current = i.current;
                    dst.current = i.current + 1;
                    maze.connect(src, dst);
                    dst.current = i.current - 1;
                    maze.connect(src, dst);
                    dst.current = i.current + cols;
                    maze.connect(src, dst);
                    dst.current = i.current - cols;
                    maze.connect(src, dst);
                }
            }
        ++i;
        }
        Graph::edge_iter p = maze.edges_begin();
        Graph::edge_iter k = maze.edges_end();
        while(p!=k){
            p.current->e.wall = true;
            ++p;
        }
    }

    //Tworzenie labiryntu
    {
        std::stack<int> s;
        Graph::vertex_iter w(maze);
        w.current = 0;
        maze.listArray[w.current].v.visited = "black";
        s.push(w.current);
        while(!s.empty()){
            w.current = s.top();
            s.pop();
            Graph::outedge_iter i = w.outedges_begin();
            Graph::outedge_iter e = w.outedges_end();
            std::set<int> neighbors;
            while(i!=e){
                if(maze.listArray[i.destination().current].v.visited == "white"){
                    neighbors.insert(i.destination().current);
                }
                ++i;
            }
            if(!neighbors.empty()){
                s.push(w.current);
                auto r = rand() % neighbors.size(); // not _really_ random
                auto x = *select_random(neighbors, r);
                Graph::outedge_iter witer = w.outedges_begin();
                while(maze.listArray[witer.destination().current].v.label != std::to_string(x)){
                    ++witer;
                }
                Graph::vertex_iter xv(maze);
                xv.current = x;
                Graph::outedge_iter xiter = xv.outedges_begin();
                while(maze.listArray[xiter.destination().current].v.label != std::to_string(w.current)){
                    ++xiter;
                }
                xiter.current->e.wall = false;
                witer.current->e.wall = false;
                maze.listArray[x].v.visited = "black";
                s.push(x);  
            }
        }
    }

    draw(rows, cols, maze);
}