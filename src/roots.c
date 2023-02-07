
void remove_leaf(Roots *roots, int index)
{
    if (roots->number_of_leafs == 0) return;
    roots->leafs[index] = roots->leafs[roots->number_of_leafs - 1];
    roots->number_of_leafs -= 1;
}

void remove_segment(Roots *roots, int index)
{
    if (roots->number_of_segments == 0) return;
    int last_segment_index = roots->number_of_segments - 1;
    roots->segments[index] = roots->segments[last_segment_index];
    roots->number_of_segments -= 1;

    for (int leaf_index = 0;
         leaf_index < roots->number_of_leafs;
         ++leaf_index)
    {
        if (roots->leafs[leaf_index] == last_segment_index)
        {
            roots->leafs[leaf_index] = index;
            break;
        }
    }
    for (int leaf_index = 0;
         leaf_index < roots->number_of_leafs;
         ++leaf_index)
    {
        if (roots->leafs[leaf_index] == index)
        {
            remove_leaf(roots, leaf_index);
            break;
        }
    }
    for (int segment_index = 0;
         segment_index < roots->number_of_segments;
         ++segment_index)
    {
        if (roots->segments[segment_index].parent == index)
            roots->segments[segment_index].parent = -1;
        if (roots->segments[segment_index].parent == last_segment_index)
            roots->segments[segment_index].parent = index;
    }
}

void spawn_leaf(Roots *roots,
                int parent_index)
{
    if (roots->number_of_leafs == MAX_LEAFS)
    {
        remove_leaf(roots, utils_random_int(0, MAX_LEAFS - 1));
    }
    if (roots->number_of_segments == MAX_ROOT_SEGMENTS)
    {
        for (int index = 0; index < roots->number_of_segments; ++index)
        {
            if (roots->segments[index].length > roots->segments[index].max_length)
            {
                printf("removing non leaf start %d\n", roots->number_of_leafs);
                remove_segment(roots, index);
                printf("removing non leaf end %d\n", roots->number_of_leafs);
                break;
            }
        }
    }
    int leaf_index = roots->number_of_leafs++;
    int segment_index = roots->number_of_segments++;
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
        roots->leafs[leaf_index] = segment_index;
        roots->segments[segment_index] = new_segment;
        return;
    }
    if (parent_index < 0 || parent_index >= roots->number_of_segments)
        return;

    RootSegment parent = roots->segments[parent_index];
    Vector2 start = Vector2Add(parent.start,
                               Vector2Scale(parent.direction, parent.length));
    if (start.x < 0 || start.x > WIDTH || start.y < 0)
        return;
    float angle_change = utils_random_float(-45.0f, 45.0f);
    Vector2 direction = Vector2Rotate(parent.direction, DEG2RAD * angle_change);
    float grow_speed = utils_random_float(0.5f, 0.7f);
    float max_length = utils_random_float(10.0f, 100.0f);
    RootSegment new_segment =
    {
        .start = start,
        .direction = direction,
        .length = 0.0f,
        .max_length = max_length,
        .grow_speed = grow_speed,
        .start_radius = 1.0f,
        .parent = parent_index,
    };
    roots->leafs[leaf_index] = segment_index;
    roots->segments[segment_index] = new_segment;
}

void roots_reset(Roots *roots)
{
    roots->number_of_segments = 0;
    roots->number_of_leafs = 0;
    spawn_leaf(roots, -1);
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
        RootSegment *leaf  = roots->segments + segment_index;

        if (leaf->length >= leaf->max_length)
        {
            remove_leaf(roots, index);
            --index;

            int new_leafs = 5;
            if (roots->number_of_leafs > 70)
                new_leafs = utils_random_int(0, 1);
            if (roots->number_of_leafs > 40)
                new_leafs = utils_random_int(0, 2);
            if (roots->number_of_leafs > 10)
                new_leafs = utils_random_int(0, 4);


            printf("i died making %d new leafs\n", new_leafs);
            for (int count = 0; count < new_leafs; ++count)
                spawn_leaf(roots, segment_index);

            printf("segments: %d, leafs: %d\n",
                   roots->number_of_segments,
                   roots->number_of_leafs);
        }
        else
        {
            leaf->length += leaf->grow_speed;
            int parent_index = leaf->parent;
            RootSegment *child = leaf;
            while(parent_index != -1)
            {
                RootSegment *parent = roots->segments + parent_index;
                parent->start_radius =
                    child->start_radius + child->length * growth_per_pixel_length;
                parent_index = parent->parent;
                child = parent;
            }
        }
    }

    // grow segments
    for (int index = 0;
         index < roots->number_of_segments;
         ++index)
    {
        RootSegment *root_segment = roots->segments + index;
        if (root_segment->start.y < -100.0f)
        {
            remove_segment(roots, index);
            --index;
            continue;
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

