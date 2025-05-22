{
  description = "Clay (short for C Layout) is a high performance 2D UI layout library.";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs = {
    self,
    nixpkgs,
  }: let
    inherit (nixpkgs) lib;
    systems = lib.systems.flakeExposed;
    forAllSystems = lib.genAttrs systems;
    nixpkgsFor = forAllSystems (system: nixpkgs.legacyPackages.${system});
  in {
    checks = forAllSystems (
      system: let
        pkgs = nixpkgsFor.${system};
      in {
        formatting =
          pkgs.runCommand "format-clay" {
            nativeBuildInputs = with pkgs; [
              alejandra
              clang-tools
              deadnix
              statix
            ];
          } ''
            echo "Checking nix formatting"
            alejandra --check .

            echo "Checking dead code"
            deadnix --fail .

            echo "Checking statix"
            statix check .

            echo "Checking formatting of C code"
            clang-format --dry-run --Werror --style=file examples/**.c
            clang-format --dry-run --Werror --style=file renderers/**.c
          '';
      }
    );

    devShells = forAllSystems (
      system: let
        pkgs = nixpkgsFor.${system};
      in {
        default = pkgs.mkShell {
          # For testing renderers and examples, aswell as tests
          packages = with pkgs; [
            # Renderers
            SDL2
            SDL2_image
            SDL2_ttf

            sdl3
            sdl3-ttf
            sdl3-image

            cairo
            raylib
            sokol

            cmake # this is needed for building examples and tests
            gcc
            clang-tools # for clangd and clang-tidy
            pkg-config
            alejandra # for formatting nix

            # for tests
            docker
            docker-compose

            # dependencies for examples and tests
            xorg.libXrandr
            xorg.libXinerama
            xorg.libXcursor
            xorg.libXi
          ];

          LD_LIBRARY_PATH = "${lib.makeLibraryPath [
            # Renderers
            pkgs.SDL2
            pkgs.SDL2_image
            pkgs.SDL2_ttf

            pkgs.sdl3
            pkgs.sdl3-ttf
            pkgs.sdl3-image

            pkgs.cairo
            pkgs.raylib
            pkgs.sokol

            # dependencies for examples and tests

            pkgs.xorg.libXrandr
            pkgs.xorg.libXinerama
            pkgs.xorg.libXcursor
            pkgs.xorg.libXi
          ]}";
        };
      }
    );

    formatter = forAllSystems (system: nixpkgsFor.${system}.alejandra);

    packages = forAllSystems (
      system: let
        pkgs = nixpkgsFor.${system};
        date = let
          # YYYYMMDD
          date' = lib.substring 0 8 self.lastModifiedDate;
          year = lib.substring 0 4 date';
          month = lib.substring 4 2 date';
          date = lib.substring 6 2 date';
        in
          if (self ? "lastModifiedDate")
          then
            lib.concatStringsSep "-" [
              year
              month
              date
            ]
          else "unknown";
      in {
        default = pkgs.stdenv.mkDerivation {
          pname = "clay";
          version = "v0.13-master-${date}";
          src = ./.;

          dontBuild = true;
          dontInstall = false;

          installPhase = ''
            mkdir -p $out/include
            cp -r clay.h $out/include

            mkdir -p $out/lib/pkgconfig
            echo "prefix=$out
              includedir=$out/include

              Name: Clay
              Description: Clay (short for C Layout) is a high performance 2D UI layout library.
              Version: v0.13-master-${date}
              Cflags: -I$out/include" > $out/lib/pkgconfig/clay.pc
          '';

          meta = with lib; {
            description = "Clay (short for C Layout) is a high performance 2D UI layout library.";
            homepage = "https://github.com/nicbarker/clay";
            license = licenses.zlib;
            maintainers = with maintainers; [nicbarker];
            # FIXME: I'm not sure what platforms this supports.
            platforms = platforms.all;
          };
        };

        defaultPackage = self.packages.${system}.default;
      }
    );
  };
}
