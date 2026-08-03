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
        "aarch64-linux"
        "x86_64-linux"
      ];

      perSystem =
        { pkgs, ... }:
        let
          devPkgs = [
            pkgs.clang-tools
            pkgs.criterion
          ];

          chipdale = pkgs.clangStdenv.mkDerivation {
            pname = "chipdale";
            version = "0";
            src = ./.;

            dontConfigure = true;

            buildPhase = ''
              runHook preBuild
              make
              runHook postBuild
            '';

            installPhase = ''
              runHook preInstall
              mkdir -p $out/bin
              cp bin/chipdale $out/bin/
              runHook postInstall
            '';

            meta.mainProgram = "chipdale";
          };
        in
        {
          packages.default = chipdale;

          checks.default = chipdale.overrideAttrs (prev: {
            nativeBuildInputs = (prev.nativeBuildInputs or [ ]) ++ devPkgs;

            doCheck = true;

            checkPhase = ''
              runHook preCheck
              make test
              make format-check
              runHook postCheck
            '';
          });

          devShells.default = pkgs.mkShell.override { stdenv = pkgs.clangStdenv; } {
            packages = devPkgs ++ [
              pkgs.frama-c
              pkgs.frama-c-gui
              pkgs.cbmc
            ];
          };
        };
    };
}
