//
// Created by 程龙昊 on 2021/12/21.
//

#ifndef INC_1_OUTPUTIMAGE_HITTABLE_LIST_H
#define INC_1_OUTPUTIMAGE_HITTABLE_LIST_H
#include <hittable.h>
#include <memory>
#include <vector>
#include <aabb.h>
using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable{
public:
    hittable_list(){}
    hittable_list(shared_ptr<hittable> object){ add(object); }
    void clear(){objects.clear();}
    void add(shared_ptr<hittable> object) {objects.push_back(object);}
    virtual bool hit(const ray& r,double t_min,double t_max,hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
    double pdf_value(const point3& o, const vec3& v) const override{
        auto weight = 1.0/objects.size();
        auto sum = 0.0;

        for (const auto& object : objects)
            sum += weight * object->pdf_value(o, v);

        return sum;
    }

    vec3 random(const vec3& o) const override{
        auto int_size = static_cast<int>(objects.size());
        return objects[random_int(0, int_size-1)]->random(o);
    }
public:
    std::vector<shared_ptr<hittable>> objects;
};
#endif //INC_1_OUTPUTIMAGE_HITTABLE_LIST_H
