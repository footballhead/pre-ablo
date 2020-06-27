#pragma once

#include <vector>

// requires PATCH_NAME to be defined
#define DESCRIBE_PATCH__(name, desc) char const* name ## _description = desc;
#define DESCRIBE_PATCH_(name, desc) DESCRIBE_PATCH__(name, desc)
#define DESCRIBE_PATCH(desc) DESCRIBE_PATCH_(PATCH_NAME, desc)

// requires PATCH_NAME to be defined
#define PATCH_MAIN__(name) bool name##_main()
#define PATCH_MAIN_(name) PATCH_MAIN__(name)
#define PATCH_MAIN PATCH_MAIN_(PATCH_NAME)

struct Patch {
    const char* name;
    const char* description;
    bool (*main)();
    bool checked;
};

std::vector<Patch>& get_patches();
bool save_checked_patches();
