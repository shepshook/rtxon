rgb_t get_triangle_color(void* data, Vec point);
Vec norm_triangle(void* data, Vec point);
bool trace_triangle(void* data, Ray ray, Vec &point, float &dist);

struct Triangle {
    Vec A, B, C;
    Vec normal;
    rgb_t color;
    float D;

    Triangle(Vec A, Vec B, Vec C, rgb_t color) {
        this->A = A;
        this->B = B;
        this->C = C;
        this->color = color;
        normal = Vec::cross(Vec::sub(C, A), Vec::sub(B, C));
        D = -Vec::dot(normal, A);
    }

    Object* make_object() {
        Object* obj = new Object();
        obj->data = this;
        obj->get_color = get_triangle_color;
        obj->norm = norm_triangle;
        obj->trace = trace_triangle;
        return obj;
    }
};

rgb_t get_triangle_color(void* data, Vec point) {
    //if (sin(point.x / 50 + point.y / 50) * cos(point.x / 50 + point.y / 50) * sin(point.y / 50) * cos(point.y / 50) > 0) return { 255, 255, 255 };
    //else return { 0, 0, 0 };
    return ((Triangle*)data)->color;
}

Vec norm_triangle(void* data, Vec point) {
    return ((Triangle*)data)->normal;
}

bool check_same_clock_dir(Vec v1, Vec v2, Vec norm) {
    Vec norm_v1_v2 = Vec::cross(v2, v1);
    return Vec::dot(norm_v1_v2, norm) >= 0;
}

bool trace_triangle(void* data, Ray ray, Vec &point, float &dist) {
    Triangle* tr = (Triangle*)data;
    float dot_product = Vec::dot(tr->normal, ray.dir);

    //return if ray is perpendicular to the normal vector
    if (f_abs(dot_product) < EPSILON) return false;

    float lambda = -(tr->D + Vec::dot(tr->normal, ray.from)) / dot_product;

    //calculate the point of intersection with the plane
    point = Vec::sum(ray.from, Vec::mult(ray.dir, lambda));
    // Vec u = Vec::sub(tr->B, tr->A);
    // Vec v = Vec::sub(tr->C, tr->A);
    // Vec w = Vec::sub(point, tr->A);
    // float uv = Vec::dot(u, v);
    // float denom = uv * uv - Vec::dot(u, u) * Vec::dot(v, v);
    // float s = (uv * Vec::dot(w, v) - Vec::dot(v, v) * Vec::dot(w, u)) / denom;
    // float t = (uv * Vec::dot(w, u) - Vec::dot(u, u) * Vec::dot(w, v)) / denom;
    //
    // //check if the point is inside of the triangle
    // if (s >= EPSILON && t >= EPSILON && s + t <= 1) {
    //     return true;
    // }

    // if (check_same_clock_dir(Vec::sub(tr->B, tr->A), Vec::sub(point, tr->A), tr->normal) &&
    //     check_same_clock_dir(Vec::sub(tr->C, tr->B), Vec::sub(point, tr->B), tr->normal) &&
    //     check_same_clock_dir(Vec::sub(tr->A, tr->C), Vec::sub(point, tr->C), tr->normal)) {
    //         return true;
    // }

    if (Vec::dot(Vec::cross(Vec::sub(point, tr->A), Vec::sub(tr->B, tr->A)), tr->normal) >= 0 &&
        Vec::dot(Vec::cross(Vec::sub(point, tr->B), Vec::sub(tr->C, tr->B)), tr->normal) >= 0 &&
        Vec::dot(Vec::cross(Vec::sub(point, tr->C), Vec::sub(tr->A, tr->C)), tr->normal) >= 0) {
            dist = Vec::sub(point, ray.from).len();
            return true;
    }

    return false;
}
