#include <iostream>
#include <queue>
#include "graph.hpp"

void drawRight(Graph::vertex_iter i, Graph &g){
    Graph::vertex_iter right = Graph::vertex_iter(g);
    right.current = i.current + 1;
    if(!g.are_connected(i,right)){
        std::cout << " ";
    }else{
        std::cout << "|";
    }
}

void drawDown(Graph::vertex_iter i, Graph &g, std::queue<char> &buf, int cols){
    Graph::vertex_iter down = Graph::vertex_iter(g);
    down.current = i.current + cols;
    if(!g.are_connected(i,down)){
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

int main(int argc, char *argv[]){
    if(argc < 2){
        std::cerr << "Nie wprowadzono wymaganych argumentow wejscia." << std::endl;
        return 1;
    }

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
    }

    

    draw(rows, cols, maze);
}