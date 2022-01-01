//
// Created by 程龙昊 on 2021/12/21.
//
#include <camera.h>

camera::camera(
        point3 lookfrom,
        point3 lookat,
        vec3   vup,
        double vfov,
        double aspect_ratio,
        double aperture,
        double focus_dist,
        double _time0,
        double _time1
        ) {
    // the z-value of viewport is set to -1
    // the origin is set to (0,0,0)

    // the viewport size is same ratio with the image
    auto theta = degrees_to_radians(vfov);
    auto h = tan(theta/2);
    auto viewport_height = 2.0 * h;
    auto viewport_width = viewport_height * aspect_ratio;

    // w,u,v all is unit vector
    w = unit_vector(lookfrom - lookat);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);

    origin = lookfrom;
    // the size and location of viewport is saved as vec3
    horizontal = focus_dist * viewport_width * u;
    vertical = focus_dist * viewport_height * v;
    // the cam is on the focal point
    lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist*w;

    lens_radius = aperture / 2;
    time0 = _time0;
    time1 = _time1;
    /*auto focal_length = 1.0;

    origin = point3(0.0,0.0,0.0);
    horizontal = vec3(viewport_width,0,0);
    vertical = vec3(0,viewport_height,0);
    lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0,0,focal_length);*/
}

ray camera::get_ray(double s, double t) const{
    // get ray from origin to (u,v), the (u,v) is scale, not absolute
    // return ray(origin,lower_left_corner + u*horizontal + v*vertical-origin);

    vec3 rd = lens_radius * random_in_unit_disk();
    vec3 offset = u * rd.x() + v * rd.y();

    return ray(
            origin + offset,
            lower_left_corner + s*horizontal + t*vertical - origin - offset,
            random_double(time0, time1)
    );
}
