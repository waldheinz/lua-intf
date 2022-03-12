{ stdenv, cmake, lua }:

stdenv.mkDerivation {
  name = "LuaIntf";
  src = ./src;
  nativeBuildInputs = [ cmake ];
  buildInputs = [ lua ];
}
