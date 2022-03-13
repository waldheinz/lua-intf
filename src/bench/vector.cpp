#include <benchmark/benchmark.h>
#include <LuaIntf.h>

#include <vector>

namespace LuaIntf {
    LUA_USING_LIST_TYPE(std::vector)
}

static void vector_push_int(benchmark::State & state) {
    LuaIntf::LuaContext ctx { false };

    std::vector<int> m(state.range(0), 42);

    for (auto _ : state) {
        ctx.setGlobal("foo", m);
    }
}

BENCHMARK(vector_push_int)->Range(1, 8 * 1024);


static void vector_get_int(benchmark::State & state) {
    LuaIntf::LuaContext ctx { false };

    std::vector<int> m(state.range(0), 42);
    ctx.setGlobal("foo", m);

    for (auto _ : state) {
        ctx.getGlobal<std::vector<int>>("foo");
    }
}

BENCHMARK(vector_get_int)->Range(1, 8 * 1024);
