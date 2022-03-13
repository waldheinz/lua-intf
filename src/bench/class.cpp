#include <benchmark/benchmark.h>
#include <LuaIntf.h>

class rect final {
public:
    rect(double x, double y, double w, double h)
        : x(x), y(y), w(w), h(h) { }

private:
    double x, y, w, h;
};

static void class_ctr(benchmark::State & state) {
    using namespace LuaIntf;

    LuaContext ctx { false };

    LuaBinding(ctx).beginClass<rect>("rect")
        .addConstructor(LUA_ARGS(double, double, double, double))
    .endClass();

    ctx.doString(
        "fun = function()"
        "  rect(1, 2, 3, 4)"
        "end"
    );

    auto const fun = ctx.getGlobal("fun");

    for (auto _ : state) {
        fun();
    }
}

BENCHMARK(class_ctr);
