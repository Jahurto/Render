//
// Created by 程龙昊 on 2021/12/24.
//

#ifndef RAYTRACING_NEXTWEEK_DIFFUSE_LIGHT_H
#define RAYTRACING_NEXTWEEK_DIFFUSE_LIGHT_H
#include <material.h>
#include <texture.h>
class diffuse_light : public material  {
public:
    diffuse_light(shared_ptr<texture> a) : emit(a) {}
    diffuse_light(color c) : emit(make_shared<solid_color>(c)) {}

    virtual bool scatter(
            const ray& r_in, const hit_record& rec, scatter_record& srec
    ) const override{
        return false;
    }

    virtual color emitted(const ray& r_in, const hit_record& rec, double u, double v,
                          const point3& p) const override {
        if (rec.front_face)
            return emit->value(u, v, p);
        else
            return color(0,0,0);
    }

public:
    shared_ptr<texture> emit;
};
#endif //RAYTRACING_NEXTWEEK_DIFFUSE_LIGHT_H
