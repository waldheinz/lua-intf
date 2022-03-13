{ stdenv, cmake, lua5_4, gbenchmark ? null }:

let
  # a Lua compiled with C++ for exception support
  lua-cpp = lua5_4.overrideAttrs (old: {
    makeFlags = old.makeFlags ++ [ "CC=${stdenv.cc.targetPrefix}c++" ];
    dontStrip = true;
  });

in stdenv.mkDerivation {
  name = "LuaIntf";
  src = ./src;
  nativeBuildInputs = [ cmake ];
  buildInputs = [ lua-cpp gbenchmark ];
}
