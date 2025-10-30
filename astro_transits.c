/**
 * @file astro_transits.c
 * @brief Transit and progression calculation implementation
 */

#include "astro_transits.h"
#include "astro_core.h"
#include "astro_aspects.h"
#include "astro_chart.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* ========================================================================
 * Transit Calculations
 * ======================================================================== */

int astro_transits_calc_current(const AstroChart *natal_chart,
                                const AstroDateTime *transit_time,
                                AstroTransit transits[],
                                int max_transits,
                                int *num_transits) {
    if (natal_chart == NULL || transit_time == NULL ||
        transits == NULL || num_transits == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    /* Calculate current transit positions */
    AstroPlanetPosition transit_positions[ASTRO_MAX_PLANETS];
    int num_transit_planets;

    int result = astro_core_calc_planets(transit_time, 0,
                                        transit_positions, &num_transit_planets);
    if (result != ASTRO_OK) {
        return result;
    }

    /* Calculate aspects between transit and natal positions */
    AstroAspect aspects[ASTRO_MAX_ASPECTS];
    int num_aspects;

    result = astro_aspects_calc_between_charts(
        transit_positions, num_transit_planets,
        natal_chart->planets, natal_chart->num_planets,
        aspects, ASTRO_MAX_ASPECTS, &num_aspects);

    if (result != ASTRO_OK) {
        return result;
    }

    /* Convert aspects to transit events */
    int count = 0;
    for (int i = 0; i < num_aspects && count < max_transits; i++) {
        transits[count].transiting_planet = aspects[i].planet1;
        transits[count].natal_planet = aspects[i].planet2;
        transits[count].aspect_type = aspects[i].aspect_type;
        memcpy(&transits[count].event_time, transit_time, sizeof(AstroDateTime));
        transits[count].orb = aspects[i].difference;
        transits[count].is_exact = aspects[i].is_exact;

        snprintf(transits[count].description, 256, "Transit %s %s natal %s",
                astro_core_get_planet_name(aspects[i].planet1),
                astro_aspects_get_name(aspects[i].aspect_type),
                astro_core_get_planet_name(aspects[i].planet2));

        count++;
    }

    *num_transits = count;
    return ASTRO_OK;
}

int astro_transits_calc_period(const AstroChart *natal_chart,
                               const AstroDateTime *start_time,
                               const AstroDateTime *end_time,
                               AstroTransit transits[],
                               int max_transits,
                               int *num_transits) {
    if (natal_chart == NULL || start_time == NULL || end_time == NULL ||
        transits == NULL || num_transits == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    int count = 0;
    double start_jd = astro_core_get_julian_day(start_time);
    double end_jd = astro_core_get_julian_day(end_time);

    /* Sample every day (can be made configurable) */
    double step = 1.0; /* 1 day */

    for (double jd = start_jd; jd <= end_jd && count < max_transits; jd += step) {
        AstroDateTime current_time;
        astro_core_get_calendar_date(jd, &current_time);

        AstroTransit current_transits[ASTRO_MAX_TRANSITS];
        int num_current;

        int result = astro_transits_calc_current(natal_chart, &current_time,
                                                current_transits, ASTRO_MAX_TRANSITS,
                                                &num_current);

        if (result == ASTRO_OK) {
            for (int i = 0; i < num_current && count < max_transits; i++) {
                transits[count++] = current_transits[i];
            }
        }
    }

    *num_transits = count;
    return ASTRO_OK;
}

int astro_transits_find_exact(AstroPlanet transiting_planet,
                              double natal_longitude,
                              AstroAspectType aspect_type,
                              const AstroDateTime *start_time,
                              const AstroDateTime *end_time,
                              AstroDateTime *exact_time) {
    if (start_time == NULL || end_time == NULL || exact_time == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    double start_jd = astro_core_get_julian_day(start_time);
    double end_jd = astro_core_get_julian_day(end_time);
    double target_angle = (double)aspect_type;

    /* Binary search for exact time */
    double tolerance = 0.01; /* ~15 minutes */
    double best_jd = start_jd;
    double best_diff = 360.0;

    /* First, do a coarse search */
    double step = 1.0; /* 1 day steps */
    for (double jd = start_jd; jd <= end_jd; jd += step) {
        AstroDateTime temp_time;
        astro_core_get_calendar_date(jd, &temp_time);

        AstroPlanetPosition pos;
        if (astro_core_calc_planet(transiting_planet, &temp_time, 0, &pos) == ASTRO_OK) {
            double angle = astro_core_angular_distance(pos.longitude, natal_longitude);
            double diff = fabs(angle - target_angle);

            if (diff < best_diff) {
                best_diff = diff;
                best_jd = jd;
            }
        }
    }

    /* Refine with smaller steps around best match */
    double refine_start = best_jd - 1.0;
    double refine_end = best_jd + 1.0;
    step = 1.0 / 24.0; /* 1 hour steps */

    for (double jd = refine_start; jd <= refine_end; jd += step) {
        AstroDateTime temp_time;
        astro_core_get_calendar_date(jd, &temp_time);

        AstroPlanetPosition pos;
        if (astro_core_calc_planet(transiting_planet, &temp_time, 0, &pos) == ASTRO_OK) {
            double angle = astro_core_angular_distance(pos.longitude, natal_longitude);
            double diff = fabs(angle - target_angle);

            if (diff < best_diff) {
                best_diff = diff;
                best_jd = jd;
            }
        }
    }

    /* Return best match */
    return astro_core_get_calendar_date(best_jd, exact_time);
}

int astro_transits_calc_return(const AstroChart *natal_chart,
                               AstroPlanet planet,
                               int year,
                               AstroChart *return_chart) {
    if (natal_chart == NULL || return_chart == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    /* Get natal planet position */
    const AstroPlanetPosition *natal_pos = astro_chart_get_planet(natal_chart, planet);
    if (natal_pos == NULL) {
        return ASTRO_ERROR_INVALID_PLANET;
    }

    /* Search for return around birthday in target year */
    AstroDateTime search_start = natal_chart->birth_time;
    search_start.year = year;
    search_start.month = natal_chart->birth_time.month;
    search_start.day = natal_chart->birth_time.day;

    AstroDateTime search_end = search_start;
    search_end.day += 5; /* Search within 5 days */

    AstroDateTime return_time;
    int result = astro_transits_find_exact(planet, natal_pos->longitude,
                                          ASTRO_ASPECT_CONJUNCTION,
                                          &search_start, &search_end,
                                          &return_time);

    if (result != ASTRO_OK) {
        return result;
    }

    /* Create return chart */
    return astro_chart_create(&return_time, &natal_chart->birth_location,
                             NULL, return_chart);
}

/* ========================================================================
 * Progression Calculations
 * ======================================================================== */

double astro_transits_calc_age(const AstroDateTime *birth_time,
                               const AstroDateTime *target_date) {
    if (birth_time == NULL || target_date == NULL) {
        return 0.0;
    }

    double birth_jd = astro_core_get_julian_day(birth_time);
    double target_jd = astro_core_get_julian_day(target_date);

    return (target_jd - birth_jd) / 365.25;
}

int astro_transits_calc_secondary_progression_date(const AstroDateTime *birth_time,
                                                   const AstroDateTime *target_date,
                                                   AstroDateTime *progressed_date) {
    if (birth_time == NULL || target_date == NULL || progressed_date == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    /* Secondary progressions: 1 day = 1 year */
    double age = astro_transits_calc_age(birth_time, target_date);
    double birth_jd = astro_core_get_julian_day(birth_time);
    double progressed_jd = birth_jd + age; /* Add age in days */

    return astro_core_get_calendar_date(progressed_jd, progressed_date);
}

int astro_transits_calc_progressed_chart(const AstroChart *natal_chart,
                                        AstroProgressionType progression_type,
                                        const AstroDateTime *target_date,
                                        AstroChart *progressed_chart) {
    if (natal_chart == NULL || target_date == NULL || progressed_chart == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    AstroDateTime progressed_date;
    int result;

    switch (progression_type) {
        case ASTRO_PROGRESSION_SECONDARY:
            result = astro_transits_calc_secondary_progression_date(
                &natal_chart->birth_time, target_date, &progressed_date);
            break;

        case ASTRO_PROGRESSION_TERTIARY:
            /* Tertiary: 1 day = 1 lunar month (~27.3 days) */
            {
                double age = astro_transits_calc_age(&natal_chart->birth_time, target_date);
                double birth_jd = astro_core_get_julian_day(&natal_chart->birth_time);
                double progressed_jd = birth_jd + (age * 27.3);
                result = astro_core_get_calendar_date(progressed_jd, &progressed_date);
            }
            break;

        case ASTRO_PROGRESSION_MINOR:
            /* Minor: 1 lunar month = 1 year */
            {
                double age = astro_transits_calc_age(&natal_chart->birth_time, target_date);
                double birth_jd = astro_core_get_julian_day(&natal_chart->birth_time);
                double progressed_jd = birth_jd + (age / 27.3);
                result = astro_core_get_calendar_date(progressed_jd, &progressed_date);
            }
            break;

        default:
            return ASTRO_ERROR_OUT_OF_RANGE;
    }

    if (result != ASTRO_OK) {
        return result;
    }

    /* Create progressed chart */
    return astro_chart_create(&progressed_date, &natal_chart->birth_location,
                             NULL, progressed_chart);
}

/* ========================================================================
 * Solar Arc Directions
 * ======================================================================== */

int astro_transits_calc_solar_arc(const AstroChart *natal_chart,
                                  const AstroDateTime *target_date,
                                  AstroChart *directed_chart) {
    if (natal_chart == NULL || target_date == NULL || directed_chart == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    /* Calculate progressed Sun position */
    AstroDateTime progressed_date;
    int result = astro_transits_calc_secondary_progression_date(
        &natal_chart->birth_time, target_date, &progressed_date);

    if (result != ASTRO_OK) {
        return result;
    }

    AstroPlanetPosition progressed_sun;
    result = astro_core_calc_planet(ASTRO_SUN, &progressed_date, 0, &progressed_sun);
    if (result != ASTRO_OK) {
        return result;
    }

    /* Get natal Sun position */
    const AstroPlanetPosition *natal_sun = astro_chart_get_planet(natal_chart, ASTRO_SUN);
    if (natal_sun == NULL) {
        return ASTRO_ERROR_INVALID_PLANET;
    }

    /* Calculate solar arc */
    double solar_arc = progressed_sun.longitude - natal_sun->longitude;

    /* Create directed chart by adding solar arc to all natal positions */
    memcpy(directed_chart, natal_chart, sizeof(AstroChart));

    for (int i = 0; i < directed_chart->num_planets; i++) {
        directed_chart->planets[i].longitude =
            astro_core_normalize_angle(directed_chart->planets[i].longitude + solar_arc);
        directed_chart->planets[i].sign =
            astro_core_get_sign(directed_chart->planets[i].longitude);
        directed_chart->planets[i].sign_position =
            astro_core_get_sign_position(directed_chart->planets[i].longitude);
    }

    /* Update houses with solar arc */
    for (int i = 1; i <= ASTRO_MAX_HOUSES; i++) {
        directed_chart->houses.cusp[i] =
            astro_core_normalize_angle(directed_chart->houses.cusp[i] + solar_arc);
    }
    directed_chart->houses.ascendant =
        astro_core_normalize_angle(directed_chart->houses.ascendant + solar_arc);
    directed_chart->houses.mc =
        astro_core_normalize_angle(directed_chart->houses.mc + solar_arc);

    /* Recalculate aspects */
    return astro_chart_update_aspects(directed_chart);
}

/* ========================================================================
 * Transit Analysis
 * ======================================================================== */

static int is_outer_planet(AstroPlanet planet) {
    return (planet >= ASTRO_JUPITER && planet <= ASTRO_PLUTO);
}

static double calc_transit_strength(const AstroTransit *transit) {
    double strength = 50.0;

    /* Outer planet transits are stronger */
    if (is_outer_planet(transit->transiting_planet)) {
        strength += 20.0;
    }

    /* Exact aspects are stronger */
    if (transit->is_exact) {
        strength += 20.0;
    }

    /* Major aspects are stronger */
    if (transit->aspect_type == ASTRO_ASPECT_CONJUNCTION ||
        transit->aspect_type == ASTRO_ASPECT_OPPOSITION ||
        transit->aspect_type == ASTRO_ASPECT_SQUARE) {
        strength += 10.0;
    }

    /* Adjust by orb (tighter orb = stronger) */
    strength *= (1.0 - transit->orb / 10.0);

    return (strength < 0) ? 0 : (strength > 100) ? 100 : strength;
}

int astro_transits_analyze_importance(const AstroTransit transits[],
                                      int num_transits,
                                      AstroTransitScore scores[],
                                      int max_scores,
                                      int *num_scores) {
    if (transits == NULL || scores == NULL || num_scores == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    int count = 0;
    for (int i = 0; i < num_transits && count < max_scores; i++) {
        scores[count].transit = transits[i];
        scores[count].strength = calc_transit_strength(&transits[i]);
        scores[count].is_major = is_outer_planet(transits[i].transiting_planet);

        snprintf(scores[count].interpretation, 512,
                "Transit %s %s natal %s - Strength: %.0f%%",
                astro_core_get_planet_name(transits[i].transiting_planet),
                astro_aspects_get_name(transits[i].aspect_type),
                astro_core_get_planet_name(transits[i].natal_planet),
                scores[count].strength);

        count++;
    }

    *num_scores = count;
    return ASTRO_OK;
}

const AstroTransit* astro_transits_get_most_important(const AstroTransit transits[],
                                                       int num_transits) {
    if (transits == NULL || num_transits == 0) {
        return NULL;
    }

    const AstroTransit *most_important = &transits[0];
    double max_strength = calc_transit_strength(&transits[0]);

    for (int i = 1; i < num_transits; i++) {
        double strength = calc_transit_strength(&transits[i]);
        if (strength > max_strength) {
            max_strength = strength;
            most_important = &transits[i];
        }
    }

    return most_important;
}

int astro_transits_filter_by_planet(const AstroTransit all_transits[],
                                    int num_all,
                                    AstroPlanet planet,
                                    AstroTransit filtered[],
                                    int max_filtered,
                                    int *num_filtered) {
    if (all_transits == NULL || filtered == NULL || num_filtered == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    int count = 0;
    for (int i = 0; i < num_all && count < max_filtered; i++) {
        if (all_transits[i].transiting_planet == planet) {
            filtered[count++] = all_transits[i];
        }
    }

    *num_filtered = count;
    return ASTRO_OK;
}

int astro_transits_filter_by_aspect(const AstroTransit all_transits[],
                                    int num_all,
                                    AstroAspectType aspect_type,
                                    AstroTransit filtered[],
                                    int max_filtered,
                                    int *num_filtered) {
    if (all_transits == NULL || filtered == NULL || num_filtered == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    int count = 0;
    for (int i = 0; i < num_all && count < max_filtered; i++) {
        if (all_transits[i].aspect_type == aspect_type) {
            filtered[count++] = all_transits[i];
        }
    }

    *num_filtered = count;
    return ASTRO_OK;
}

/* ========================================================================
 * Transit Output
 * ======================================================================== */

int astro_transits_print(const AstroTransit transits[],
                        int num_transits,
                        FILE *fp) {
    if (transits == NULL || fp == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    fprintf(fp, "\nTransits (%d found):\n", num_transits);
    fprintf(fp, "%-12s %-12s %-12s %20s %6s\n",
            "Transit", "Aspect", "Natal", "Date", "Orb");
    fprintf(fp, "%-12s %-12s %-12s %20s %6s\n",
            "-------", "------", "-----", "----", "---");

    for (int i = 0; i < num_transits; i++) {
        const AstroTransit *t = &transits[i];
        fprintf(fp, "%-12s %-12s %-12s %04d-%02d-%02d %02d:%02d  %.2f°%s\n",
                astro_core_get_planet_name(t->transiting_planet),
                astro_aspects_get_name(t->aspect_type),
                astro_core_get_planet_name(t->natal_planet),
                t->event_time.year, t->event_time.month, t->event_time.day,
                t->event_time.hour, t->event_time.minute,
                t->orb,
                t->is_exact ? " *" : "");
    }
    fprintf(fp, "\n");

    return ASTRO_OK;
}

char* astro_transits_format(const AstroTransit *transit,
                            char *buffer,
                            int buffer_size) {
    if (transit == NULL || buffer == NULL || buffer_size < 128) {
        return NULL;
    }

    snprintf(buffer, buffer_size,
            "Transit %s %s natal %s on %04d-%02d-%02d (orb: %.2f°)%s",
            astro_core_get_planet_name(transit->transiting_planet),
            astro_aspects_get_name(transit->aspect_type),
            astro_core_get_planet_name(transit->natal_planet),
            transit->event_time.year,
            transit->event_time.month,
            transit->event_time.day,
            transit->orb,
            transit->is_exact ? " EXACT" : "");

    return buffer;
}
