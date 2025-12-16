#include "spell.h"
#include "prefix.h"
#include "kit/hashmap.h"
#include "kit/math.h"
#include "kit/mem.h"
#include "stat.h"
#include "kit/debug.h"
#include "kit/str.h"

static Spell spells[] = {
  // Cantrips (Level 0)
  {
    "Acid Splash",
    evocation,
    0,
    "Action",
    "Inst",
    "60 ft",
    false,
    false,
    "You create an acidic bubble at a point within range, where it explodes in a 5-foot-radius Sphere. Each creature in that Sphere must succeed on a Dexterity saving throw or take 1d6 Acid damage.\nThe damage increases by 1d6 at levels 5 (2d6), 11 (3d6), and 17 (4d6)."
  },
  {
    "Blade Ward",
    abjuration,
    0,
    "Action",
    "1 min",
    "Self",
    false,
    true,
    "Whenever a creature makes an attack roll against you before the spell ends, the attacker subtracts 1d4 from the attack roll."
  },
  {
    "Chill Touch",
    necromancy,
    0,
    "Action",
    "Inst",
    "Touch",
    false,
    false,
    "You channel grave energy as you make a melee spell attack against a creature. On a hit, the target takes 1d10 Necrotic damage and can't regain Hit Points until the end of your next turn.\nThe damage increases by 1d10 at levels 5 (2d10), 11 (3d10), and 17 (4d10)."
  },
  {
    "Dancing Lights",
    illusion,
    0,
    "Action",
    "1 min",
    "120 ft",
    false,
    true,
    "You create up to four torch-sized lights within range, making them appear as torches, lanterns, or glowing orbs that hover. You can combine the four lights into one Medium glowing humanoid form. Each light sheds Dim Light in a 10-foot radius.\nAs a Bonus Action, you can move the lights up to 60 feet. A light must be within 20 feet of another light and winks out if it exceeds the spell's range."
  },
  {
    "Elementalism",
    transmutation,
    0,
    "Action",
    "Inst",
    "30 ft",
    false,
    false,
    "You exert control over the elements, creating one of the following effects within range:\nBeckon Air: You create a breeze strong enough to ripple cloth and close doors.\nBeckon Earth: You create a thin coating of dust or cause dirt to write words.\nBeckon Fire: You create a harmless burst of embers and smoke that can light candles or torches.\nBeckon Water: You create a spray of mist or produce 1 cup of water.\nSculpt Element: You shape dirt, sand, fire, smoke, mist, or water into a crude form for 1 hour."
  },
  {
    "Fire Bolt",
    evocation,
    0,
    "Action",
    "Inst",
    "120 ft",
    false,
    false,
    "You hurl a mote of fire at a creature or object within range. Make a ranged spell attack against the target. On a hit, the target takes 1d10 Fire damage. A flammable object hit by this spell starts burning if it isn't being worn or carried.\nThe damage increases by 1d10 at levels 5 (2d10), 11 (3d10), and 17 (4d10)."
  },
  {
    "Friends",
    enchantment,
    0,
    "Action",
    "1 min",
    "10 ft",
    false,
    true,
    "You magically emanate a sense of friendship toward one Humanoid within range. The target must succeed on a Wisdom saving throw or have the Charmed condition for the duration. The spell fails if combat is occurring or if the target was recently affected by this spell.\nThe spell ends early if the target takes damage or if you attack, damage, or force a saving throw from it. When the spell ends, the target knows it was Charmed."
  },
  {
    "Guidance",
    divination,
    0,
    "Action",
    "1 min",
    "Touch",
    false,
    true,
    "You touch a willing creature and choose a skill. Until the spell ends, the creature adds 1d4 to any ability check using that skill."
  },
  {
    "Light",
    evocation,
    0,
    "Action",
    "1 hr",
    "Touch",
    false,
    false,
    "You touch one Large or smaller object that isn't being worn or carried by another creature. The object sheds Bright Light in a 20-foot radius and Dim Light for an additional 20 feet. The light can be colored as you like. Completely covering the object blocks the light. The spell ends if you cast it again."
  },
  {
    "Mage Hand",
    conjuration,
    0,
    "Action",
    "1 min",
    "30 ft",
    false,
    false,
    "A spectral hand appears at a point you choose within range. It can manipulate objects, open unlocked doors and containers, stow or retrieve items, or pour out a vial's contents.\nAs a Magic action, you can control the hand and move it up to 30 feet. The hand can't attack, activate magic items, or carry more than 10 pounds. It vanishes if it moves beyond range or if you cast this spell again."
  },
  {
    "Mending",
    transmutation,
    0,
    "1 min",
    "Inst",
    "Touch",
    false,
    false,
    "This spell repairs a single break or tear in an object you touch, such as a broken chain link, two halves of a broken key, a torn cloak, or a leaking wineskin. The break must be no larger than 1 foot in any dimension. This spell can repair a magic item but can't restore magic to it."
  },
  {
    "Message",
    transmutation,
    0,
    "Action",
    "1 round",
    "120 ft",
    false,
    false,
    "You point toward a creature within range and whisper a message. The target (and only the target) hears the message and can reply in a whisper that only you can hear.\nYou can cast this spell through solid barriers if you are familiar with the target and know it is beyond the barrier. Magical silence, 1 foot of stone or metal, or thin lead blocks the spell."
  },
  {
    "Mind Sliver",
    enchantment,
    0,
    "Action",
    "1 round",
    "60 ft",
    false,
    false,
    "You try to temporarily sliver the mind of one creature you can see within range. The target must succeed on an Intelligence saving throw or take 1d6 Psychic damage and subtract 1d4 from the next saving throw it makes before the end of your next turn.\nThe damage increases by 1d6 at levels 5 (2d6), 11 (3d6), and 17 (4d6)."
  },
  {
    "Minor Illusion",
    illusion,
    0,
    "Action",
    "1 min",
    "30 ft",
    false,
    false,
    "You create a sound or an image of an object within range for the duration. The illusion ends if you cast this spell again.\nWith a Study action, a creature can determine the illusion with a successful Intelligence (Investigation) check against your spell save DC.\nSound: Continuous or discrete sounds, from a whisper to a scream.\nImage: An image of an object no larger than a 5-foot cube. It can't create sound, light, smell, or other sensory effects."
  },
  {
    "Poison Spray",
    necromancy,
    0,
    "Action",
    "Inst",
    "30 ft",
    false,
    false,
    "You spray a puff of noxious gas at a creature within range. Make a ranged spell attack. On a hit, the target takes 1d12 Poison damage.\nThe damage increases by 1d12 at levels 5 (2d12), 11 (3d12), and 17 (4d12)."
  },
  {
    "Prestidigitation",
    transmutation,
    0,
    "Action",
    "1 hr",
    "10 ft",
    false,
    false,
    "You create a magical effect within range. Choose from: a harmless sensory effect, light or snuff a small flame, clean or soil an object up to 1 cubic foot, chill/warm/flavor nonliving material for 1 hour, make a color/mark/symbol appear on an object for 1 hour, or create a nonmagical trinket or illusory image that fits in your hand until the end of your next turn.\nYou can have up to three non-instantaneous effects active at a time."
  },
  {
    "Ray of Frost",
    evocation,
    0,
    "Action",
    "Inst",
    "60 ft",
    false,
    false,
    "A frigid beam of blue-white light streaks toward a creature within range. Make a ranged spell attack. On a hit, the target takes 1d8 Cold damage, and its Speed is reduced by 10 feet until the start of your next turn.\nThe damage increases by 1d8 at levels 5 (2d8), 11 (3d8), and 17 (4d8)."
  },
  {
    "Resistance",
    abjuration,
    0,
    "Action",
    "1 min",
    "Touch",
    false,
    true,
    "You touch a willing creature and choose a damage type: Acid, Bludgeoning, Cold, Fire, Lightning, Necrotic, Piercing, Poison, Radiant, Slashing, or Thunder. When that creature takes damage of that type, it can reduce the total by 1d4 (once per turn)."
  },
  {
    "Shocking Grasp",
    evocation,
    0,
    "Action",
    "Inst",
    "Touch",
    false,
    false,
    "Lightning springs from you to a creature that you try to touch. Make a melee spell attack against the target. On a hit, the target takes 1d8 Lightning damage, and it can't make Opportunity Attacks until the start of its next turn.\nThe damage increases by 1d8 at levels 5 (2d8), 11 (3d8), and 17 (4d8)."
  },
  {
    "Thunderclap",
    evocation,
    0,
    "Action",
    "Inst",
    "Self",
    false,
    false,
    "Each creature in a 5-foot Emanation originating from you must succeed on a Constitution saving throw or take 1d6 Thunder damage. The sound is audible up to 100 feet away.\nThe damage increases by 1d6 at levels 5 (2d6), 11 (3d6), and 17 (4d6)."
  },
  {
    "Toll the Dead",
    necromancy,
    0,
    "Action",
    "Inst",
    "60 ft",
    false,
    false,
    "You point at one creature you can see within range, and the sound of a dolorous bell fills the air around it. The target must succeed on a Wisdom saving throw or take 1d8 Necrotic damage. If the target is missing any Hit Points, it instead takes 1d12 Necrotic damage.\nThe damage increases by one die at levels 5, 11, and 17."
  },
  {
    "True Strike",
    divination,
    0,
    "Action",
    "Inst",
    "Self",
    false,
    false,
    "Guided by magical insight, you strike with a weapon, using your spellcasting ability instead of Strength or Dexterity for the attack and damage rolls. The damage is Radiant or the weapon's normal type (your choice).\nYou deal an extra 1d6 Radiant damage at level 5, 2d6 at level 11, and 3d6 at level 17."
  },
  // Level 1 Spells
  {
    "Alarm",
    abjuration,
    1,
    "1 min",
    "8 hr",
    "30 ft",
    true,
    false,
    "You set an alarm against unwanted intrusion. Choose a door, window, or area within range (up to a 20-foot cube). An alarm alerts you whenever a creature touches or enters the warded area. You can designate creatures that won't trigger it.\nMental Alarm: Alerts you telepathically if within 1 mile, even waking you.\nAudible Alarm: Produces a bell sound for 10 seconds, audible within 60 feet."
  },
  {
    "Comprehend Languages",
    divination,
    1,
    "Action",
    "1 hr",
    "Self",
    true,
    false,
    "For the duration, you understand the literal meaning of any language that you hear or see signed. You also understand any written language that you see, but you must be touching the surface on which the words are written. It takes about 1 minute to read one page of text. This spell doesn't decode symbols or secret messages."
  },
  {
    "Detect Magic",
    divination,
    1,
    "Action",
    "10 min",
    "Self",
    true,
    true,
    "For the duration, you sense the presence of magical effects within 30 feet of yourself. If you sense such effects, you can take the Magic action to see a faint aura around any visible creature or object in the area that bears the magic, and if an effect was created by a spell, you learn the spell's school of magic.\nThe spell is blocked by 1 foot of stone, dirt, or wood; 1 inch of metal; or a thin sheet of lead."
  },
  {
    "False Life",
    necromancy,
    1,
    "Action",
    "Inst",
    "Self",
    false,
    false,
    "You gain 2d4 + 4 Temporary Hit Points.\nUsing a Higher-Level Spell Slot: You gain 5 additional Temporary Hit Points for each spell slot level above 1."
  },
  {
    "Feather Fall",
    transmutation,
    1,
    "Reaction",
    "1 min",
    "60 ft",
    false,
    false,
    "Reaction: When you or a creature within 60 feet falls.\nChoose up to five falling creatures within range. A falling creature's rate of descent slows to 60 feet per round. If the creature lands before the spell ends, it takes no falling damage and lands on its feet."
  },
  {
    "Healing Word",
    abjuration,
    1,
    "Bonus",
    "Inst",
    "60 ft",
    false,
    false,
    "A creature of your choice that you can see within range regains Hit Points equal to 2d4 plus your spellcasting ability modifier.\nUsing a Higher-Level Spell Slot: The healing increases by 2d4 for each spell slot level above 1."
  },
  {
    "Identify",
    divination,
    1,
    "1 min",
    "Inst",
    "Touch",
    true,
    false,
    "You touch an object throughout the spell's casting. If the object is a magic item or some other magical object, you learn its properties and how to use them, whether it requires Attunement, and how many charges it has, if any. You learn whether any ongoing spells are affecting the item and what they are. If the item was created by a spell, you learn that spell's name.\nIf you instead touch a creature throughout the casting, you learn which ongoing spells, if any, are currently affecting it."
  },
  {
    "Sleep",
    enchantment,
    1,
    "Action",
    "1 min",
    "60 ft",
    false,
    true,
    "Each creature of your choice in a 5-foot-radius Sphere centered on a point within range must succeed on a Wisdom saving throw or have the Incapacitated condition until the end of its next turn, at which point it must repeat the save. If the target fails the second save, the target has the Unconscious condition for the duration. The spell ends on a target if it takes damage or someone within 5 feet of it takes an action to shake it out of the spell's effect.\nCreatures that don't sleep, such as elves, or that have Immunity to the Exhaustion condition automatically succeed on saves against this spell."
  },
  {
    "Thunderwave",
    evocation,
    1,
    "Action",
    "Inst",
    "Self",
    false,
    false,
    "You unleash a wave of thunderous energy. Each creature in a 15-foot Cube originating from you makes a Constitution saving throw. On a failed save, a creature takes 2d8 Thunder damage and is pushed 10 feet away from you. On a successful save, a creature takes half as much damage only.\nIn addition, unsecured objects that are entirely within the Cube are pushed 10 feet away from you, and a thunderous boom is audible within 300 feet."
  },
  {
    "Witch Bolt",
    evocation,
    1,
    "Action",
    "1 min",
    "60 ft",
    false,
    true,
    "A beam of crackling energy lances toward a creature within range. Make a ranged spell attack. On a hit, the target takes 2d12 Lightning damage.\nOn subsequent turns, you can use a Bonus Action to deal 1d12 Lightning damage to the target automatically. The spell ends if the target moves beyond range or gains total cover from you.\nUsing a Higher-Level Spell Slot: The initial damage increases by 1d12 for each slot level above 1."
  },
  // Level 2 Spells
  {
    "Augury",
    divination,
    2,
    "1 min",
    "Inst",
    "Self",
    true,
    false,
    "You receive an omen about the results of a course of action you plan to take within 30 minutes. The DM chooses from: Weal (good), Woe (bad), Weal and Woe (both), or Nothing (neither).\nThe spell doesn't account for possible circumstances that might change the outcome. If you cast it again before a Long Rest, there is a cumulative 25% chance per casting of getting a random reading."
  },
  {
    "Detect Thoughts",
    divination,
    2,
    "Action",
    "1 min",
    "Self",
    false,
    true,
    "You can use one of the following effects. As a Magic action, you can switch between them.\nSense Thoughts: You sense the presence of thoughts within 30 feet. Blocked by 2 feet of rock, 2 inches of metal, or thin lead.\nRead Thoughts: Focus on one creature within 30 feet to learn its surface thoughts. As an action, you can probe deeper; the target makes a Wisdom save. On a failure, you gain insight into its reasoning and emotional state. On success, the spell ends. Either way, the target knows you probed its mind."
  },
  {
    "Hold Person",
    enchantment,
    2,
    "Action",
    "1 min",
    "60 ft",
    false,
    true,
    "Choose a Humanoid within range. The target must succeed on a Wisdom saving throw or have the Paralyzed condition for the duration. At the end of each of its turns, the target repeats the save, ending the spell on a success.\nUsing a Higher-Level Spell Slot: You can target one additional Humanoid for each slot level above 2."
  },
  {
    "Invisibility",
    illusion,
    2,
    "Action",
    "1 hr",
    "Touch",
    false,
    true,
    "A creature you touch has the Invisible condition until the spell ends. The spell ends early if the target makes an attack roll, deals damage, or casts a spell.\nUsing a Higher-Level Spell Slot: You can target one additional creature for each slot level above 2."
  },
  {
    "Locate Object",
    divination,
    2,
    "Action",
    "10 min",
    "Self",
    false,
    true,
    "Describe or name an object that is familiar to you. You sense the direction to the object's location if it is within 1,000 feet. If the object is moving, you know the direction of its movement.\nThe spell can locate a specific object you know or the nearest object of a particular kind. This spell can't locate an object if any thickness of lead blocks a direct path between you and the object."
  },
  {
    "Magic Mouth",
    illusion,
    2,
    "1 min",
    "Forever",
    "30 ft",
    true,
    false,
    "You implant a message within an object. The message (up to 25 words) is spoken when a trigger condition you set occurs. The trigger must be visual or audible within 30 feet of the object.\nA magical mouth appears on the object and recites the message in your voice. You can make it repeat or deliver the message once. The spell lasts until dispelled."
  },
  {
    "Magic Weapon",
    transmutation,
    2,
    "Bonus",
    "1 hr",
    "Touch",
    false,
    false,
    "You touch a nonmagical weapon. Until the spell ends, that weapon becomes a +1 magic weapon.\nUsing a Higher-Level Spell Slot: The bonus increases to +2 when cast with a 3rd-5th level slot, or +3 with a 6th+ level slot."
  },
  {
    "Mind Spike",
    divination,
    2,
    "Action",
    "1 hr",
    "120 ft",
    false,
    true,
    "You drive a psychic spike into the mind of a creature. The target must make a Wisdom saving throw, taking 3d8 Psychic damage on a failed save, or half as much on a success.\nOn a failed save, you always know the target's location while concentrating, and the target can't become hidden from you or benefit from being invisible against you.\nUsing a Higher-Level Spell Slot: The damage increases by 1d8 for each slot level above 2."
  },
  {
    "See Invisibility",
    divination,
    2,
    "Action",
    "1 hr",
    "Self",
    false,
    false,
    "For the duration, you see invisible creatures and objects as if they were visible, and you can see into the Ethereal Plane. Ethereal creatures and objects appear ghostly and translucent."
  },
  // Level 3 Spells
  {
    "Clairvoyance",
    divination,
    3,
    "10 min",
    "10 min",
    "1 mile",
    false,
    true,
    "You create an invisible sensor within range in a location familiar to you or in an obvious location. The sensor is intangible and invulnerable.\nWhen you cast the spell, choose seeing or hearing. You perceive through the sensor as if you were there. As a Bonus Action, you can switch between seeing and hearing.\nA creature that can see invisible things sees the sensor as a luminous orb about the size of your fist."
  },
  {
    "Counterspell",
    abjuration,
    3,
    "Reaction",
    "Inst",
    "60 ft",
    false,
    false,
    "Reaction: You see a creature within 60 feet casting a spell.\nThe creature must make a Constitution saving throw. On a failed save, the spell dissipates with no effect and the action, Bonus Action, or Reaction used to cast it is wasted. If the spell used a spell slot, that slot is not expended."
  },
  {
    "Dispel Magic",
    abjuration,
    3,
    "Action",
    "Inst",
    "120 ft",
    false,
    false,
    "Choose one creature, object, or magical effect within range. Any ongoing spell of level 3 or lower on the target ends. For each spell of 4th level or higher, make an ability check using your spellcasting ability (DC = 10 + the spell's level). On a success, the spell ends.\nUsing a Higher-Level Spell Slot: You automatically end spells of that slot's level or lower."
  },
  {
    "Sending",
    divination,
    3,
    "Action",
    "Inst",
    "Unlimited",
    false,
    false,
    "You send a message of 25 words or fewer to a creature you have met or one described to you by someone who has met it. The creature hears the message mentally, recognizes you if it knows you, and can answer immediately in the same manner.\nThe message can travel any distance and across planes, but sending to another plane has a 5% chance of failure."
  },
  {
    "Tongues",
    divination,
    3,
    "Action",
    "1 hr",
    "Touch",
    false,
    false,
    "This spell grants the creature you touch the ability to understand any spoken or signed language that it hears or sees. Moreover, when the target speaks, any creature that knows at least one language and can hear or see the target understands what it says."
  }
};

static HashMap spellMap = EmptyHashMap;
static HashMap preparedSpells = EmptyHashMap;

static i32 CompareSpell(Spell *a, Spell *b)
{
  if (a->level == b->level) {
    return CompareStr(a->name, b->name);
  }
  return (i32)a->level - (i32)b->level;
}

static u32 PartitionSpells(u32 lo, u32 hi)
{
  Spell *pivot = &spells[lo];

  while (true) {
    while (CompareSpell(&spells[lo], pivot) < 0) lo++;
    while (CompareSpell(pivot, &spells[hi]) < 0) hi--;
    if (lo >= hi) return hi;
    Spell tmp = spells[lo];
    spells[lo] = spells[hi];
    spells[hi] = tmp;
  }
}

static void SortSpells(u32 lo, u32 hi)
{
  if (lo < hi) {
    u32 p = PartitionSpells(lo, hi);
    SortSpells(lo, p);
    SortSpells(p+1, hi);
  }
}

void InitSpells(void)
{
  SortSpells(0, ArrayCount(spells)-1);
  for (u32 i = 0; i < ArrayCount(spells); i++) {
    HashMapSet(&spellMap, HashStr(spells[i].name), i);
  }
}

Spell *GetSpell(char *name)
{
  u32 index;
  if (HashMapFetch(&spellMap, HashStr(name), &index)) {
    return spells + index;
  }
  char msg[256] = {0};
  CopyStr("Could not find spell \"^\"", msg);
  FormatStr(msg, name, 256);
  Error(msg);
  return 0;
}

Spell *GetIndSpell(u32 i)
{
  return spells + i;
}

u32 NumSpells(void)
{
  return ArrayCount(spells);
}

void PrepareSpell(char *name)
{
  Stat *maxStat = GetStat("PreparedSpells");
  if ((i32)preparedSpells.count >= maxStat->value) return;
  u32 key = HashStr(name);
  if (HashMapContains(&preparedSpells, key)) return;
  HashMapSet(&preparedSpells, key, 1);
}

void UnprepareSpell(char *name)
{
  u32 key = HashStr(name);
  HashMapDelete(&preparedSpells, key);
}

bool IsSpellPrepared(char *name)
{
  return HashMapContains(&preparedSpells, HashStr(name));
}
