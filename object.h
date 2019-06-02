struct Object {
    void* data;
    bool (*trace)(void* data, Ray ray, Vec &point, float &dist);
    rgb_t (*get_color)(void* data, Vec point);
    Vec (*norm)(void* data, Vec point);
};
