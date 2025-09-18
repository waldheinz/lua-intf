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

static void table_has_multiple(benchmark::State & state) {
    LuaIntf::LuaContext ctx { false };

    auto tbl = LuaIntf::LuaRef::createTable(ctx);
    tbl.set("foo", 1.0);
    tbl.set("bar", 1.0);

    for (auto _ : state) {
        tbl.has("foo") && tbl.has("bar") && tbl.has("foobar") && tbl.has("barfoo");
    }
}

BENCHMARK(table_has_multiple);

static void table_get_rect(benchmark::State & state) {
    LuaIntf::LuaContext ctx { false };

    auto tbl = LuaIntf::LuaRef::createTable(ctx);
    tbl.set("x", 1.0);
    tbl.set("y", 1.0);
    tbl.set("w", 1.0);
    tbl.set("h", 1.0);

    tbl.pushToStack();

    for (auto _ : state) {
        LuaIntf::LuaRef ref { ctx, -1 };
        tbl.rawget("x");
        tbl.rawget("y");
        tbl.rawget("w");
        tbl.rawget("h");
    }

    LuaIntf::Lua::pop(ctx);
}

BENCHMARK(table_get_rect);


static void table_get_rect_fast(benchmark::State & state) {
    LuaIntf::LuaContext ctx { false };

    auto tbl = LuaIntf::LuaRef::createTable(ctx);
    tbl.set("x", 1.0);
    tbl.set("y", 1.0);
    tbl.set("w", 1.0);
    tbl.set("h", 1.0);

    tbl.pushToStack();

    LuaIntf::LuaState st { ctx };

    for (auto _ : state) {
        st.popField<double>(-1, "x");
        st.popField<double>(-1, "y");
        st.popField<double>(-1, "w");
        st.popField<double>(-1, "h");
    }

    st.pop();
}

BENCHMARK(table_get_rect_fast);
