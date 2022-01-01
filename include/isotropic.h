//
// Created by 程龙昊 on 2021/12/24.
//

#ifndef RAYTRACING_NEXTWEEK_ISOTROPIC_H
#define RAYTRACING_NEXTWEEK_ISOTROPIC_H
#include <material.h>
class isotropic : public material {
public:
    isotropic(color c) : albedo(make_shared<solid_color>(c)) {}
    isotropic(shared_ptr<texture> a) : albedo(a) {}

    /*virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const override {
        scattered = ray(rec.p, random_in_unit_sphere(), r_in.time());
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }*/

public:
    shared_ptr<texture> albedo;
};
#endif //RAYTRACING_NEXTWEEK_ISOTROPIC_H
