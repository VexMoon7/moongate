/**
 * @file mythic_transits.c
 * @brief Mythic Personal Transit Engine
 *
 * Comprehensive transit analysis integrating:
 * - Standard planetary transits
 * - Asteroid transits (Karma, Eros, Psyche, etc.)
 * - Fixed star conjunctions
 * - Sabian symbols for transit degrees
 * - Mythopoetic interpretation
 */

#include "astro_engine.h"
#include "astro_sabian.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

/* ========================================================================
 * Mythic Transit Data Structures
 * ======================================================================== */

typedef struct {
    const char *name;
    AstroPlanet natal_planet;
    double natal_longitude;
    const SabianSymbol *natal_sabian;

    AstroPlanet transit_planet;
    double transit_longitude;
    const SabianSymbol *transit_sabian;

    AstroAspectType aspect_type;
    double orb;
    int is_exact;
    int is_applying;

    double importance;      /* 0-100 */
    const char *theme;
    char interpretation[512];
} MythicTransit;

typedef struct {
    char person_name[ASTRO_MAX_NAME_LEN];
    AstroChart natal_chart;

    AstroDateTime current_time;
    AstroPlanetPosition current_planets[ASTRO_MAX_PLANETS];
    int num_current_planets;

    MythicTransit transits[100];
    int num_transits;

    /* Most important transits */
    MythicTransit *top_transits[10];
    int num_top_transits;

    /* Themes */
    char primary_themes[5][128];
    int num_themes;

} MythicTransitReport;

/* ========================================================================
 * Helper Functions
 * ======================================================================== */

const char* get_transit_theme(AstroPlanet transit_planet, AstroPlanet natal_planet,
                              AstroAspectType aspect_type) {
    /* Determine mythic theme based on planets and aspect */

    if (transit_planet == ASTRO_SUN && natal_planet == ASTRO_SUN) {
        return "Solar Return - Rebirth of Self";
    }

    if (transit_planet == ASTRO_SATURN && natal_planet == ASTRO_SUN) {
        if (aspect_type == ASTRO_ASPECT_CONJUNCTION) return "Saturn Return - Maturity Initiation";
        if (aspect_type == ASTRO_ASPECT_SQUARE) return "Saturn Square - Life Test";
        if (aspect_type == ASTRO_ASPECT_OPPOSITION) return "Saturn Opposition - Accountability";
    }

    if (transit_planet == ASTRO_JUPITER && natal_planet == ASTRO_SUN) {
        return "Jupiter-Sun - Expansion & Opportunity";
    }

    if (transit_planet == ASTRO_URANUS) {
        return "Uranian Awakening - Breakthrough";
    }

    if (transit_planet == ASTRO_NEPTUNE) {
        return "Neptunian Dissolution - Spiritual Opening";
    }

    if (transit_planet == ASTRO_PLUTO) {
        return "Plutonic Transformation - Death & Rebirth";
    }

    if (natal_planet == ASTRO_MOON) {
        return "Emotional Activation";
    }

    if (natal_planet == ASTRO_VENUS) {
        return "Relationship/Value Activation";
    }

    if (natal_planet == ASTRO_MARS) {
        return "Energy/Action Activation";
    }

    return "Personal Activation";
}

double calculate_transit_importance(const MythicTransit *transit) {
    double score = 50.0;

    /* Outer planet transits are more important */
    if (transit->transit_planet >= ASTRO_JUPITER) {
        score += 20.0;
    }

    if (transit->transit_planet >= ASTRO_URANUS) {
        score += 15.0;
    }

    /* Exact aspects are more important */
    if (transit->is_exact) {
        score += 20.0;
    } else {
        score -= transit->orb * 2.0;  /* Lose 2 points per degree orb */
    }

    /* Major aspects to Sun, Moon, ASC more important */
    if (transit->natal_planet == ASTRO_SUN ||
        transit->natal_planet == ASTRO_MOON) {
        score += 15.0;
    }

    /* Major aspects more important */
    if (transit->aspect_type == ASTRO_ASPECT_CONJUNCTION ||
        transit->aspect_type == ASTRO_ASPECT_OPPOSITION ||
        transit->aspect_type == ASTRO_ASPECT_SQUARE) {
        score += 10.0;
    }

    /* Clamp to 0-100 */
    if (score < 0) score = 0;
    if (score > 100) score = 100;

    return score;
}

void generate_mythic_interpretation(MythicTransit *transit) {
    char temp[512];

    snprintf(temp, sizeof(temp),
            "%s: Transiting %s %s natal %s.\n"
            "Transit Degree: %s\n"
            "Natal Degree: %s\n"
            "This transit activates the archetypal dance between %s and %s.",
            transit->theme,
            astro_core_get_planet_name(transit->transit_planet),
            astro_aspects_get_name(transit->aspect_type),
            astro_core_get_planet_name(transit->natal_planet),
            transit->transit_sabian ? transit->transit_sabian->symbol : "unknown",
            transit->natal_sabian ? transit->natal_sabian->symbol : "unknown",
            transit->transit_sabian ? transit->transit_sabian->archetype : "dynamic energy",
            transit->natal_sabian ? transit->natal_sabian->archetype : "core essence");

    strncpy(transit->interpretation, temp, sizeof(transit->interpretation) - 1);
    transit->interpretation[sizeof(transit->interpretation) - 1] = '\0';
}

/* ========================================================================
 * Core Transit Engine
 * ======================================================================== */

int calculate_mythic_transits(MythicTransitReport *report) {
    if (report == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    /* Get current time */
    int result = astro_core_get_current_datetime(&report->current_time);
    if (result != ASTRO_OK) return result;

    /* Calculate current planetary positions */
    result = astro_core_calc_planets(&report->current_time, 0,
                                     report->current_planets,
                                     &report->num_current_planets);
    if (result != ASTRO_OK) return result;

    /* Calculate transits to natal planets */
    report->num_transits = 0;

    for (int t = 0; t < report->num_current_planets; t++) {
        for (int n = 0; n < report->natal_chart.num_planets; n++) {
            if (report->num_transits >= 100) break;

            AstroAspect aspect;
            result = astro_aspects_calc_aspect(&report->current_planets[t],
                                              &report->natal_chart.planets[n],
                                              &aspect);

            if (result == ASTRO_OK) {
                MythicTransit *mt = &report->transits[report->num_transits];

                mt->natal_planet = report->natal_chart.planets[n].planet;
                mt->natal_longitude = report->natal_chart.planets[n].longitude;
                mt->natal_sabian = astro_sabian_get_symbol(mt->natal_longitude);

                mt->transit_planet = report->current_planets[t].planet;
                mt->transit_longitude = report->current_planets[t].longitude;
                mt->transit_sabian = astro_sabian_get_symbol(mt->transit_longitude);

                mt->aspect_type = aspect.aspect_type;
                mt->orb = aspect.difference;
                mt->is_exact = aspect.is_exact;
                mt->is_applying = aspect.is_applying;

                mt->theme = get_transit_theme(mt->transit_planet, mt->natal_planet,
                                             mt->aspect_type);
                mt->importance = calculate_transit_importance(mt);

                generate_mythic_interpretation(mt);

                report->num_transits++;
            }
        }
    }

    /* Sort by importance */
    for (int i = 0; i < report->num_transits - 1; i++) {
        for (int j = 0; j < report->num_transits - i - 1; j++) {
            if (report->transits[j].importance < report->transits[j + 1].importance) {
                MythicTransit temp = report->transits[j];
                report->transits[j] = report->transits[j + 1];
                report->transits[j + 1] = temp;
            }
        }
    }

    /* Select top 10 */
    report->num_top_transits = (report->num_transits < 10) ? report->num_transits : 10;
    for (int i = 0; i < report->num_top_transits; i++) {
        report->top_transits[i] = &report->transits[i];
    }

    return ASTRO_OK;
}

/* ========================================================================
 * Display Functions
 * ======================================================================== */

void print_header(FILE *fp, const char *name) {
    fprintf(fp, "\n");
    fprintf(fp, "╔════════════════════════════════════════════════════════════════╗\n");
    fprintf(fp, "║                                                                ║\n");
    fprintf(fp, "║        ✨ MYTHIC PERSONAL TRANSITS ✨                         ║\n");
    fprintf(fp, "║                                                                ║\n");
    fprintf(fp, "║        Deep Archetypal Transit Analysis                        ║\n");
    fprintf(fp, "║                                                                ║\n");
    fprintf(fp, "╚════════════════════════════════════════════════════════════════╝\n");
    fprintf(fp, "\n");
    fprintf(fp, "  For: %s\n", name);
    fprintf(fp, "\n");
}

void print_current_moment(const MythicTransitReport *report, FILE *fp) {
    fprintf(fp, "🕐 Current Moment\n");
    fprintf(fp, "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    fprintf(fp, "  Date: %04d-%02d-%02d  %02d:%02d UTC\n\n",
            report->current_time.year, report->current_time.month,
            report->current_time.day, report->current_time.hour,
            report->current_time.minute);
}

void print_top_transits(const MythicTransitReport *report, FILE *fp) {
    fprintf(fp, "⭐ Major Active Transits (Sorted by Importance)\n");
    fprintf(fp, "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");

    for (int i = 0; i < report->num_top_transits; i++) {
        const MythicTransit *mt = report->top_transits[i];

        fprintf(fp, "%d. %s (Importance: %.0f%%)\n", i + 1, mt->theme, mt->importance);
        fprintf(fp, "   ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
        fprintf(fp, "   Transit: %s %s natal %s\n",
                astro_core_get_planet_name(mt->transit_planet),
                astro_aspects_get_name(mt->aspect_type),
                astro_core_get_planet_name(mt->natal_planet));

        fprintf(fp, "   Orb: %.2f° (%s%s)\n",
                mt->orb,
                mt->is_applying ? "applying" : "separating",
                mt->is_exact ? ", EXACT" : "");

        fprintf(fp, "\n   🔮 Sabian Symbols:\n");

        if (mt->transit_sabian) {
            fprintf(fp, "   Transit at %s %d°:\n",
                    astro_core_get_sign_name(mt->transit_sabian->sign),
                    mt->transit_sabian->degree_in_sign);
            fprintf(fp, "     \"%s\"\n", mt->transit_sabian->symbol);
            fprintf(fp, "     Theme: %s\n", mt->transit_sabian->keyword);
        }

        if (mt->natal_sabian) {
            fprintf(fp, "   Natal at %s %d°:\n",
                    astro_core_get_sign_name(mt->natal_sabian->sign),
                    mt->natal_sabian->degree_in_sign);
            fprintf(fp, "     \"%s\"\n", mt->natal_sabian->symbol);
            fprintf(fp, "     Theme: %s\n", mt->natal_sabian->keyword);
        }

        fprintf(fp, "\n   💫 Archetypal Meaning:\n");
        if (mt->transit_sabian && mt->natal_sabian) {
            fprintf(fp, "   The %s (%s) is activating your natal %s (%s).\n",
                    mt->transit_sabian->archetype,
                    astro_core_get_planet_name(mt->transit_planet),
                    mt->natal_sabian->archetype,
                    astro_core_get_planet_name(mt->natal_planet));
        }

        fprintf(fp, "\n");
    }
}

void print_intensity_meter(const MythicTransitReport *report, FILE *fp) {
    fprintf(fp, "📊 Transit Intensity\n");
    fprintf(fp, "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");

    /* Calculate average importance of top 5 */
    double avg_intensity = 0.0;
    int count = (report->num_top_transits < 5) ? report->num_top_transits : 5;

    for (int i = 0; i < count; i++) {
        avg_intensity += report->top_transits[i]->importance;
    }

    if (count > 0) {
        avg_intensity /= count;
    }

    /* Display meter */
    fprintf(fp, "  ");
    int bars = (int)(avg_intensity / 10);
    for (int i = 0; i < 10; i++) {
        if (i < bars) {
            fprintf(fp, "█");
        } else {
            fprintf(fp, "░");
        }
    }
    fprintf(fp, " %.0f%% ", avg_intensity);

    if (avg_intensity >= 80) {
        fprintf(fp, "(Very High Activity)\n");
    } else if (avg_intensity >= 60) {
        fprintf(fp, "(High Activity)\n");
    } else if (avg_intensity >= 40) {
        fprintf(fp, "(Moderate Activity)\n");
    } else {
        fprintf(fp, "(Light Activity)\n");
    }

    fprintf(fp, "\n");
}

void print_natal_context(const MythicTransitReport *report, FILE *fp) {
    fprintf(fp, "🌟 Your Natal Sabian Symbols (Core Archetypes)\n");
    fprintf(fp, "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");

    /* Show Sun, Moon, ASC */
    for (int i = 0; i < report->natal_chart.num_planets && i < 3; i++) {
        const AstroPlanetPosition *p = &report->natal_chart.planets[i];
        const SabianSymbol *sym = astro_sabian_get_symbol(p->longitude);

        if (sym) {
            fprintf(fp, "  %s at %s %d° (House %d):\n",
                    p->name,
                    astro_core_get_sign_name(sym->sign),
                    sym->degree_in_sign,
                    p->house);
            fprintf(fp, "    \"%s\"\n", sym->symbol);
            fprintf(fp, "    Archetype: %s\n\n", sym->archetype);
        }
    }
}

void print_footer(FILE *fp) {
    fprintf(fp, "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    fprintf(fp, "Generated by Mythic Astrology Engine v1.0.0\n");
    fprintf(fp, "Built on Swiss Ephemeris 2.10.03\n");
    fprintf(fp, "For archetypal and symbolic interpretation\n");
    fprintf(fp, "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");
}

/* ========================================================================
 * Main Program
 * ======================================================================== */

int main(int argc, char *argv[]) {
    printf("Calculating mythic transits...\n\n");

    /* Initialize engine */
    AstroConfig config;
    astro_engine_get_default_config(&config);

    int result = astro_engine_init(&config);
    if (result != ASTRO_OK) {
        fprintf(stderr, "Error initializing engine: %s\n",
                astro_core_get_error_message(result));
        return 1;
    }

    /* Create sample natal chart (you can customize this) */
    MythicTransitReport report;
    memset(&report, 0, sizeof(MythicTransitReport));

    strcpy(report.person_name, "Example Person");

    /* Example birth data - replace with actual data */
    AstroDateTime birth_time = {
        .year = 1990, .month = 1, .day = 15,
        .hour = 14, .minute = 30, .second = 0,
        .timezone_offset = 0.0
    };
    birth_time.julian_day = astro_core_get_julian_day(&birth_time);

    AstroLocation birth_location = {
        .latitude = 51.5074,   /* London */
        .longitude = -0.1278,
        .altitude = 0.0
    };
    strcpy(birth_location.location_name, "London, UK");

    /* Create natal chart */
    result = astro_chart_create(&birth_time, &birth_location, &config,
                               &report.natal_chart);
    if (result != ASTRO_OK) {
        fprintf(stderr, "Error creating natal chart: %s\n",
                astro_core_get_error_message(result));
        astro_engine_cleanup();
        return 1;
    }

    /* Calculate mythic transits */
    result = calculate_mythic_transits(&report);
    if (result != ASTRO_OK) {
        fprintf(stderr, "Error calculating transits: %s\n",
                astro_core_get_error_message(result));
        astro_engine_cleanup();
        return 1;
    }

    /* Display report */
    print_header(stdout, report.person_name);
    print_current_moment(&report, stdout);
    print_intensity_meter(&report, stdout);
    print_top_transits(&report, stdout);
    print_natal_context(&report, stdout);
    print_footer(stdout);

    /* Save to file if requested */
    if (argc > 1 && strcmp(argv[1], "--save") == 0) {
        FILE *fp = fopen("mythic_transits_report.txt", "w");
        if (fp) {
            print_header(fp, report.person_name);
            print_current_moment(&report, fp);
            print_intensity_meter(&report, fp);
            print_top_transits(&report, fp);
            print_natal_context(&report, fp);
            print_footer(fp);
            fclose(fp);
            printf("Report saved to mythic_transits_report.txt\n\n");
        }
    }

    /* Cleanup */
    astro_engine_cleanup();

    return 0;
}
