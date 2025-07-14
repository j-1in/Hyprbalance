#pragma once

class IBalancer {
  public:
    virtual ~IBalancer() = default;

    /**
     * @brief Balance the windows in the current active column.
     *
     * This function should divide the width of the windows in the current active column evenly.
     */
    virtual void balanceColOfWindows() = 0;

    /**
     * @brief Balance the windows in the current active row.
     *
     * This function should divide the height of the windows in the current active row evenly.
     */
    virtual void balanceRowOfWindows() = 0;

    /**
     * @brief Balance all windows across all columns and rows.
     *
     * This function should ensure that all windows are balanced across all columns and rows.
     */
    virtual void balanceAllWindows() = 0;
  private:
};
