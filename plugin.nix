{
  lib,
  gcc13Stdenv,
  meson,
  ninja,
  pkg-config,
  hyprland,
}: let
  version = builtins.readFile ./VERSION;
in
  gcc13Stdenv.mkDerivation {
    pname = "hyprbalance";
    inherit version;
    src = ./.;

    nativeBuildInputs =
      hyprland.nativeBuildInputs
      ++ [ninja meson pkg-config];

    buildInputs = [hyprland] ++ hyprland.buildInputs;

    meta = with lib; {
      # homepage = "https://example.com";
      description = "Balance window sizes";
      license = licenses.bsd3;
      platforms = platforms.linux;
    };
  }
