//
// Created by 程龙昊 on 2021/12/24.
//
#include <moving_sphere.h>

point3 moving_sphere::center(double time) const {
    return center0 + ((time - time0) / (time1 - time0))*(center1 - center0);
}

bool moving_sphere::hit(const ray& r, double t_min,double t_max,hit_record& rec) const{
    // ray = origin + dir * t
    // a*t*t + b*t + c = 0
    vec3 oc = r.origin() - center(r.time());
    auto a = r.direction().length_squared();
    auto half_b = dot(r.direction(),oc);
    auto c = oc.length_squared() - radius*radius;
    // solve t*t
    auto discrimination = half_b*half_b - a*c;
    // d < 0 ---- no solution, no hit
    // d = 0 ---- one hit
    // d > 0 ---- two hit
    if (discrimination < 0)
        return false;
    auto sqrtd = sqrt(discrimination);

    // T-value
    // ray = origin + dir * t
    // t1 = (-b - sqrt(b*b-4*a*c)) / (2*a)
    // t2 = (-b + sqrt(b*b-4*a*c)) / (2*a)
    // if two results, the bigger one will be obst..
    // t have to be in (t_min,t_max)

    auto root = (-half_b - sqrtd) / a;
    if(root<t_min || root>t_max){
        root = (-half_b + sqrtd) / a;
        if(root<t_min || root>t_max)
            return false;
    }

    rec.t = root;
    rec.p = r.at(root);
    // unit_normal
    vec3 outward_normal = (rec.p-center(r.time())) / radius;
    rec.set_front_face(r,outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;
}

bool moving_sphere::bounding_box(double _time0, double _time1, aabb& output_box) const {
    aabb box0(
            center(_time0) - vec3(radius, radius, radius),
            center(_time0) + vec3(radius, radius, radius));
    aabb box1(
            center(_time1) - vec3(radius, radius, radius),
            center(_time1) + vec3(radius, radius, radius));
    output_box = surrounding_box(box0, box1);
    return true;
}


