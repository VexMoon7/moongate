/**
 * @file astro_aspects.c
 * @brief Aspect calculation and analysis implementation
 */

#include "astro_aspects.h"
#include "astro_core.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* ========================================================================
 * Module-level aspect configuration
 * ======================================================================== */

static AstroAspectConfig g_aspect_configs[] = {
    {ASTRO_ASPECT_CONJUNCTION, 8.0, 3.0, 1},
    {ASTRO_ASPECT_OPPOSITION, 8.0, 3.0, 1},
    {ASTRO_ASPECT_TRINE, 8.0, 3.0, 1},
    {ASTRO_ASPECT_SQUARE, 8.0, 3.0, 1},
    {ASTRO_ASPECT_SEXTILE, 6.0, 2.0, 1},
    {ASTRO_ASPECT_QUINCUNX, 3.0, 1.0, 0},
    {ASTRO_ASPECT_SEMISEXTILE, 3.0, 1.0, 0},
    {ASTRO_ASPECT_SEMISQUARE, 3.0, 1.0, 0},
    {ASTRO_ASPECT_SESQUIQUADRATE, 3.0, 1.0, 0},
    {ASTRO_ASPECT_QUINTILE, 2.0, 0.5, 0},
    {ASTRO_ASPECT_BIQUINTILE, 2.0, 0.5, 0}
};

static const int g_num_aspect_configs = sizeof(g_aspect_configs) / sizeof(g_aspect_configs[0]);

/* ========================================================================
 * Helper Functions
 * ======================================================================== */

static AstroAspectConfig* get_aspect_config(AstroAspectType aspect_type) {
    for (int i = 0; i < g_num_aspect_configs; i++) {
        if (g_aspect_configs[i].aspect_type == aspect_type) {
            return &g_aspect_configs[i];
        }
    }
    return NULL;
}

/* ========================================================================
 * Aspect Configuration
 * ======================================================================== */

void astro_aspects_set_orb(AstroAspectType aspect_type, double orb) {
    AstroAspectConfig *config = get_aspect_config(aspect_type);
    if (config != NULL) {
        config->default_orb = orb;
    }
}

double astro_aspects_get_orb(AstroAspectType aspect_type) {
    AstroAspectConfig *config = get_aspect_config(aspect_type);
    if (config != NULL) {
        return config->default_orb;
    }
    return 0.0;
}

void astro_aspects_reset_orbs(void) {
    /* Reset to default values - already initialized */
    g_aspect_configs[0] = (AstroAspectConfig){ASTRO_ASPECT_CONJUNCTION, 8.0, 3.0, 1};
    g_aspect_configs[1] = (AstroAspectConfig){ASTRO_ASPECT_OPPOSITION, 8.0, 3.0, 1};
    g_aspect_configs[2] = (AstroAspectConfig){ASTRO_ASPECT_TRINE, 8.0, 3.0, 1};
    g_aspect_configs[3] = (AstroAspectConfig){ASTRO_ASPECT_SQUARE, 8.0, 3.0, 1};
    g_aspect_configs[4] = (AstroAspectConfig){ASTRO_ASPECT_SEXTILE, 6.0, 2.0, 1};
    g_aspect_configs[5] = (AstroAspectConfig){ASTRO_ASPECT_QUINCUNX, 3.0, 1.0, 0};
    g_aspect_configs[6] = (AstroAspectConfig){ASTRO_ASPECT_SEMISEXTILE, 3.0, 1.0, 0};
    g_aspect_configs[7] = (AstroAspectConfig){ASTRO_ASPECT_SEMISQUARE, 3.0, 1.0, 0};
    g_aspect_configs[8] = (AstroAspectConfig){ASTRO_ASPECT_SESQUIQUADRATE, 3.0, 1.0, 0};
    g_aspect_configs[9] = (AstroAspectConfig){ASTRO_ASPECT_QUINTILE, 2.0, 0.5, 0};
    g_aspect_configs[10] = (AstroAspectConfig){ASTRO_ASPECT_BIQUINTILE, 2.0, 0.5, 0};
}

/* ========================================================================
 * Aspect Calculations
 * ======================================================================== */

int astro_aspects_check_aspect(double lon1, double lon2,
                               AstroAspectType *aspect_type,
                               double *orb) {
    double angular_dist = astro_core_angular_distance(lon1, lon2);

    /* Check each aspect type */
    for (int i = 0; i < g_num_aspect_configs; i++) {
        AstroAspectConfig *config = &g_aspect_configs[i];
        double aspect_angle = (double)config->aspect_type;
        double diff = fabs(angular_dist - aspect_angle);

        if (diff <= config->default_orb) {
            if (aspect_type != NULL) {
                *aspect_type = config->aspect_type;
            }
            if (orb != NULL) {
                *orb = diff;
            }
            return 1;
        }
    }

    return 0;
}

int astro_aspects_calc_aspect(const AstroPlanetPosition *pos1,
                              const AstroPlanetPosition *pos2,
                              AstroAspect *aspect) {
    if (pos1 == NULL || pos2 == NULL || aspect == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    /* Don't calculate aspect of a planet with itself */
    if (pos1->planet == pos2->planet) {
        return ASTRO_ERROR_INVALID_PLANET;
    }

    AstroAspectType aspect_type;
    double orb;

    if (astro_aspects_check_aspect(pos1->longitude, pos2->longitude,
                                   &aspect_type, &orb)) {
        aspect->planet1 = pos1->planet;
        aspect->planet2 = pos2->planet;
        aspect->aspect_type = aspect_type;
        aspect->orb = astro_core_angular_distance(pos1->longitude, pos2->longitude);
        aspect->difference = orb;

        /* Determine if applying or separating */
        aspect->is_applying = astro_aspects_is_applying(pos1, pos2, aspect);

        /* Check if exact (within tight orb) */
        AstroAspectConfig *config = get_aspect_config(aspect_type);
        aspect->is_exact = (orb <= config->tight_orb) ? 1 : 0;

        /* Format name */
        snprintf(aspect->name, ASTRO_MAX_NAME_LEN, "%s %s %s",
                pos1->name, astro_aspects_get_name(aspect_type), pos2->name);

        return ASTRO_OK;
    }

    return ASTRO_ERROR_CALCULATION;
}

int astro_aspects_calc_all(const AstroPlanetPosition positions[],
                           int num_planets,
                           AstroAspect aspects[],
                           int max_aspects,
                           int *num_aspects) {
    if (positions == NULL || aspects == NULL || num_aspects == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    int count = 0;

    /* Check all planet pairs */
    for (int i = 0; i < num_planets && count < max_aspects; i++) {
        for (int j = i + 1; j < num_planets && count < max_aspects; j++) {
            AstroAspect aspect;
            if (astro_aspects_calc_aspect(&positions[i], &positions[j], &aspect) == ASTRO_OK) {
                aspects[count++] = aspect;
            }
        }
    }

    *num_aspects = count;
    return ASTRO_OK;
}

int astro_aspects_calc_between_charts(const AstroPlanetPosition natal_positions[],
                                      int num_natal,
                                      const AstroPlanetPosition transit_positions[],
                                      int num_transit,
                                      AstroAspect aspects[],
                                      int max_aspects,
                                      int *num_aspects) {
    if (natal_positions == NULL || transit_positions == NULL ||
        aspects == NULL || num_aspects == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    int count = 0;

    /* Check all combinations */
    for (int i = 0; i < num_transit && count < max_aspects; i++) {
        for (int j = 0; j < num_natal && count < max_aspects; j++) {
            AstroAspect aspect;
            if (astro_aspects_calc_aspect(&transit_positions[i], &natal_positions[j],
                                         &aspect) == ASTRO_OK) {
                aspects[count++] = aspect;
            }
        }
    }

    *num_aspects = count;
    return ASTRO_OK;
}

int astro_aspects_is_applying(const AstroPlanetPosition *pos1,
                              const AstroPlanetPosition *pos2,
                              const AstroAspect *aspect) {
    if (pos1 == NULL || pos2 == NULL || aspect == NULL) {
        return -1;
    }

    /* Calculate if the faster planet is moving toward the aspect */
    double speed_diff = pos1->longitude_speed - pos2->longitude_speed;

    /* If speeds are nearly equal, aspect is stationary */
    if (fabs(speed_diff) < 0.01) {
        return -1;
    }

    /* Determine direction based on relative motion */
    double target_angle = (double)aspect->aspect_type;
    double current_angle = astro_core_angular_distance(pos1->longitude, pos2->longitude);

    /* If faster planet is moving toward exact aspect, it's applying */
    if (speed_diff > 0) {
        return (current_angle < target_angle) ? 1 : 0;
    } else {
        return (current_angle > target_angle) ? 1 : 0;
    }
}

/* ========================================================================
 * Aspect Patterns
 * ======================================================================== */

int astro_aspects_find_patterns(const AstroPlanetPosition positions[],
                                int num_planets,
                                const AstroAspect aspects[],
                                int num_aspects,
                                AstroPattern patterns[],
                                int max_patterns,
                                int *num_patterns) {
    if (positions == NULL || aspects == NULL || patterns == NULL || num_patterns == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    int count = 0;

    /* Find Grand Trines (3 planets all in trine) */
    for (int i = 0; i < num_aspects && count < max_patterns; i++) {
        if (aspects[i].aspect_type == ASTRO_ASPECT_TRINE) {
            for (int j = i + 1; j < num_aspects; j++) {
                if (aspects[j].aspect_type == ASTRO_ASPECT_TRINE) {
                    /* Check if forms triangle */
                    AstroPlanet p1 = aspects[i].planet1;
                    AstroPlanet p2 = aspects[i].planet2;
                    AstroPlanet p3a = aspects[j].planet1;
                    AstroPlanet p3b = aspects[j].planet2;

                    AstroPlanet third_planet = -1;
                    if (p3a == p1 || p3a == p2) {
                        third_planet = p3b;
                    } else if (p3b == p1 || p3b == p2) {
                        third_planet = p3a;
                    }

                    if (third_planet >= 0) {
                        /* Check third side of triangle */
                        for (int k = 0; k < num_aspects; k++) {
                            if (aspects[k].aspect_type == ASTRO_ASPECT_TRINE &&
                                ((aspects[k].planet1 == p1 && aspects[k].planet2 == third_planet) ||
                                 (aspects[k].planet2 == p1 && aspects[k].planet1 == third_planet) ||
                                 (aspects[k].planet1 == p2 && aspects[k].planet2 == third_planet) ||
                                 (aspects[k].planet2 == p2 && aspects[k].planet1 == third_planet))) {

                                patterns[count].pattern_type = ASTRO_PATTERN_GRAND_TRINE;
                                patterns[count].planets[0] = p1;
                                patterns[count].planets[1] = p2;
                                patterns[count].planets[2] = third_planet;
                                patterns[count].num_planets = 3;
                                snprintf(patterns[count].description, 256,
                                        "Grand Trine: %s, %s, %s",
                                        positions[p1].name,
                                        positions[p2].name,
                                        positions[third_planet].name);
                                count++;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    /* Find T-Squares (opposition with square to third planet) */
    for (int i = 0; i < num_aspects && count < max_patterns; i++) {
        if (aspects[i].aspect_type == ASTRO_ASPECT_OPPOSITION) {
            AstroPlanet p1 = aspects[i].planet1;
            AstroPlanet p2 = aspects[i].planet2;

            /* Look for planet square to both */
            for (int j = 0; j < num_aspects; j++) {
                if (aspects[j].aspect_type == ASTRO_ASPECT_SQUARE) {
                    AstroPlanet sq1 = aspects[j].planet1;
                    AstroPlanet sq2 = aspects[j].planet2;

                    if ((sq1 == p1 || sq1 == p2) && sq2 != p1 && sq2 != p2) {
                        /* Check if sq2 is square to the other planet */
                        for (int k = 0; k < num_aspects; k++) {
                            if (aspects[k].aspect_type == ASTRO_ASPECT_SQUARE &&
                                ((aspects[k].planet1 == sq2 && (aspects[k].planet2 == p1 || aspects[k].planet2 == p2)) ||
                                 (aspects[k].planet2 == sq2 && (aspects[k].planet1 == p1 || aspects[k].planet1 == p2)))) {

                                patterns[count].pattern_type = ASTRO_PATTERN_T_SQUARE;
                                patterns[count].planets[0] = p1;
                                patterns[count].planets[1] = p2;
                                patterns[count].planets[2] = sq2;
                                patterns[count].num_planets = 3;
                                snprintf(patterns[count].description, 256,
                                        "T-Square: %s opp %s, both square %s",
                                        positions[p1].name,
                                        positions[p2].name,
                                        positions[sq2].name);
                                count++;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    /* Find Stelliums (3+ planets in same sign) */
    for (int sign = 0; sign < 12 && count < max_patterns; sign++) {
        int planets_in_sign[ASTRO_MAX_PLANETS];
        int num_in_sign = 0;

        for (int i = 0; i < num_planets && num_in_sign < ASTRO_MAX_PLANETS; i++) {
            if (positions[i].sign == sign) {
                planets_in_sign[num_in_sign++] = i;
            }
        }

        if (num_in_sign >= 3) {
            patterns[count].pattern_type = ASTRO_PATTERN_STELLIUM;
            patterns[count].num_planets = num_in_sign;
            for (int i = 0; i < num_in_sign; i++) {
                patterns[count].planets[i] = positions[planets_in_sign[i]].planet;
            }
            snprintf(patterns[count].description, 256,
                    "Stellium in %s (%d planets)",
                    astro_core_get_sign_name(sign), num_in_sign);
            count++;
        }
    }

    *num_patterns = count;
    return ASTRO_OK;
}

/* ========================================================================
 * Aspect Interpretation
 * ======================================================================== */

const char* astro_aspects_get_name(AstroAspectType aspect_type) {
    switch (aspect_type) {
        case ASTRO_ASPECT_CONJUNCTION: return "conjunction";
        case ASTRO_ASPECT_OPPOSITION: return "opposition";
        case ASTRO_ASPECT_TRINE: return "trine";
        case ASTRO_ASPECT_SQUARE: return "square";
        case ASTRO_ASPECT_SEXTILE: return "sextile";
        case ASTRO_ASPECT_QUINCUNX: return "quincunx";
        case ASTRO_ASPECT_SEMISEXTILE: return "semi-sextile";
        case ASTRO_ASPECT_SEMISQUARE: return "semi-square";
        case ASTRO_ASPECT_SESQUIQUADRATE: return "sesquiquadrate";
        case ASTRO_ASPECT_QUINTILE: return "quintile";
        case ASTRO_ASPECT_BIQUINTILE: return "biquintile";
        default: return "unknown";
    }
}

const char* astro_aspects_get_symbol(AstroAspectType aspect_type) {
    switch (aspect_type) {
        case ASTRO_ASPECT_CONJUNCTION: return "☌";
        case ASTRO_ASPECT_OPPOSITION: return "☍";
        case ASTRO_ASPECT_TRINE: return "△";
        case ASTRO_ASPECT_SQUARE: return "□";
        case ASTRO_ASPECT_SEXTILE: return "⚹";
        case ASTRO_ASPECT_QUINCUNX: return "⚻";
        case ASTRO_ASPECT_SEMISEXTILE: return "⚺";
        case ASTRO_ASPECT_SEMISQUARE: return "∠";
        case ASTRO_ASPECT_SESQUIQUADRATE: return "⚼";
        case ASTRO_ASPECT_QUINTILE: return "Q";
        case ASTRO_ASPECT_BIQUINTILE: return "bQ";
        default: return "?";
    }
}

int astro_aspects_is_harmonious(AstroAspectType aspect_type) {
    switch (aspect_type) {
        case ASTRO_ASPECT_TRINE:
        case ASTRO_ASPECT_SEXTILE:
        case ASTRO_ASPECT_QUINTILE:
        case ASTRO_ASPECT_BIQUINTILE:
            return 1; /* Harmonious */
        case ASTRO_ASPECT_SQUARE:
        case ASTRO_ASPECT_OPPOSITION:
        case ASTRO_ASPECT_SEMISQUARE:
        case ASTRO_ASPECT_SESQUIQUADRATE:
        case ASTRO_ASPECT_QUINCUNX:
            return 0; /* Challenging */
        case ASTRO_ASPECT_CONJUNCTION:
        case ASTRO_ASPECT_SEMISEXTILE:
            return -1; /* Neutral/depends on planets */
        default:
            return -1;
    }
}

double astro_aspects_get_strength(const AstroAspect *aspect) {
    if (aspect == NULL) {
        return 0.0;
    }

    AstroAspectConfig *config = get_aspect_config(aspect->aspect_type);
    if (config == NULL) {
        return 0.0;
    }

    /* Strength is inverse of orb - closer to exact is stronger */
    double strength = 1.0 - (aspect->difference / config->default_orb);
    return (strength < 0.0) ? 0.0 : strength;
}

char* astro_aspects_format(const AstroAspect *aspect,
                           const AstroPlanetPosition positions[],
                           char *buffer,
                           int buffer_size) {
    if (aspect == NULL || buffer == NULL || buffer_size < 64) {
        return NULL;
    }

    const char *applying_str = aspect->is_applying == 1 ? "applying" :
                               aspect->is_applying == 0 ? "separating" : "stationary";

    snprintf(buffer, buffer_size, "%s %s %s (%.2f° %s%s)",
            astro_core_get_planet_name(aspect->planet1),
            astro_aspects_get_name(aspect->aspect_type),
            astro_core_get_planet_name(aspect->planet2),
            aspect->difference,
            applying_str,
            aspect->is_exact ? ", exact" : "");

    return buffer;
}
