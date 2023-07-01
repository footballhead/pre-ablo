"""Setup:

1. Copy prdemo plrgfx into a folder
2. Copy assets/plrgfx into that folder and override all files

Then run this script.

Outputs any files that violate game constraints, such as memory or frames.
"""

from pathlib import Path
from dataclasses import dataclass
from cel_utils import decompose_with_groups


# plrgfx have 8 groups: one for each direction (S, SW, W, NW, N, NE, E, SE)
_GROUPS = 8

#
# File generation
#

_ARMOUR_CHAR = [
    'L', # TODO: Extract all warrior light animations
    'M',
    'H',
]
_WEP_CHAR = [
    'N', # No weapon
    'U', # Unarmed (only shield)
    'S', # Sword (no shield)
    'D', # sworD with shielD
    'B', # Bow
    'A', # Axe
    'M', # Mace (no shield)
    'H', # mace with shield
    'T'  # sTaff
]
_CLASS_STR_TBL = [
    'WARRIOR',
    'ROGUE',
]
_ANIM_TYPE_STR = [
    'AS', # Active stand (dungeon)
    'AW', # Active walk (dungeon)
    'AT', # Attack
    'HT', # Hit
    'LM', # Lightning Magic
    'FM', # Fire Magic
    'QM', # Q? Magic
    'DT', # Death
    'BL', # Block
    'ST', # Stand (town)
    'WL', # Walk (town)
]

#
# Constraints
#

_ALLOC_ALIGNMENT=1024
# These are taken from InitPlrGFXMem decomp (.text:00465DC2)
_N_DATA_MAX_SIZE = 0x45800
_W_DATA_MAX_SIZE = 0x1D000
_A_DATA_MAX_SIZE = 0x57000
_H_DATA_MAX_SIZE = 0x16C00
_L_DATA_MAX_SIZE = 0x5A000
_F_DATA_MAX_SIZE = 0x78800
_T_DATA_MAX_SIZE = 0x91800
_D_DATA_MAX_SIZE = 0x41C00
_B_DATA_MAX_SIZE = 0xB400
_ANIM_TO_SIZE = {
    'ST': _N_DATA_MAX_SIZE, # town
    'AS': _N_DATA_MAX_SIZE, # dungeon
    'WL': _W_DATA_MAX_SIZE, # town
    'AW': _W_DATA_MAX_SIZE, # dungeon
    'AT': _A_DATA_MAX_SIZE,
    'HT': _H_DATA_MAX_SIZE,
    'LM': _L_DATA_MAX_SIZE,
    'FM': _F_DATA_MAX_SIZE,
    'QM': _T_DATA_MAX_SIZE,
    'DT': _D_DATA_MAX_SIZE,
    'BL': _B_DATA_MAX_SIZE,
}
_HAS_BLOCK = {
    'N': False,
    'U': True,
    'S': False,
    'D': True,
    'B': False,
    'A': False,
    'M': False,
    'H': True,
    'T': False,
}
_DEFAULT_ANIM_LENS = {
    'WARRIOR': {
        'AS': 10, # Stand (N)
        'AT': 16, # Attack (A)
        'AW': 8,  # Walk (W)
        'BL': 2,  # Block (B)
        'DT': 15, # Death (D)
        'FM': 20, # Spell (fire/lightning/magic) (S)
        'LM': 20, # Spell (fire/lightning/magic) (S)
        'QM': 20, # Spell (fire/lightning/magic) (S)
        'HT': 6,  # Got hit (H)
        'ST': 20, # Stand in town (N)
        'WL': 8,  # Walk in town (W)
    },
    # Rogue
    'ROGUE': {
        'AS': 8,  # N
        'AT': 18, # A
        'AW': 6,  # W
        'BL': 4,  # B
        'DT': 20, # D
        'FM': 16, # S
        'LM': 16, # S
        'QM': 16, # S
        'HT': 7,  # H
        'ST': 20, # town N
        'WL': 8,  # town W
    },
}
# TODO: Account for our patch that fixes death anims
_OVERRIDE_ANIM_LENS = {
    # 'WLDDT': 21, # Fixed in plrgfx_frame_fix
    # 'WLNDT': 20, # Fixed in plrgfx_frame_fix
    # 'WMNDT': 15, # Fixed in plrgfx_frame_fix
    # 'WHNDT': 20, # Fixed in plrgfx_frame_fix
    # 'WLUDT': 20, # Fixed in plrgfx_frame_fix
    # 'WMUDT': 20, # Fixed in plrgfx_frame_fix
    # 'WHUDT': 15, # Fixed in plrgfx_frame_fix
    # 'WLSDT': 21, # Fixed in plrgfx_frame_fix
    # 'WMSDT': 21, # Fixed in plrgfx_frame_fix
    # 'WHSDT': 15, # Fixed in plrgfx_frame_fix
    'WLBAS': 8,
    'WMBAS': 8,
    'WHBAS': 8,
    'WLAAT': 20,
    'WMAAT': 20,
    'WHAAT': 20,
    # 'WLMDT': 16, # Fixed in plrgfx_frame_fix
    # 'WMMDT': 16, # Fixed in plrgfx_frame_fix
    # 'WHMDT': 16, # Fixed in plrgfx_frame_fix
    # 'WLHDT': 16, # Fixed in plrgfx_frame_fix
    # 'WMHDT': 16, # Fixed in plrgfx_frame_fix
    'WLTAT': 16,
    'WMTAT': 16,
    'WHTAT': 16,
    # 'WLTDT': 20, # Fixed in plrgfx_frame_fix
    # 'WMTDT': 20, # Fixed in plrgfx_frame_fix
    # 'WHTDT': 20, # Fixed in plrgfx_frame_fix
    'RLAAT': 22,
    'RLBAT': 12,
    # 'RLBDT': 21, # Fixed in plrgfx_frame_fix
    'RLTAT': 16,
    # THESE ARE FROM plrgfx_frame_fix.cpp
    'WLNDT': 20,
    'WLUDT': 20,
    'WLSDT': 21,
    'WLDDT': 21,
    'WLBDT': 21,
    'WLMDT': 16,
    'WLHDT': 16,
    'WLTDT': 20,
}

@dataclass
class Constraint:
    max_file_size: int
    expected_anim_len: int


def build_animation_paths_and_constraints() -> dict[Path, Constraint]:
    constriants = {}

    for pc in _CLASS_STR_TBL:
        pc_char = pc[0]
        for armor_char in _ARMOUR_CHAR:
            # We only use rogue light armor animations
            if pc == 'ROGUE' and not armor_char == 'L':
                continue

            for weapon_char in _WEP_CHAR:
                for anim in _ANIM_TYPE_STR:
                    # Not every weapon has a block animation
                    if anim == 'BL' and not _HAS_BLOCK[weapon_char]:
                        continue

                    # Build path
                    prefix = f'{pc_char}{armor_char}{weapon_char}'
                    base = f'{prefix}{anim}'
                    path = Path(pc) / prefix / f'{base}.CEL'

                    # Build constraint
                    max_file_size = _ANIM_TO_SIZE[anim]

                    anim_len = _DEFAULT_ANIM_LENS[pc][anim]
                    if base in _OVERRIDE_ANIM_LENS:
                        anim_len = _OVERRIDE_ANIM_LENS[base]

                    constraint = Constraint(
                        max_file_size=max_file_size,
                        expected_anim_len=anim_len)

                    # Associate path and constraint
                    constriants[path] = constraint

    return constriants


def count_animations(cel_with_groups) -> int:
    count = 0
    for group in cel_with_groups:
        count += len(group)
    return count


def main():
    constraints = build_animation_paths_and_constraints()
    for path in constraints:
        if not path.exists():
            print(f'Skipping {path}')
            continue

        constraint = constraints[path]

        # Load CEL in order to count frames
        # TODO: This could be sped up by only looking at frame table, not all
        # cel data
        cel_data = path.read_bytes()
        cel = decompose_with_groups(cel_data, _GROUPS)
        for group_idx in range(0, _GROUPS):
            group = cel[group_idx]
            anim_len = len(group)
            expected_len = constraint.expected_anim_len
            if not anim_len == expected_len:
                print(f'{path}: group={group_idx}: {anim_len} != {expected_len}')

        size = path.stat().st_size
        max_size = constraint.max_file_size
        if size > max_size:
            # Allocs are 1024-aligned for some reason so keep it that way?

            suggested_alloc = (int(size / _ALLOC_ALIGNMENT) + 1) * _ALLOC_ALIGNMENT
            print(f'{path}: {size} bytes > {max_size} bytes! Suggested alloc={hex(suggested_alloc)}')


if __name__ == '__main__':
    main()
