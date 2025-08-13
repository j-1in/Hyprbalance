#pragma once

#include "Balancer.hpp"

#include <hyprland/src/Compositor.hpp>

#define private public
#include <hyprland/src/managers/LayoutManager.hpp>
#undef private

class CMasterBalancer: public IBalancer {
  public:
    CMasterBalancer() = default;
    ~CMasterBalancer() override = default;

    void balanceColOfWindows() override;
    void balanceRowOfWindows() override;
    void balanceAllWindows() override;

};

static auto* mLayout = reinterpret_cast<CHyprMasterLayout*>(g_pLayoutManager->getCurrentLayout());
