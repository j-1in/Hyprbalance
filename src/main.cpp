#include "include/globals.hpp"
#include "include/DwindleBalancer.hpp"

#include <unistd.h>

#define private public
#include <hyprland/src/SharedDefs.hpp>
#include <hyprland/src/Compositor.hpp>
#include <hyprland/src/managers/LayoutManager.hpp>
#include <hyprland/src/layout/DwindleLayout.hpp>
#undef private

// Do NOT change this function.
APICALL EXPORT std::string PLUGIN_API_VERSION() {
    return HYPRLAND_API_VERSION;
}

/**
 * @brief Dispatcher for the balance command
 *
 * This function is called when the "hyprbalance:balance" command is dispatched.
 * It should divide the size of windows evenly for a specified mode argument
 *
 * @param arg Specifies what local area to apply the balance to
 *
 * @return SDispatchResult indicating success or failure of the operation.
 */
SDispatchResult onBalanceDispatcher(std::string arg) {
    if (arg == "vertical")
        g_balancer->balanceColOfWindows();
        // balanceColOfWindows();
    if (arg == "horizontal")
        g_balancer->balanceRowOfWindows();
    if (arg == "all")
        g_balancer->balanceAllWindows();
        // g_BalanceDwindleLayout->balanceAllWindows();
    else
        return SDispatchResult {.success = false, .error = "Unknown balance type" + arg};
        // throw std::invalid_argument("Invalid balance type: " + arg);
    return SDispatchResult {.success = true};
}



APICALL EXPORT PLUGIN_DESCRIPTION_INFO PLUGIN_INIT(HANDLE handle) {
    PHANDLE = handle;

    const std::string HASH = __hyprland_api_get_hash();

    if (HASH != GIT_COMMIT_HASH) {
        HyprlandAPI::addNotification(PHANDLE, "[hyprwinwrap] Failure in initialization: Version mismatch (headers ver is not equal to running hyprland ver)",
                                     CHyprColor{1.0, 0.2, 0.2, 1.0}, 5000);
        throw std::runtime_error("[hww] Version mismatch");
    }

    HyprlandAPI::addDispatcherV2(PHANDLE, "hyprbalance:balance", onBalanceDispatcher);

    // read the config for which layout to use
    if (g_pLayoutManager->m_currentLayoutID == CLayoutManager::LAYOUT_DWINDLE) {
        g_balancer = std::make_unique<CDwindleBalancer>();
    } else if (g_pLayoutManager->m_currentLayoutID == CLayoutManager::LAYOUT_MASTER) {
        // TODO
    } else {
        // invalid layout or incompatible with Hyprbalance
        HyprlandAPI::addNotification(
            PHANDLE,
            std::string("Invalid layout for Hyprbalance: ")
                + g_pLayoutManager->getCurrentLayout()->getLayoutName(),
            CHyprColor {1.0, 0.2, 0.2, 1.0},
            5000
        );
    }

    // g_BalanceDwindleLayout = std::make_unique<CBalanceDwindleLayout>();
    // HyprlandAPI::addLayout(PHANDLE, "bd", g_BalanceDwindleLayout.get());

    return {"hyprbalance", "Balance window sizes", "J-L1N", "1.0"};
}

APICALL EXPORT void PLUGIN_EXIT() {
}
