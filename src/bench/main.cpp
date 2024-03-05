#include <benchmark/benchmark.h>
#include <LuaIntf.h>

static void ctx_instantiate(benchmark::State & state) {
    for (auto _ : state) {
        LuaIntf::LuaContext ctx { false };
    }
}

static void global_call(benchmark::State & state) {
    LuaIntf::LuaContext ctx { false };

    ctx.doString("fun = function(v) return v end");
    LuaIntf::LuaRef fun = ctx.getGlobal("fun");
    float result;

    for (auto _ : state) {
        benchmark::DoNotOptimize(result = fun.call<float>(42.0f));
    }
}

static void global_set_number(benchmark::State & state) {
    LuaIntf::LuaContext ctx { false };

    for (auto _ : state) {
        ctx.setGlobal("foo", 5.0f);
    }
}

static void global_set_table(benchmark::State & state) {
    LuaIntf::LuaContext ctx { false };

    for (auto _ : state) {
        LuaIntf::Lua::pushTable<std::string_view, int>(ctx, {
            { "x", 1 }, { "y", 1 }, { "w", 1 }, { "h", 1 }
        });
        LuaIntf::Lua::pop(ctx);
    }
}

static void global_set_via_pusher(benchmark::State & state) {
    LuaIntf::LuaContext ctx { false };
    for (auto _ : state) {
        {
            LuaIntf::TablePusher p(ctx, 4);
            p.push("x", 1)
             .push("y", 1)
             .push("w", 1)
             .push("h", 1);
        }

        LuaIntf::Lua::pop(ctx);
    }
}

static void global_set_table_slow(benchmark::State & state) {
    LuaIntf::LuaContext ctx { false };

    for (auto _ : state) {
        auto tbl = LuaIntf::LuaRef::createTable(ctx, 0, 4);

        tbl.rawset("x", 1);
        tbl.rawset("y", 1);
        tbl.rawset("w", 1);
        tbl.rawset("h", 1);

        LuaIntf::Lua::push(ctx, tbl);
        LuaIntf::Lua::pop(ctx);
    }
}

BENCHMARK(ctx_instantiate);
BENCHMARK(global_call);
BENCHMARK(global_set_number);
BENCHMARK(global_set_table_slow);
BENCHMARK(global_set_table);
BENCHMARK(global_set_via_pusher);

BENCHMARK_MAIN();
