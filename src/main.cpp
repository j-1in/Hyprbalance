#include "include/globals.hpp"
#include "src/SharedDefs.hpp"

#include <unistd.h>

// Do NOT change this function.
APICALL EXPORT std::string PLUGIN_API_VERSION() {
    return HYPRLAND_API_VERSION;
}

/**
 * It should divide the width of the windows in the current active column evenly.
 */
static void balanceColOfWindows() {
}

/**
 * It should divide the width of the windows in the current active row evenly.
 */
static void balanceRowOfWindows() {
}

/**
 * Divide the size of all windows evenly across the screen (workspace).
 */
static void balanceAllWindows() {
    // call balance vertical column on all columns

    // call balance horizontal row on all rows
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
        balanceColOfWindows();
    if (arg == "horizontal")
        balanceRowOfWindows();
    if (arg == "all")
        balanceAllWindows();
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


    return {"hyprbalance", "Balance window sizes", "J-L1N", "1.0"};
}

APICALL EXPORT void PLUGIN_EXIT() {

}
