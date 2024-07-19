{
  description = "A Very Hard Game, an action game for the GameTank";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/release-24.05";
    gitignore = {
      url = "github:hercules-ci/gitignore.nix";
      inputs.nixpkgs.follows = "nixpkgs";
    };
    cc65.url = "github:nickgirardo/nix-cc65-unstable/b6ce9c5b4dfe5622dc9460da673a907326a23f74";
    GameTankEmulator.url = "github:nickgirardo/nix-GameTankEmulator/12b36c60a5f0da85405b6c02df8b5e911079481d";
  };

  outputs = { self, nixpkgs, gitignore, cc65, GameTankEmulator }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
      node_scripts = pkgs.buildNpmPackage {
        name = "node_scripts";
        src = gitignore.lib.gitignoreSource ./scripts;
        npmDepsHash = "sha256-q+CvN0cnzBB9Kn1+6kaGHhgJcqMdf4piuzvedtyWJDo=";
        dontNpmBuild = true;
        installPhase = ''
          mkdir -p $out/bin
          cp -r . $out/bin
        '';
      };
      avhg = pkgs.stdenv.mkDerivation {
        inherit system;
        name = "A Very Hard Game";

        src = gitignore.lib.gitignoreSource ./.;

        nativeBuildInputs = [
          cc65.outputs.packages.x86_64-linux.default
          pkgs.gnumake
          pkgs.zip
          pkgs.nodejs
          pkgs.zopfli
        ];

        NODE_SCRIPTS="${node_scripts}/bin";
        CC65_LIB="${cc65.outputs.packages.${system}.default}/share/cc65/lib";

        phases = [
          "unpackPhase"
          "patchPhase"
          "preBuildPhase"
          "buildPhase"
          "installPhase"
        ];

        preBuildPhase = "node $NODE_SCRIPTS/build_setup/import_assets.js";

        buildPhase = "make bin/game.gtr";

        installPhase = ''
            mkdir -p $out/bin
            cp bin/game.gtr $out/bin
        '';
      };

    in {
      packages.${system}.default = avhg;
      apps.${system} = let
        emu = pkgs.writeShellApplication {
          name = "emulate";
          runtimeInputs = [ GameTankEmulator.outputs.packages.${system}.default ];
          text = "GameTankEmulator ${avhg}/bin/game.gtr";
        };
        emulate = {
          type = "app";
          program = "${emu}/bin/emulate";
        };
      in {
        inherit emulate;
        default = emulate;
      };
    };
}
