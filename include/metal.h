//
// Created by 程龙昊 on 2021/12/22.
//

#ifndef INC_1_OUTPUTIMAGE_METAL_H
#define INC_1_OUTPUTIMAGE_METAL_H
#include <material.h>
class metal : public material {
public:
    metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    virtual bool scatter(
            const ray& r_in, const hit_record& rec, scatter_record& srec
    ) const override {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        srec.specular_ray = ray(rec.p, reflected+fuzz*random_in_unit_sphere());
        srec.attenuation = albedo;
        srec.is_specular = true;
        srec.pdf_ptr = 0;
        return true;
    }

    /*virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const override {
        // Just reflect in an accurate direction
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        // fuzz control the random offset of the scattered ray
        scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere(),r_in.time());
        attenuation = albedo;
        // Ensure the direction of out-ray is on the same side of normal
        return (dot(scattered.direction(), rec.normal) > 0);
    }*/

public:
    color albedo;
    double fuzz;
};
#endif //INC_1_OUTPUTIMAGE_METAL_H
