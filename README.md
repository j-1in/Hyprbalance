# Hyprland Plugin Template

This is a fork of [hyprland-community/hyprland-plugin-template](https://github.com/hyprland-community/hyprland-plugin-template/fork),
with more opinionated build tools (meson and optionally nix).

The goal of this repository is to create a robust `Hyprland` plugin template, with
- nix
- A working, extensible `meson.build`
- [`hyprload`](https://github.com/Duckonaut/hyprload) support out of the box
- Environment set up guide
- Clangd flags set up for autocomplete and error checking

It is highly recommended to read the [Plugin development](https://wiki.hyprland.org/Plugins/Development/Getting-Started/)
section of the Hyprland Wiki first. Some stuff will be different in this template, but it gives
you a general idea about what's going on

## Support
If you have any issues setting this up, open an issue in this repository. I will try to help.

## Setup
This is a github template repository. To use it, use the green **Use this template** button
at the top of the repository file view.

### Setting up a development environment

**With nix**:

Run `nix develop` before opening your editor, or use [direnv](https://github.com/direnv/direnv).

> `nix develop` will:
> 1. Create a shell with all dependencies installed
> 2. Run `meson setup build`
> 3. Create a **clangd** compatible `compile_commands.json`

**Without nix**:

Run this command to setup the build directory and generate a **clangd** compatible
`compile_commands.json`

```bash
meson setup build && sed -e 's/c++23/c++2b/g' ./build/compile_commands.json > ./compile_commands.json
```

#### Making it Your Own
To change your plugin name, version, and author (that's you!) there are a few things that need
changing (I would like to streamline it somehow, but it's manageable for now)
- `main.cpp`: The `PLUGIN_INIT` function returns a struct with the plugin name, description,
  author and version. Change those.
- `meson.build`: At the top of the file, the project name `example` should be changed.
- `src/meson.build`: The name of the `shared_library` (first argument) should be changed. The
  resulting library will be changed to `build/src/libYOUR_PLUGIN.so` so you should change the
`[examplePlugin.build.output]` entry as well
- `hyprload.toml`: The `[examplePlugin]` and `[examplePlugin.build]` should be changed to match
  the name of your plugin. `hyprload` will look at these dictionaries for info about the plugin.
  For more info, see [hyprload docs](https://github.com/Duckonaut/hyprload#format)
- `plugin.nix`: Change `pname` and the `meta` section.

## Building and testing

After `nix develop`, the steps to build are simple

### Manually

This is how you'll build the plugin during development

- `meson setup build`: This will create the `build` directory. You don't always need to run this
after the first time, but sometimes things get borked (don't know why) and you need to `rm -rf
build` and re-setup
- `meson compile -Cbuild`: This will build the `PLUGIN_NAME.so` file.
- `hyprctl plugin unload $PWD/build/src/PLUGIN_NAME.so`: If you have an old version loaded, unload it
- `hyprctl plugin load $PWD/build/src/PLUGIN_NAME.so`: Load the plugin

Do note that if you only load/unload from the same path, Hyprland can ignore your changes.

### Via nix

You don't need this that often for development but you can build directly without `nix develop`
with `nix build`.

### Using `hyprload`

TODO: update

This works rather well in nested Hyprland sessions, since `hyprload` keeps sessions separate.
- `make install`: This will build and copy the plugin to the `hyprload` plugin directory.
- Reload `hyprload` for the changes to take effect

This doesn't have the issue of ignoring changes, because of how `hyprload` handles its loaded
plugins.

### Nested Hyprland
Developing a plugin may be tough. You might crash Hyprland a couple times. For this reason,
it's a good idea to develop them in a nested Hyprland session. If you run `Hyprland` from an
existing Hyprland session, it'll open in a window. If this window crashes, it's pretty much fine!
Refer to the [Hyprland wiki](http://wiki.hyprland.org/Plugins/Development/Getting-Started/#setting-up-a-development-environment)
for more info.

## ""Publishing""
If you haven't messed up your `hyprload.toml` manifest too badly, anyone should be able to use 
your plugin by just adding `'YOUR_GITHUB_NAME/YOUR_PLUGIN'` to their own `hyprload.toml` config!
