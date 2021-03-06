//
// Created by 程龙昊 on 2021/12/20.
//

#ifndef INC_1_OUTPUTIMAGE_COLOR_H
#define INC_1_OUTPUTIMAGE_COLOR_H
#include <vec3.h>
#include <iostream>

void write_color(std::ostream &out,color pixel_color,int samples_per_pixel){
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    if (r != r) r = 0.0;
    if (g != g) g = 0.0;
    if (b != b) b = 0.0;

    // Divide the color by the number of samples.
    auto scale = 1.0 / samples_per_pixel;
    // sqrt to Gamma Fix
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    // Assure the r,g,b is (0,1)
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}
#endif //INC_1_OUTPUTIMAGE_COLOR_H
