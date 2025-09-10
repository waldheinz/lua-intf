#include <benchmark/benchmark.h>
#include <LuaIntf.h>

#include <functional>

struct size_double {
    size_double() = default;

    size_double(double w, double h)
        : w(w), h(h) { }

    double w, h;
};

class rect final {
public:
    rect() = default;

    rect(double x, double y, double w, double h)
        : x(x), y(y), sz({ w, h }) { }

    double get_x() const {
        return x;
    }

    void set_x(double x) {
        this->x = x;
    }

    size_double get_size() const {
        return sz;
    }

    void set_size(size_double sz) {
        this->sz = sz;
    }

    void move(double dx, double dy) {
        x += dx;
        y += dy;
    }

    double x, y;
    size_double sz;
};

class change_notifier final {
public:
    using cb_t = std::function<rect(rect)>;

    change_notifier() = default;

    void set_cb(cb_t cb) {
        this->cb = cb;
    }

    void set_rect(rect r) {
        if (cb) {
            this->r = cb(r);
        } else {
            this->r = r;
        }
    }

private:
    rect r;
    cb_t cb;
};

static void register_lua(LuaIntf::LuaContext & ctx) {
    LuaIntf::LuaBinding(ctx)
    .beginClass<size_double>("size_double")
        .addConstructor(LUA_ARGS(double, double))
        .addVariable("w", &size_double::w)
        .addVariable("h", &size_double::h)
    .endClass()
    .beginClass<rect>("rect")
        .addConstructor(LUA_ARGS(double, double, double, double))
        .addFunction("move", &rect::move)
        .addProperty("x", &rect::get_x, &rect::set_x)
        .addProperty("size", &rect::get_size, &rect::set_size)
    .endClass()
    .beginClass<change_notifier>("change_notifier")
        .addConstructor(LUA_ARGS())
        .addFunction("set_cb", &change_notifier::set_cb)
        .addFunction("set_rect", &change_notifier::set_rect)
    .endClass();
}

static void class_call_method(benchmark::State & state) {
    using namespace LuaIntf;

    LuaContext ctx { false };
    register_lua(ctx);

    ctx.doString(
        "local r = rect(1, 2, 3, 4)\n"
        "fun = function()\n"
        "  r:move(0.1, 0.1)\n"
        "end\n"
    );

    auto const fun = ctx.getGlobal("fun");

    for (auto _ : state) {
        fun();
    }
}

static void class_callback(benchmark::State & state) {
    using namespace LuaIntf;

    LuaContext ctx { false };
    register_lua(ctx);
    ctx.doString(
        "set_up = function(cn)\n"
        "  cn:set_cb(function(r)\n"
        "    return r\n"
        "  end)\n"
        "end\n"
    );

    change_notifier cn;
    ctx.getGlobal("set_up").call(&cn);
    rect const r(1, 2, 3, 4);

    for (auto _ : state) {
        cn.set_rect(r);
    }
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

static void class_prop_get_class(benchmark::State & state) {
    using namespace LuaIntf;

    LuaContext ctx { false };
    register_lua(ctx);

    ctx.doString(
        "local r = rect(1, 2, 3, 4)\n"
        "fun = function()\n"
        "  local sz = r.size\n"
        "end\n"
    );

    auto const fun = ctx.getGlobal("fun");

    for (auto _ : state) {
        fun();
    }
}

static void class_prop_get_double(benchmark::State & state) {
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

static void class_prop_set_class(benchmark::State & state) {
    using namespace LuaIntf;

    LuaContext ctx { false };
    register_lua(ctx);

    ctx.doString(
        "local r = rect(1, 2, 3, 4)\n"
        "local some_size = size_double(6, 7)\n"
        "fun = function()\n"
        "  r.size = some_size\n"
        "end\n"
    );

    auto const fun = ctx.getGlobal("fun");

    for (auto _ : state) {
        fun();
    }
}

static void class_prop_set_double(benchmark::State & state) {
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

static void class_register(benchmark::State & state) {
    using namespace LuaIntf;

    LuaContext ctx { false };

    for (auto _ : state) {
        register_lua(ctx);
    }
}

BENCHMARK(class_call_method);
BENCHMARK(class_callback);
BENCHMARK(class_ctr);
BENCHMARK(class_prop_get_class);
BENCHMARK(class_prop_get_double);
BENCHMARK(class_prop_set_class);
BENCHMARK(class_prop_set_double);
BENCHMARK(class_register);
