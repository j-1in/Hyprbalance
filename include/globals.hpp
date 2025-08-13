#pragma once

#include "include/Balancer.hpp"
#include <hyprland/src/plugins/PluginAPI.hpp>
#include <hyprland/src/Compositor.hpp>

inline HANDLE PHANDLE = nullptr;

inline std::unique_ptr<IBalancer> g_balancer;
