#pragma once

#include "Balancer.hpp"

#include <hyprland/src/Compositor.hpp>

#define private public
#include <hyprland/src/managers/LayoutManager.hpp>
#undef private

class CDwindleBalancer: public IBalancer {
  public:
    CDwindleBalancer() = default;
    ~CDwindleBalancer() override = default;

    void balanceColOfWindows() override;
    void balanceRowOfWindows() override;
    void balanceAllWindows() override;

  // private:
  //   PHLWINDOWREF activeWindow = g_pCompositor->m_lastWindow.lock();
  //   CHyprDwindleLayout* dLayout = reinterpret_cast<CHyprDwindleLayout*>(g_pLayoutManager->getCurrentLayout());
};

static const auto activeWindow = g_pCompositor->m_lastWindow.lock();
static auto* dLayout = reinterpret_cast<CHyprDwindleLayout*>(g_pLayoutManager->getCurrentLayout());
