// This file checks that all structs are the desired size. This will not compile
// if sizes don't match. This won't be linked.
//
// This produces compiler output without redundant asserts
#include <windows.h>

#include "dead.h"
#include "itemdat.h"
#include "items.h"
#include "missiles.h"
#include "monster.h"
#include "objdat.h"
#include "objects.h"
#include "player.h"
#include "quests.h"

// Scope to contain subscopes. Not actually called.
void typecheck()
{
    // Each assert is scoped to bypass ODR complaints
    {
        // We need a 32-bit compiler
        _STATIC_ASSERT(sizeof(void *) == 4);
    }

    {
        _STATIC_ASSERT(sizeof(CMonster) == 0x2B8);
    }
    {
        _STATIC_ASSERT(sizeof(DeadStruct) == 0x2D);
    }
    {
        _STATIC_ASSERT(sizeof(ItemDataStruct) == 0x48);
    }
    {
        _STATIC_ASSERT(sizeof(ItemStruct) == 0x138);
    }
    {
        _STATIC_ASSERT(sizeof(MissileStruct) == 0xA0);
    }
    {
        _STATIC_ASSERT(sizeof(MonsterData) == 0x70);
    }
    {
        _STATIC_ASSERT(sizeof(MonsterStruct) == 0xC4);
    }
    {
        _STATIC_ASSERT(sizeof(ObjDataStruct) == 0x2C);
    }
    {
        _STATIC_ASSERT(sizeof(ObjectStruct) == 0x74);
    }
    {
        _STATIC_ASSERT(sizeof(PLStruct) == 0x34);
    }
    {
        // TODO: This is currently not the right size!
        _STATIC_ASSERT(sizeof(PlayerStruct) == 0x4270);
    }
    {
        _STATIC_ASSERT(sizeof(QuestStruct) == 0x10);
    }
    {
        _STATIC_ASSERT(sizeof(SpellStruct) == 0x6C);
    }
    {
        _STATIC_ASSERT(sizeof(UItemStruct) == 0x58);
    }
}