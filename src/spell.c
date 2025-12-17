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
    "Channeling the chill of the grave, make a melee spell attack against a target within reach. On a hit, the target takes 1d10 Necrotic damage, and it can't regain Hit Points until the end of your next turn.\nCantrip Upgrade. The damage increases by 1d10 when you reach levels 5 (2d10), 11 (3d10), and 17 (4d10)."
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
    "You exert control over the elements, creating one of the following effects within range:\nBeckon Air. You create a breeze strong enough to ripple cloth, stir dust, rustle leaves, and close open doors and shutters in a 5-foot Cube. Doors and shutters being held open are unaffected.\nBeckon Earth. You create a thin coating of dust or sand covering a 5-foot-square area, or you cause dirt or sand to write a single word in your handwriting.\nBeckon Fire. You create a thin cloud of harmless embers and colored, scented smoke in a 5-foot Cube. You choose the color and scent. The cloud can ignite candles, torches, or lamps. The scent lingers for 1 minute.\nBeckon Water. You create a spray of cool mist that dampens creatures and objects in a 5-foot Cube, or you create 1 cup of clean water in a container or on a surface. The water evaporates in 1 minute.\nSculpt Element. You shape dirt, sand, fire, smoke, mist, or water that can fit in a 1-foot Cube into a crude form for 1 hour."
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
    "You hurl a mote of fire at a creature or an object within range. Make a ranged spell attack against the target. On a hit, the target takes 1d10 Fire damage. A flammable object hit by this spell ignites if it isn't being worn or carried.\nCantrip Upgrade. The damage increases by 1d10 when you reach levels 5 (2d10), 11 (3d10), and 17 (4d10)."
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
    "You magically emanate a sense of friendship toward one creature you can see within range. The target must succeed on a Wisdom saving throw or have the Charmed condition for the duration. The target automatically succeeds if it is non-Humanoid, if you are in combat with it, or if you have cast this spell on it within the past 24 hours.\nThe spell ends early if the target takes damage or if you attack it, deal damage to it, or force it to make a saving throw. When the spell ends, the target knows it was Charmed by you."
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
    "You touch one Large or smaller object that isn't being worn or carried by someone else. Until the spell ends, the object sheds Bright Light in a 20-foot radius and Dim Light for an additional 20 feet. The light can be colored as you like. Completely covering the object with something opaque blocks the light. The spell ends if you cast it again."
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
    "A spectral, floating hand appears at a point you choose within range. The hand lasts for the duration. The hand vanishes if it is ever more than 30 feet away from you or if you cast this spell again.\nYou can use the hand to manipulate an object, open an unlocked door or container, stow or retrieve an item from an open container, or pour out a vial's contents. You can move the hand up to 30 feet each time you use it.\nThe hand can't attack, activate magic items, or carry more than 10 pounds."
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
    "This spell repairs a single break or tear in an object you touch, such as a broken chain link, two halves of a broken key, a torn cloak, or a leaking wineskin. As long as the break or tear is no larger than 1 foot in any dimension, you mend it, leaving no trace of the former damage.\nThis spell can physically repair a magic item or Construct, but the spell can't restore magic to such an object."
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
    "You point toward a creature within range and whisper a message. The target (and only the target) hears the message and can reply in a whisper that only you can hear.\nYou can cast this spell through solid objects if you are familiar with the target and know it is beyond the barrier. Magical silence, 1 foot of stone, 1 inch of metal, 1 foot of wood, or a thin sheet of lead blocks the spell."
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
    "You create a sound or an image of an object within range that lasts for the duration. The illusion ends if you dismiss it as a Magic action or cast this spell again.\nIf a creature takes a Study action to examine the sound or image, the creature can determine that it is an illusion with a successful Intelligence (Investigation) check against your spell save DC. If a creature discerns the illusion for what it is, the illusion becomes faint to the creature.\nSound. If you create a sound, its volume can range from a whisper to a scream. It can be your voice, someone else's voice, a lion's roar, a beating of drums, or any other sound you choose. The sound continues unabated throughout the duration, or you can make discrete sounds at different times before the spell ends.\nImage. If you create an image of an object, such as a chair, muddy footprints, or a small chest, it must be no larger than a 5-foot Cube. The image can't create sound, light, smell, or any other sensory effect. Physical interaction with the image reveals it to be an illusion, since things can pass through it."
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
    "You spray toxic mist at a creature within range. Make a ranged spell attack against the target. On a hit, the target takes 1d12 Poison damage.\nCantrip Upgrade. The damage increases by 1d12 when you reach levels 5 (2d12), 11 (3d12), and 17 (4d12)."
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
    "You create a magical effect within range. Choose one of the following effects:\nSensory Effect. You create an instantaneous, harmless sensory effect, such as a shower of sparks, a puff of wind, faint musical notes, or an odd odor.\nFire Play. You instantaneously light or snuff out a candle, a torch, or a small campfire.\nClean or Soil. You instantaneously clean or soil an object no larger than 1 cubic foot.\nMinor Sensation. You chill, warm, or flavor up to 1 cubic foot of nonliving material for 1 hour.\nMagic Mark. You make a color, a small mark, or a symbol appear on an object or a surface for 1 hour.\nMinor Creation. You create a nonmagical trinket or an illusory image that can fit in your hand. It lasts until the end of your next turn. A trinket can deal no damage and has no monetary value.\nIf you cast this spell multiple times, you can have up to three of its non-instantaneous effects active at a time."
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
    "You touch a willing creature and choose a damage type: Acid, Bludgeoning, Cold, Fire, Lightning, Necrotic, Piercing, Poison, Radiant, Slashing, or Thunder. When the creature takes damage of the chosen type before the spell ends, the creature reduces the total damage taken by 1d4. A creature can benefit from this spell only once per turn."
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
    "Each creature in a 5-foot Emanation originating from you must succeed on a Constitution saving throw or take 1d6 Thunder damage. The spell's sound is audible up to 100 feet away.\nCantrip Upgrade. The damage increases by 1d6 when you reach levels 5 (2d6), 11 (3d6), and 17 (4d6)."
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
    "You point at one creature you can see within range, and the single chime of a dolorous bell is audible within 10 feet of the target. The target must succeed on a Wisdom saving throw or take 1d8 Necrotic damage. If the target is missing any Hit Points, it instead takes 1d12 Necrotic damage.\nCantrip Upgrade. The damage increases by one die when you reach levels 5 (2d8 or 2d12), 11 (3d8 or 3d12), and 17 (4d8 or 4d12)."
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
    "Guided by a flash of magical insight, you make one attack with the weapon used in the spell's casting. The attack uses your spellcasting ability for the attack and damage rolls instead of using Strength or Dexterity. If the attack deals damage, it can be Radiant damage or the weapon's normal damage type (your choice).\nCantrip Upgrade. The attack deals extra Radiant damage when you reach levels 5 (1d6), 11 (2d6), and 17 (3d6)."
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
    "You set an alarm against intrusion. Choose a door, a window, or an area within range that is no larger than a 20-foot Cube. Until the spell ends, an alarm alerts you whenever a Tiny or larger creature touches or enters the warded area. When you cast the spell, you can designate creatures that won't set off the alarm. You also choose whether the alarm is audible or mental:\nAudible Alarm. The alarm produces the sound of a handbell for 10 seconds within 60 feet of the warded area.\nMental Alarm. You are alerted by a mental ping if you are within 1 mile of the warded area. This ping awakens you if you're asleep."
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
    "React",
    "1 min",
    "60 ft",
    false,
    false,
    "Trigger: You or a creature you can see within 60 feet of you falls.\nChoose up to five falling creatures within range. A falling creature's rate of descent slows to 60 feet per round until the spell ends. If a creature lands before the spell ends, the creature takes no Falling damage, and the spell ends for that creature."
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
    "A beam of crackling energy lances toward a creature within range, forming a sustained arc of lightning between you and the target. Make a ranged spell attack against that creature. On a hit, the target takes 2d12 Lightning damage.\nOn each of your subsequent turns, you can take a Bonus Action to deal 1d12 Lightning damage to the target automatically, even if the first attack missed. The spell ends if the target is ever outside the spell's range or if it has Total Cover from you.\nUsing a Higher-Level Spell Slot. The initial damage increases by 1d12 for each spell slot level above 1."
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
    "You receive an omen from an otherworldly entity about the results of a course of action that you plan to take within the next 30 minutes. The DM chooses the omen from the following options:\nWeal, for good results.\nWoe, for bad results.\nWeal and woe, for both good and bad results.\nIndifference, for results that aren't especially good or bad.\nThe spell doesn't account for circumstances, such as other spells, that might change the results. If you cast this spell two or more times before finishing a Long Rest, there is a cumulative 25 percent chance for each casting after the first that you get no answer."
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
    "For the duration, you can read the thoughts of certain creatures. When you cast the spell and as a Magic action on each turn until the spell ends, you can focus your mind on any one creature that you can see within 30 feet. The target makes a Wisdom saving throw. On a failed save, you gain insight into the creature's reasoning, its emotional state, and something that looms large in its mind. On a successful save, the spell ends.\nEither way, the target knows that you are probing into its mind, and the target can use an action on its turn to make an Intelligence (Arcana) check contested by your Intelligence (Arcana) check; if the target succeeds, the spell ends.\nQuestions verbally directed at the target naturally shape the course of its thoughts, so this spell is particularly effective as part of an interrogation.\nThe spell is blocked by 1 foot of stone, 1 inch of metal, or a thin sheet of lead."
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
    "A creature you touch has the Invisible condition until the spell ends. The spell ends early immediately after the target makes an attack roll, deals damage, or casts a spell.\nUsing a Higher-Level Spell Slot. You can target one additional creature for each spell slot level above 2."
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
    "You touch a nonmagical weapon. Until the spell ends, that weapon becomes a magic weapon with a +1 bonus to attack rolls and damage rolls. The spell ends early if you cast it again.\nUsing a Higher-Level Spell Slot. The bonus increases to +2 when you use a spell slot of levels 3-5, or +3 when you use a spell slot of level 6 or higher."
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
    "You create an invisible sensor within range in a location familiar to you (a place you have visited or seen before) or in an obvious location that is unfamiliar to you (such as behind a door, around a corner, or in a grove of trees). The intangible, invulnerable sensor remains in place for the duration.\nWhen you cast the spell, choose seeing or hearing. You can use the chosen sense through the sensor as if you were in its space. As a Bonus Action, you can switch between seeing and hearing.\nA creature that sees the sensor (such as a creature benefiting from See Invisibility or Truesight) sees a luminous orb about the size of your fist."
  },
  {
    "Counterspell",
    abjuration,
    3,
    "React",
    "Inst",
    "60 ft",
    false,
    false,
    "Trigger: You see a creature within 60 feet of yourself casting a spell that has Verbal, Somatic, or Material components.\nYou attempt to interrupt a creature in the process of casting a spell. The creature must make a Constitution saving throw. On a failed save, the spell dissipates with no effect, and the action, Bonus Action, or Reaction used to cast it is wasted. If that spell was cast with a spell slot, the slot isn't expended."
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
    "Any",
    false,
    false,
    "You send a short message of 25 words or fewer to a creature you have met or a creature described to you by someone who has met it. The creature hears the message in its mind, recognizes you as the sender if it knows you, and can answer in a like manner immediately. The spell enables creatures with Intelligence 1 or higher to understand the meaning of your message.\nYou can send the message across any distance and even to other planes of existence, but if the target is on a different plane than you, there is a 5 percent chance that the message doesn't arrive. The target knows you sent the message but can choose to block future Sending spells from you for 8 hours by making this choice as a free action when your message arrives."
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
