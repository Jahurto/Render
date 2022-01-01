//
// Created by 程龙昊 on 2021/12/21.
//
#include <sphere.h>
#include <pdf.h>

bool sphere::hit(const ray& r, double t_min,double t_max,hit_record& rec) const{
    // ray = origin + dir * t
    // a*t*t + b*t + c = 0
    vec3 oc = r.origin() - center;
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
    vec3 outward_normal = (rec.p-center) / radius;
    rec.set_front_face(r,outward_normal);
    get_sphere_uv(outward_normal, rec.u, rec.v);
    rec.mat_ptr = mat_ptr;

    return true;
}

bool sphere::bounding_box(double time0, double time1, aabb& output_box) const{
    output_box = aabb(
            center - vec3(radius, radius, radius),
            center + vec3(radius, radius, radius));
    return true;
}

double sphere::pdf_value(const point3& o, const vec3& v) const{
hit_record rec;
if (!this->hit(ray(o, v), 0.001, infinity, rec))
return 0;

auto cos_theta_max = sqrt(1 - radius*radius/(center-o).length_squared());
auto solid_angle = 2*pi*(1-cos_theta_max);

return  1 / solid_angle;
}

vec3 sphere::random(const point3& o) const{
    vec3 direction = center - o;
    auto distance_squared = direction.length_squared();
    onb uvw;
    uvw.build_from_w(direction);
    return uvw.local(random_to_sphere(radius, distance_squared));
}

