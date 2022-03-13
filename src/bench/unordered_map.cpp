#include <benchmark/benchmark.h>
#include <LuaIntf.h>

#include <unordered_map>

namespace LuaIntf {
    LUA_USING_MAP_TYPE(std::unordered_map)
}

static void unordered_map_push(benchmark::State & state) {
    LuaIntf::LuaContext ctx { false };

    std::unordered_map<int, int> m;

    for (int k = 0; k < state.range(0); k++) {
        m.insert({ k, 42 });
    }

    for (auto _ : state) {
        ctx.setGlobal("foo", m);
    }
}

BENCHMARK(unordered_map_push)->Range(1, 1024);
