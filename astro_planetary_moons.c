/**
 * @file astro_planetary_moons.c
 * @brief Implementation of planetary moons calculations with mythology
 */

#include "astro_planetary_moons.h"
#include "astro_core.h"
#include "astro_aspects.h"
#include "swephexp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>

/* ========================================================================
 * Planetary Moon Database - Mythology & Data for All 27 Moons
 * ======================================================================== */

const PlanetaryMoonData moon_database[] = {
    /* Mars Moons - Fear & Terror */
    {
        MOON_PHOBOS, "Phobos", ASTRO_MARS, "Mars",
        "Phobos, son of Ares/Mars, is the personification of fear and panic in battle. "
        "He accompanied his father into war, spreading terror among enemies.",
        "Primal Fear",
        {"fear", "panic", "terror", "instinct", "survival", "adrenaline", "fight", "flight"},
        "Triggers primal survival instincts and fear responses. Where we face our deepest terrors.",
        "Confronting fears that drive aggressive or defensive reactions",
        "Paralysis, blind panic, overwhelming terror",
        "Courage through facing fear, warrior spirit",
        0.319, 1877, "Asaph Hall"
    },
    {
        MOON_DEIMOS, "Deimos", ASTRO_MARS, "Mars",
        "Deimos, brother of Phobos, represents dread and slow-building anxiety. "
        "Where Phobos is sudden terror, Deimos is the creeping unease before battle.",
        "Slow Dread",
        {"dread", "anxiety", "unease", "foreboding", "anticipation", "worry", "tension", "apprehension"},
        "The anticipatory anxiety before conflict. Slow-building tension and worry.",
        "Chronic anxiety patterns and anticipatory dread",
        "Paralyzing worry, catastrophizing",
        "Preparedness, healthy caution",
        1.263, 1877, "Asaph Hall"
    },

    /* Jupiter Moons - Divine Drama & Transformation */
    {
        MOON_IO, "Io", ASTRO_JUPITER, "Jupiter",
        "Io was a priestess of Hera transformed into a heifer by Zeus/Jupiter to hide their affair. "
        "Tormented by a gadfly sent by Hera, she wandered the earth in agony until finally freed. "
        "Io's moon is the most volcanically active body in the solar system.",
        "Volcanic Transformation",
        {"passion", "suffering", "transformation", "volcanic", "intense", "torment", "eros", "purification"},
        "Intense passions and transformative suffering. Where devotion leads to transmutation.",
        "Passion that burns and transforms through suffering",
        "Tormented love, obsessive passion, victim of jealousy",
        "Transformation through intensity, purified passion",
        1.769, 1610, "Galileo"
    },
    {
        MOON_EUROPA, "Europa", ASTRO_JUPITER, "Jupiter",
        "Europa was a Phoenician princess abducted by Zeus in the form of a bull. "
        "She represents forbidden love and hidden depths - her moon has a subsurface ocean "
        "beneath its icy crust, potentially harboring life.",
        "Hidden Depths",
        {"mystery", "depths", "hidden", "forbidden", "beauty", "ocean", "secrets", "potential"},
        "What lies beneath the surface. Hidden emotional depths and secret passions.",
        "The unconscious emotional realm, what we keep hidden",
        "Repression, frozen emotions, inaccessible feelings",
        "Deep emotional wisdom, hidden beauty revealed",
        3.551, 1610, "Galileo"
    },
    {
        MOON_GANYMEDE, "Ganymede", ASTRO_JUPITER, "Jupiter",
        "Ganymede was a beautiful Trojan prince abducted by Zeus to serve as cupbearer "
        "to the gods on Olympus, granted immortality and eternal youth. "
        "The largest moon in the solar system.",
        "Divine Service",
        {"beauty", "service", "elevation", "immortality", "youth", "devotion", "divine", "chosen"},
        "Being chosen for a higher purpose. Service to the divine or to ideals.",
        "The call to higher service, being selected or elevated",
        "Loss of autonomy, being taken from home, forced service",
        "Willing devotion, honored service, spiritual calling",
        7.155, 1610, "Galileo"
    },
    {
        MOON_CALLISTO, "Callisto", ASTRO_JUPITER, "Jupiter",
        "Callisto, a companion of Artemis, was seduced by Zeus and transformed into a bear "
        "by jealous Hera. Later placed in the sky as Ursa Major constellation. "
        "Represents wilderness, independence, and transformation through exile.",
        "Wilderness Spirit",
        {"wilderness", "independence", "transformation", "exile", "nature", "bear", "freedom", "wild"},
        "The untamed self. Independence through separation from the collective.",
        "Finding authenticity in wilderness, transformation through isolation",
        "Exile, banishment, loss of human form, loneliness",
        "Wild wisdom, authentic nature, freedom in solitude",
        16.689, 1610, "Galileo"
    },

    /* Saturn Moons - Structure & Boundaries */
    {
        MOON_MIMAS, "Mimas", ASTRO_SATURN, "Saturn",
        "Named after a Giant in Greek mythology, slain by Hephaestus/Ares. "
        "Mimas bears a massive crater (Herschel) that nearly destroyed it - "
        "a testament to survival against overwhelming force.",
        "Survival After Impact",
        {"survival", "impact", "resilience", "crater", "structure", "endurance", "near-death", "strength"},
        "Surviving catastrophic events. Bearing the scars of near-destruction.",
        "Trauma survival, resilience after major life impacts",
        "Permanent damage, fragility, vulnerability to destruction",
        "Incredible resilience, strength forged through trials",
        0.942, 1789, "William Herschel"
    },
    {
        MOON_ENCELADUS, "Enceladus", ASTRO_SATURN, "Saturn",
        "Named after a Giant buried under Mount Etna by Athena. "
        "Enceladus moon has geysers erupting from its south pole, suggesting a "
        "subsurface ocean - hidden life beneath an icy Saturnian exterior.",
        "Hidden Life",
        {"geysers", "underground", "hidden", "life", "eruption", "buried", "potential", "emergence"},
        "Life force hidden beneath a cold exterior. Underground currents of vitality.",
        "Hidden vitality, suppressed life force seeking expression",
        "Buried alive, suppressed expression, frozen potential",
        "Breakthrough of hidden vitality, life force emerging",
        1.370, 1789, "William Herschel"
    },
    {
        MOON_TETHYS, "Tethys", ASTRO_SATURN, "Saturn",
        "Tethys was a Titan goddess of fresh water, daughter of Uranus and Gaia, "
        "wife of Oceanus. She represents the nourishing, flowing aspect of water - "
        "rivers, springs, and sources of life.",
        "Flowing Nourishment",
        {"water", "nourishment", "flow", "source", "fresh", "rivers", "springs", "sustenance"},
        "The flow of emotional nourishment. Sources of sustenance and renewal.",
        "Emotional flow and nourishment, giving and receiving",
        "Dried up sources, emotional drought, inability to nourish",
        "Abundant flow, generous nourishment, sustaining others",
        1.888, 1684, "Giovanni Cassini"
    },
    {
        MOON_DIONE, "Dione", ASTRO_SATURN, "Saturn",
        "Dione was an oracular goddess at Dodona, sometimes considered mother of Aphrodite. "
        "She represents divine feminine wisdom and prophecy - the voice of the goddess.",
        "Oracular Wisdom",
        {"oracle", "wisdom", "prophecy", "feminine", "divine", "voice", "goddess", "knowing"},
        "Divine feminine wisdom. Prophetic insight and oracular knowing.",
        "Intuitive wisdom, prophetic vision, feminine knowing",
        "Unheeded warnings, silenced wisdom, ignored intuition",
        "Clear prophetic sight, trusted wisdom, voice of truth",
        2.737, 1684, "Giovanni Cassini"
    },
    {
        MOON_RHEA, "Rhea", ASTRO_SATURN, "Saturn",
        "Rhea was a Titan goddess, mother of the Olympian gods including Zeus. "
        "She represents the mother earth principle - fertility, nourishment, "
        "and the power of the mother to protect her children.",
        "Mother Goddess",
        {"mother", "earth", "fertility", "protection", "children", "goddess", "nurture", "foundation"},
        "The great mother principle. Fertility, protection, and maternal strength.",
        "Maternal instinct, protective power, fertility",
        "Smothering, possessiveness, fear of losing children",
        "Wise mother, protective strength, nurturing power",
        4.518, 1672, "Giovanni Cassini"
    },
    {
        MOON_TITAN, "Titan", ASTRO_SATURN, "Saturn",
        "Named after the Titans, primordial deities who preceded the Olympians. "
        "Titan is the only moon with a substantial atmosphere and surface liquids - "
        "a world unto itself, representing primordial power and atmospheric depth.",
        "Primordial Power",
        {"primordial", "power", "atmosphere", "depth", "ancient", "titan", "world", "substance"},
        "Ancient primordial power. Deep atmospheric forces that shape reality.",
        "Connection to ancient powers, depth of consciousness",
        "Overwhelming force, crushing pressure, titanic struggle",
        "Primordial wisdom, atmospheric depth, substantial presence",
        15.945, 1655, "Christiaan Huygens"
    },
    {
        MOON_HYPERION, "Hyperion", ASTRO_SATURN, "Saturn",
        "Hyperion was a Titan, 'the high one,' associated with watchfulness and observation. "
        "His moon has a chaotic rotation and sponge-like appearance - "
        "representing observation from multiple shifting perspectives.",
        "Watchful Observer",
        {"watchfulness", "observation", "light", "high", "perspective", "witness", "chaos", "seeing"},
        "The witness consciousness. Observation from elevated perspective.",
        "Detached observation, witnessing, multiple perspectives",
        "Disassociation, inability to engage, paralysis by analysis",
        "Clear seeing, wise witnessing, elevated perspective",
        21.277, 1848, "William Bond"
    },
    {
        MOON_IAPETUS, "Iapetus", ASTRO_SATURN, "Saturn",
        "Iapetus was a Titan, father of Atlas and Prometheus, 'the piercer' - "
        "associated with mortality and violence. His moon is strikingly two-toned, "
        "one side bright, one dark - representing duality, choice, and mortality.",
        "Duality & Mortality",
        {"duality", "mortality", "piercing", "choice", "light-dark", "violence", "death", "contrast"},
        "The stark reality of duality and mortality. Where opposites meet.",
        "Confronting mortality, making crucial choices, seeing duality",
        "Violence, harsh judgment, rigid either-or thinking",
        "Wisdom through mortality, integration of opposites",
        79.330, 1671, "Giovanni Cassini"
    },

    /* Uranus Moons - Shakespearean Archetypes */
    {
        MOON_ARIEL, "Ariel", ASTRO_URANUS, "Uranus",
        "From Shakespeare's 'The Tempest' - an air spirit bound to serve Prospero, "
        "longing for freedom. Represents airy intellect, magic, and the yearning for liberation.",
        "Air Spirit",
        {"air", "spirit", "magic", "freedom", "intellect", "bound", "service", "liberation"},
        "The airy intellect seeking freedom. Magical thinking and liberation.",
        "Mental agility, magical thinking, desire for freedom",
        "Bound service, intellectual imprisonment, frustrated freedom",
        "Liberated mind, magical creativity, free spirit",
        2.520, 1851, "William Lassell"
    },
    {
        MOON_UMBRIEL, "Umbriel", ASTRO_URANUS, "Uranus",
        "From Alexander Pope's 'The Rape of the Lock' - a dark sprite who brings melancholy. "
        "Represents shadow, depression, and the darkening of spirits.",
        "Shadow & Melancholy",
        {"shadow", "melancholy", "darkness", "sprite", "depression", "gloom", "night", "sorrow"},
        "The shadow self and melancholic moods. Darkening of the spirit.",
        "Shadow work, confronting melancholy, dark night",
        "Depression, despair, overwhelming gloom",
        "Wisdom through darkness, depth of feeling",
        4.144, 1851, "William Lassell"
    },
    {
        MOON_TITANIA, "Titania", ASTRO_URANUS, "Uranus",
        "From Shakespeare's 'A Midsummer Night's Dream' - Queen of the Fairies, "
        "representing enchantment, dreams, illusion, and the power of the imagination.",
        "Fairy Queen",
        {"fairy", "queen", "dreams", "enchantment", "illusion", "imagination", "magic", "sovereignty"},
        "The realm of dreams and imagination. Enchantment and fairy magic.",
        "Dream work, imagination, enchantment, creative sovereignty",
        "Lost in illusion, bewitched, loss of reality",
        "Sovereign imagination, dream wisdom, enchanted creativity",
        8.706, 1787, "William Herschel"
    },
    {
        MOON_OBERON, "Oberon", ASTRO_URANUS, "Uranus",
        "From Shakespeare's 'A Midsummer Night's Dream' - King of the Fairies, "
        "representing authority in the dream realm, testing, and magical sovereignty.",
        "Fairy King",
        {"king", "authority", "testing", "fairy", "magic", "sovereignty", "judgment", "power"},
        "Authority in the dream realm. Testing through enchantment.",
        "Dream authority, magical testing, sovereign power",
        "Cruel testing, jealous authority, misuse of magic",
        "Wise authority, beneficial testing, magical sovereignty",
        13.463, 1787, "William Herschel"
    },
    {
        MOON_MIRANDA, "Miranda", ASTRO_URANUS, "Uranus",
        "From Shakespeare's 'The Tempest' - Prospero's daughter who grew up isolated "
        "on an enchanted island, experiencing wonder upon meeting other humans. "
        "Represents innocent wonder, suffering, and transformation through experience.",
        "Wonder & Suffering",
        {"wonder", "innocence", "suffering", "transformation", "exile", "beauty", "experience", "awakening"},
        "Innocent wonder meeting harsh reality. Transformation through suffering.",
        "Awakening to reality, loss of innocence, wonder through pain",
        "Naive victimhood, crushed innocence, overwhelming experience",
        "Wisdom through wonder, beauty in suffering, transformed innocence",
        1.413, 1948, "Gerard Kuiper"
    },

    /* Neptune Moons - Oceanic Depths */
    {
        MOON_TRITON, "Triton", ASTRO_NEPTUNE, "Neptune",
        "Triton was a sea god, son of Poseidon/Neptune and Amphitrite, "
        "messenger of the sea carrying a conch shell trumpet to calm or raise storms. "
        "Represents the voice of the ocean depths and mastery over emotional storms.",
        "Ocean's Voice",
        {"ocean", "messenger", "trumpet", "storms", "voice", "depths", "sound", "waves"},
        "The voice of the deep. Calling forth or calming emotional storms.",
        "Emotional expression, depth of feeling, oceanic consciousness",
        "Overwhelming emotions, tidal waves of feeling",
        "Mastery of emotions, voice of depth, calm and storm",
        5.877, 1846, "William Lassell"
    },
    {
        MOON_NEREID, "Nereid", ASTRO_NEPTUNE, "Neptune",
        "The Nereids were 50 sea nymphs, daughters of Nereus, dancing and playing "
        "in the waves. Represent joy, play, and the dancing spirit of the ocean.",
        "Sea Nymph",
        {"nymph", "dance", "play", "joy", "waves", "beauty", "maiden", "ocean"},
        "Playful dancing spirit of the waves. Joy and beauty in emotional flow.",
        "Emotional playfulness, joy in feeling, dancing with life",
        "Superficiality, avoidance through play, flighty emotions",
        "Joyful depth, playful wisdom, dancing spirit",
        360.136, 1949, "Gerard Kuiper"
    },
    {
        MOON_PROTEUS, "Proteus", ASTRO_NEPTUNE, "Neptune",
        "Proteus was an early sea god with the gift of prophecy who would change shape "
        "to avoid having to tell the future. Represents shape-shifting, evasion, and prophecy.",
        "Shape-Shifter",
        {"shapeshifter", "prophecy", "evasion", "transformation", "sea", "elder", "change", "vision"},
        "Shape-shifting consciousness. Evasion and prophecy intertwined.",
        "Adaptability, prophetic vision masked by change",
        "Evasion, inability to commit to form, slippery truth",
        "Wise adaptability, prophetic flexibility, transformative vision",
        1.122, 1989, "Voyager 2"
    },

    /* Pluto Moons - Underworld Mysteries */
    {
        MOON_CHARON, "Charon", ASTRO_PLUTO, "Pluto",
        "Charon was the ferryman who carried souls across the river Styx to Hades. "
        "Represents the threshold between life and death, transition, and the one "
        "who guides souls through transformation.",
        "Ferryman",
        {"ferryman", "threshold", "transition", "death", "guide", "passage", "souls", "crossing"},
        "The threshold guardian and guide through transformation and death.",
        "Major life transitions, death and rebirth, threshold experiences",
        "Stuck at the threshold, fear of transition, refusal to cross",
        "Wise guidance through change, easy transitions, soul guide",
        6.387, 1978, "James Christy"
    },
    {
        MOON_NIX, "Nix", ASTRO_PLUTO, "Pluto",
        "Nyx (Nix) was the primordial goddess of night, mother of Charon and many others. "
        "She represents the darkness from which all things emerge, the void, the night.",
        "Primordial Night",
        {"night", "darkness", "void", "mother", "primordial", "goddess", "mystery", "origin"},
        "Primordial darkness. The void from which forms emerge.",
        "Deep unconscious, primordial mystery, creative void",
        "Terror of the void, overwhelming darkness, nihilism",
        "Wisdom of the void, creative darkness, night wisdom",
        24.856, 2005, "Hubble Team"
    },
    {
        MOON_HYDRA, "Hydra", ASTRO_PLUTO, "Pluto",
        "The Hydra was a many-headed serpent; cut off one head and two grow back. "
        "Guardian of the underworld, represents regeneration, multiplication of problems, "
        "and the need for clever solutions.",
        "Multi-Headed Guardian",
        {"hydra", "regeneration", "multiplying", "serpent", "guardian", "heads", "growth", "problem"},
        "Problems that multiply. Regeneration through destruction.",
        "Regenerative power, creative multiplication, protective complexity",
        "Overwhelming problems, exponential difficulty, endless struggle",
        "Regenerative wisdom, creative complexity, protective power",
        38.202, 2005, "Hubble Team"
    },
    {
        MOON_KERBEROS, "Kerberos", ASTRO_PLUTO, "Pluto",
        "Cerberus (Kerberos) was the three-headed dog guarding the gates of Hades. "
        "Represents the guardian at the threshold, the one who ensures no one leaves "
        "the underworld without permission.",
        "Three-Headed Guardian",
        {"cerberus", "guardian", "three-headed", "dog", "gates", "threshold", "guard", "keeper"},
        "The fierce guardian of boundaries. Protector of sacred thresholds.",
        "Boundary protection, threshold guarding, fierce loyalty",
        "Vicious boundaries, attack dog, inability to allow passage",
        "Wise protection, discerning guardian, threshold wisdom",
        32.167, 2011, "Hubble Team"
    },
    {
        MOON_STYX, "Styx", ASTRO_PLUTO, "Pluto",
        "The Styx was the river of unbreakable oaths in the underworld. "
        "Gods swore upon the Styx and could not break such vows. Represents binding, "
        "oath-keeping, invincibility, and the power of sacred commitment.",
        "River of Oaths",
        {"styx", "oath", "binding", "river", "vow", "invincible", "sacred", "commitment"},
        "Sacred oaths and binding commitments. The power of unbreakable vows.",
        "Deep commitments, sacred vows, invincible resolve",
        "Trapped by oaths, unable to break free, bound by past",
        "Power of commitment, sacred integrity, oath-keeping",
        20.162, 2012, "Hubble Team"
    }
};

const int NUM_MOONS = sizeof(moon_database) / sizeof(PlanetaryMoonData);

/* ========================================================================
 * Helper Functions
 * ======================================================================== */

/**
 * Convert our moon ID to Swiss Ephemeris planet number
 */
static int moon_id_to_se_planet(PlanetaryMoonID moon_id) {
    /* Moon IDs are already in Swiss Ephemeris format (9401-9905) */
    return (int)moon_id;
}

/**
 * Get parent planet from moon ID
 */
static AstroPlanet get_parent_planet(PlanetaryMoonID moon_id) {
    if (moon_id >= 9401 && moon_id <= 9402) return ASTRO_MARS;
    if (moon_id >= 9501 && moon_id <= 9504) return ASTRO_JUPITER;
    if (moon_id >= 9601 && moon_id <= 9608) return ASTRO_SATURN;
    if (moon_id >= 9701 && moon_id <= 9705) return ASTRO_URANUS;
    if (moon_id >= 9801 && moon_id <= 9808) return ASTRO_NEPTUNE;
    if (moon_id >= 9901 && moon_id <= 9905) return ASTRO_PLUTO;
    return -1;
}

/* ========================================================================
 * Core Data Functions
 * ======================================================================== */

const PlanetaryMoonData* astro_moons_get_data(PlanetaryMoonID moon_id) {
    for (int i = 0; i < 27; i++) {  /* We have exactly 27 moons */
        if (moon_database[i].id == moon_id) {
            return &moon_database[i];
        }
    }
    return NULL;
}

const char* astro_moons_get_mythology(PlanetaryMoonID moon_id) {
    const PlanetaryMoonData *data = astro_moons_get_data(moon_id);
    return data ? data->mythology : "Unknown moon";
}

const char* astro_moons_get_archetype(PlanetaryMoonID moon_id) {
    const PlanetaryMoonData *data = astro_moons_get_data(moon_id);
    return data ? data->archetype : "Unknown";
}

const char* astro_moons_get_shadow(PlanetaryMoonID moon_id) {
    const PlanetaryMoonData *data = astro_moons_get_data(moon_id);
    return data ? data->shadow_expression : "Unknown";
}

const char* astro_moons_get_gift(PlanetaryMoonID moon_id) {
    const PlanetaryMoonData *data = astro_moons_get_data(moon_id);
    return data ? data->gift_expression : "Unknown";
}

/* ========================================================================
 * Position Calculation Functions
 * ======================================================================== */

int astro_moons_calc_position(PlanetaryMoonID moon_id,
                              const AstroDateTime *datetime,
                              PlanetaryMoonPosition *position) {
    if (datetime == NULL || position == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    /* Get moon data */
    const PlanetaryMoonData *data = astro_moons_get_data(moon_id);
    if (data == NULL) {
        return ASTRO_ERROR_INVALID_PLANET;
    }

    /* Calculate Julian Day */
    double jd = astro_core_get_julian_day(datetime);

    /* Calculate moon position using Swiss Ephemeris */
    double xx[6];
    char serr[256];
    int se_planet = moon_id_to_se_planet(moon_id);

    int result = swe_calc_ut(jd, se_planet, SEFLG_SWIEPH | SEFLG_SPEED, xx, serr);
    if (result < 0) {
        fprintf(stderr, "Error calculating %s: %s\n", data->name, serr);
        return ASTRO_ERROR_CALCULATION;
    }

    /* Fill in moon position */
    position->moon_id = moon_id;
    position->moon_name = data->name;
    position->data = data;

    /* Fill in astronomical position */
    position->position.planet = se_planet;
    position->position.longitude = xx[0];
    position->position.latitude = xx[1];
    position->position.distance = xx[2];
    position->position.longitude_speed = xx[3];
    position->position.latitude_speed = xx[4];
    position->position.distance_speed = xx[5];

    /* Calculate sign */
    position->position.sign = (AstroSign)((int)(xx[0] / 30.0) % 12);
    position->position.sign_position = fmod(xx[0], 30.0);
    position->position.is_retrograde = (xx[3] < 0.0) ? 1 : 0;

    strncpy(position->position.name, data->name, ASTRO_MAX_NAME_LEN - 1);
    position->position.name[ASTRO_MAX_NAME_LEN - 1] = '\0';
    position->position.house = 0;  /* Will be set if houses are available */

    /* Calculate orbital phase (simplified - would need parent planet position) */
    position->orbital_phase = 0.0;  /* TODO: Calculate actual phase */
    position->is_eclipsed = 0;      /* TODO: Calculate eclipse status */

    return ASTRO_OK;
}

int astro_moons_calc_all_positions(const AstroDateTime *datetime,
                                   PlanetaryMoonPosition positions[],
                                   int max_moons,
                                   int *num_moons) {
    if (datetime == NULL || positions == NULL || num_moons == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    int count = 0;

    /* List of all 27 moons */
    PlanetaryMoonID all_moons[] = {
        /* Mars */
        MOON_PHOBOS, MOON_DEIMOS,
        /* Jupiter */
        MOON_IO, MOON_EUROPA, MOON_GANYMEDE, MOON_CALLISTO,
        /* Saturn */
        MOON_MIMAS, MOON_ENCELADUS, MOON_TETHYS, MOON_DIONE,
        MOON_RHEA, MOON_TITAN, MOON_HYPERION, MOON_IAPETUS,
        /* Uranus */
        MOON_ARIEL, MOON_UMBRIEL, MOON_TITANIA, MOON_OBERON, MOON_MIRANDA,
        /* Neptune */
        MOON_TRITON, MOON_NEREID, MOON_PROTEUS,
        /* Pluto */
        MOON_CHARON, MOON_NIX, MOON_HYDRA, MOON_KERBEROS, MOON_STYX
    };

    int total_moons = sizeof(all_moons) / sizeof(PlanetaryMoonID);

    for (int i = 0; i < total_moons && count < max_moons; i++) {
        int result = astro_moons_calc_position(all_moons[i], datetime, &positions[count]);
        if (result == ASTRO_OK) {
            count++;
        }
    }

    *num_moons = count;
    return ASTRO_OK;
}

int astro_moons_get_system_moons(AstroPlanet parent_planet,
                                 const AstroDateTime *datetime,
                                 PlanetaryMoonPosition positions[],
                                 int max_moons,
                                 int *num_moons) {
    if (datetime == NULL || positions == NULL || num_moons == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    int count = 0;

    /* Define moon ranges for each planet */
    PlanetaryMoonID start_id = 0, end_id = 0;

    switch (parent_planet) {
        case ASTRO_MARS:
            start_id = MOON_PHOBOS;
            end_id = MOON_DEIMOS;
            break;
        case ASTRO_JUPITER:
            start_id = MOON_IO;
            end_id = MOON_CALLISTO;
            break;
        case ASTRO_SATURN:
            start_id = MOON_MIMAS;
            end_id = MOON_IAPETUS;
            break;
        case ASTRO_URANUS:
            start_id = MOON_ARIEL;
            end_id = MOON_MIRANDA;
            break;
        case ASTRO_NEPTUNE:
            start_id = MOON_TRITON;
            end_id = MOON_PROTEUS;
            break;
        case ASTRO_PLUTO:
            start_id = MOON_CHARON;
            end_id = MOON_STYX;
            break;
        default:
            return ASTRO_ERROR_INVALID_PLANET;
    }

    /* Calculate positions for this system */
    for (PlanetaryMoonID id = start_id; id <= end_id && count < max_moons; id++) {
        const PlanetaryMoonData *data = astro_moons_get_data(id);
        if (data != NULL) {
            int result = astro_moons_calc_position(id, datetime, &positions[count]);
            if (result == ASTRO_OK) {
                count++;
            }
        }
    }

    *num_moons = count;
    return ASTRO_OK;
}

/* ========================================================================
 * Specialized System Functions
 * ======================================================================== */

int astro_moons_get_galilean_moons(const AstroDateTime *datetime,
                                   PlanetaryMoonPosition positions[4]) {
    if (datetime == NULL || positions == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    PlanetaryMoonID galilean[] = {MOON_IO, MOON_EUROPA, MOON_GANYMEDE, MOON_CALLISTO};

    for (int i = 0; i < 4; i++) {
        int result = astro_moons_calc_position(galilean[i], datetime, &positions[i]);
        if (result != ASTRO_OK) {
            return result;
        }
    }

    return ASTRO_OK;
}

int astro_moons_get_saturn_major_moons(const AstroDateTime *datetime,
                                       PlanetaryMoonPosition positions[],
                                       int max_moons,
                                       int *num_moons) {
    if (datetime == NULL || positions == NULL || num_moons == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    PlanetaryMoonID saturn_major[] = {
        MOON_TITAN, MOON_RHEA, MOON_IAPETUS,
        MOON_DIONE, MOON_TETHYS, MOON_ENCELADUS
    };

    int count = 0;
    for (int i = 0; i < 6 && count < max_moons; i++) {
        int result = astro_moons_calc_position(saturn_major[i], datetime, &positions[count]);
        if (result == ASTRO_OK) {
            count++;
        }
    }

    *num_moons = count;
    return ASTRO_OK;
}

int astro_moons_get_pluto_moons(const AstroDateTime *datetime,
                                PlanetaryMoonPosition positions[],
                                int max_moons,
                                int *num_moons) {
    if (datetime == NULL || positions == NULL || num_moons == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    PlanetaryMoonID pluto_moons[] = {
        MOON_CHARON, MOON_NIX, MOON_HYDRA, MOON_KERBEROS, MOON_STYX
    };

    int count = 0;
    for (int i = 0; i < 5 && count < max_moons; i++) {
        int result = astro_moons_calc_position(pluto_moons[i], datetime, &positions[count]);
        if (result == ASTRO_OK) {
            count++;
        }
    }

    *num_moons = count;
    return ASTRO_OK;
}

/* ========================================================================
 * Conjunction and Aspect Functions
 * ======================================================================== */

int astro_moons_find_conjunctions(const AstroChart *chart,
                                  const AstroDateTime *datetime,
                                  double orb,
                                  MoonPlanetConjunction conjunctions[],
                                  int max_conjunctions,
                                  int *num_conjunctions) {
    if (chart == NULL || datetime == NULL || conjunctions == NULL || num_conjunctions == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    int count = 0;

    /* Calculate all moon positions */
    PlanetaryMoonPosition moon_positions[27];
    int num_moons;
    int result = astro_moons_calc_all_positions(datetime, moon_positions, 27, &num_moons);
    if (result != ASTRO_OK) {
        return result;
    }

    /* Check each moon against each natal planet */
    for (int m = 0; m < num_moons && count < max_conjunctions; m++) {
        for (int p = 0; p < chart->num_planets && count < max_conjunctions; p++) {
            /* Calculate angular distance */
            double diff = fabs(moon_positions[m].position.longitude -
                              chart->planets[p].longitude);

            /* Normalize to 0-360 */
            while (diff > 360.0) diff -= 360.0;
            if (diff > 180.0) diff = 360.0 - diff;

            /* Check if within orb */
            if (diff <= orb) {
                conjunctions[count].moon_id = moon_positions[m].moon_id;
                conjunctions[count].moon_name = moon_positions[m].moon_name;
                conjunctions[count].planet = chart->planets[p].planet;
                conjunctions[count].planet_name = chart->planets[p].name;
                conjunctions[count].orb = diff;
                conjunctions[count].is_exact = (diff <= 1.0) ? 1 : 0;

                /* Create basic interpretation */
                static char interp_buffer[512];
                snprintf(interp_buffer, sizeof(interp_buffer),
                        "%s conjunct natal %s: %s meets %s energy",
                        moon_positions[m].moon_name,
                        chart->planets[p].name,
                        moon_positions[m].data->archetype,
                        chart->planets[p].name);
                conjunctions[count].interpretation = interp_buffer;

                count++;
            }
        }
    }

    *num_conjunctions = count;
    return ASTRO_OK;
}

int astro_moons_find_transits(const AstroChart *chart,
                              const AstroDateTime *datetime,
                              double orb,
                              MoonPlanetConjunction transits[],
                              int max_transits,
                              int *num_transits) {
    /* For now, transits are the same as conjunctions */
    return astro_moons_find_conjunctions(chart, datetime, orb, transits,
                                        max_transits, num_transits);
}

int astro_moons_find_moon_aspects(const AstroDateTime *datetime,
                                  AstroAspect aspects[],
                                  int max_aspects,
                                  int *num_aspects) {
    if (datetime == NULL || aspects == NULL || num_aspects == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    int count = 0;

    /* Calculate all moon positions */
    PlanetaryMoonPosition moon_positions[27];
    int num_moons;
    int result = astro_moons_calc_all_positions(datetime, moon_positions, 27, &num_moons);
    if (result != ASTRO_OK) {
        return result;
    }

    /* Check aspects between moons */
    for (int i = 0; i < num_moons && count < max_aspects; i++) {
        for (int j = i + 1; j < num_moons && count < max_aspects; j++) {
            double angle = fabs(moon_positions[i].position.longitude -
                               moon_positions[j].position.longitude);

            /* Normalize */
            while (angle > 360.0) angle -= 360.0;
            if (angle > 180.0) angle = 360.0 - angle;

            /* Check for major aspects (0, 60, 90, 120, 180) */
            double aspect_angles[] = {0, 60, 90, 120, 180};
            double orbs[] = {8, 6, 6, 6, 8};

            for (int a = 0; a < 5; a++) {
                double orb = fabs(angle - aspect_angles[a]);
                if (orb <= orbs[a]) {
                    aspects[count].planet1 = moon_positions[i].position.planet;
                    aspects[count].planet2 = moon_positions[j].position.planet;
                    aspects[count].aspect_type = a;  /* Simplified */
                    aspects[count].orb = orb;
                    aspects[count].is_applying = 0;  /* TODO: Calculate */
                    count++;
                    break;
                }
            }
        }
    }

    *num_aspects = count;
    return ASTRO_OK;
}

/* ========================================================================
 * Orbital Phase Functions
 * ======================================================================== */

int astro_moons_orbital_phase(PlanetaryMoonID moon_id,
                              const AstroDateTime *datetime,
                              double *phase,
                              const char **phase_name) {
    if (datetime == NULL || phase == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    /* Get moon and parent planet positions */
    PlanetaryMoonPosition moon_pos;
    int result = astro_moons_calc_position(moon_id, datetime, &moon_pos);
    if (result != ASTRO_OK) {
        return result;
    }

    AstroPlanet parent = get_parent_planet(moon_id);
    if (parent < 0) {
        return ASTRO_ERROR_INVALID_PLANET;
    }

    AstroPlanetPosition parent_pos;
    result = astro_core_calc_planet(parent, datetime, SEFLG_SWIEPH, &parent_pos);
    if (result != ASTRO_OK) {
        return result;
    }

    /* Calculate phase angle relative to parent */
    double phase_angle = moon_pos.position.longitude - parent_pos.longitude;
    while (phase_angle < 0) phase_angle += 360.0;
    while (phase_angle >= 360.0) phase_angle -= 360.0;

    *phase = phase_angle;

    /* Determine phase name */
    if (phase_name != NULL) {
        if (phase_angle < 45.0 || phase_angle >= 315.0) {
            *phase_name = "New";
        } else if (phase_angle >= 45.0 && phase_angle < 135.0) {
            *phase_name = "Waxing";
        } else if (phase_angle >= 135.0 && phase_angle < 225.0) {
            *phase_name = "Full";
        } else {
            *phase_name = "Waning";
        }
    }

    return ASTRO_OK;
}

/* ========================================================================
 * Search and Query Functions
 * ======================================================================== */

int astro_moons_get_by_name(const char *name) {
    if (name == NULL) {
        return -1;
    }

    for (int i = 0; i < 27; i++) {
        if (strcasecmp(moon_database[i].name, name) == 0) {
            return moon_database[i].id;
        }
    }

    return -1;
}

int astro_moons_get_for_parent(AstroPlanet parent_planet,
                               PlanetaryMoonID moon_ids[],
                               int max_moons,
                               int *num_moons) {
    if (moon_ids == NULL || num_moons == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    int count = 0;

    for (int i = 0; i < 27 && count < max_moons; i++) {
        if (moon_database[i].parent_planet == parent_planet) {
            moon_ids[count++] = moon_database[i].id;
        }
    }

    *num_moons = count;
    return ASTRO_OK;
}

int astro_moons_search_by_keyword(const char *keyword,
                                  PlanetaryMoonID moon_ids[],
                                  int max_moons,
                                  int *num_moons) {
    if (keyword == NULL || moon_ids == NULL || num_moons == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    int count = 0;

    for (int i = 0; i < 27 && count < max_moons; i++) {
        /* Search in keywords */
        for (int k = 0; k < 8 && moon_database[i].keywords[k]; k++) {
            if (strcasestr(moon_database[i].keywords[k], keyword) != NULL) {
                moon_ids[count++] = moon_database[i].id;
                break;
            }
        }

        /* Also search in mythology and archetype */
        if (count < max_moons) {
            if (strcasestr(moon_database[i].mythology, keyword) != NULL ||
                strcasestr(moon_database[i].archetype, keyword) != NULL) {
                /* Check if not already added */
                int already_added = 0;
                for (int j = 0; j < count; j++) {
                    if (moon_ids[j] == moon_database[i].id) {
                        already_added = 1;
                        break;
                    }
                }
                if (!already_added) {
                    moon_ids[count++] = moon_database[i].id;
                }
            }
        }
    }

    *num_moons = count;
    return ASTRO_OK;
}

/* ========================================================================
 * Interpretation Functions
 * ======================================================================== */

char* astro_moons_interpret_conjunction(PlanetaryMoonID moon_id,
                                        AstroPlanet planet,
                                        char *buffer,
                                        int buffer_size) {
    if (buffer == NULL || buffer_size <= 0) {
        return NULL;
    }

    const PlanetaryMoonData *data = astro_moons_get_data(moon_id);
    if (data == NULL) {
        snprintf(buffer, buffer_size, "Unknown moon");
        return buffer;
    }

    const char *planet_name = astro_core_get_planet_name(planet);

    snprintf(buffer, buffer_size,
            "%s (%s) conjunct %s:\n\n"
            "Archetype: %s\n\n"
            "This aspect brings the energy of %s to your natal %s. "
            "%s\n\n"
            "Shadow: %s\n"
            "Gift: %s",
            data->name,
            data->parent_name,
            planet_name,
            data->archetype,
            data->name,
            planet_name,
            data->psychological_theme,
            data->shadow_expression,
            data->gift_expression);

    return buffer;
}

/* ========================================================================
 * Output Functions
 * ======================================================================== */

void astro_moons_print_position(const PlanetaryMoonPosition *position, FILE *fp) {
    if (position == NULL || fp == NULL) {
        return;
    }

    fprintf(fp, "%s: %.2f° %s (%.2f°) - %s\n",
            position->moon_name,
            position->position.sign_position,
            astro_core_get_sign_name(position->position.sign),
            position->position.longitude,
            position->data->archetype);
}

int astro_moons_print_report(const AstroDateTime *datetime, FILE *fp) {
    if (datetime == NULL || fp == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    fprintf(fp, "\n");
    fprintf(fp, "═══════════════════════════════════════════════════════════════\n");
    fprintf(fp, "  PLANETARY MOONS REPORT\n");
    fprintf(fp, "═══════════════════════════════════════════════════════════════\n\n");

    PlanetaryMoonPosition positions[27];
    int num_moons;

    int result = astro_moons_calc_all_positions(datetime, positions, 27, &num_moons);
    if (result != ASTRO_OK) {
        return result;
    }

    /* Group by planetary system */
    const char *systems[] = {"Mars", "Jupiter", "Saturn", "Uranus", "Neptune", "Pluto"};
    AstroPlanet parents[] = {ASTRO_MARS, ASTRO_JUPITER, ASTRO_SATURN,
                            ASTRO_URANUS, ASTRO_NEPTUNE, ASTRO_PLUTO};

    for (int s = 0; s < 6; s++) {
        fprintf(fp, "\n%s System:\n", systems[s]);
        fprintf(fp, "───────────────────────────────────────────────────────────────\n");

        for (int i = 0; i < num_moons; i++) {
            if (positions[i].data->parent_planet == parents[s]) {
                astro_moons_print_position(&positions[i], fp);
            }
        }
    }

    fprintf(fp, "\n═══════════════════════════════════════════════════════════════\n\n");

    return ASTRO_OK;
}

int astro_moons_print_system(AstroPlanet parent_planet,
                             const AstroDateTime *datetime,
                             FILE *fp) {
    if (datetime == NULL || fp == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    PlanetaryMoonPosition positions[10];
    int num_moons;

    int result = astro_moons_get_system_moons(parent_planet, datetime,
                                             positions, 10, &num_moons);
    if (result != ASTRO_OK) {
        return result;
    }

    fprintf(fp, "\n%s System Moons:\n", astro_core_get_planet_name(parent_planet));
    fprintf(fp, "───────────────────────────────────────────────────────────────\n");

    for (int i = 0; i < num_moons; i++) {
        astro_moons_print_position(&positions[i], fp);
    }

    fprintf(fp, "\n");

    return ASTRO_OK;
}

int astro_moons_print_conjunctions(const AstroChart *chart,
                                   const AstroDateTime *datetime,
                                   double orb,
                                   FILE *fp) {
    if (chart == NULL || datetime == NULL || fp == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    MoonPlanetConjunction conjunctions[100];
    int num_conjunctions;

    int result = astro_moons_find_conjunctions(chart, datetime, orb,
                                              conjunctions, 100, &num_conjunctions);
    if (result != ASTRO_OK) {
        return result;
    }

    fprintf(fp, "\n");
    fprintf(fp, "═══════════════════════════════════════════════════════════════\n");
    fprintf(fp, "  PLANETARY MOON CONJUNCTIONS (orb: %.1f°)\n", orb);
    fprintf(fp, "═══════════════════════════════════════════════════════════════\n\n");

    if (num_conjunctions == 0) {
        fprintf(fp, "No conjunctions found within orb.\n\n");
        return ASTRO_OK;
    }

    for (int i = 0; i < num_conjunctions; i++) {
        fprintf(fp, "%s %s conjunct natal %s (orb: %.2f°)%s\n",
                conjunctions[i].is_exact ? "★" : " ",
                conjunctions[i].moon_name,
                conjunctions[i].planet_name,
                conjunctions[i].orb,
                conjunctions[i].is_exact ? " [EXACT]" : "");
    }

    fprintf(fp, "\n═══════════════════════════════════════════════════════════════\n\n");

    return ASTRO_OK;
}

char* astro_moons_format_position(const PlanetaryMoonPosition *position,
                                  char *buffer,
                                  int buffer_size) {
    if (position == NULL || buffer == NULL || buffer_size <= 0) {
        return NULL;
    }

    snprintf(buffer, buffer_size,
            "%s: %02.0f°%s%02d' (%s) - %s",
            position->moon_name,
            floor(position->position.sign_position),
            astro_core_get_sign_symbol(position->position.sign),
            (int)((position->position.sign_position -
                   floor(position->position.sign_position)) * 60),
            position->data->parent_name,
            position->data->archetype);

    return buffer;
}
