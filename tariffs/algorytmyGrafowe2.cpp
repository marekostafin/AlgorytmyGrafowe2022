#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <queue>
#include "graph.hpp"

int main(int argc, char *argv[]){
    if(argc < 2){
        std::cerr << "Nie wprowadzono wymaganych argumentow wejscia." << std::endl;
        return 1;
    }

    std::string fileName = argv[1];

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
        g.listArray[i-2].v.label = std::to_string(i-1);
        g.listArray[i-2].v.price = std::stof(buf);
        g.listArray[i-2].v.tariff = (g.listArray[i-2].v.price)/2;
        g.listArray[i-2].v.vertexNumber = i-2;
        g.listArray[i-2].v.transmuteTo = "-";
    }
    std::string buf;
    while(std::getline(input, buf)){
        std::string token;
        std::stringstream ss;
        ss << buf;
        std::getline(ss, token, ' ');
        int from = std::stoi(token);
        std::getline(ss, token, ' ');
        int to = std::stoi(token);
        std::getline(ss, token, '\n');
        float transformationPrice = std::stof(token);

        Graph::vertex_iter src(g);
        Graph::vertex_iter dst(g);
        src.current = from - 1;
        dst.current = to - 1;

        auto edge = g.connect(src, dst);
        edge.current->e.weight = transformationPrice;
    }
    input.close();

    //Algorytm Floyda
    float transmutations[g.vertex_count()][g.vertex_count()];
    int next[g.vertex_count()][g.vertex_count()];

    for (int i = 0; i < g.vertex_count(); i++){
        for(int j = 0; j < g.vertex_count(); j++){
            transmutations[i][j] = std::numeric_limits<float>::max();
            next[i][j] = -1;
        }
    }

    Graph::edge_iter i = g.edges_begin();
    Graph::edge_iter e = g.edges_end();
    while(i!=e){
        transmutations[i.source().current][i.destination().current] = i.current->e.weight;
        next[i.source().current][i.destination().current] = i.destination().current;
        ++i;
    }

    for (int i = 0; i < g.vertex_count(); i++){
        transmutations[i][i] = 0;
        next[i][i] = i;
    }

    for(int i = 0; i < g.vertex_count(); i++){
        for (int v = 0; v < g.vertex_count(); v++){
            for(int w = 0; w < g.vertex_count(); w++){
                float d = transmutations[v][i] + transmutations[i][w];
                if(transmutations[v][w] > d){
                    transmutations[v][w] = d;
                    next[v][w] = next[v][i];
                }
            }
        }        
    }
    
    //Obliczam clo
    Graph::vertex_iter p = g.vertices_begin();
    Graph::vertex_iter q = g.vertices_end();
    while(p!=q){
        float lowestTariff = g.listArray[p.current].v.tariff;
        std::string transmuteTo = "-";
        for(int t = 0; t < g.vertex_count(); t++){
            float newTariff = transmutations[p.current][t] + g.listArray[t].v.tariff + transmutations[t][p.current];
            if(lowestTariff > newTariff){
                lowestTariff = newTariff;
                transmuteTo = g.listArray[t].v.label;
            }
        }
        g.listArray[p.current].v.transmutedPrice = lowestTariff;
        g.listArray[p.current].v.transmuteTo = transmuteTo;
        ++p;
    }

    //Wypisuje obliczone cla
    Graph::vertex_iter b = g.vertices_begin();
    Graph::vertex_iter f = g.vertices_end();
    while(b!=f){
        std::cout << "(cło: " << g.listArray[b.current].v.tariff << " d/lb) : ";

        std::string cmp1 = "-";
        std::string cmp2 = g.listArray[b.current].v.transmuteTo;
        if(cmp1 == cmp2){
            std::cout << g.listArray[b.current].v.label;
        }else{
            int from = b.current;
            int to = std::stoi(g.listArray[b.current].v.transmuteTo) - 1;
            std::queue<int> path;
            if(next[from][to] != -1){
                path.push(from);
                while(from != to){
                    from = next[from][to];
                    path.push(from);
                }
            }
            from = std::stoi(g.listArray[b.current].v.transmuteTo) - 1;
            to = b.current;
            if(next[from][to] != -1){
                while(from != to){
                    from = next[from][to];
                    path.push(from);
                }
            }
            while(!path.empty()){
                std::cout << g.listArray[path.front()].v.label;
                path.pop();
                if(!path.empty()){
                    std::cout << "-";
                }
            }
        }

        std::cout << " (cło i reagenty: " << g.listArray[b.current].v.transmutedPrice << " d/lb)" << std::endl;
        ++b;
    }
}