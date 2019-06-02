rgb_t get_sphere_color(void* data, Vec point);
Vec norm_sphere(void* data, Vec point);
bool trace_sphere(void* data, Ray ray, Vec &point, float &dist);

struct Sphere {
    Vec center;
    float r;
    rgb_t color;

    Sphere(Vec center, float radius, rgb_t color) {
        this->center = center;
        this->r = radius;
        this->color = color;
    }

    Object* make_object() {
        Object* obj = new Object();
        obj->data = this;
        obj->get_color = get_sphere_color;
        obj->norm = norm_sphere;
        obj->trace = trace_sphere;
        return obj;
    }
};


rgb_t get_sphere_color(void* data, Vec point) {
    //rgb_t col = color;
    //col.red += 100 * cos(point.x / 10);
    //col.green += 50 * cos(point.y / 15);
    //col.red += 70 * cos(point.z / 20);
    return ((Sphere*)data)->color;
}

Vec norm_sphere(void* data, Vec point) {
    return Vec::mult(Vec::sub(point, ((Sphere*)data)->center), 1 / ((Sphere*)data)->r);
}

bool trace_sphere(void* data, Ray ray, Vec &point, float &dist) {
    Sphere* s = (Sphere*)data;

    float l = ray.dir.x;
    float m = ray.dir.y;
    float n = ray.dir.z;

    float k = (ray.from.x - s->center.x);
    float p = (ray.from.y - s->center.y);
    float f = (ray.from.z - s->center.z);

    float a = l * l + m * m + n * n;
    float b = 2 * (k * l + p * m + f * n);
    float c = k * k + p * p + f * f - s->r * s->r;

    float D = b * b - 4 * a * c;

    if (D < 0) return false;

    float sqrt_D = sqrt(D);
    float t1 = (-b + sqrt_D) / (2 * a);
    float t2 = (-b - sqrt_D) / (2 * a);

    float min_t = (t1 < t2) ? t1 : t2;
    float max_t = (t1 > t2) ? t1 : t2;

    float t = (min_t > EPSILON) ? min_t : max_t;

    if (t < EPSILON) return false;

    point = Vec(l * t + ray.from.x, m * t + ray.from.y, n * t + ray.from.z);
    dist = Vec::sub(point, ray.from).len();

    return true;
}
