# A Very Hard Game

[Play it now!!](https://nickgirardo.com/a-very-hard-game/index.html)

I wrote [a post](https://nickgirardo.com/posts/avhg-thoughts.html) about the process of making this. Please read it if you're interested!

A game built for the [GameTank](https://gametank.zone)!

Inspired by the classic flash game [World's Hardest Game](https://www.coolmathgames.com/0-worlds-hardest-game).

## Building:

AVHG can be built manually, but I'd recommend using Nix.  With Nix installed (and flakes enabled), the game can be built with `nix build` (or `nix build .#avhg`).

It is possible to build AVHG manually without the use of Nix.  AVHG has the following dependencies which must be installed:
* [cc65](https://github.com/cc65/cc65) (make sure to use a recent unstable release, it's been years since the most recent stable release)
* [NodeJS](https://nodejs.org/)
* [Zopfli](https://github.com/google/zopfli)
* [GNU Make](https://www.gnu.org/software/make/)

With all of the dependencies installed, first run `make import` to process AVHG's assets (this will need to be executed again when the game's assets are changed) and then run `make`.

### Building for web

Building for the web is also best done through Nix.  You can build web assets with `nix build .#web-emulator` or `nix build .#web-emulator-embed` for the version meant to be embedded.

Building for the web manually is a fairly involved process and unfortunately cannot be recommended.  First the AVHG rom must be built (as above). The rom and any desired web assets then must be included in the build process of the [GameTank Emulator](https://github.com/clydeshaffer/GameTankEmulator). As far as I am aware, there is no automated way to achieve this aside from using Nix.

## Running

You can run the game in the emulator with `nix run` (or `nix run .#emulate`).

`nix run .#emulate-web` will open a server (default port `8080`) with the game running in the web emulator. An alternative port can be specified as the first argument, e.g. `nix run .#emulate-web -- 3000`.

## Credits

- [Nick Girardo](https://github.com/nickgirardo)
- Blackfish - Music
- Special Thanks:
   - [Clyde Shaffer](https://github.com/clydeshaffer) - Tons of help across the board
   - [darthbeep](https://github.com/darthbeep) - Playtesting and level design feedback
