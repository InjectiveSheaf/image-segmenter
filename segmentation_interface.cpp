#include <QImage>
#include "graph.h"
#include "segmentation_interface.h"

/* Методы класса GraphSegmentation:
 *      buildGraph - строит граф.
 *      oversegmentGraph - первичная сегментация графа. Сегментация производится по алгоритму Фельценсвальба и Гёттенлокера.
 *      Алгоритм:
 *          Отсортировать множество рёбер в последовательность по неубыванию их весов
 *          Начиная с сегментации, где каждый пиксель - своя собственная компонента:
 *              Пусть вершина V_i и V_j соединены ребром O_q
 *              Если V_i и V_j в разных компонентах связности и вес O_q мал по сравнению с разницей весов компонент, то сливаем их,
 *              Иначе - ничего не делаем
 *          Повторить для всего множества рёбер
 *      EnforceMinimumSegmentSize - сортируем рёбра по весу ещё раз, сливаем, если размеры обеих нод меньше порога.
 *      Благодаря сортировке мы сливаем наиболее близкие по весу рёбра.
 *      GetLabel - раскраска и получение изображения. */

void GraphSegmentation::buildGraph(const QImage &image) {
    Height = image.height();
    Width = image.width();

    int N = Width*Height;
    Graph = graph(N);

    for(int y = 0; y < Height; y++) {
        for(int x = 0; x < Width; x++) {
            int n = Width*y + x;
            Node &node = Graph.Get_Node(n);
            node.Gray_Value = qGray(image.pixel(x, y));
            node.Node_Label = n;
            node.Node_Id = n;
            node.Node_Size = 1;
        }
    }

    for(int y = 0; y < Height; y++) {
        for(int x = 0; x < Width; x++) {
            int n = Width*y + x;
            const Node &node = Graph.Get_Node(n);
            if(y < Height - 1) {
                int m = Width*(y+1) + x;
                Node &other = Graph.Get_Node(m);
                Edge edge;
                edge.FirstNode = n;
                edge.SecondNode = m;
                edge.Edge_Weight = (*metric)(node, other);
                Graph.Add_Edge(edge);
            }
            if(x < Width - 1) {
                int m = Width*y + (x+1);
                Node &other = Graph.Get_Node(m);
                Edge edge;
                edge.FirstNode = n;
                edge.SecondNode = m;
                edge.Edge_Weight = (*metric)(node, other);
                Graph.Add_Edge(edge);
            }
        }
    }
}

void GraphSegmentation::GraphPrimarySegmentation() {
    Graph.Sort_Edges();
    for(int e = 0; e < Graph.Edges_Number(); e++) {
        Edge edge = Graph.Get_Edge(e % Graph.Edges_Number());

        Node &a = Graph.Get_Node(edge.FirstNode);
        Node &b = Graph.Get_Node(edge.SecondNode);

        Node &A = Graph.Find_Component(a);
        Node &B = Graph.Find_Component(b);

        if (A.Node_Id != B.Node_Id) {
            if ((*Merge_Threshold)(A, B, edge)) {
                Graph.merge(A, B, edge);
            }
        }
    }
}

void GraphSegmentation::MinimumSegmentSize(uint M) {
    Graph.Sort_Edges();
    for(int e = 0; e < Graph.Edges_Number(); e++) {
        Edge edge = Graph.Get_Edge(e);

        Node &a = Graph.Get_Node(edge.FirstNode);
        Node &b = Graph.Get_Node(edge.SecondNode);

        Node &A = Graph.Find_Component(a);
        Node &B = Graph.Find_Component(b);

        if (A.Node_Label != B.Node_Label) {
            if (A.Node_Size < M || B.Node_Size < M) {
                Graph.merge(A, B, edge);
            }
        }
    }
}

QImage GraphSegmentation::GetLabel() {
    QImage Label(Width, Height, QImage::Format_RGB32);
    QColor NodeColor;
    for(int y = 0; y < Height; y++) {
        for(int x = 0; x < Width; x++) {
            int  n = Width*y + x;
            Node &a = Graph.Get_Node(n);
            Node &A = Graph.Find_Component(a);

            NodeColor = QColor::fromHsv(A.Gray_Value, A.Gray_Value, A.Gray_Value);
            Label.setPixelColor(x, y, NodeColor);
        }
    }
    return Label;
}
