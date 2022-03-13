{
  description = "LuaIntf Lua <-> C++ Bindings";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        liblua-intf = pkgs.callPackage (import self) { };
      in {
        packages.liblua-intf = liblua-intf;
        defaultPackage = self.packages.${system}.liblua-intf;
        devShell = pkgs.mkShell {
          buildInputs = with pkgs; [ linuxPackages.perf ];
          inputsFrom = [ self.packages.${system}.liblua-intf ];
        };
      }
    );
}
