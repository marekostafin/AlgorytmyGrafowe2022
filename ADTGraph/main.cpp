#include <iostream>
#include <string>
#include "graph.hpp"
using namespace std;

int main() {
    Graph g(15);

    for(int i = 0 ; i < 15 ; i ++) {
        g.listArray[i].v.label = to_string(i);
    }

    Graph::vertex_iter src(g);
    Graph::vertex_iter dst(g);

    for(int i = 0 ; i < 14 ; i ++ ) {
        src.current = i;
        dst.current = i+1;
        g.connect(src,dst);
    }

    src.current = 2;
    dst.current = 5;
    g.connect(src,dst);

    src.current = 10;
    dst.current = 2;
    g.connect(src,dst);
    g.connect(src,dst);

    auto i = g.vertices_begin();
    auto e = g.vertices_end();

    cout << "WIERZCHOLKI\n";

    while(i != e) {
        cout << (*i).label << endl;
        ++i;
    }

    auto p = g.edges_begin();
    auto u = g.edges_end();

    cout << "KRAWEDZIE\n";

    while(p != u) {
        cout << g.listArray[p.source().current].v.label;
        cout << " ";
        cout << g.listArray[p.destination().current].v.label << endl;
        ++p;
    }

    Graph::vertex_iter v(g);
    v.current = 2;
    auto j = v.outedges_begin();
    auto k = v.outedges_end();

    cout << "KRAWEDZIE OD 2\n";

    while(j != k) {
        cout << g.listArray[j.source().current].v.label << " " << g.listArray[j.destination().current].v.label << endl;
        ++j;
    }

    auto m = v.inedges_begin();
    auto n = v.inedges_end();

    cout << "KRAWEDZIE DO 2\n";

    while(m != n) {
        cout << g.listArray[m.source().current].v.label << " " << g.listArray[m.destination().current].v.label << endl;
        ++m;
    }

    g.disconnect(src,dst);
    g.disconnect(src,dst);

    auto t = g.edges_begin();
    auto y = g.edges_end();

    cout << "KRAWEDZIE\n";

    while(t != y) {
        cout << g.listArray[t.source().current].v.label << " " << g.listArray[t.destination().current].v.label << endl;
        ++t;
    }

    auto q = v.inedges_begin();
    auto r = v.inedges_end();

    cout << "KRAWEDZIE DO 2\n";

    while(q != r) {
        cout << g.listArray[q.source().current].v.label << " " << g.listArray[q.destination().current].v.label << endl;
        ++q;
    }

    if(!g.are_connected(src,dst)) {
        cout << "10 i 2 nie sa polaczone\n";
    }

    return 0;
}