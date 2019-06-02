rgb_t get_plane_color(void* data, Vec point);
Vec norm_plane(void* data, Vec point);
bool trace_plane(void* data, Ray ray, Vec &point, float &dist);

struct Plane {
    Triangle* A;
    Triangle* B;
    rgb_t color;

    Plane(Vec a, Vec b, Vec c, Vec d, rgb_t color) {
        A = new Triangle(a, b, c, color);
        B = new Triangle(a, c, d, color);
        this->color = color;
    }

    Object* make_object() {
        Object* obj = new Object();
        obj->data = this;
        obj->get_color = get_plane_color;
        obj->norm = norm_plane;
        obj->trace = trace_plane;
        return obj;
    }
};

rgb_t get_plane_color(void* data, Vec point) {
    return ((Plane*)data)->color;
}

Vec norm_plane(void* data, Vec point) {
    return norm_triangle(((Plane*)data)->A, point);
}

bool trace_plane(void* data, Ray ray, Vec &point, float &dist) {
    Plane* plane = (Plane*)data;
    if (trace_triangle(plane->A, ray, point, dist)) return true;
    if (trace_triangle(plane->B, ray, point, dist)) return true;
    return false;
}
