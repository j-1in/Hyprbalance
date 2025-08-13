#include "include/MasterBalancer.hpp"

#include <hyprland/src/Compositor.hpp>
#include <hyprland/src/helpers/Monitor.hpp>

#define private public
#include <hyprland/src/managers/LayoutManager.hpp>
#undef private

static bool shouldBalance(const PHLWINDOWREF& activeWindow, const PHLWINDOW& w) {
    // check if the window is in the same column as the active window
    if (w->m_workspace != activeWindow->m_workspace)
        return false;
    if (w->m_isFloating || w->m_isPseudotiled)
        return false;
    if (w->m_monitor != activeWindow->m_monitor)
        return false;
    return true;
}

void CMasterBalancer::balanceColOfWindows() {
    // get all windows in the column with the active window
    const auto             activeWindow = g_pCompositor->m_lastWindow;

    std::vector<PHLWINDOW> windowsInColumn;
    for (const auto& w : g_pCompositor->m_windows) {
        if (!shouldBalance(activeWindow, w))
            continue;

        if (w->m_position.x != activeWindow->m_position.x || w->m_size.x != activeWindow->m_size.x)
            continue;

        windowsInColumn.push_back(w);
    }

    std::cout << "Balancing column of windows: " << windowsInColumn.size() << " windows found." << std::endl;

    if (windowsInColumn.empty() || windowsInColumn.size() == 1) {
        return;
    } else {
        // const auto totalHeight = g_pCompositor->m_realMonitors[activeWindow->monitorID()]->m_si;
        const auto   totalHeight = activeWindow->m_monitor->m_size.y;
        const double newHeight   = totalHeight / windowsInColumn.size();

        // set the height of each window in the column
        for (const auto& w : windowsInColumn) {
            w->m_realSize->setValue(Vector2D(w->m_realSize->value().x, newHeight));
            w->sendWindowSize(true);
        }
    }
}

void CMasterBalancer::balanceRowOfWindows() {}

void CMasterBalancer::balanceAllWindows() {
    const auto activeWindow = g_pCompositor->m_lastWindow.lock();
    for (auto& n : mLayout->m_masterNodesData) {
        // if (n.workspaceID == activeWindow->workspaceID() && n.valid) {
        //     n.splitRatio = 1.f; // reset split ratio
        // }
    }

    mLayout->recalculateMonitor(activeWindow->monitorID());
}
