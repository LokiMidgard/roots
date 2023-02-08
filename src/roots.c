#include "roots.h"

void roots_randomize_paths(Roots *roots)
{
    for (int index = 0; index < MAX_ROOT_PATHS; ++index)
    {
        RootPath *path = roots->paths + index;
        path->first_node  = 0;
        Vector2 direction = {0.0f, -1.0f};
        float offset = WIDTH / (float)MAX_ROOT_PATHS;
        Vector2 start     = {index * offset + 10.0f, -100.0f};
        path->growth_direction = (Vector2){0.0f, 1.0f};
        path->tip_size  = 0.0f;
        path->tip_size_target = 50.0f;
        path->growth_speed = 2.0f;
        for (int node_index = 0; node_index < ROOT_PATH_SIZE; ++node_index)
        {
            path->nodes[node_index] = start;
            start = Vector2Add(start,
                               Vector2Scale(direction,
                                            utils_random_float(20.0f, 100.0f)));
            float angle = utils_random_float(-10.0f, 10.0f);
            direction = Vector2Rotate(direction, angle * DEG2RAD);
        }
    }
}

void roots_reset(Roots *roots)
{
    roots->number_of_paths = MAX_ROOT_PATHS;
    roots_randomize_paths(roots);
}

void roots_set_world(World *world, Vector2 from, Vector2 to, float *radius)
{
    float growth_per_pixel_length = 3.0f / 100.0f;
    Vector2 direction = Vector2Subtract(to, from);
    float total_length = Vector2Length(direction);
    direction = Vector2Scale(direction, 1.0f / total_length);
    float length = 0.0f;
    while(length < total_length)
    {
        world_set_radius(world, from.x, from.y, *radius, TERRA_ROOT);
        from = Vector2Add(from, Vector2Scale(direction, *radius));
        *radius += growth_per_pixel_length * *radius;
        length += *radius;
    }
}


void roots_update(Roots *roots, World *world)
{
    for (int index = 0; index < roots->number_of_paths; ++index)
    {
        RootPath *path = roots->paths + index;
        int node = path->first_node;
        do {
            path->nodes[node].y -= world->last_scroll;
            node = (node + 1) % ROOT_PATH_SIZE;
        }
        while (node != path->first_node);

    }
    for (int index = 0; index < roots->number_of_paths; ++index)
    {
        float radius = 1.0f;
        RootPath *path = roots->paths + index;

        path->tip_size += path->growth_speed;
        Vector2 end = path->nodes[path->first_node];
        Vector2 start =
            Vector2Add(end,
                       Vector2Scale(path->growth_direction, path->tip_size));
        roots_set_world(world, start, end, &radius);

        int node = path->first_node;
        int last_node = (node + ROOT_PATH_SIZE - 1) % ROOT_PATH_SIZE;
        do {
            start = path->nodes[node];
            end   = path->nodes[(node + 1) % ROOT_PATH_SIZE];
            roots_set_world(world, start, end, &radius);
            node = (node + 1) % ROOT_PATH_SIZE;
        }
        while (node != last_node);

        if (path->tip_size > path->tip_size_target)
        {
            Vector2 first  = path->nodes[path->first_node];
            Vector2 second = path->nodes[(path->first_node + 1) % ROOT_PATH_SIZE];

            path->first_node = (path->first_node + ROOT_PATH_SIZE - 1) % ROOT_PATH_SIZE;
            path->nodes[path->first_node] =
                Vector2Add(first,
                           Vector2Scale(path->growth_direction, path->tip_size));

            float left_angle = -10.0f;
            float right_angle = 10.0f;
            if (first.x > WIDTH * 0.9f)
                left_angle = 1.0f;
            if (first.x < WIDTH * 0.1f)
                right_angle = -1.0f;
            float angle = utils_random_float(left_angle, right_angle);
            Vector2 last_direction = Vector2Normalize(Vector2Subtract(first, second));
            Vector2 next_direction = Vector2Rotate(last_direction, angle * DEG2RAD);

            path->growth_direction = next_direction;
            path->tip_size = 0;
            path->tip_size_target = utils_random_float(10.0f, 50.0f);
            path->growth_speed = utils_random_float(0.1f, 4.0f);
            if (first.y < 0.0f) path->growth_speed = 15.0f;
        }
    }
}

