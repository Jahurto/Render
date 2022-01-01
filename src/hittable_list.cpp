//
// Created by 程龙昊 on 2021/12/24.
//
#include <hittable_list.h>
bool hittable_list::hit(const ray& r,double t_min,double t_max,hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closet_so_far = t_max; // the most far t (hit something!) by now

    //Test hits the object one by one
    for(const auto& object :objects){
        if(object->hit(r,t_min,closet_so_far,temp_rec)){
            hit_anything = true;
            closet_so_far = temp_rec.t; // hit it and the t is smaller than CSF, renew the CSF
            rec = temp_rec; //renew the hit point
        }
    }

    return hit_anything;
}

bool hittable_list::bounding_box(double time0, double time1, aabb& output_box) const {
    if (objects.empty()) return false;

    aabb temp_box;
    bool first_box = true;

    for (const auto& object : objects) {
        if (!object->bounding_box(time0, time1, temp_box)) return false;
        output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
        first_box = false;
    }

    return true;
}
