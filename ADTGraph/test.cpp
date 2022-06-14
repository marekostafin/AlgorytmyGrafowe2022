//#include <cppunit/extensions/HelperMacros.h>
//
//#include <iostream>
//#include <string>
//#include "graph.hpp"
//
//class Ostafin01 : public CppUnit::TestFixture{
//    CPPUNIT_TEST_SUITE(Ostafin01);
//    CPPUNIT_TEST(testOstafin);
//    CPPUNIT_TEST_SUITE_END();
//
//    public:
//        void testOstafin() {
//            Graph graph(12);
//
//            for (int i = 0; i < 12; i++) {
//                graph.listArray[i].v.label = std::to_string('a' + i);
//            }
//
//            {
//                Graph::vertex_iter src(graph);
//                Graph::vertex_iter dst(graph);
//
//                src.current = 0;
//                dst.current = 1;
//                graph.connect(src, dst);
//                CPPUNIT_ASSERT(graph.are_connected(src, dst));
//
//                src.current = 1;
//                dst.current = 4;
//                graph.connect(src, dst);
//                CPPUNIT_ASSERT(graph.are_connected(src, dst));
//
//                src.current = 4;
//                dst.current = 9;
//                graph.connect(src, dst);
//                CPPUNIT_ASSERT(graph.are_connected(src, dst));
//
//                src.current = 9;
//                dst.current = 11;
//                graph.connect(src, dst);
//                CPPUNIT_ASSERT(graph.are_connected(src, dst));
//
//                src.current = 11;
//                dst.current = 10;
//                graph.connect(src, dst);
//                CPPUNIT_ASSERT(graph.are_connected(src, dst));
//
//                src.current = 10;
//                dst.current = 7;
//                graph.connect(src, dst);
//                CPPUNIT_ASSERT(graph.are_connected(src, dst));
//
//                src.current = 7;
//                dst.current = 2;
//                graph.connect(src, dst);
//                CPPUNIT_ASSERT(graph.are_connected(src, dst));
//
//                src.current = 2;
//                dst.current = 0;
//                graph.connect(src, dst);
//                CPPUNIT_ASSERT(graph.are_connected(src, dst));
//
//                src.current = 3;
//                dst.current = 6;
//                graph.connect(src, dst);
//                CPPUNIT_ASSERT(graph.are_connected(src, dst));
//
//                src.current = 6;
//                dst.current = 8;
//                graph.connect(src, dst);
//                CPPUNIT_ASSERT(graph.are_connected(src, dst));
//
//                src.current = 8;
//                dst.current = 5;
//                graph.connect(src, dst);
//                CPPUNIT_ASSERT(graph.are_connected(src, dst));
//
//                src.current = 5;
//                dst.current = 3;
//                graph.connect(src, dst);
//                CPPUNIT_ASSERT(graph.are_connected(src, dst));
//            }
//
//            Graph::vertex_iter src(graph);
//            Graph::vertex_iter dst(graph);
//            for (int i = 0; i < 11; i++) {
//                src.current = i;
//                dst.current = i + 1;
//                graph.connect(src, dst);
//                CPPUNIT_ASSERT(graph.are_connected(src, dst));
//            }
//
//                Graph::vertex_iter v(graph);
//            {
//                auto i = v.outedges_begin();
//                auto e = v.outedges_end();
//                int outedgeNum = 0;
//                while(i != e){
//                    outedgeNum++;
//                }
//                i.current->v.label
//                CPPUNIT_ASSERT(outedgeNum == 1);
//            }
//            {
//                auto i = v.inedges_begin();
//                i.current->v.label
//            }
//        }
//};
//
//int main(){
//
//}