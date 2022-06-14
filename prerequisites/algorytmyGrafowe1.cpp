#include <iostream>
#include <fstream>
#include "graph.hpp"

int main(int argc, char *argv[]){
    if(argc < 3){
        std::cerr << "Nie wprowadzono wymaganych argumentow wejscia." << std::endl;
        return 1;
    }

    std::string fileName = argv[1];
    std::string checkedCourse = argv[2];

    int subjectNum = 0;

    //Przetwarzanie pliku
    std::ifstream input( fileName );
    if(!input){
        std::cerr << "Wprowadzony plik nie istnieje." << std::endl;
        return 1;
    }else{
        std::string buf;
        std::getline(input, buf);
        subjectNum = std::stoi(buf);
    }

    Graph g(subjectNum);
    for(int i = 2; i <= subjectNum + 1; i++){
        std::string buf;
        std::getline(input, buf);
        g.listArray[i-2].v.label = buf;
        g.listArray[i-2].v.vertexNumber = i-2;
    }
    std::string buf;
    std::string delim = " ";
    while(std::getline(input, buf)){
        std::string token = buf.substr(0, buf.find(delim));
        int from = std::stoi(token);
        token = buf.substr(buf.find(delim));
        int to = std::stoi(token);

        Graph::vertex_iter src(g);
        Graph::vertex_iter dst(g);
        src.current = from - 1;
        dst.current = to - 1;

        g.connect(src, dst);
    }

    {
        //Sprawdzam, czy wczytany graf zawiera przedmiot dla ktorego mamy znalezc semestr
        auto i = g.vertices_begin();
        auto e = g.vertices_end();
        while(i!=e){
            if(i.parentGraph.listArray[i.current].v.label == checkedCourse){
                break;
            }
            ++i;
        }
        if(i==e){
            std::cerr << "Graf nie zawiera podanego przedmiotu." << std::endl;
            return 1;
        }
    }
    input.close();

    //Wykonuje sortowanie topologiczne grafu
    std::stack<int> topologicalOrder = g.topologicalSort();

    //W kolejnosci topologicznej szukam najwieszej drogi do danego wierzcholka - konczac na tym, ktorego szukamy
    int semester;
    while(!topologicalOrder.empty()){
        semester = g.calculateSemesters(topologicalOrder.top());
        if(g.listArray[topologicalOrder.top()].v.label == checkedCourse){
            break;
        }
        topologicalOrder.pop();
    }
    std::cout << semester << std::endl;
    return 0;
}