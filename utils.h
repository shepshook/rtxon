#include <cmath>

const float EPSILON = 1e-5;

struct Vec {
    float x, y, z;
    Vec() {}
    Vec(float X, float Y, float Z) { x = X; y = Y; z = Z; }
    static Vec sum(Vec a, Vec b) {
        return Vec(a.x + b.x, a.y + b.y, a.z + b.z);
    }
    static Vec sub(Vec a, Vec b) {
        return Vec(a.x - b.x, a.y - b.y, a.z - b.z);
    }
    static float dot(Vec a, Vec b) {
        return (a.x * b.x + a.y * b.y + a.z * b.z);
    }
    static Vec cross(Vec a, Vec b) {
        return Vec(a.y * b.z - a.z * b.y, -a.x * b.z + a.z * b.x, a.x * b.y - a.y * b.x);
    }
    static Vec mult(Vec vec, float k) {
        return Vec(vec.x * k, vec.y * k, vec.z * k);
    }
    float len() {
        return sqrt(x * x + y * y + z * z);
    }
    static float cos(Vec a, Vec b) {
        return dot(a, b) / a.len() / b.len();
    }
    Vec normalize() {
        float l = len();
        return Vec(x / l, y / l, z / l);
    }
    static Vec reflect(Vec a, Vec norm) {
        norm = norm.normalize();
        return sub(a, mult(norm, 2 * dot(a, norm)));
    }
};

struct Ray {
  Vec from;
  Vec dir;
  Ray() {}
  static Ray Ray_two_points(Vec from, Vec where) {
    Ray ray;
    ray.from = from;
    ray.dir = Vec::sub(where, from).normalize();
    return ray;
  }
  static Ray Ray_dir(Vec from, Vec dir) {
    Ray ray;
    ray.from = from;
    ray.dir = dir;
    return ray;
  }
};

float constrain_rgb(float num) {
  float left = 0;
  float right = 255;
  if (num < left) return left;
  if (num > right) return right;
  return num;
}

rgb_t rgb_mult(rgb_t col, float num) {
    col.red = constrain_rgb(col.red * num);
    col.green = constrain_rgb(col.green * num);
    col.blue = constrain_rgb(col.blue * num);
    return col;
}

rgb_t rgb_sum(rgb_t col1, rgb_t col2) {
    rgb_t res;
    res.red = constrain_rgb(col1.red + col2.red);
    res.green = constrain_rgb(col1.green + col2.green);
    res.blue = constrain_rgb(col1.blue + col2.blue);
    return res;
}

float f_abs(float a) {
    return (a > 0) ? a : -a;
}
