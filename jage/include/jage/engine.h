#pragma once

namespace jage
{
    inline int Sub(int a, int b) { return a - b; }
    int Add(int a, int b);

    /// @brief Print information about the current configuration and platform.
    void GetInfo();
}