{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    flake-parts.url = "github:hercules-ci/flake-parts";
  };

  outputs =
    inputs@{ flake-parts, ... }:
    flake-parts.lib.mkFlake { inherit inputs; } {
      systems = [
        "aarch64-darwin"
        "x86_64-darwin"
        "aarch64-linux"
        "x86_64-linux"
      ];

      perSystem =
        { pkgs, ... }:
        {
          devShells.default = pkgs.mkShell.override { stdenv = pkgs.clangStdenv; } {
            packages = [
              pkgs.clang-tools
              pkgs.criterion
              pkgs.frama-c
              pkgs.frama-c-gui
              pkgs.cbmc
            ];
          };
        };
    };
}
