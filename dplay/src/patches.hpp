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

/// Aggregate grouping all data related to a patch. To avoid possible static initialization fiasco, this must remain a POD type.
/// TODO: static_assert on POD-ness of `struct Patch`
struct Patch {
    /// Textual name to show on options menu. Must be unique
    const char* name;
    /// Textual description to show on options menu
    const char* description;
    /// The function, run in `DllMain()`, to register the patch
    bool (*main)();
    /// true if the user wants this patch applied, false otherwise
    bool checked;
    /// Optional MPQ to load related to this patch. Ignored if nullptr.
    /// Patch loading is done by `load_mpq_customizations`
    const char* mpq_filename;
};

std::vector<Patch>& get_patches();
bool save_checked_patches();
