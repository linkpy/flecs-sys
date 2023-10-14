const std = @import("std");

pub fn build(b: *std.Build) void {
	const target = b.standardTargetOptions(.{});
	const optimize = b.standardOptimizeOption(.{});

	const lib = b.addStaticLibrary(.{
		.name = "flecs",
		.root_source_file = null,
		.target = target,
		.optimize = optimize,
		.version = .{
			.major = 3,
			.minor = 2,
			.patch = 0,
		},
	});

	lib.linkLibC();

	lib.addIncludePath(.{ .path = "include/" });
	lib.addCSourceFiles(c_files, c_flags);

	b.installArtifact(lib);

	_ = b.addModule("flecs-sys", .{
		.source_file = .{ .path = "src/flecs.zig" },
	});
}


const c_flags: []const []const u8 = &.{
	"--std=c99",
	"-Dflecs_STATIC",
	"-Dflecs_EXPORTS",
};

const c_files: []const []const u8 = &.{
    "src/addons/alerts.c",
    "src/addons/coredoc.c",
    "src/addons/doc.c",
    "src/addons/expr/deserialize.c",
    "src/addons/expr/serialize.c",
    "src/addons/expr/utils.c",
    "src/addons/expr/vars.c",
    "src/addons/flecs_cpp.c",
    "src/addons/http.c",
    "src/addons/journal.c",
    "src/addons/json/deserialize.c",
    "src/addons/json/serialize.c",
    "src/addons/json/serialize_type_info.c",
    "src/addons/json/json.c",
    "src/addons/log.c",
    "src/addons/meta/api.c",
    "src/addons/meta/meta.c",
    "src/addons/meta/serialized.c",
    "src/addons/meta/cursor.c",
    "src/addons/meta_c.c",
    "src/addons/metrics.c",
    "src/addons/module.c",
    "src/addons/monitor.c",
    "src/addons/os_api_impl/os_api_impl.c",
    "src/addons/parser.c",
    "src/addons/pipeline/pipeline.c",
    "src/addons/pipeline/worker.c",
    "src/addons/plecs.c",
    "src/addons/rest.c",
    "src/addons/rules/api.c",
    "src/addons/rules/compile.c",
    "src/addons/rules/engine.c",
    "src/addons/rules/trav_cache.c",
    "src/addons/snapshot.c",
    "src/addons/stats.c",
    "src/addons/system/system.c",
    "src/addons/timer.c",
    "src/addons/units.c",
    "src/datastructures/allocator.c",
    "src/datastructures/bitset.c",
    "src/datastructures/block_allocator.c",
    "src/datastructures/hash.c",
    "src/datastructures/hashmap.c",
    "src/datastructures/map.c",
    "src/datastructures/stack_allocator.c",
    "src/datastructures/name_index.c",
    "src/datastructures/sparse.c",
    "src/datastructures/strbuf.c",
    "src/datastructures/switch_list.c",
    "src/datastructures/vec.c",
    "src/storage/entity_index.c",
    "src/storage/id_index.c",
    "src/storage/table.c",
    "src/storage/table_cache.c",
    "src/storage/table_graph.c",
    "src/bootstrap.c",
    "src/entity.c",
    "src/entity_filter.c",
    "src/entity_name.c",
    "src/filter.c",
    "src/iter.c",
    "src/misc.c",
    "src/observable.c",
    "src/observer.c",
    "src/os_api.c",
    "src/poly.c",
    "src/query.c",
    "src/stage.c",
    "src/search.c",
    "src/value.c",
    "src/world.c",
};