#include <benchmark/benchmark.h>
#include <LuaIntf.h>

class rect final {
public:
    rect(double x, double y, double w, double h)
        : x(x), y(y), w(w), h(h) { }

    double get_x() const {
        return x;
    }

    void set_x(double x) {
        this->x = x;
    }

private:
    double x, y, w, h;
};

static void register_lua(LuaIntf::LuaContext & ctx) {
    LuaIntf::LuaBinding(ctx).beginClass<rect>("rect")
        .addConstructor(LUA_ARGS(double, double, double, double))
        .addProperty("x", &rect::get_x, &rect::set_x)
    .endClass();
}

static void class_ctr(benchmark::State & state) {
    using namespace LuaIntf;

    LuaContext ctx { false };
    register_lua(ctx);

    ctx.doString(
        "fun = function()\n"
        "  rect(1, 2, 3, 4)\n"
        "end\n"
    );

    auto const fun = ctx.getGlobal("fun");

    for (auto _ : state) {
        fun();
    }
}

BENCHMARK(class_ctr);

static void class_prop_get(benchmark::State & state) {
    using namespace LuaIntf;

    LuaContext ctx { false };
    register_lua(ctx);

    ctx.doString(
        "local r = rect(1, 2, 3, 4)\n"
        "fun = function()\n"
        "  local x = r.x\n"
        "end\n"
    );

    auto const fun = ctx.getGlobal("fun");

    for (auto _ : state) {
        fun();
    }
}

BENCHMARK(class_prop_get);


static void class_prop_set(benchmark::State & state) {
    using namespace LuaIntf;

    LuaContext ctx { false };
    register_lua(ctx);

    ctx.doString(
        "local r = rect(1, 2, 3, 4)\n"
        "fun = function()\n"
        "  r.x = 5\n"
        "end\n"
    );

    auto const fun = ctx.getGlobal("fun");

    for (auto _ : state) {
        fun();
    }
}

BENCHMARK(class_prop_set);
