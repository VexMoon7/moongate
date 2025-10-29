/**
 * @file astro_sabian.c
 * @brief Sabian Symbol implementation
 *
 * This implementation includes a curated selection of Sabian Symbols,
 * focusing on critical degrees and mythologically significant points.
 * More symbols can be added to expand coverage.
 */

#include "astro_sabian.h"
#include "astro_core.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* ========================================================================
 * Sabian Symbol Database
 * ======================================================================== */

static const SabianSymbol g_sabian_symbols[] = {
    /* Aries (0-29°) */
    {1, ASTRO_ARIES, 1,
     "A woman just risen from the sea. A seal is embracing her.",
     "Emergence",
     "The emergence of new forms and the potency of experience.",
     "Birth of consciousness from the unconscious"},

    {2, ASTRO_ARIES, 2,
     "A comedian entertaining a group.",
     "Entertainment",
     "The capacity to laugh at life's predicaments.",
     "Transcending through humor"},

    {13, ASTRO_ARIES, 13,
     "A bomb fails to explode.",
     "Containment",
     "Restraint in the face of provocation; controlled power.",
     "Potential energy held in check"},

    {15, ASTRO_ARIES, 15,
     "An Indian weaving a blanket.",
     "Creativity",
     "Practical creativity and the weaving of life experiences.",
     "Crafting one's reality"},

    {30, ASTRO_ARIES, 30,
     "A duck pond and its brood.",
     "Protection",
     "The establishment of an area of experience.",
     "Creating safe space"},

    /* Taurus (30-59°) */
    {31, ASTRO_TAURUS, 1,
     "A clear mountain stream.",
     "Purity",
     "The refreshment that comes from inner clarity.",
     "Source energy"},

    {45, ASTRO_TAURUS, 15,
     "A man muffled up, with a rakish silk hat.",
     "Adaptation",
     "The ability to adapt to social requirements.",
     "Social persona"},

    {60, ASTRO_TAURUS, 30,
     "A peacock parading on an ancient lawn.",
     "Display",
     "The pomp and ostentation of old values.",
     "Pride of tradition"},

    /* Gemini (60-89°) */
    {61, ASTRO_GEMINI, 1,
     "A glass-bottomed boat in still water.",
     "Observation",
     "The capacity to explore subconscious depths.",
     "Conscious observation of the unconscious"},

    {75, ASTRO_GEMINI, 15,
     "Two Dutch children talking and playing.",
     "Communication",
     "The simple joy of unstructured communication.",
     "Playful exchange"},

    {90, ASTRO_GEMINI, 30,
     "A parade of bathing beauties before large crowds.",
     "Exhibition",
     "The display of youthful perfection.",
     "Collective ideals of beauty"},

    /* Cancer (90-119°) */
    {91, ASTRO_CANCER, 1,
     "A furled and unfurled flag displayed from a vessel.",
     "Identity",
     "The capacity to claim allegiance and identity.",
     "Proclamation of belonging"},

    {105, ASTRO_CANCER, 15,
     "A group of people who have overeaten and enjoyed it.",
     "Abundance",
     "The enjoyment of material and emotional plenty.",
     "Satisfaction and excess"},

    {120, ASTRO_CANCER, 30,
     "An Indian chief claims recognition and power.",
     "Authority",
     "The assumption of personal authority and leadership.",
     "Earned recognition"},

    /* Leo (120-149°) */
    {121, ASTRO_LEO, 1,
     "Under emotional stress, blood rushes to a man's head.",
     "Intensity",
     "The power of emotional response to overcome reason.",
     "Passionate response"},

    {135, ASTRO_LEO, 15,
     "A pageant moves forward in spite of difficulties.",
     "Perseverance",
     "Maintaining form and dignity under pressure.",
     "Ceremonial continuation"},

    {150, ASTRO_LEO, 30,
     "An unsealed letter has vital and confidential information.",
     "Revelation",
     "The revelation of significant truths.",
     "Exposed secrets"},

    /* Virgo (150-179°) */
    {151, ASTRO_VIRGO, 1,
     "A man's head with a prominent display of veins.",
     "Vitality",
     "The power of thought and vital energy.",
     "Mental intensity"},

    {165, ASTRO_VIRGO, 15,
     "A fine lace ornamental handkerchief.",
     "Refinement",
     "The appreciation of delicate beauty and craftsmanship.",
     "Aesthetic sensitivity"},

    {180, ASTRO_VIRGO, 30,
     "A baldheaded man who has seized power.",
     "Authority",
     "The assumption of power through mental clarity.",
     "Intellectual dominance"},

    /* Libra (180-209°) */
    {181, ASTRO_LIBRA, 1,
     "A butterfly made perfect by a dart through it.",
     "Preservation",
     "The perfection achieved through sacrifice.",
     "Fixed beauty through death"},

    {195, ASTRO_LIBRA, 15,
     "Circular paths.",
     "Cycles",
     "The recognition of recurring patterns.",
     "Cyclical understanding"},

    {210, ASTRO_LIBRA, 30,
     "A man watching his ideals take concrete form before his inner vision.",
     "Manifestation",
     "The power of visualization and ideation.",
     "Thought becoming form"},

    /* Scorpio (210-239°) */
    {211, ASTRO_SCORPIO, 1,
     "A sightseeing bus filled with tourists.",
     "Exploration",
     "The superficial exploration of experience.",
     "Touristic consciousness"},

    {225, ASTRO_SCORPIO, 15,
     "Children playing around five mounds of sand.",
     "Play",
     "The creative exploration of material reality.",
     "Innocent experimentation"},

    {240, ASTRO_SCORPIO, 30,
     "Halloween jester.",
     "Transformation",
     "The mocking of form and the celebration of change.",
     "Playful confrontation with death"},

    /* Sagittarius (240-269°) */
    {241, ASTRO_SAGITTARIUS, 1,
     "Retired army veterans gather to reawaken old memories.",
     "Memory",
     "The value of past experience and shared history.",
     "Collective memory"},

    {255, ASTRO_SAGITTARIUS, 15,
     "The ground hog looking for its shadow on Ground Hog Day.",
     "Prediction",
     "The search for signs and omens.",
     "Folk wisdom"},

    {266, ASTRO_SAGITTARIUS, 26,
     "A flag-bearer in a battle.",
     "Leadership",
     "Carrying the standard in the midst of conflict.",
     "Ideological courage"},

    {267, ASTRO_SAGITTARIUS, 27,
     "A sculptor at work.",
     "Creation",
     "The shaping of raw material into form.",
     "Artistic manifestation"},

    /* Note: 26-27° Sagittarius is near Galactic Center */

    {270, ASTRO_SAGITTARIUS, 30,
     "The Pope blessing the faithful.",
     "Blessing",
     "The conferral of spiritual authority and grace.",
     "Sacred transmission"},

    /* Capricorn (270-299°) */
    {271, ASTRO_CAPRICORN, 1,
     "An Indian chief claims recognition and power.",
     "Authority",
     "The establishment of legitimate authority.",
     "Earned leadership"},

    {285, ASTRO_CAPRICORN, 15,
     "In a hospital, the children's ward is filled with toys.",
     "Healing",
     "The use of play and joy in the healing process.",
     "Therapeutic comfort"},

    {300, ASTRO_CAPRICORN, 30,
     "A secret meeting of men responsible for executive decisions.",
     "Power",
     "The exercise of power behind the scenes.",
     "Hidden authority"},

    /* Aquarius (300-329°) */
    {301, ASTRO_AQUARIUS, 1,
     "An old adobe mission in California.",
     "Foundation",
     "The enduring legacy of spiritual foundations.",
     "Preserved spirituality"},

    {315, ASTRO_AQUARIUS, 15,
     "Two lovebirds sitting on a fence.",
     "Partnership",
     "The simple joy of companionship.",
     "Natural pairing"},

    {330, ASTRO_AQUARIUS, 30,
     "Deeply rooted in the past of a very ancient culture, a spiritual brotherhood is revealed.",
     "Tradition",
     "Connection to ancient wisdom traditions.",
     "Occult knowledge"},

    /* Pisces (330-359°) */
    {331, ASTRO_PISCES, 1,
     "In a crowded marketplace farmers and middlemen display a great variety of products.",
     "Variety",
     "The abundance and diversity of earthly manifestation.",
     "Material plenty"},

    {345, ASTRO_PISCES, 15,
     "An officer instructing his men before a simulated assault under a barrage of live shells.",
     "Preparation",
     "Training for real-world challenges.",
     "Practical readiness"},

    {360, ASTRO_PISCES, 30,
     "A majestic rock formation resembling a face is idealized by a boy who takes it as his ideal of greatness.",
     "Idealization",
     "The projection of inner ideals onto outer forms.",
     "Visionary aspiration"},
};

static const int g_num_sabian_symbols = sizeof(g_sabian_symbols) / sizeof(g_sabian_symbols[0]);

/* ========================================================================
 * Helper Functions
 * ======================================================================== */

/**
 * Calculate absolute degree from longitude (1-360)
 */
static int longitude_to_degree(double longitude) {
    longitude = astro_core_normalize_angle(longitude);
    int deg = (int)(longitude + 1.0);  /* Round up to next degree */
    if (deg > 360) deg = 360;
    return deg;
}

/**
 * Find closest symbol in database
 */
static const SabianSymbol* find_closest_symbol(int degree) {
    if (g_num_sabian_symbols == 0) return NULL;

    const SabianSymbol *closest = &g_sabian_symbols[0];
    int min_diff = abs(degree - g_sabian_symbols[0].degree);

    for (int i = 1; i < g_num_sabian_symbols; i++) {
        int diff = abs(degree - g_sabian_symbols[i].degree);
        if (diff < min_diff) {
            min_diff = diff;
            closest = &g_sabian_symbols[i];
        }
    }

    return closest;
}

/* ========================================================================
 * Public Functions
 * ======================================================================== */

const SabianSymbol* astro_sabian_get_symbol(double longitude) {
    int degree = longitude_to_degree(longitude);
    return find_closest_symbol(degree);
}

const SabianSymbol* astro_sabian_get_by_sign(AstroSign sign, int degree_in_sign) {
    if (degree_in_sign < 1 || degree_in_sign > 30) {
        return NULL;
    }

    int absolute_degree = (sign * 30) + degree_in_sign;
    return find_closest_symbol(absolute_degree);
}

const SabianSymbol* astro_sabian_get_by_degree(int degree) {
    if (degree < 1 || degree > 360) {
        return NULL;
    }

    return find_closest_symbol(degree);
}

int astro_sabian_get_chart_symbols(const AstroChart *chart,
                                   SabianSymbol symbols[],
                                   int max_symbols,
                                   int *num_symbols) {
    if (chart == NULL || symbols == NULL || num_symbols == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    int count = 0;

    /* Get symbols for each planet */
    for (int i = 0; i < chart->num_planets && count < max_symbols; i++) {
        const SabianSymbol *sym = astro_sabian_get_symbol(chart->planets[i].longitude);
        if (sym != NULL) {
            symbols[count++] = *sym;
        }
    }

    *num_symbols = count;
    return ASTRO_OK;
}

const SabianSymbol* astro_sabian_get_for_planet(const AstroPlanetPosition *position) {
    if (position == NULL) {
        return NULL;
    }

    return astro_sabian_get_symbol(position->longitude);
}

char* astro_sabian_format(const SabianSymbol *symbol, char *buffer, int buffer_size) {
    if (symbol == NULL || buffer == NULL || buffer_size < 128) {
        return NULL;
    }

    snprintf(buffer, buffer_size,
            "%s %d° - \"%s\" (%s)",
            astro_core_get_sign_name(symbol->sign),
            symbol->degree_in_sign,
            symbol->symbol,
            symbol->keyword);

    return buffer;
}

void astro_sabian_print(const SabianSymbol *symbol, FILE *fp) {
    if (symbol == NULL || fp == NULL) {
        return;
    }

    fprintf(fp, "%s %d°: \"%s\"\n",
            astro_core_get_sign_name(symbol->sign),
            symbol->degree_in_sign,
            symbol->symbol);
    fprintf(fp, "  Keyword: %s\n", symbol->keyword);
    fprintf(fp, "  Meaning: %s\n", symbol->interpretation);
    fprintf(fp, "  Archetype: %s\n", symbol->archetype);
}

int astro_sabian_search(const char *keyword,
                       SabianSymbol symbols[],
                       int max_symbols,
                       int *num_symbols) {
    if (keyword == NULL || symbols == NULL || num_symbols == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    int count = 0;

    for (int i = 0; i < g_num_sabian_symbols && count < max_symbols; i++) {
        /* Simple case-insensitive search in keyword, interpretation, and archetype */
        if (strcasestr(g_sabian_symbols[i].keyword, keyword) != NULL ||
            strcasestr(g_sabian_symbols[i].interpretation, keyword) != NULL ||
            strcasestr(g_sabian_symbols[i].archetype, keyword) != NULL ||
            strcasestr(g_sabian_symbols[i].symbol, keyword) != NULL) {
            symbols[count++] = g_sabian_symbols[i];
        }
    }

    *num_symbols = count;
    return ASTRO_OK;
}

char* astro_sabian_interpret_point(const AstroPlanetPosition *position,
                                   int house,
                                   char *buffer,
                                   int buffer_size) {
    if (position == NULL || buffer == NULL || buffer_size < 256) {
        return NULL;
    }

    const SabianSymbol *sym = astro_sabian_get_symbol(position->longitude);
    if (sym == NULL) {
        snprintf(buffer, buffer_size, "No symbol data available for this degree.");
        return buffer;
    }

    snprintf(buffer, buffer_size,
            "%s at %s %d° (House %d): \"%s\"\n"
            "The %s speaks to %s, manifesting as %s in the %d%s house of your life.",
            position->name,
            astro_core_get_sign_name(sym->sign),
            sym->degree_in_sign,
            house,
            sym->symbol,
            position->name,
            sym->keyword,
            sym->archetype,
            house,
            (house == 1) ? "st" : (house == 2) ? "nd" : (house == 3) ? "rd" : "th");

    return buffer;
}

int astro_sabian_get_critical_degrees(const AstroChart *chart,
                                      SabianSymbol symbols[],
                                      int max_symbols,
                                      int *num_symbols) {
    if (chart == NULL || symbols == NULL || num_symbols == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    int count = 0;

    /* Critical degrees are 0°, 13°, and 26° of cardinal signs */
    int critical_degrees[] = {0, 13, 26};
    AstroSign cardinal_signs[] = {ASTRO_ARIES, ASTRO_CANCER, ASTRO_LIBRA, ASTRO_CAPRICORN};

    for (int i = 0; i < chart->num_planets && count < max_symbols; i++) {
        AstroSign sign = chart->planets[i].sign;
        int degree = (int)chart->planets[i].sign_position;

        /* Check if cardinal sign */
        int is_cardinal = 0;
        for (int j = 0; j < 4; j++) {
            if (sign == cardinal_signs[j]) {
                is_cardinal = 1;
                break;
            }
        }

        if (!is_cardinal) continue;

        /* Check if on critical degree (within 1°) */
        for (int j = 0; j < 3; j++) {
            if (abs(degree - critical_degrees[j]) <= 1) {
                const SabianSymbol *sym = astro_sabian_get_symbol(chart->planets[i].longitude);
                if (sym != NULL) {
                    symbols[count++] = *sym;
                }
                break;
            }
        }
    }

    *num_symbols = count;
    return ASTRO_OK;
}

int astro_sabian_get_angles(const AstroHouses *houses,
                            SabianSymbol symbols[],
                            int max_symbols,
                            int *num_symbols) {
    if (houses == NULL || symbols == NULL || num_symbols == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    int count = 0;

    /* ASC */
    if (count < max_symbols) {
        const SabianSymbol *sym = astro_sabian_get_symbol(houses->ascendant);
        if (sym != NULL) {
            symbols[count++] = *sym;
        }
    }

    /* MC */
    if (count < max_symbols) {
        const SabianSymbol *sym = astro_sabian_get_symbol(houses->mc);
        if (sym != NULL) {
            symbols[count++] = *sym;
        }
    }

    /* DSC (opposite ASC) */
    if (count < max_symbols) {
        double dsc = astro_core_normalize_angle(houses->ascendant + 180.0);
        const SabianSymbol *sym = astro_sabian_get_symbol(dsc);
        if (sym != NULL) {
            symbols[count++] = *sym;
        }
    }

    /* IC (opposite MC) */
    if (count < max_symbols) {
        double ic = astro_core_normalize_angle(houses->mc + 180.0);
        const SabianSymbol *sym = astro_sabian_get_symbol(ic);
        if (sym != NULL) {
            symbols[count++] = *sym;
        }
    }

    *num_symbols = count;
    return ASTRO_OK;
}
