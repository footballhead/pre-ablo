#include <decomp/itemdat.h>
#include <decomp/spells.h>

#include <array>
#include <cstdio>
#include <diabutil/file.hpp>

namespace {

// Virtual address of the .data segment if DIABLO.EXE was loading into memory
constexpr size_t data_virtual_base = 0x004A2000;
// Offset in DIABLO.EXE for contents of .data
constexpr size_t data_offset = 0xA0800;

//
// MISC UTILS
//

// Pointers assume the .data segment is loaded into memory. Translate to the
// actual location in DIABLO.EXE.
//
// This adds quotes around the string if it's valid, or just NULL otherwise.
std::string find_string_maybe_null(diabutil::byte_vector const& data,
                                   const char* addr) {
  if (addr == nullptr) {
    return "NULL";
  }
  std::string builder;
  builder += '"';
  builder += reinterpret_cast<char const* const>(
      data.data() +
      (reinterpret_cast<size_t>(addr) - data_virtual_base + data_offset));
  builder += '"';
  return builder;
}

char const* bool_to_str(BOOL b) { return b ? "TRUE" : "FALSE"; }

std::vector<size_t> calculate_max_column_lengths(
    std::vector<std::vector<std::string>> const& rows) {
  std::vector<size_t> column_max(rows.size(), 0);

  for (auto const& row : rows) {
    for (size_t i = 0; i < row.size(); ++i) {
      auto const& column = row.at(i);
      column_max[i] = max(column_max.at(i), column.size());
    }
  }

  return column_max;
}

std::string right_pad_string(std::string const& str, size_t length) {
  if (length <= str.size()) {
    return str;
  }

  std::string builder = str;

  auto const pad_size = length - str.size();
  builder.insert(builder.end(), pad_size, ' ');

  return builder;
}

void pretty_print_table(std::vector<std::vector<std::string>> rows,
                        char const* struct_name, char const* var) {
  std::vector<size_t> column_max = calculate_max_column_lengths(rows);

  printf("%s %s[] = {\n", struct_name, var);
  for (const auto& row : rows) {
    printf("{");
    for (size_t i = 0; i < row.size(); ++i) {
      auto const& column = row.at(i);
      // +2 for ", "
      auto const& column_desired_length = column_max.at(i) + 2;

      printf("%s, ", right_pad_string(column, column_desired_length).c_str());
    }
    printf("},\n");
  }
  printf("};\n");
}

//
// ENUM FORMATTING FUNCTIONS
//

char const* to_str(goodorevil goe) {
  if (goe == GOE_ANY) return "GOE_ANY";
  if (goe == GOE_EVIL) return "GOE_EVIL";
  if (goe == GOE_GOOD) return "GOE_GOOD";
  return "???";
}

char const* to_str(item_effect_type ipl) {
  static const std::array string_lookup = {
      "IPL_INVALID",  // -1
      "IPL_TOHIT",    // 0
      "IPL_TOHIT_CURSE",
      "IPL_DAMP",
      "IPL_DAMP_CURSE",
      "IPL_TOHIT_DAMP",
      "IPL_TOHIT_DAMP_CURSE",
      "IPL_ACP",
      "IPL_ACP_CURSE",
      "IPL_FIRERES",
      "IPL_LIGHTRES",
      "IPL_MAGICRES",  // 10
      "IPL_ALLRES",
      "IPL_SPLCOST",
      "IPL_SPLDUR",
      "IPL_SPLLVLADD",
      "IPL_CHARGES",
      "IPL_FIREDAM",
      "IPL_LIGHTDAM",
      "IPL_RANDSPL",
      "IPL_STR",
      "IPL_STR_CURSE",  // 20
      "IPL_MAG",
      "IPL_MAG_CURSE",
      "IPL_DEX",
      "IPL_DEX_CURSE",
      "IPL_VIT",
      "IPL_VIT_CURSE",
      "IPL_ATTRIBS",
      "IPL_ATTRIBS_CURSE",
      "IPL_GETHIT_CURSE",
      "IPL_GETHIT",  // 30
      "IPL_LIFE",
      "IPL_LIFE_CURSE",
      "IPL_MANA",
      "IPL_MANA_CURSE",
      "IPL_DUR",
      "IPL_DUR_CURSE",
      "IPL_INDESTRUCTIBLE",
      "IPL_LIGHT",
      "IPL_LIGHT_CURSE",
      "IPL_INVIS",  // 40
      "IPL_MULT_ARROWS",
      "IPL_FIRE_ARROWS",
      "IPL_LIGHT_ARROWS",
      "IPL_HOMING_ARROWS",
      "IPL_THORNS",
      "IPL_NOMANA",
      "IPL_NOHEALPLR",
      "IPL_SCARE",
      "IPL_DOUBLE_STRIKE",
      "IPL_EXP_DAM",  // 50
      "IPL_SEE_INVIS",
      "IPL_ABSHALFTRAP",
      "IPL_KNOCKBACK",
      "IPL_NOHEALMON",
      "IPL_DAMMOD",
      "IPL_RNDARROWVEL",
      "IPL_SETDAM",
      "IPL_SETDUR",
      "IPL_GLOOMFANG",
      "IPL_GONNAGAL",  // 60
      "IPL_FALCON",
      "IPL_GRANDFATHER",
      "IPL_SKULLCHAIN",
      "IPL_DUFUS",
      "IPL_GLEAMSONG",
      "IPL_DRAINLIFE",
      "IPL_X",
  };
  return string_lookup[ipl + 1];
}

char const* to_str(item_class ic) {
  static std::array string_lookup = {
      "ICLASS_NONE", "ICLASS_WEAPON", "ICLASS_ARMOR",
      "ICLASS_MISC", "ICLASS_GOLD",   "ICLASS_QUEST",
  };
  return string_lookup[ic];
}

char const* to_str(item_equip_type iet) {
  static std::array string_lookup = {
      "ILOC_INVALID",  // -1
      "ILOC_NONE",     // 0
      "ILOC_ONEHAND",  // ...  etc
      "ILOC_TWOHAND", "ILOC_ARMOR",       "ILOC_HELM",
      "ILOC_RING",    "ILOC_UNEQUIPABLE",
  };
  return string_lookup[iet + 1];
}

char const* to_str(item_special_effect ise) {
  static std::array string_lookup = {
      "ISPL_NONE",
      "ISPL_INFRAVISION",
      "ISPL_RNDSTEALLIFE",
  };
  return string_lookup[ise];
}

char const* to_str(item_misc_id imi) {
  std::array string_lookup = {
      "IMISC_NONE",
      "IMISC_HEAL",
      "???",
      "???",
      "IMISC_MANA",
      "IMISC_FULLMANA",
      "IMISC_POTEXP",
      "???",
      "IMISC_ELIXSTR",
      "IMISC_ELIXMAG",
      "IMISC_ELIXDEX",
      "IMISC_ELIXVIT",
      "IMISC_ELIXWEAK",
      "IMISC_ELIXDIS",
      "IMISC_ELIXCLUM",
      "IMISC_ELIXSICK",
      "IMISC_FULLHEAL",
      "IMISC_SCROLL",
      "IMISC_BOOK",
      "IMISC_STAFF",
      "IMISC_RING",
      "IMISC_UNIQUE",
      "IMISC_FOOD",
  };
  return string_lookup[imi];
}

char const* to_str(spell_id si) {
  std::array string_lookup = {
      "SPL_NULL",     "SPL_FIREBOLT",    "SPL_HEAL",        "SPL_LIGHTNING",
      "SPL_FLASH",    "SPL_IDENTIFY",    "SPL_FIREWALL",    "SPL_TOWN",
      "SPL_STONE",    "SPL_INFRA",       "SPL_RNDTELEPORT", "SPL_MANASHIELD",
      "SPL_FIREBALL", "SPL_GUARDIAN",    "SPL_CHAIN",       "SPL_WAVE",
      "SPL_DOOMSERP", "SPL_BLODRIT",     "SPL_NOVA",        "SPL_INVISIBIL",
      "SPL_SENTINEL", "SPL_GOLEM",       "SPL_BLODBOIL",    "SPL_TELEPORT",
      "SPL_APOCA",    "SPL_ETHEREALIZE", "SPL_REPAIR",      "SPL_RECHARGE",
      "SPL_DISARM",
  };
  return string_lookup[si];
}

char const* to_str(item_type it) {
  static std::array string_lookup = {
      "ITYPE_NONE",   // -1
      "ITYPE_MISC",   // 0
      "ITYPE_SWORD",  // etc
      "ITYPE_AXE",    "ITYPE_BOW",  "ITYPE_MACE",   "ITYPE_SHIELD",
      "ITYPE_LARMOR", "ITYPE_HELM", "ITYPE_MARMOR", "ITYPE_HARMOR",
      "ITYPE_STAFF",  "ITYPE_GOLD", "ITYPE_RING",   "ITYPE_FOOD",
  };
  return string_lookup[it + 1];
}

char const* to_str(unique_base_item ubi) {
  static std::array string_lookup = {
      "UITYPE_INVALID",   // -1
      "UITYPE_NONE",      // 0
      "UITYPE_SHORTBOW",  // 1
      "UITYPE_LONGBOW",   // etc
      "UITYPE_BOW",         "UITYPE_COMPBOW",     "UITYPE_WARBOW",
      "UITYPE_BATTLEBOW",   "UITYPE_DAGGER",      "UITYPE_FALCHION",
      "UITYPE_CLAYMORE",    "UITYPE_BROADSWR",    "UITYPE_SABRE",
      "UITYPE_SCIMITAR",    "UITYPE_LONGSWR",     "UITYPE_BASTARDSWR",
      "UITYPE_TWOHANDSWR",  "UITYPE_GREATSWR",    "UITYPE_CLEAVER",
      "UITYPE_LARGEAXE",    "UITYPE_BROADAXE",    "UITYPE_SMALLAXE",
      "UITYPE_BATTLEAXE",   "UITYPE_GREATAXE",    "UITYPE_MACE",
      "UITYPE_MORNSTAR",    "UITYPE_CLUB",        "UITYPE_MAUL",
      "UITYPE_WARHAMMER",   "UITYPE_FLAIL",       "UITYPE_LONGSTAFF",
      "UITYPE_SHORTSTAFF",  "UITYPE_COMPSTAFF",   "UITYPE_QUARSTAFF",
      "UITYPE_WARSTAFF",    "UITYPE_SKULLCAP",    "UITYPE_HELM",
      "UITYPE_GREATHELM",   "UITYPE_CROWN",       "UITYPE_STUDARMOR",
      "UITYPE_CLOAK",       "UITYPE_ROBE",        "UITYPE_CHAINMAIL",
      "UITYPE_LEATHARMOR",  "UITYPE_BREASTPLATE", "UITYPE_CAPE",
      "UITYPE_PLATEMAIL",   "UITYPE_FULLPLATE",   "UITYPE_BUCKLER",
      "UITYPE_SMALLSHIELD", "UITYPE_LARGESHIELD", "UITYPE_KITESHIELD",
      "UITYPE_TOWERSHIELD", "UITYPE_RING",
  };
  return string_lookup[ubi + 1];
}

std::string plt_to_str(int plt) {
  auto append_flag_with_or = [](std::string& str, std::string const& flag) {
    if (!str.empty()) {
      str += " | ";
    }
    str += flag;
  };

  std::string builder;
  if (plt & PLT_MISC) append_flag_with_or(builder, "PLT_MISC");
  if (plt & PLT_BOW) append_flag_with_or(builder, "PLT_BOW");
  if (plt & PLT_STAFF) append_flag_with_or(builder, "PLT_STAFF");
  if (plt & PLT_WEAP) append_flag_with_or(builder, "PLT_WEAP");
  if (plt & PLT_SHLD) append_flag_with_or(builder, "PLT_SHLD");
  if (plt & PLT_ARMO) append_flag_with_or(builder, "PLT_ARMO");

  if (builder.empty()) {
    return "0";
  }

  return builder;
}

//
// STRUCT FORMATTING FUNCTIONS
//

std::vector<std::string> to_row(ItemDataStruct const& item,
                                diabutil::byte_vector const& diablo) {
  std::vector<std::string> builder;
  builder.push_back(bool_to_str(item.iRnd));
  builder.push_back(to_str(static_cast<item_class>(item.iClass)));
  builder.push_back(to_str(static_cast<item_equip_type>(item.iLoc)));
  builder.push_back(std::to_string(item.iCurs));
  builder.push_back(to_str(static_cast<item_type>(item.iType)));
  builder.push_back(to_str(static_cast<unique_base_item>(item.iItemId)));
  builder.push_back(find_string_maybe_null(diablo, item.iName));
  builder.push_back(find_string_maybe_null(diablo, item.iSName));
  builder.push_back(std::to_string(item.iMinMLvl));
  builder.push_back(std::to_string(item.iDurability));
  builder.push_back(std::to_string(item.iMinDam));
  builder.push_back(std::to_string(item.iMaxDam));
  builder.push_back(std::to_string(item.iMinAC));
  builder.push_back(std::to_string(item.iMaxAC));
  builder.push_back(std::to_string(item.iMinStr));
  builder.push_back(std::to_string(item.iMinMag));
  builder.push_back(std::to_string(item.iMinDex));
  builder.push_back(to_str(static_cast<item_special_effect>(item.iFlags)));
  builder.push_back(to_str(static_cast<item_misc_id>(item.iMiscId)));
  builder.push_back(to_str(static_cast<spell_id>(item.iSpell)));
  builder.push_back(bool_to_str(item.iUsable));
  builder.push_back(std::to_string(item.iValue));
  return builder;
}

std::vector<std::string> to_row(PLStruct const& affix,
                                diabutil::byte_vector const& diablo) {
  std::vector<std::string> builder;
  builder.push_back(bool_to_str(affix.isWorking));
  builder.push_back(find_string_maybe_null(diablo, affix.PLName));
  builder.push_back(to_str(static_cast<item_effect_type>(affix.PLPower)));
  builder.push_back(std::to_string(affix.PLParam1));
  builder.push_back(std::to_string(affix.PLParam2));
  builder.push_back(std::to_string(affix.PLMinLvl));
  builder.push_back(plt_to_str(affix.PLIType).c_str());
  builder.push_back(to_str(static_cast<goodorevil>(affix.PLGOE)));
  builder.push_back(bool_to_str(affix.PLDouble));
  builder.push_back(bool_to_str(affix.PLOk));
  builder.push_back(std::to_string(affix.PLMinVal));
  builder.push_back(std::to_string(affix.PLMaxVal));
  builder.push_back(std::to_string(affix.PLMultVal));
  return builder;
}

std::vector<std::string> to_row(UItemStruct const& item,
                                diabutil::byte_vector const& diablo) {
  std::vector<std::string> builder;
  builder.push_back(find_string_maybe_null(diablo, item.UIName));
  builder.push_back(to_str(static_cast<unique_base_item>(item.UIItemId)));
  builder.push_back(std::to_string(item.UIMinLvl));
  builder.push_back(std::to_string(item.UINumPL));
  builder.push_back(std::to_string(item.UIValue));
  builder.push_back(bool_to_str(item.field_C));
  builder.push_back(to_str(static_cast<item_effect_type>(item.UIPower1)));
  builder.push_back(std::to_string(item.UIParam1));
  builder.push_back(std::to_string(item.UIParam2));
  builder.push_back(to_str(static_cast<item_effect_type>(item.UIPower2)));
  builder.push_back(std::to_string(item.UIParam3));
  builder.push_back(std::to_string(item.UIParam4));
  builder.push_back(to_str(static_cast<item_effect_type>(item.UIPower3)));
  builder.push_back(std::to_string(item.UIParam5));
  builder.push_back(std::to_string(item.UIParam6));
  builder.push_back(to_str(static_cast<item_effect_type>(item.UIPower4)));
  builder.push_back(std::to_string(item.UIParam7));
  builder.push_back(std::to_string(item.UIParam8));
  builder.push_back(to_str(static_cast<item_effect_type>(item.UIPower5)));
  builder.push_back(std::to_string(item.UIParam9));
  builder.push_back(std::to_string(item.UIParam10));
  builder.push_back(to_str(static_cast<item_effect_type>(item.UIPower6)));
  builder.push_back(std::to_string(item.UIParam11));
  builder.push_back(std::to_string(item.UIParam12));
  return builder;
}

//
// READING FUNCTIONS
//

template <typename Struct>
std::vector<std::vector<std::string>> read_table(
    diabutil::byte_vector const& diablo, size_t offset, size_t size) {
  std::vector<std::vector<std::string>> builder;

  for (size_t i = 0; i < size; ++i) {
    auto const item =
        reinterpret_cast<Struct const* const>(diablo.data() + offset) + i;
    builder.push_back(to_row(*item, diablo));
  }

  return builder;
}

}  // namespace

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s diablo.exe\n", argv[0]);
    return 1;
  }

  auto const diablo_exe = argv[1];
  auto const file = diabutil::read_file(diablo_exe);
  if (!file) {
    fprintf(stderr, "Could not read file\n");
    return 2;
  }

  {
    pretty_print_table(read_table<ItemDataStruct>(*file, 0xA2300, 118),
                       "ItemDataStruct", "AllItemsList");
  }
  {
    pretty_print_table(read_table<PLStruct>(*file, 0xA4430, 75), "PLStruct",
                       "PL_Prefix");
  }
  {
    pretty_print_table(read_table<PLStruct>(*file, 0xA5370, 94), "PLStruct",
                       "PL_Suffix");
  }
  {
    pretty_print_table(read_table<UItemStruct>(*file, 0xA6688, 60),
                       "UItemStruct", "UniqueItemList");
  }

  return 0;
}