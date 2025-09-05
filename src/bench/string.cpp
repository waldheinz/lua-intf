#include <benchmark/benchmark.h>
#include <LuaIntf.h>

static void string_set_same(benchmark::State & state) {
    LuaIntf::LuaContext ctx { false };

    std::string const str(state.range(0), 'x');

    ctx.setGlobal("foo", str);

    for (auto _ : state) {
        ctx.setGlobal("foo", str);
    }
}

BENCHMARK(string_set_same)->Range(1, 2048);

static void string_get(benchmark::State & state) {
    LuaIntf::LuaContext ctx { false };
    ctx.setGlobal("foo", std::string(state.range(0), 'x'));
    std::string result;

    for (auto _ : state) {
        benchmark::DoNotOptimize(result = ctx.getGlobal<std::string>("foo"));
    }
}

BENCHMARK(string_get)->Range(1, 2048);;

static void string_view_get(benchmark::State & state) {
    LuaIntf::LuaContext ctx { false };
    ctx.setGlobal("foo", std::string(state.range(0), 'x'));

    for (auto _ : state) {
        benchmark::DoNotOptimize(ctx.getGlobal<std::string_view>("foo"));
    }
}

BENCHMARK(string_view_get)->Range(1, 2048);;
