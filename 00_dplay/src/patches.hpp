#pragma once

#include <vector>

struct Patch {
    const char* name;
    bool (*main)();
    bool checked;
};

std::vector<Patch>& get_patches();
bool save_checked_patches();
