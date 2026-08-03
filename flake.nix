{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    flake-parts.url = "github:hercules-ci/flake-parts";
    git-hooks.url = "github:cachix/git-hooks.nix";
    git-hooks.inputs.nixpkgs.follows = "nixpkgs";
  };

  outputs =
    inputs@{ flake-parts, git-hooks, ... }:
    flake-parts.lib.mkFlake { inherit inputs; } {
      systems = [
        "aarch64-darwin"
        "aarch64-linux"
        "x86_64-linux"
      ];

      imports = [
        git-hooks.flakeModule
      ];

      perSystem = { config, lib, pkgs, ... }:
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
              make lint
              runHook postCheck
            '';
          });

          pre-commit.check.enable = true;
          pre-commit.settings.hooks = {
            nixpkgs-fmt.enable = true;
            clang-format.enable = true;
            clang-format.files = "\\.[ch]$";
          };

          devShells.default = pkgs.mkShell.override { stdenv = pkgs.clangStdenv; } {
            shellHook = config.pre-commit.shellHook;

            packages = devPkgs ++ config.pre-commit.settings.enabledPackages ++ [
              pkgs.frama-c
              pkgs.frama-c-gui
              pkgs.cbmc
            ];
          };
        };
    };
}
