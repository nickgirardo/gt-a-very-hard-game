{
  description = "A Very Hard Game, an action game for the GameTank";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/release-24.05";
    gitignore = {
      url = "github:hercules-ci/gitignore.nix";
      inputs.nixpkgs.follows = "nixpkgs";
    };
    cc65.url = "github:nickgirardo/nix-cc65-unstable/18b6c7417518c54bb95a2071782f2603d883edc9";
    GameTankEmulator.url = "github:nickgirardo/nix-GameTankEmulator/2e4950d668e1110add3ab7b6407bed718073d8cd";
    GTFO.url = "github:nickgirardo/nix-GTFO/e159f175b9ef3c2698f8c81a6843fac2fd3fcef0";
  };

  outputs = { self, nixpkgs, gitignore, cc65, GameTankEmulator, GTFO }:
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
          cc65.outputs.packages.${system}.default
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
            cp -r bin $out
        '';
      };

      # TODO do I need to mark avhg as a dependency?
      web-emulator = GameTankEmulator.outputs.packages.${system}.gte-web.overrideAttrs (final: prev: {
        ROMFILE_SRC = "${avhg}/bin/game.gtr";
        ROMFILE = "roms/avhg.gtr";
      });
    in {
      packages.${system} = {
          inherit avhg web-emulator;
          default = avhg;
      };
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
        emu-web = pkgs.writeShellApplication {
          name = "emulate-web";
          runtimeInputs = [ pkgs.caddy ];
          text = ''
            # Takes the port as first argument
            # Default to port 8080
            PORT="''${1:-8080}"
            caddy file-server --listen :"$PORT" --root ${web-emulator.outPath}/dist
          '';
        };
        emulate-web = {
          type = "app";
          program = "${emu-web}/bin/emulate-web";
        };
        flash_ = pkgs.writeShellApplication {
          name = "flash";
          runtimeInputs = [ GTFO.outputs.packages.${system}.default ];
          text = ''
               if [ "$#" -ne 1 ]; then
                  echo "Please specify the port to flash onto"
                  exit 1
               fi
               GTFO -p "$1" ${avhg}/bin/game.gtr.bank*
            '';
        };
        flash = {
          type = "app";
          program = "${flash_}/bin/flash";
        };
      in {
        inherit emulate emulate-web flash;
        default = emulate;
      };
    };
}
