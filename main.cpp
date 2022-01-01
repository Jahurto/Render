#include <iostream>
#include <rtweekend.h>
#include <pbrt.h>

#include <aarect.h>
#include <box.h>
#include <color.h>
#include <hittable_list.h>
#include <sphere.h>
#include <camera.h>
#include <material.h>
#include <bvh.h>
#include <pdf.h>
#include <moving_sphere.h>
#include <spectrum.h>

using namespace pbrt;

pbrt::Spectrum ray_color(
        const ray& r, const pbrt::Spectrum& background, const hittable& world,
        shared_ptr<hittable> lights, int depth
) {
    // r: the ray
    // world: the list of the object

    hit_record rec;
    if(depth <= 0){ return Spectrum(0);}

    if (!world.hit(r, 0.001, infinity, rec))
        return background;

    scatter_record srec;
    color emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);
    if (!rec.mat_ptr->scatter(r, rec, srec))
        return emitted;

    if (srec.is_specular) {
        return srec.attenuation
               * ray_color(srec.specular_ray, background, world, lights, depth-1);
    }

    auto light_ptr = make_shared<hittable_pdf>(lights, rec.p);
    mixture_pdf p(light_ptr, srec.pdf_ptr);

    ray scattered = ray(rec.p, p.generate(), r.time());
    auto pdf_val = p.value(scattered.direction());

    return emitted
           + srec.attenuation * rec.mat_ptr->scattering_pdf(r, rec, scattered)
             * ray_color(scattered, background, world, lights, depth-1) / pdf_val;
    /*
    ray scattered;
    color attenuation;
    color emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);
    double pdf_val;
    color albedo;
    if (!rec.mat_ptr->scatter(r, rec, albedo, scattered, pdf_val))
        return emitted;


    /*auto on_light = point3(random_double(213,343), 554, random_double(227,332));
    auto to_light = on_light - rec.p;
    auto distance_squared = to_light.length_squared();
    to_light = unit_vector(to_light);

    if (dot(to_light, rec.normal) < 0)
        return emitted;

    double light_area = (343-213)*(332-227);
    auto light_cosine = fabs(to_light.y());
    if (light_cosine < 0.000001)
        return emitted;*/

    /*if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        return emitted;

    return emitted + attenuation * ray_color(scattered, background, world, depth-1);
*/
    /*
    if(world.hit(r,0.001,infinity,rec)){
        // Hit it, have hit record, then to generate new light
        ray scattered; //the reflection light
        color attenuation; //the color of the reflection light or the color of the object
        // scatter is associate with material
        // scatter function compute the color and new ray
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            //After scatter, the new ray will continue move
            //Mix_color = color * ray_color(new_ray)
            return attenuation * ray_color(scattered, world, depth-1);
        return color(0,0,0);
    }

    // if not hit sth
    // color = background
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);*/
}

hittable_list random_scene() {
    hittable_list world;

    auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(checker)));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    auto center2 = center + vec3(0, random_double(0,.5), 0);
                    world.add(make_shared<moving_sphere>(
                            center, center2, 0.0, 1.0, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return static_cast<hittable_list>(std::make_shared<bvh_node>(world,0,1));
}

hittable_list two_spheres() {
    hittable_list objects;

    auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));

    objects.add(make_shared<sphere>(point3(0,-10, 0), 10, make_shared<lambertian>(checker)));
    objects.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));

    return objects;
}

hittable_list two_perlin_spheres() {
    hittable_list objects;
    auto pertext = make_shared<noise_texture>(4);
    objects.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(pertext)));
    objects.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

    return objects;
}

hittable_list earth() {
    auto earth_texture = make_shared<image_texture>("../earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(point3(0,0,0), 2, earth_surface);

    return hittable_list(globe);
}

hittable_list simple_light() {
    hittable_list objects;

    auto pertext = make_shared<noise_texture>(4);
    objects.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(pertext)));
    objects.add(make_shared<sphere>(point3(0,2,0), 2, make_shared<lambertian>(pertext)));

    auto difflight = make_shared<diffuse_light>(color(4,4,4));
    objects.add(make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));

    return objects;
}

hittable_list cornell_box() {
    hittable_list objects;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<flip_face>(make_shared<xz_rect>(213, 343, 227, 332, 554, light)));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    shared_ptr<material> aluminum = make_shared<metal>(color(0.8, 0.85, 0.88), 0.0);
    shared_ptr<hittable> box1 = make_shared<box>(point3(0,0,0), point3(165,330,165), aluminum);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265,0,295));
    objects.add(box1);

    auto glass = make_shared<dielectric>(1.5);
    objects.add(make_shared<sphere>(point3(190,90,190), 90 , glass));

    return objects;
}
int main() {
    //image
    const auto aspect_ratio = 1.0 / 1.0;
    const int image_width = 600;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 1000;
    const int max_depth = 50;
    //world

    auto world = cornell_box();

    auto lights = make_shared<hittable_list>();
    lights->add(make_shared<xz_rect>(213, 343, 227, 332, 554, make_shared<material>()));
    lights->add(make_shared<sphere>(point3(190, 90, 190), 90, make_shared<material>()));

    //color background(0,0,0);
    pbrt::Float bgcolor[3] = {0.0,0.0,0.0};
    pbrt::Spectrum(pbrt::Spectrum::FromRGB(bgcolor));
    // Camera

    point3 lookfrom(278, 278, -800);
    point3 lookat(278, 278, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.0;
    auto vfov = 40.0;
    auto time0 = 0.0;
    auto time1 = 1.0;

    camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, time0, time1);

    //render
    std::cout << "P3\n" << image_width<< ' '<<image_height<<"\n255\n";
    for(int j=image_height-1;j>=0;--j){
        std::cerr << "\rScanlines remaining: " << j << ' ' <<std::flush;
        for(int i=0;i<image_width;++i){
            color pixel_color(0,0,0);
            //Set N samples, average N samples
            for (int s = 0; s < samples_per_pixel; ++s) {
                // u,v: the scale offset
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);

                ray r = cam.get_ray(u, v); // from the cam to the viewport(u,v)
                pixel_color += ray_color(r, background, world, lights, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
    std::cerr<<"\rDone\n";
    return 0;
}
