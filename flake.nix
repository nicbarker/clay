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

    date = let
      # YYYYMMDD
      date' = lib.substring 0 8 self.lastModifiedDate;
      year = lib.substring 0 4 date';
      month = lib.substring 4 2 date';
      day = lib.substring 6 2 date';
    in
      if (self ? lastModifiedDate)
      then lib.concatStringsSep "-" [year month day]
      else "unknown";

    mkClayPackage = {
      system,
      version,
      src,
      suffix,
    }:
      nixpkgsFor.${system}.stdenv.mkDerivation {
        pname = "clay";
        version = "${version}${suffix}";
        inherit src;

        dontBuild = true;

        postInstall = ''
          mkdir -p $out/include
          mkdir -p $out/lib/pkgconfig

          cp clay.h $out/include

          cat <<EOF >$out/lib/pkgconfig/clay.pc
          prefix=$out
          includedir=$out/include
          Name: Clay
          Description: Clay (short for C Layout) is a high performance 2D UI layout library.
          Version: ${version}
          Cflags: -I$out/include
          EOF
        '';

        passthru.tests.pkg-config = nixpkgs.lib.testing.testMetaPkgConfig self.packages.${system}.${version};

        meta = with lib; {
          description = "Clay (short for C Layout) is a high performance 2D UI layout library.";
          homepage = "https://github.com/nicbarker/clay";
          license = licenses.zlib;
          maintainers = with maintainers; [nicbarker];
          pkgConfigModules = ["clay"];
          platforms = platforms.all;
        };
      };
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
            cd ${self}

            echo "Checking nix formatting"
            alejandra --check .

            echo "Checking dead code"
            deadnix --fail .

            echo "Checking statix"
            statix check .

            touch $out
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
            cmake # this is needed for building examples and tests
            clang-tools # for clangd and clang-tidy (optional)
            pkg-config
            alejandra # for formatting nix

            lld # for wasm
          ];

          buildInputs = with pkgs; [
            SDL2
            SDL2_image
            SDL2_ttf

            sdl3
            sdl3-ttf
            sdl3-image

            cairo
            raylib
            sokol

            # dependencies for examples and tests
            xorg.libXrandr
            xorg.libXinerama
            xorg.libXcursor
            xorg.libXi
          ];
        };
      }
    );

    formatter = forAllSystems (system: nixpkgsFor.${system}.alejandra);

    packages = forAllSystems (system: let
      pkgs = nixpkgsFor.${system};
      inherit (pkgs) fetchFromGitHub;
    in {
      stable = mkClayPackage {
        inherit system;
        version = "0.14";
        # To update this use nurl https://github.com/nicbarker/clay {tag}
        src = fetchFromGitHub {
          owner = "nicbarker";
          repo = "clay";
          rev = "v0.14"; # This will need to be updated when a new version is released
          hash = "sha256-6h1aQXqwzPc4oPuid3RfV7W0WzQFUiddjW7OtkKM0P8="; # This too likely
        };
        suffix = "";
      };

      unstable = mkClayPackage {
        inherit system;
        version = "0.14";
        src = ./.;
        suffix = "-master-${date}";
      };

      # I have no idea how to expose both unstable and stable at the same time
      # so I'm going to just expose the unstable version for now
      default = self.packages.${system}.unstable;
    });
  };
}
