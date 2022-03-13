#include <benchmark/benchmark.h>
#include <LuaIntf.h>

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

BENCHMARK_MAIN();
