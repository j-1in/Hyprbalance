#include "include/DwindleBalancer.hpp"

#include <hyprland/src/Compositor.hpp>

#define private public
#include <hyprland/src/managers/LayoutManager.hpp>
#undef private

void CDwindleBalancer::balanceColOfWindows() {}

void CDwindleBalancer::balanceRowOfWindows() {}

void CDwindleBalancer::balanceAllWindows() {
    for (auto& n : dLayout->m_dwindleNodesData) {
        if (n.workspaceID == activeWindow->workspaceID() && n.valid) {
            n.splitRatio = 1.f; // reset split ratio
        }
    }

    dLayout->recalculateMonitor(activeWindow->monitorID());
}
