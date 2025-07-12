{
  inputs.hyprland.url = "github:hyprwm/Hyprland";

  outputs = {
    self,
    hyprland,
  }: let
    inherit (hyprland.inputs) nixpkgs;
    genPerSystem = fn: nixpkgs.lib.genAttrs (builtins.attrNames hyprland.packages) (system: fn system nixpkgs.legacyPackages.${system});
  in {
    packages = genPerSystem (system: pkgs: {
      default = pkgs.callPackage ./plugin.nix {
        inherit (hyprland.packages.${system}) hyprland;
      };
    });

    devShells = genPerSystem (system: pkgs: {
      # default = pkgs.mkShell {
      default = pkgs.mkShell.override {inherit (hyprland.packages.${system}.hyprland-debug) stdenv;} {
        name = "hyprland-plugin-shell";
        shellHook = ''
          meson setup build --reconfigure
          # clangd has different flags than gcc because... why not?
          sed -e 's/c++26/c++2c/g' ./build/compile_commands.json > ./compile_commands.json
        '';

        nativeBuildInputs = with pkgs; [gcc14 meson pkg-config ninja];
        buildInputs = [hyprland.packages.${system}.hyprland];
        inputsFrom = [
          hyprland.packages.${system}.hyprland
          self.packages.${system}.default
        ];
      };
    });
  };
}
