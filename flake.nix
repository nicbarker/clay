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
          pkgs.runCommand "format-flake" {
            nativeBuildInputs = with pkgs; [
              alejandra
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
        git_tag
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
        default = pkgs.stdenv.mkDerivation (finalAttrs {
          pname = "clay";
          version = "v0.14-master-${date}";
          src = ./.;

          dontBuild = true;

          installPhase = ''
            mkdir -p $out/include
            mkdir -p $out/lib/pkgconfig

            cp clay.h $out/include

            cat <<EOF >$out/lib/pkgconfig/clay.pc
            prefix=$out
            includedir=$out/include
            Name: Clay
            Description: Clay (short for C Layout) is a high performance 2D UI layout library.
            Version: $version
            Cflags: -I$out/include
            EOF
          '';

          passthru.tests.pkg-config = testers.testMetaPkgConfig finalAttrs.finalPackage;

          meta = with lib; {
            description = "Clay (short for C Layout) is a high performance 2D UI layout library.";
            homepage = "https://github.com/nicbarker/clay";
            license = licenses.zlib;
            maintainers = with maintainers; [nicbarker];
            pkgConfigModules = ["clay"];
            # FIXME: I'm not sure what platforms this supports.
            platforms = platforms.all;
          };
        });

        defaultPackage = self.packages.${system}.default;
      }
    );
  };
}
