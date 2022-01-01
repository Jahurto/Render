//
// Created by 程龙昊 on 2021/12/24.
//

#ifndef RAYTRACING_NEXTWEEK_BVH_H
#define RAYTRACING_NEXTWEEK_BVH_H
#include <rtweekend.h>

#include <hittable.h>
#include <hittable_list.h>


class bvh_node : public hittable {
public:
    bvh_node();

    bvh_node(const hittable_list& list, double time0, double time1)
            : bvh_node(list.objects, 0, list.objects.size(), time0, time1)
    {}

    bvh_node(
            const std::vector<shared_ptr<hittable>>& src_objects,
            size_t start, size_t end, double time0, double time1);

    virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

public:
    shared_ptr<hittable> left;
    shared_ptr<hittable> right;
    aabb box;
};
#endif //RAYTRACING_NEXTWEEK_BVH_H
