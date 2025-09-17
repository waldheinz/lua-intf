#include <benchmark/benchmark.h>
#include <LuaIntf.h>

static void table_has_string(benchmark::State & state) {
    LuaIntf::LuaContext ctx { false };

    auto tbl = LuaIntf::LuaRef::createTable(ctx);
    std::string key { "foo" };
    tbl.set(key, 1);

    for (auto _ : state) {
        tbl.has(key);
    }
}

BENCHMARK(table_has_string);

static void table_has_cstring(benchmark::State & state) {
    LuaIntf::LuaContext ctx { false };

    auto tbl = LuaIntf::LuaRef::createTable(ctx);
    std::string key { "foo" };
    tbl.set(key, 1);

    for (auto _ : state) {
        tbl.has(key.c_str());
    }
}

BENCHMARK(table_has_cstring);

static void table_has_sv(benchmark::State & state) {
    LuaIntf::LuaContext ctx { false };

    auto tbl = LuaIntf::LuaRef::createTable(ctx);
    std::string_view key { "foo" };
    tbl.set(key, 1);

    for (auto _ : state) {
        tbl.has(key);
    }
}

BENCHMARK(table_has_sv);

static void table_get_string(benchmark::State & state) {
    LuaIntf::LuaContext ctx { false };

    auto tbl = LuaIntf::LuaRef::createTable(ctx);
    std::string key { "foo" };
    tbl.set(key, 1.0);

    for (auto _ : state) {
        tbl.get<double>(key);
    }
}

BENCHMARK(table_get_string);

static void table_get_cstring(benchmark::State & state) {
    LuaIntf::LuaContext ctx { false };

    auto tbl = LuaIntf::LuaRef::createTable(ctx);
    std::string key { "foo" };
    tbl.set(key, 1.0);

    for (auto _ : state) {
        tbl.get<double>(key.c_str());
    }
}

BENCHMARK(table_get_cstring);

static void table_get_sv(benchmark::State & state) {
    LuaIntf::LuaContext ctx { false };

    auto tbl = LuaIntf::LuaRef::createTable(ctx);
    std::string_view key { "foo" };
    tbl.set(key, 1.0);

    for (auto _ : state) {
        tbl.get<double>(key);
    }
}

BENCHMARK(table_get_sv);
