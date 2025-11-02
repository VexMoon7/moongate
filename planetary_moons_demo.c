/**
 * @file planetary_moons_demo.c
 * @brief Demo program showcasing planetary moons with mythology
 *
 * This program demonstrates the unique planetary moons feature,
 * showing their current positions and mythological associations.
 */

#include "astro_engine.h"
#include "astro_planetary_moons.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ========================================================================
 * Mythological Data for All 27 Planetary Moons
 * ======================================================================== */

static const PlanetaryMoonData moon_database[] = {
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

#define NUM_MOONS (sizeof(moon_database) / sizeof(PlanetaryMoonData))

/* ========================================================================
 * Implementation of lookup functions
 * ======================================================================== */

const PlanetaryMoonData* astro_moons_get_data(PlanetaryMoonID moon_id) {
    for (int i = 0; i < NUM_MOONS; i++) {
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
 * Demo Display Functions
 * ======================================================================== */

void print_moon_mythology(PlanetaryMoonID moon_id) {
    const PlanetaryMoonData *data = astro_moons_get_data(moon_id);
    if (!data) return;

    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  🌙 %s (%s)  \n", data->name, data->parent_name);
    printf("╚════════════════════════════════════════════════════════════════╝\n\n");

    printf("🎭 Archetype: %s\n", data->archetype);
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");

    printf("📖 Mythology:\n%s\n\n", data->mythology);

    printf("🔮 Astrological Meaning:\n%s\n\n", data->astrological_meaning);

    printf("🧠 Psychological Theme:\n%s\n\n", data->psychological_theme);

    printf("🌑 Shadow Expression:\n%s\n\n", data->shadow_expression);

    printf("✨ Gift Expression:\n%s\n\n", data->gift_expression);

    printf("🏷️  Keywords: ");
    for (int i = 0; i < 8 && data->keywords[i]; i++) {
        printf("%s%s", data->keywords[i], i < 7 && data->keywords[i+1] ? ", " : "");
    }
    printf("\n\n");

    printf("🔭 Discovered: %d by %s\n", data->discovery_year, data->discoverer);
    printf("⚡ Orbital Period: %.2f days\n", data->orbital_period_days);
    printf("\n");
}

void print_system_overview(const char *system_name, int start_id, int end_id) {
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  %s System Moons  \n", system_name);
    printf("╚════════════════════════════════════════════════════════════════╝\n");

    for (int id = start_id; id <= end_id; id++) {
        const PlanetaryMoonData *data = astro_moons_get_data(id);
        if (data) {
            printf("\n  %d. %-12s - %s\n", id, data->name, data->archetype);
            printf("     %s\n", data->keywords[0]);
        }
    }
    printf("\n");
}

/* ========================================================================
 * Main Program
 * ======================================================================== */

int main(int argc, char *argv[]) {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════╗\n");
    printf("║                                                                ║\n");
    printf("║        🌙 PLANETARY MOONS MYTHOLOGY 🌙                        ║\n");
    printf("║                                                                ║\n");
    printf("║        27 Moons Across 6 Planetary Systems                     ║\n");
    printf("║        With Full Mythological Associations                     ║\n");
    printf("║                                                                ║\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n");

    printf("\n🪐 Overview of Planetary Moon Systems\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");

    printf("\n♂️  Mars (2 moons): Fear & Terror\n");
    printf("   Phobos, Deimos\n");

    printf("\n♃ Jupiter (4 moons): Divine Drama & Transformation\n");
    printf("   Io, Europa, Ganymede, Callisto (Galilean Moons)\n");

    printf("\n♄ Saturn (8 moons): Structure & Boundaries\n");
    printf("   Mimas, Enceladus, Tethys, Dione, Rhea, Titan, Hyperion, Iapetus\n");

    printf("\n♅ Uranus (5 moons): Shakespearean Archetypes\n");
    printf("   Ariel, Umbriel, Titania, Oberon, Miranda\n");

    printf("\n♆ Neptune (3 moons): Oceanic Depths\n");
    printf("   Triton, Nereid, Proteus\n");

    printf("\n♇ Pluto (5 moons): Underworld Mysteries\n");
    printf("   Charon, Nix, Hydra, Kerberos, Styx\n");

    // Check for specific moon request
    if (argc > 1) {
        printf("\n\n🔍 Detailed Mythology\n");
        printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");

        if (strcmp(argv[1], "--all") == 0) {
            // Show all moons
            for (int i = 0; i < NUM_MOONS; i++) {
                print_moon_mythology(moon_database[i].id);
            }
        } else if (strcmp(argv[1], "--jupiter") == 0) {
            print_moon_mythology(MOON_IO);
            print_moon_mythology(MOON_EUROPA);
            print_moon_mythology(MOON_GANYMEDE);
            print_moon_mythology(MOON_CALLISTO);
        } else if (strcmp(argv[1], "--saturn") == 0) {
            print_moon_mythology(MOON_TITAN);
            print_moon_mythology(MOON_RHEA);
            print_moon_mythology(MOON_IAPETUS);
            print_moon_mythology(MOON_DIONE);
            print_moon_mythology(MOON_TETHYS);
            print_moon_mythology(MOON_ENCELADUS);
        } else if (strcmp(argv[1], "--pluto") == 0) {
            print_moon_mythology(MOON_CHARON);
            print_moon_mythology(MOON_NIX);
            print_moon_mythology(MOON_HYDRA);
            print_moon_mythology(MOON_KERBEROS);
            print_moon_mythology(MOON_STYX);
        } else {
            printf("\nUsage:\n");
            printf("  %s                Show overview\n", argv[0]);
            printf("  %s --all          Show all moons in detail\n", argv[0]);
            printf("  %s --jupiter      Show Jupiter's Galilean moons\n", argv[0]);
            printf("  %s --saturn       Show Saturn's major moons\n", argv[0]);
            printf("  %s --pluto        Show Pluto's underworld moons\n", argv[0]);
        }
    } else {
        // Show a few highlighted moons
        printf("\n\n🌟 Highlighted Moons (use --all to see all)\n");
        printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");

        print_moon_mythology(MOON_IO);
        print_moon_mythology(MOON_TITAN);
        print_moon_mythology(MOON_CHARON);
    }

    printf("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("Generated by Planetary Moons Module v1.0.0\n");
    printf("Built on Swiss Ephemeris\n");
    printf("A unique feature for archetypal astrology\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");

    return 0;
}
