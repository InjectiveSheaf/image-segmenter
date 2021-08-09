#ifndef DRAWER_H
#define DRAWER_H
#include <QImage>
#include <QColor>

/* Класс Drawer
 * Qimage Filter - медианный фильтр
 * Qimage Draw_Contours - рисует контуры цветом color
 * bool Is_boundary - определяет, находится ли пиксель на границе компоненты связности
 */

class Drawer{
public:
    QImage Filter(const QImage & Image);
    QImage Draw_Contours(const QImage &Label, const QImage &Original_Image, uint color);
protected:
    bool Is_Boundary(const QImage &Label, int x, int y);
};

#endif // DRAWER_H
