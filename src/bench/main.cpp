#include <benchmark/benchmark.h>
#include <LuaIntf.h>

#include <unordered_map>
#include <vector>

namespace LuaIntf {
    LUA_USING_LIST_TYPE(std::vector)
    LUA_USING_MAP_TYPE(std::unordered_map)
}

static void ctx_instantiate(benchmark::State & state) {
    for (auto _ : state) {
        LuaIntf::LuaContext ctx { false };
    }
}

BENCHMARK(ctx_instantiate);

static void global_set_number(benchmark::State & state) {
    LuaIntf::LuaContext ctx { false };

    for (auto _ : state) {
        ctx.setGlobal("foo", 5.0f);
    }
}

BENCHMARK(global_set_number);

static void global_set_string_same(benchmark::State & state) {
    LuaIntf::LuaContext ctx { false };

    std::string const str {
        "foobarfoobarfoobarfoobarfoobarfoobarfoobarfoobar"
        "foobarfoobarfoobarfoobarfoobarfoobarfoobarfoobar"
    };

    ctx.setGlobal("foo", str);

    for (auto _ : state) {
        ctx.setGlobal("foo", str);
    }
}

BENCHMARK(global_set_string_same);

static void global_call(benchmark::State & state) {
    LuaIntf::LuaContext ctx { false };

    ctx.doString("fun = function(v) return v end");
    LuaIntf::LuaRef fun = ctx.getGlobal("fun");
    float result;

    for (auto _ : state) {
        benchmark::DoNotOptimize(result = fun.call<float>(42.0f));
    }
}

BENCHMARK(global_call);

static void global_set_map_int(benchmark::State & state) {
    LuaIntf::LuaContext ctx { false };

    std::unordered_map<int, int> m;

    for (int k = 0; k < state.range(0); k++) {
        m.insert({ k, 42 });
    }

    for (auto _ : state) {
        ctx.setGlobal("foo", m);
    }
}

BENCHMARK(global_set_map_int)->Range(1, 1024);

static void global_set_vector_int(benchmark::State & state) {
    LuaIntf::LuaContext ctx { false };

    std::vector<int> m(state.range(0), 42);

    for (auto _ : state) {
        ctx.setGlobal("foo", m);
    }
}

BENCHMARK(global_set_vector_int)->Range(1, 16 * 1024);

BENCHMARK_MAIN();
