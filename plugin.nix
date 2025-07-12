{
  lib,
  gcc13Stdenv,
  meson,
  ninja,
  pkg-config,
  hyprland-debug,
}: let
  version = builtins.readFile ./VERSION;
in
  gcc13Stdenv.mkDerivation {
    pname = "example";
    inherit version;
    src = ./.;

    nativeBuildInputs =
      hyprland-debug.nativeBuildInputs
      ++ [ninja meson pkg-config];

    buildInputs = [hyprland-debug] ++ hyprland.buildInputs;

    meta = with lib; {
      homepage = "https://example.com";
      description = "Example hyprland plugin";
      license = licenses.bsd3;
      platforms = platforms.linux;
    };
  }
