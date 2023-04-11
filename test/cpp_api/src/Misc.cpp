#include <cpp_api.h>

void Misc_setup() {
    ecs_os_set_api_defaults();
}

void Misc_string_compare_w_char_ptr() {
    const char *ptr_1 = "foo";
    const char *ptr_2 = "foo";
    const char *ptr_3 = "bar";

    flecs::string str = flecs::string(ecs_os_strdup(ptr_1));

    test_assert(str == ptr_1);
    test_assert(str == ptr_2);
    test_assert(str != ptr_3);
}

void Misc_string_view_compare_w_char_ptr() {
    const char *ptr_1 = "foo";
    const char *ptr_2 = "foo";
    const char *ptr_3 = "bar";

    flecs::string str = flecs::string_view(ptr_1);

    test_assert(str == ptr_1);
    test_assert(str == ptr_2);
    test_assert(str != ptr_3);   
}

void Misc_string_compare_w_char_ptr_length_diff() {
    const char *ptr_1 = "foo";
    const char *ptr_2 = "foo";
    const char *ptr_3 = "barrr";
    const char *ptr_4 = "ba";

    flecs::string str = flecs::string_view(ptr_1);

    test_assert(str == ptr_1);
    test_assert(str == ptr_2);
    test_assert(str != ptr_3);
    test_assert(str != ptr_4);
}

void Misc_string_compare_w_string() {
    const char *ptr_1 = "foo";
    const char *ptr_2 = "bar";

    flecs::string str_1 = flecs::string(ecs_os_strdup(ptr_1));
    flecs::string str_2 = flecs::string(ecs_os_strdup(ptr_2));

    flecs::string str_3 = flecs::string(ecs_os_strdup(ptr_1));
    flecs::string str_4 = flecs::string(ecs_os_strdup(ptr_2));

    test_assert(str_1 == str_1);
    test_assert(str_1 == str_3);

    test_assert(str_2 == str_2);
    test_assert(str_2 == str_4);

    test_assert(str_1 != str_2);
    test_assert(str_2 != str_1);
}

void Misc_string_view_compare_w_string() {
    const char *ptr_1 = "foo";
    const char *ptr_2 = "bar";

    flecs::string str_1 = flecs::string_view(ptr_1);
    flecs::string str_2 = flecs::string_view(ptr_2);

    flecs::string str_3 = flecs::string(ecs_os_strdup(ptr_1));
    flecs::string str_4 = flecs::string(ecs_os_strdup(ptr_2));

    test_assert(str_1 == str_1);
    test_assert(str_1 == str_3);

    test_assert(str_2 == str_2);
    test_assert(str_2 == str_4);

    test_assert(str_1 != str_2);
    test_assert(str_2 != str_1);
}

void Misc_string_compare_nullptr() {
    const char *ptr_1 = "foo";

    flecs::string str = flecs::string_view(ptr_1);

    test_assert(str != nullptr);
}

void Misc_nullptr_string_compare() {
    const char *ptr = "foo";

    flecs::string str_1 = flecs::string_view(nullptr);
    flecs::string str_2 = flecs::string_view(ptr);

    test_assert(str_1 == "");
    test_assert(str_1 != ptr);
    test_assert(str_1 != str_2);
}

void Misc_nullptr_string_compare_nullptr() {
    const char *ptr_1 = "foo";

    flecs::string str = flecs::string_view(nullptr);

    test_assert(str == "");
    test_assert(str != ptr_1);
}

static ECS_COMPONENT_DECLARE(Velocity);
static ECS_DECLARE(TagB);
static ECS_DECLARE(E2);
static int sys_invoked_count = 0;
static int obs_invoked_count = 0;
static int trig_invoked_count = 0;

void Sys(ecs_iter_t *it) {
    Position *p = ecs_field(it, Position, 1);
    Velocity *v = ecs_field(it, Velocity, 2);

    test_assert(p != NULL);
    test_assert(v != NULL);

    sys_invoked_count += it->count;
}

void Obs(ecs_iter_t *it) {
    Position *p = ecs_field(it, Position, 1);
    Velocity *v = ecs_field(it, Velocity, 2);

    test_assert(p != NULL);
    test_assert(v != NULL);

    obs_invoked_count += it->count;
}

void Trig(ecs_iter_t *it) {
    Position *p = ecs_field(it, Position, 1);

    test_assert(p != NULL);

    trig_invoked_count += it->count;
}

void Misc_c_macros() {
    flecs::world world;

    ECS_COMPONENT(world, Position);
    ECS_COMPONENT_DEFINE(world, Velocity);

    ECS_TAG(world, TagA);
    ECS_TAG_DEFINE(world, TagB);

    ECS_ENTITY(world, E1, 0);
    ECS_ENTITY_DEFINE(world, E2, 0);

    ECS_MODULE(world, MyModule);
    ECS_SYSTEM(world, Sys, EcsOnUpdate, Position, Velocity);
    ECS_OBSERVER(world, Obs, EcsOnAdd, Position, Velocity);
    ECS_OBSERVER(world, Trig, EcsOnAdd, Position);
    ECS_PIPELINE(world, Pip, flecs.system.System, TagA);

    test_assert(ecs_id(Position) != 0);
    test_assert(ecs_id(Velocity) != 0);

    test_assert(TagA != 0);
    test_assert(TagB != 0);

    test_assert(E1 != 0);
    test_assert(E2 != 0);

    test_assert(ecs_id(MyModule) != 0);
    test_assert(ecs_id(Sys) != 0);
    test_assert(ecs_id(Obs) != 0);
    test_assert(ecs_id(Trig) != 0);
    test_assert(Pip != 0);

    ecs_add(world, E1, TagA);
    ecs_add(world, E1, TagB);

    test_assert( ecs_has(world, E1, TagA));
    test_assert( ecs_has(world, E1, TagB));

    ecs_add(world, E1, Position);
    test_int(obs_invoked_count, 0);
    test_int(trig_invoked_count, 1);
    ecs_add(world, E1, Velocity);
    test_int(obs_invoked_count, 1);

    test_assert( ecs_has(world, E1, Position));
    test_assert( ecs_has(world, E1, Velocity));

    test_assert( ecs_has_id(world, ecs_id(MyModule), EcsModule));
    test_assert( ecs_has(world, Pip, EcsPipeline));
    
    ecs_progress(world, 0);

    test_int(sys_invoked_count, 1);
}

void Misc_app_run() {
    flecs::world ecs;

    bool system_ran = false;
    ecs.system().iter([&](flecs::iter& it) {
        system_ran = true;
        it.world().quit();
    });

    test_int(ecs.app().run(), 0);
    test_bool(system_ran, true);
}

void Misc_app_run_target_fps() {
    flecs::world ecs;

    int32_t count = 0;
    ecs.system().iter([&](flecs::iter& it) {
        count ++;
        test_int(it.world().get_target_fps(), 10);
        it.world().quit();
    });

    ecs.set_target_fps(10);

    ecs.app().run();

    test_int(count, 1);
}

void Misc_app_run_set_frames() {
    flecs::world ecs;

    int32_t count = 0;
    ecs.system().iter([&](flecs::iter& it) {
        count ++;
    });

    ecs.app().frames(100).run();

    test_int(count, 100);
}

void Misc_app_on_remove_on_fini() {
    flecs::world ecs;

    ecs.entity().set<Position>({10, 20});

    int invoked = 0;
    ecs.observer<Position>()
        .event(flecs::OnRemove)
        .each([&](Position& p) {
            invoked ++;
            test_int(p.x, 10);
            test_int(p.y, 20);
        });

    test_int(invoked, 0);
    ecs.app().frames(1).run();
    test_int(invoked, 1);
}

void Misc_member_gauge_metric() {
    flecs::world ecs;

    ecs.import<flecs::metrics>();

    ecs.component<Position>()
        .member<float>("x")
        .member<float>("y");

    ecs.metric("metrics::position_y")
        .kind<flecs::metrics::Gauge>()
        .member<Position>("y");

    flecs::entity e1 = ecs.entity().set<Position>({10, 20});
    flecs::entity e2 = ecs.entity().set<Position>({20, 30});

    ecs.progress();

    int32_t count = 0;
    ecs.filter<flecs::metrics::Source, flecs::metrics::Instance>()
        .iter([&](flecs::iter& it, flecs::metrics::Source *s, flecs::metrics::Instance *i) {
            count += it.count();

            test_int(count, 2);
            test_uint(s[0].entity, e1);
            test_uint(s[1].entity, e2);

            test_int(i[0].value, 20);
            test_int(i[1].value, 30);
        });

    test_int(count, 2);
}

void Misc_id_gauge_metric() {
    flecs::world ecs;

    ecs.import<flecs::metrics>();

    ecs.metric("metrics::has_foo")
        .kind<flecs::metrics::Gauge>()
        .id<Tag>();

    flecs::entity e1 = ecs.entity().add<Tag>();
    flecs::entity e2 = ecs.entity().add<Tag>();

    ecs.progress(1.0);

    int32_t count = 0;
    ecs.filter<flecs::metrics::Source, flecs::metrics::Instance>()
        .iter([&](flecs::iter& it, flecs::metrics::Source *s, flecs::metrics::Instance *i) {
            count += it.count();
            test_int(count, 2);
            test_uint(s[0].entity, e1);
            test_uint(s[1].entity, e2);

            test_int(i[0].value, 1);
            test_int(i[1].value, 1);
        });

    ecs.progress(1.0);
    test_int(count, 2);
    count = 0;

    ecs.filter<flecs::metrics::Source, flecs::metrics::Instance>()
        .iter([&](flecs::iter& it, flecs::metrics::Source *s, flecs::metrics::Instance *i) {
            count += it.count();
            test_int(count, 2);
            test_uint(s[0].entity, e1);
            test_uint(s[1].entity, e2);

            test_int(i[0].value, 1);
            test_int(i[1].value, 1);
        });

    test_int(count, 2);
}

enum Color {
    Red, Green, Blue
};

void Misc_oneof_gauge_metric() {
    flecs::world ecs;

    ecs.import<flecs::metrics>();

    struct ColorMetric {
        double value[3];
    };

    flecs::entity m = ecs.metric("metrics::has_oneof")
        .kind<flecs::metrics::Gauge>()
        .id<Color>(flecs::Wildcard)
        .targets();

    flecs::entity e1 = ecs.entity().add(Color::Red);
    flecs::entity e2 = ecs.entity().add(Color::Green);
    flecs::entity e3 = ecs.entity().add(Color::Blue);

    ecs.progress(1.0);

    int32_t count = 0;
    ecs.filter<flecs::metrics::Source>()
        .iter([&](flecs::iter& it, flecs::metrics::Source *s) {
            ColorMetric *i = static_cast<ColorMetric*>(it.range().get(m, ecs.id<flecs::metrics::Instance>()));
            test_assert(i != nullptr);

            count += it.count();
            test_int(count, 3);
            test_uint(s[0].entity, e1);
            test_uint(s[1].entity, e2);
            test_uint(s[2].entity, e3);

            test_str(ecs.to_json(m, &i[0]), "{\"Blue\":0, \"Green\":0, \"Red\":1}");
            test_str(ecs.to_json(m, &i[1]), "{\"Blue\":0, \"Green\":1, \"Red\":0}");
            test_str(ecs.to_json(m, &i[2]), "{\"Blue\":1, \"Green\":0, \"Red\":0}");
        });

    test_int(count, 3);
    count = 0;

    ecs.filter<flecs::metrics::Source>()
        .iter([&](flecs::iter& it, flecs::metrics::Source *s) {
            ColorMetric *i = static_cast<ColorMetric*>(it.range().get(m, ecs.id<flecs::metrics::Instance>()));
            test_assert(i != nullptr);

            count += it.count();
            test_int(count, 3);
            test_uint(s[0].entity, e1);
            test_uint(s[1].entity, e2);
            test_uint(s[2].entity, e3);

            test_str(ecs.to_json(m, &i[0]), "{\"Blue\":0, \"Green\":0, \"Red\":1}");
            test_str(ecs.to_json(m, &i[1]), "{\"Blue\":0, \"Green\":1, \"Red\":0}");
            test_str(ecs.to_json(m, &i[2]), "{\"Blue\":1, \"Green\":0, \"Red\":0}");
        });

    test_int(count, 3);
}

void Misc_id_counter_metric() {
    flecs::world ecs;

    ecs.import<flecs::metrics>();

    ecs.metric("metrics::has_foo")
        .kind<flecs::metrics::Counter>()
        .id<Tag>();

    flecs::entity e1 = ecs.entity().add<Tag>();
    flecs::entity e2 = ecs.entity().add<Tag>();

    ecs.progress(1.0);

    int32_t count = 0;
    ecs.filter<flecs::metrics::Source, flecs::metrics::Instance>()
        .iter([&](flecs::iter& it, flecs::metrics::Source *s, flecs::metrics::Instance *i) {
            count += it.count();
            test_int(count, 2);
            test_uint(s[0].entity, e1);
            test_uint(s[1].entity, e2);

            test_int(i[0].value, 1);
            test_int(i[1].value, 1);
        });

    ecs.progress(1.0);
    test_int(count, 2);
    count = 0;

    ecs.filter<flecs::metrics::Source, flecs::metrics::Instance>()
        .iter([&](flecs::iter& it, flecs::metrics::Source *s, flecs::metrics::Instance *i) {
            count += it.count();
            test_int(count, 2);
            test_uint(s[0].entity, e1);
            test_uint(s[1].entity, e2);

            test_int(i[0].value, 2);
            test_int(i[1].value, 2);
        });

    test_int(count, 2);
}

void Misc_oneof_counter_metric() {
    flecs::world ecs;

    ecs.import<flecs::metrics>();

    struct ColorMetric {
        double value[3];
    };

    flecs::entity m = ecs.metric("metrics::has_oneof")
        .kind<flecs::metrics::Counter>()
        .id<Color>(flecs::Wildcard)
        .targets();

    flecs::entity e1 = ecs.entity().add(Color::Red);
    flecs::entity e2 = ecs.entity().add(Color::Green);
    flecs::entity e3 = ecs.entity().add(Color::Blue);

    ecs.progress(1.0);

    int32_t count = 0;
    ecs.filter<flecs::metrics::Source>()
        .iter([&](flecs::iter& it, flecs::metrics::Source *s) {
            ColorMetric *i = static_cast<ColorMetric*>(it.range().get(m, ecs.id<flecs::metrics::Instance>()));
            test_assert(i != nullptr);

            count += it.count();
            test_int(count, 3);
            test_uint(s[0].entity, e1);
            test_uint(s[1].entity, e2);
            test_uint(s[2].entity, e3);

            test_str(ecs.to_json(m, &i[0]), "{\"Blue\":0, \"Green\":0, \"Red\":1}");
            test_str(ecs.to_json(m, &i[1]), "{\"Blue\":0, \"Green\":1, \"Red\":0}");
            test_str(ecs.to_json(m, &i[2]), "{\"Blue\":1, \"Green\":0, \"Red\":0}");
        });

    test_int(count, 3);
    ecs.progress(1.0);
    count = 0;

    ecs.filter<flecs::metrics::Source>()
        .iter([&](flecs::iter& it, flecs::metrics::Source *s) {
            ColorMetric *i = static_cast<ColorMetric*>(it.range().get(m, ecs.id<flecs::metrics::Instance>()));
            test_assert(i != nullptr);

            count += it.count();
            test_int(count, 3);
            test_uint(s[0].entity, e1);
            test_uint(s[1].entity, e2);
            test_uint(s[2].entity, e3);

            test_str(ecs.to_json(m, &i[0]), "{\"Blue\":0, \"Green\":0, \"Red\":2}");
            test_str(ecs.to_json(m, &i[1]), "{\"Blue\":0, \"Green\":2, \"Red\":0}");
            test_str(ecs.to_json(m, &i[2]), "{\"Blue\":2, \"Green\":0, \"Red\":0}");
        });

    test_int(count, 3);
}

void Misc_component_mixin_member_metric() {
    flecs::world ecs;

    ecs.import<flecs::metrics>();

    ecs.component<Position>()
        .member<float>("x")
        .member<float>("y").metric<flecs::metrics::Gauge>();

    flecs::entity e1 = ecs.entity().set<Position>({10, 20});
    flecs::entity e2 = ecs.entity().set<Position>({20, 30});

    ecs.progress();

    int32_t count = 0;
    ecs.filter<flecs::metrics::Source, flecs::metrics::Instance>()
        .iter([&](flecs::iter& it, flecs::metrics::Source *s, flecs::metrics::Instance *i) {
            count += it.count();

            test_int(count, 2);
            test_uint(s[0].entity, e1);
            test_uint(s[1].entity, e2);

            test_int(i[0].value, 20);
            test_int(i[1].value, 30);
        });

    test_int(count, 2);
}

void Misc_component_mixin_member_metric_custom_parent_string() {
    flecs::world ecs;

    ecs.import<flecs::metrics>();

    flecs::entity parent = ecs.entity("parent");

    ecs.component<Position>()
        .member<float>("x")
        .member<float>("y").metric<flecs::metrics::Gauge>("parent");

    test_assert(parent.lookup("y") != 0);

    flecs::entity e1 = ecs.entity().set<Position>({10, 20});
    flecs::entity e2 = ecs.entity().set<Position>({20, 30});

    ecs.progress();

    int32_t count = 0;
    ecs.filter<flecs::metrics::Source, flecs::metrics::Instance>()
        .iter([&](flecs::iter& it, flecs::metrics::Source *s, flecs::metrics::Instance *i) {
            count += it.count();

            test_int(count, 2);
            test_uint(s[0].entity, e1);
            test_uint(s[1].entity, e2);

            test_int(i[0].value, 20);
            test_int(i[1].value, 30);

            test_assert(it.entity(0).parent().parent() == parent);
            test_assert(it.entity(1).parent().parent() == parent);
        });

    test_int(count, 2);
}

void Misc_component_mixin_member_metric_custom_parent_entity() {
    flecs::world ecs;

    ecs.import<flecs::metrics>();

    flecs::entity parent = ecs.entity();

    ecs.component<Position>()
        .member<float>("x")
        .member<float>("y").metric<flecs::metrics::Gauge>(parent);

    test_assert(parent.lookup("y") != 0);

    flecs::entity e1 = ecs.entity().set<Position>({10, 20});
    flecs::entity e2 = ecs.entity().set<Position>({20, 30});

    ecs.progress();

    int32_t count = 0;
    ecs.filter<flecs::metrics::Source, flecs::metrics::Instance>()
        .iter([&](flecs::iter& it, flecs::metrics::Source *s, flecs::metrics::Instance *i) {
            count += it.count();

            test_int(count, 2);
            test_uint(s[0].entity, e1);
            test_uint(s[1].entity, e2);

            test_int(i[0].value, 20);
            test_int(i[1].value, 30);

            test_assert(it.entity(0).parent().parent() == parent);
            test_assert(it.entity(1).parent().parent() == parent);
        });

    test_int(count, 2);
}
