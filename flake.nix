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
      cc65_ = cc65.outputs.packages.${system}.default;
      avhg = pkgs.stdenv.mkDerivation {
        inherit system;
        name = "A Very Hard Game";

        src = gitignore.lib.gitignoreSource ./.;

        nativeBuildInputs = with pkgs; [ gnumake zip nodejs zopfli ];

        preBuildPhase = ''
            node ${node_scripts}/bin/build_setup/import_assets.js
        '';

        buildPhase = ''
            runHook preBuildPhase

            export PATH="${cc65_}/bin:$PATH"
            export NODE_SCRIPTS="${node_scripts}/bin"
            export CC65_LIB="${cc65_}/share/cc65/lib"

            make bin/game.gtr

            mkdir -p $out/bin
            cp bin/game.gtr $out/bin
        '';

        installPhase = "ls -la";
      };

    in {
      packages.${system}.default = avhg;
      apps.${system} = let
        emu = pkgs.writeShellApplication {
          name = "emulate";
          text = ''
            ${GameTankEmulator.outputs.packages.${system}.default}/bin/GameTankEmulator ${avhg}/bin/game.gtr
          '';
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
