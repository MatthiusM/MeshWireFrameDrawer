#include <vector>
#include <cmath>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
Model *model = NULL;
const int width  = 800 ;
const int height = 800;

TGAColor RandomColour() { 
    return TGAColor(rand() % 256, rand() % 256, rand() % 256, 255);;
}

template <typename t>
void swap(t* a, t* b) {
    t temp = *b;
    *b = *a;
    *a = temp;
}

// Write the line method here
//line (x0, y0, x1, y1, image, colour)
void line(int x0, int y0, int x1, int y1, TGAImage* image, TGAColor colour) {
    bool steep = false;
    
    if (abs(x0 - x1) < abs(y0 - y1)) {
        swap(&x0, &y0);
        swap(&x1, &y1);
        steep = true;
    }

    if (x0 > x1) {
        swap(&x0, &x1);
        swap(&y0, &y1);
    }

    

    for (float x = x0; x <= x1; x++) {
        float t = (x - x0) / (x1 - x0);
        float y = y0 * (1 - t) + (y1 * t);
        
        if (steep) {
            image->set(y, x, RandomColour());
        }
        else {
            image->set(x, y, RandomColour());
        }
        
    }

   
}



int main(int argc, char** argv) {
    srand(time(0));
    TGAImage image(width, height, TGAImage::RGB);
    
    if (2 == argc) {
        model = new Model(argv[1]);
    }
    else {
        model = new Model("cc.obj");
    }

    for (int i = 0; i < model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
        for (int j = 0; j < 3; j++) {
            Vec3f v0 = model->vert(face[j]);
            Vec3f v1 = model->vert(face[(j + 1) % 3]);
            int x0 = (v0.x + 1.) * width / 2.;
            int y0 = (v0.y + 1.) * height / 2.;
            int x1 = (v1.x + 1.) * width / 2.;
            int y1 = (v1.y + 1.) * height / 2.;
            line(x0, y0, x1, y1, &image, white);
        }
    }   

    //line(60, 53, 312, 312, &image, white);
    image.flip_vertically(); // we want to have the origin at the left bottom corner of the image    
    image.write_tga_file("outputL.tga");
    delete model;
    return 0;
}

