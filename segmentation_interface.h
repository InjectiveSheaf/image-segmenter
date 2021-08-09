#ifndef SEGMENTATION_INTERFACE_H
#define SEGMENTATION_INTERFACE_H

#include "graph.h"
#include "math.h"
#include <QImage>

/*  Введённые классы:
 *      Metric - расстояние, т.е. модуль разности значений серого (0-255).
 *      Merge - решает, сливать ноды, или нет.
 *      GraphSegmentation - интерфейс нашей сегментации. Построение графа, начальная сегментация и закрепление минимального размера сегмента. Вывод Label, т.е. изображения самих компонент.
 *      Методы класса GraphSegmentation описаны в файле segmentation_interface.cpp. */

class Metric {
public:
    Metric() {} // конструктор
    virtual ~Metric() {} // деструктор
    virtual float operator()(const Node &First_Node, const Node &Second_Node){
        float dist = std::abs(First_Node.Gray_Value - Second_Node.Gray_Value);
        return dist;
    }
};

class Merge{
public:
    Merge() {}
    Merge(float c) : c(c) {}
    virtual bool operator()(const Node & First_Node, const Node &Second_Node, const Edge & edge) {
        float Threshold = std::min(First_Node.Max_Weight + c/First_Node.Node_Size, Second_Node.Max_Weight + c/Second_Node.Node_Size);
        if (edge.Edge_Weight < Threshold){
            return true;
        }
        return false;
    }
private:
    float c;
};

class GraphSegmentation {
public:
    GraphSegmentation() {}
    GraphSegmentation(Metric* _metric, Merge* _Merge_Threshold) {
        metric = _metric;
        Merge_Threshold = _Merge_Threshold;
    }
    virtual ~GraphSegmentation() {}

    void buildGraph(const QImage &image);
    void GraphPrimarySegmentation(); // начальная сегментация графа
    void MinimumSegmentSize(uint M); // закрепляем минимальный размер сегмента
    QImage GetLabel();

protected:
    int Height; // высота изображения
    int Width; // ширина изображения
    graph Graph; // граф изображения
    Metric* metric; // используемая метрика
    Merge* Merge_Threshold; // пороговая часть сегментации ()
};

#endif // SEGMENTATION_INTERFACE_H
