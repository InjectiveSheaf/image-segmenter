#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <map>
#include <algorithm>

/* Введённые классы:
 * 1. Node - компонента связности графа.
 * Переменные в ней:
 * Gray_Value - канал серого, Node_Label - индекс компоненты связности, которой нода принадлежит, Node_Size, Node_Id и Max_Weight - размер, индекс и максимальный вес ребра в ноде.
 * 2. Edge - ребро между двумя нодами, обладает весом.
 * 3. Comparator - сравнение рёбер (нужно для std::sort, для этой же цели массивы нод и рёбер реализованы векторами)
 * 4. graph - граф.
 * Переменные в нём:
 *      Количество компонент связности, массив нод, массив рёбер.
 * Функции в нём:
 *      Set_Node(a, node) - устанавливает ноду node на индекс a;
 *      Add_Node(node) - добавляет ноду в вектор и прибавляет 1 к числу нод
 *      Add_Edge(edge) - добавляет ребро в вектор
 *      Get_Node и Get_Edge - получаем ноду и ребро по соответствующему номеру
 *      Nodes_Number, Edges_Number, Components_Number - получаем количество нод, рёбер и компонент связности графа соответственно
 *      Sort_Edges - стандартная сортировка вектора рёбер
 *      Find_Component - находим, к какой компоненте связности принадлежит нода
 *      Merge(First, Second, Edge) - сливаем две ноды ребром Edge */

class Edge{
public:
    Edge(){}
    unsigned long int FirstNode; // первая нода
    unsigned long int SecondNode; // вторая нода
    float Edge_Weight; // вес ребра между ними
};

class Comparator{
public:
    inline bool operator()(const Edge &First, const Edge Second){
        return (Second.Edge_Weight > First.Edge_Weight);
    }
};

class Node{
public:
    Node() : Max_Weight(0){}
    unsigned char Gray_Value;
    unsigned long int Node_Label;
    unsigned long int Node_Size;
    unsigned long int Node_Id;
    float Max_Weight;
};


class graph{
public:
    graph() {
        Component_Num = 0;
    }
    graph(int N) {
        Nodes = std::vector<Node>(N);
        Component_Num = N;
    }
    void operator=(const graph &Graph) {
        Nodes = Graph.Nodes;
        Edges = Graph.Edges;
        Component_Num = Graph.Component_Num;
    }

    void Set_Node(int a, Node &node) {
        Nodes[a] = node;
    }

    void Add_Node(Node &node) {
        Nodes.push_back(node);
        Component_Num++;
    }

    void Add_Edge(Edge &edge) {
        Edges.push_back(edge);
    }

    Node &Get_Node(int n) {
        return Nodes[n];
    }

    Edge &Get_Edge(int e) {
        return Edges[e];
    }

    int Nodes_Number() {
        return Nodes.size();
    }

    int Edges_Number() {
        return Edges.size();
    }

    int Components_Number() {
        return Component_Num;
    }

    void Sort_Edges() {
        std::sort(Edges.begin(), Edges.end(), Comparator());
    }

    Node &Find_Component(Node &a) {
        int Label = a.Node_Label;
        int Id = a.Node_Id;
        while (Label != Id){
            Id = Nodes[Label].Node_Id;
            Label = Nodes[Label].Node_Label;
        }
        Node &A = Nodes[Label];
        a.Node_Label = A.Node_Id;
        return A;
    }

    void merge(Node &Second, Node &First, Edge &edge){
        Second.Node_Label = First.Node_Id;
        First.Node_Size += Second.Node_Size;
        First.Max_Weight = std::max(std::max(First.Max_Weight, Second.Max_Weight), edge.Edge_Weight);
        Component_Num--;
    }

private:
    int Component_Num;
    std::vector<Node> Nodes;
    std::vector<Edge> Edges;

};

#endif // GRAPH_H
