/**
 * ScriptDev2 is an extension for mangos providing enhanced features for
 * area triggers, creatures, game objects, instances, items, and spells beyond
 * the default database scripting in mangos.
 *
 * Copyright (C) 2006-2013  ScriptDev2 <http://www.scriptdev2.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * World of Warcraft, and all World of Warcraft or Warcraft art, images,
 * and lore are copyrighted by Blizzard Entertainment, Inc.
 */

/**
 * ScriptData
 * SDName:      Item_Scripts
 * SD%Complete: 100
 * SDComment: Items for a range of different items. See content below (in script)
 * SDCategory:  Items
 * EndScriptData
 */

/**
 * ContentData
 * item_arcane_charges                 Prevent use if player is not flying (cannot cast while on ground)
 * item_flying_machine(i34060,i34061)  Engineering crafted flying machines
 * item_gor_dreks_ointment(i30175)     Protecting Our Own(q10488)
 * item_ogre_brew					   Prevent use if player not in 3522 area
 * EndContentData
 */

#include "precompiled.h"
#include "Spell.h"

/*#####
# item_ogre_brew
#####*/

enum
{
    BLUE_OGRE_BREW	= 32783,
	RED_OGRE_BREW	= 32784,
	TRANCHANTES		= 3522,
	OGRE_BREW_MANA	= 41304,
	OGRE_BREW_LIFE	= 41306
};

bool ItemUse_item_ogre_brew(Player* pPlayer, Item* pItem, const SpellCastTargets&)
{
	uint32 itemId	= pItem->GetEntry();
	uint32 pZoneId	= pPlayer->GetZoneId();
	
	if (itemId == BLUE_OGRE_BREW || itemId == RED_OGRE_BREW)
	{
		if (pZoneId == TRANCHANTES)
		{
			return false;
		}
		debug_log("SD2: Player attempt to use item %u, but did not meet zone requirement : %u", itemId,pZoneId);
		if (itemId == BLUE_OGRE_BREW)
		{
			const SpellEntry* pSpellInfo = GetSpellStore()->LookupEntry(OGRE_BREW_LIFE);
			Spell::SendCastResult(pPlayer, pSpellInfo, 1, SPELL_FAILED_NOT_HERE);
		}
		if (itemId == RED_OGRE_BREW)
		{
			const SpellEntry* pSpellInfo = GetSpellStore()->LookupEntry(OGRE_BREW_MANA);
			Spell::SendCastResult(pPlayer, pSpellInfo, 1, SPELL_FAILED_NOT_HERE);
		}
		return true;
	}
}

/*#####
# item_arcane_charges
#####*/

enum
{
    SPELL_ARCANE_CHARGES    = 45072
};

bool ItemUse_item_arcane_charges(Player* pPlayer, Item* pItem, const SpellCastTargets& /*pTargets*/)
{
    if (pPlayer->IsTaxiFlying())
    { return false; }

    pPlayer->SendEquipError(EQUIP_ERR_NONE, pItem, NULL);

    if (const SpellEntry* pSpellInfo = GetSpellStore()->LookupEntry(SPELL_ARCANE_CHARGES))
    { Spell::SendCastResult(pPlayer, pSpellInfo, 1, SPELL_FAILED_ERROR); }

    return true;
}

/*#####
# item_flying_machine
#####*/

bool ItemUse_item_flying_machine(Player* pPlayer, Item* pItem, const SpellCastTargets& /*pTargets*/)
{
    uint32 itemId = pItem->GetEntry();

    if (itemId == 34060)
        if (pPlayer->GetBaseSkillValue(SKILL_RIDING) >= 225)
        { return false; }

    if (itemId == 34061)
        if (pPlayer->GetBaseSkillValue(SKILL_RIDING) == 300)
        { return false; }

    debug_log("SD2: Player attempt to use item %u, but did not meet riding requirement", itemId);
    pPlayer->SendEquipError(EQUIP_ERR_CANT_EQUIP_SKILL, pItem, NULL);
    return true;
}

/*#####
# item_gor_dreks_ointment
#####*/

enum
{
    NPC_TH_DIRE_WOLF        = 20748,
    SPELL_GORDREKS_OINTMENT = 32578
};

bool ItemUse_item_gor_dreks_ointment(Player* pPlayer, Item* pItem, const SpellCastTargets& pTargets)
{
    if (pTargets.getUnitTarget() && pTargets.getUnitTarget()->GetTypeId() == TYPEID_UNIT && pTargets.getUnitTarget()->HasAura(SPELL_GORDREKS_OINTMENT))
    {
        pPlayer->SendEquipError(EQUIP_ERR_NONE, pItem, NULL);

        if (const SpellEntry* pSpellInfo = GetSpellStore()->LookupEntry(SPELL_GORDREKS_OINTMENT))
        { Spell::SendCastResult(pPlayer, pSpellInfo, 1, SPELL_FAILED_TARGET_AURASTATE); }

        return true;
    }

    return false;
}

void AddSC_item_scripts()
{
    Script* pNewScript;

	pNewScript = new Script;
    pNewScript->Name = "item_ogre_brew";
    pNewScript->pItemUse = &ItemUse_item_ogre_brew;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "item_arcane_charges";
    pNewScript->pItemUse = &ItemUse_item_arcane_charges;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "item_flying_machine";
    pNewScript->pItemUse = &ItemUse_item_flying_machine;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "item_gor_dreks_ointment";
    pNewScript->pItemUse = &ItemUse_item_gor_dreks_ointment;
    pNewScript->RegisterSelf();
}
