

void spawn_leaf(Roots *roots,
                int parent_index)
{
    if (roots->number_of_leafs == MAX_LEAFS)
        return;
    if (roots->number_of_root_segments == MAX_ROOT_SEGMENTS)
        return;
    int leaf_index = roots->number_of_leafs++;
    int index = roots->number_of_root_segments++;
    if (parent_index == -1)
    {
        RootSegment new_segment =
        {
            .start = {300.0f, 400.0f},
            .direction = {0.0f, 1.0f},
            .length = 0.0f,
            .max_length = 10.0f,
            .grow_speed = 0.1f,
            .start_radius = 1.0f,
            .parent = -1,
        };
        roots->leafs[leaf_index] = index;
        roots->root_segments[index] = new_segment;
        return;
    }
    if (parent_index < 0 || parent_index >= MAX_ROOT_SEGMENTS)
        return;

    RootSegment parent = roots->root_segments[parent_index];
    Vector2 start = Vector2Add(parent.start,
                               Vector2Scale(parent.direction, parent.length));
    if (start.x < 0 || start.x > WIDTH || start.y < 0)
        return;
    float angle_change = utils_random_float(-30.0f, 30.0f);
    Vector2 direction = Vector2Rotate(parent.direction, DEG2RAD * angle_change);
    float grow_speed = 1.3f;
    RootSegment new_segment =
    {
        .start = start,
        .direction = direction,
        .length = 0.0f,
        .max_length = 100.0f,
        .grow_speed = grow_speed,
        .start_radius = 1.0f,
        .parent = parent_index,
    };
    roots->leafs[leaf_index] = index;
    roots->root_segments[index] = new_segment;
}

void roots_reset(Roots *roots)
{
    roots->number_of_root_segments = 0;
    roots->number_of_leafs = 0;
    spawn_leaf(roots, -1);
}

void fix_parents(Roots *roots, int old_index, int new_index)
{
    for (int index = 0;
         index < roots->number_of_root_segments;
         ++index)
    {
        RootSegment *root_segment = roots->root_segments + index;
        if (root_segment->parent == new_index)
            root_segment->parent = -1;
        if (root_segment->parent == old_index)
            root_segment->parent = new_index;

    }
}
void roots_update(Roots *roots, World *world)
{
    float growth_per_pixel_length = 5.0f / 100.0f;

    // check and remove completed leafs
    for (int index = 0;
         index < roots->number_of_leafs;
         ++index)
    {
        int segment_index  = roots->leafs[index];
        RootSegment *leaf  = roots->root_segments + segment_index;

        if (leaf->length >= leaf->max_length)
        {
            if (roots->number_of_leafs == 1)
            {
                roots->number_of_leafs = 0;
            }
            else
            {
                roots->leafs[index] = roots->leafs[roots->number_of_leafs - 1];
                roots->number_of_leafs -= 1;
                --index;
            }
            spawn_leaf(roots, segment_index);
            spawn_leaf(roots, segment_index);
            spawn_leaf(roots, segment_index);
        }
        else
        {
            leaf->length += leaf->grow_speed;
            int parent_index = leaf->parent;
            RootSegment *child = leaf;
            int count = 0;
            while(parent_index != -1)
            {
                RootSegment *parent = roots->root_segments + parent_index;
                parent->start_radius =
                    child->start_radius + child->length * growth_per_pixel_length;
                parent_index = parent->parent;
                child = parent;
                ++count;
            }
        }
    }

    // grow segments
    for (int index = 0;
         index < roots->number_of_root_segments;
         ++index)
    {
        RootSegment *root_segment = roots->root_segments + index;
        if (root_segment->start.y < 0)
        {
            int old_index = roots->number_of_root_segments - 1;
            *root_segment = roots->root_segments[old_index];
            int parent = root_segment->parent;
            fix_parents(roots, old_index, index);
            for (int leaf_index = 0; leaf_index < roots->number_of_leafs; ++leaf_index)
            {
                if (roots->leafs[leaf_index] == index)
                {
                    roots->leafs[leaf_index] = roots->leafs[roots->number_of_leafs - 1];
                    roots->number_of_leafs -= 1;
                }
            }
            root_segment->parent = parent;

            roots->number_of_root_segments -= 1;
            if (roots->number_of_root_segments == 0) break;
        }
        root_segment->start.y -= world->last_scroll;

        Vector2 current_point = Vector2Add(root_segment->start,
                                           Vector2Scale(root_segment->direction,
                                                        root_segment->length));
        Vector2 direction     = Vector2Scale(root_segment->direction, -1.0f);
        float length = 0.0f;
        while(length < root_segment->length)
        {
            float radius = root_segment->start_radius + length * growth_per_pixel_length;
            radius = fmaxf(radius, 1.0f);
            world_dig(world, current_point.x, current_point.y, radius);
            current_point = Vector2Add(current_point, Vector2Scale(direction, radius));
            length += radius;
        }
    }
}

