#include "bitmap_image.hpp"
#include <cmath>
#include "utils.h"
#include "object.h"
#include "triangle.h"
#include "sphere.h"
#include "plane.h"
#include "cube.h"
#include <vector>
#include <iostream>


//////CONFIGURATIONS///////
int img_width = 1280;
int img_height = 768;
rgb_t background_color = { 20, 20, 30 };
Vec light_source = { 100, -5000, -1000 };
float light_power = 3;
Vec camera_pos(img_width / 2, img_height / 2, 0);
float focus_dist = 800;
Vec sphere_pos(img_width / 2 - 700, 600, 2000);
int sphere_radius = 500;
rgb_t sphere_color = {0, 0, 200};
///////////////////////////


struct Light {
    Vec pos;
    float power;
    Light(Vec p, float pow) {
        pos = p;
        power = pow;
    }
};

rgb_t color(std::vector<Object*> objects, std::vector<Light*> lights, Ray ray, float power);
rgb_t reflection(std::vector<Object*> objects, std::vector<Light*> lights, Ray ray, Vec point, float dist, Object* obj, float power);
rgb_t diffusion(std::vector<Object*> objects, std::vector<Light*> lights, Ray ray, Vec point, float dist, Object* obj);

//returns the closest intersection point, distance and the intersected object
bool trace(std::vector<Object*> objects, Ray ray, Vec &point, float &dist, Object* &owner) {
    dist = 99999999;
    bool collision = false;
    for (int i = 0; i < objects.size(); i++) {
        Object* obj = objects[i];
        Vec p;
        float d;
        if (obj->trace(obj->data, ray, p, d)) {
            if (Vec::cos(ray.dir, Vec::sub(p, ray.from)) > 0 && Vec::sub(p, ray.from).len() > 1) {
                collision = true;
                if (d < dist) {
                    dist = d;
                    point = p;
                    owner = obj;
                }
            }
        }
    }
    return collision;
}

//returns either the point is in shadow or not
bool in_shadow(std::vector<Object*> objects, Vec point, Vec light_source) {
    Ray ray = Ray::Ray_two_points(light_source, point);
    float min_dist;
    Vec min_point;
    Object* owner;
    bool collision = trace(objects, ray, min_point, min_dist, owner);
    //if (owner == objects[1]) std::cout << "darova";
    return !collision || Vec::sub(min_point, point).len() > 0.1;
}

rgb_t diffusion(std::vector<Object*> objects, std::vector<Light*> lights, Ray ray, Vec point, float dist, Object* obj) {
    float sum_light = 0;
    for (int i = 0; i < lights.size(); i++) {
        Light* light = lights[i];
        if (in_shadow(objects, point, light->pos)) continue;

        float light_power = light->power / (dist * dist) * 2000000; //visually picked factor
        float cos = Vec::cos(Vec::sub(light->pos, point), obj->norm(obj->data, point));

        Vec dir = Vec::sub(point, light->pos).normalize();
        Vec ref = Vec::reflect(dir, obj->norm(obj->data, point));
        float phong_cos = -Vec::dot(ref, ray.dir);

        if (phong_cos > 0) {
            float phong = pow(cos, 12);
            sum_light += light_power * phong * 1;
        }

        sum_light += light_power * cos * 0.5;
    }

    rgb_t color = obj->get_color(obj->data, point);
    return rgb_mult(color, sum_light);
}

rgb_t reflection(std::vector<Object*> objects, std::vector<Light*> lights, Ray ray, Vec point, float dist, Object* obj, float power) {
    if (power < 0.1) return { 0, 0, 0 };
    return color(objects, lights, Ray::Ray_dir(point, Vec::reflect(ray.dir, obj->norm(obj->data, point))), 0.1 * power);
}

//returns the color of intersetion of the ray with any of objects
rgb_t color(std::vector<Object*> objects, std::vector<Light*> lights, Ray ray, float power = 1) {
    Vec point;
    float dist;
    Object* obj;
    if (trace(objects, ray, point, dist, obj)) {

        rgb_t surface_color = diffusion(objects, lights, ray, point, dist, obj);
        rgb_t reflect_color = reflection(objects, lights, ray, point, dist, obj, power);

        return rgb_mult(rgb_sum(rgb_mult(surface_color, 1), rgb_mult(reflect_color, 1)), power);
    }
    else return background_color;
}

int main() {
    bitmap_image img(img_width, img_height);

    // Sphere* s = new Sphere(sphere_pos, sphere_radius, sphere_color);
    // Sphere* s2 = new Sphere(Vec(img_width / 2 + 200, img_height / 2 + 200, 2000), sphere_radius - 200, {255, 0, 0});
    // Triangle* t = new Triangle(Vec(500, 300, 1300), Vec(200, -100, 2200), Vec(1700, -100, 2600), {200, 255, 0});
    // Plane* p = new Plane(Vec(-1200, 1100, 4000), Vec(2200, 1100, 4000), Vec(2200, 1100, 1300), Vec(-1200, 1100, 1300), {50, 255, 50});
    // Cube* c = new Cube(Vec(1200, 600, 1500), Vec(1800, 900, 2100), {255, 100, 0});
    // Triangle* t2 = new Triangle(Vec(1300, 1000, 3200), Vec(1750, 0, 3500), Vec(2000, 1000, 2800), {160, 32, 240});
    Sphere* s = new Sphere(sphere_pos, sphere_radius, sphere_color);
    Sphere* s2 = new Sphere(Vec(img_width / 2 + 200, img_height / 2 + 200, 2000), sphere_radius - 200, {255, 0, 0});
    Triangle* t = new Triangle(Vec(300, 300, 1300), Vec(0, -100, 2200), Vec(1500, -100, 2600), {200, 255, 0});
    Plane* p = new Plane(Vec(-1200, 1100, 4000), Vec(2200, 1100, 4000), Vec(2200, 1100, 1300), Vec(-1200, 1100, 1300), {50, 255, 50});
    Cube* c = new Cube(Vec(1200, 600, 1500), Vec(1800, 900, 2100), {255, 100, 0});
    Triangle* t2 = new Triangle(Vec(1300, 1000, 3200), Vec(1750, 0, 3500), Vec(2000, 1000, 2800), {160, 32, 240});

    // Sphere* s = new Sphere(sphere_pos, sphere_radius, sphere_color);
    // objects.push_back(s->make_object());
    //
    // Light* l = new Light(light_pos, light_power);
    // lights.push_back(l);


    std::vector<Object*> objects;
    objects.push_back(s->make_object());
    objects.push_back(p->make_object());
    objects.push_back(t->make_object());
    objects.push_back(s2->make_object());
    objects.push_back(t2->make_object());
    //objects.push_back(c->make_object());

    std::vector<Light*> lights;
    lights.push_back(new Light(light_source, light_power));
    lights.push_back(new Light(camera_pos, 0.5));
    lights.push_back(new Light(Vec(2000, 0, 500), 0.5));

    //going through all the pixels
    for (int x = 0; x < img.width(); x++) {
        for (int y = 0; y < img.height(); y++) {
            rgb_t col = color(objects, lights, Ray::Ray_two_points(camera_pos, Vec(x, y, focus_dist)));
            img.set_pixel(x, y, col);
        }
    }

    img.save_image("render.bmp");
}
