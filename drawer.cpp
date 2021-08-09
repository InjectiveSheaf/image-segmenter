#include <iostream>
#include <algorithm>
#include "drawer.h"

bool Drawer::Is_Boundary(const QImage &Label, int x, int y){
    if (y > 0) {
        if (qGray(Label.pixel(x, y)) != qGray(Label.pixel(x, y-1))) {
            return true;
        }
    }
    if (y < Label.height() - 1) {
        if (qGray(Label.pixel(x, y)) != qGray(Label.pixel(x, y+1))) {
            return true;
        }
    }
    if (x > 0) {
        if (qGray(Label.pixel(x, y)) != qGray(Label.pixel(x-1, y))) {
            return true;
        }
    }
    if (x < Label.width() - 1) {
        if (qGray(Label.pixel(x, y)) != qGray(Label.pixel(x+1, y))) {
            return true;
        }
    }
    return false;
}

QImage Drawer::Draw_Contours(const QImage &Label, const QImage &Original_Image, uint color){
    QImage image(Original_Image.convertToFormat(QImage::Format_RGB32));
    for(int y = 0; y < image.height(); y++) {
        for(int x = 0; x < image.width(); x++) {
            if (Is_Boundary(Label, x, y)) {
                image.setPixel(x, y, color);
            }
        }
    }
    return image;
}

QImage Drawer::Filter(const QImage &Original_Image){
    int Width = Original_Image.width();
    int Height = Original_Image.height();
    int N = Width*Height;
    QImage image(Original_Image.convertToFormat(QImage::Format_RGB32));
    int *Image_Matrix = (int*)malloc(N*sizeof(int*));
    if (Image_Matrix == NULL) {
        return Original_Image;
    }

    for(int y = 0; y < Height; y++) {
        for(int x = 0; x < Width; x++) {
            int n = Width*y + x;
            Image_Matrix[n] = qGray(image.pixel(x, y));
        }
    }
    QColor color;
    QColor orig;
    int *neighborhood = (int*)malloc(9*sizeof(int));
    int median;
    for(int y = 1; y < Height-1; y++) {
        for(int x = 1; x < Width-1; x++) {
            neighborhood[0] = Image_Matrix[(y-1)*Width + (x-1)];
            neighborhood[1] = Image_Matrix[(y-1)*Width + (x)];
            neighborhood[2] = Image_Matrix[(y-1)*Width + (x+1)];

            neighborhood[3] = Image_Matrix[(y)*Width + (x-1)];
            neighborhood[4] = Image_Matrix[(y)*Width + (x)];
            neighborhood[5] = Image_Matrix[(y)*Width + (x+1)];

            neighborhood[6] = Image_Matrix[(y+1)*Width + (x-1)];
            neighborhood[7] = Image_Matrix[(y+1)*Width + (x)];
            neighborhood[8] = Image_Matrix[(y+1)*Width + (x+1)];

            std::sort(neighborhood, neighborhood+9);

            median = neighborhood[5];

            orig = image.pixel(x, y);
            color = QColor::fromHsv(orig.hue(), orig.saturation(), median);
            image.setPixelColor(x, y, color);
        }
    }
    return image;
}
