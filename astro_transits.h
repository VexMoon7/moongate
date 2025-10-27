/**
 * @file astro_transits.h
 * @brief Transit and progression calculation module
 *
 * This module provides functions for calculating transits (moving planets
 * relative to natal positions) and progressions (symbolic time progression).
 */

#ifndef ASTRO_TRANSITS_H
#define ASTRO_TRANSITS_H

#include "astro_types.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================
 * Transit Calculations
 * ======================================================================== */

/**
 * Calculate current transits to natal chart
 *
 * @param natal_chart Natal chart
 * @param transit_time Current time for transits
 * @param transits Array to store transit events
 * @param max_transits Maximum transits to find
 * @param num_transits Pointer to store number of transits found
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_transits_calc_current(const AstroChart *natal_chart,
                                const AstroDateTime *transit_time,
                                AstroTransit transits[],
                                int max_transits,
                                int *num_transits);

/**
 * Calculate transits over a time period
 *
 * @param natal_chart Natal chart
 * @param start_time Start of period
 * @param end_time End of period
 * @param transits Array to store transit events
 * @param max_transits Maximum transits to find
 * @param num_transits Pointer to store number of transits found
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_transits_calc_period(const AstroChart *natal_chart,
                               const AstroDateTime *start_time,
                               const AstroDateTime *end_time,
                               AstroTransit transits[],
                               int max_transits,
                               int *num_transits);

/**
 * Find exact transit time
 *
 * Find the exact time when a transiting planet makes an aspect
 * to a natal planet.
 *
 * @param transiting_planet Transiting planet
 * @param natal_longitude Natal planet longitude
 * @param aspect_type Aspect to find
 * @param start_time Start search time
 * @param end_time End search time
 * @param exact_time Pointer to store exact transit time
 * @return ASTRO_OK if found, error code otherwise
 */
int astro_transits_find_exact(AstroPlanet transiting_planet,
                              double natal_longitude,
                              AstroAspectType aspect_type,
                              const AstroDateTime *start_time,
                              const AstroDateTime *end_time,
                              AstroDateTime *exact_time);

/**
 * Calculate return chart (when planet returns to natal position)
 *
 * @param natal_chart Natal chart
 * @param planet Planet for return (e.g., Sun for Solar Return)
 * @param year Year for return
 * @param return_chart Pointer to store return chart
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_transits_calc_return(const AstroChart *natal_chart,
                               AstroPlanet planet,
                               int year,
                               AstroChart *return_chart);

/* ========================================================================
 * Progression Calculations
 * ======================================================================== */

/**
 * Progression types
 */
typedef enum {
    ASTRO_PROGRESSION_SECONDARY,    /* 1 day = 1 year */
    ASTRO_PROGRESSION_TERTIARY,     /* 1 day = 1 lunar month */
    ASTRO_PROGRESSION_MINOR         /* 1 lunar month = 1 year */
} AstroProgressionType;

/**
 * Calculate progressed chart
 *
 * @param natal_chart Natal chart
 * @param progression_type Type of progression
 * @param target_date Date to progress to
 * @param progressed_chart Pointer to store progressed chart
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_transits_calc_progressed_chart(const AstroChart *natal_chart,
                                        AstroProgressionType progression_type,
                                        const AstroDateTime *target_date,
                                        AstroChart *progressed_chart);

/**
 * Calculate age for progressed chart
 *
 * @param birth_time Birth date/time
 * @param target_date Target date
 * @return Age in years
 */
double astro_transits_calc_age(const AstroDateTime *birth_time,
                               const AstroDateTime *target_date);

/**
 * Calculate secondary progressed date
 *
 * @param birth_time Birth date/time
 * @param target_date Date to progress to
 * @param progressed_date Pointer to store progressed calculation date
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_transits_calc_secondary_progression_date(const AstroDateTime *birth_time,
                                                   const AstroDateTime *target_date,
                                                   AstroDateTime *progressed_date);

/* ========================================================================
 * Solar Arc Directions
 * ======================================================================== */

/**
 * Calculate solar arc directed chart
 *
 * Solar arc uses the movement of the progressed Sun to advance
 * all planets by the same amount.
 *
 * @param natal_chart Natal chart
 * @param target_date Date to direct to
 * @param directed_chart Pointer to store directed chart
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_transits_calc_solar_arc(const AstroChart *natal_chart,
                                  const AstroDateTime *target_date,
                                  AstroChart *directed_chart);

/* ========================================================================
 * Transit Analysis
 * ======================================================================== */

/**
 * Transit strength/importance score
 */
typedef struct {
    AstroTransit transit;
    double strength;          /* 0-100 */
    int is_major;            /* Major transit (outer planet to inner) */
    char interpretation[512];
} AstroTransitScore;

/**
 * Analyze and score transits by importance
 *
 * @param transits Array of transits
 * @param num_transits Number of transits
 * @param scores Array to store scored transits
 * @param max_scores Maximum scores to return
 * @param num_scores Pointer to store number of scores
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_transits_analyze_importance(const AstroTransit transits[],
                                      int num_transits,
                                      AstroTransitScore scores[],
                                      int max_scores,
                                      int *num_scores);

/**
 * Get most important transit in a period
 *
 * @param transits Array of transits
 * @param num_transits Number of transits
 * @return Pointer to most important transit, NULL if none
 */
const AstroTransit* astro_transits_get_most_important(const AstroTransit transits[],
                                                       int num_transits);

/**
 * Filter transits by planet
 *
 * @param all_transits Array of all transits
 * @param num_all Number of all transits
 * @param planet Planet to filter by
 * @param filtered Array to store filtered transits
 * @param max_filtered Maximum filtered transits
 * @param num_filtered Pointer to store number filtered
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_transits_filter_by_planet(const AstroTransit all_transits[],
                                    int num_all,
                                    AstroPlanet planet,
                                    AstroTransit filtered[],
                                    int max_filtered,
                                    int *num_filtered);

/**
 * Filter transits by aspect type
 *
 * @param all_transits Array of all transits
 * @param num_all Number of all transits
 * @param aspect_type Aspect to filter by
 * @param filtered Array to store filtered transits
 * @param max_filtered Maximum filtered transits
 * @param num_filtered Pointer to store number filtered
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_transits_filter_by_aspect(const AstroTransit all_transits[],
                                    int num_all,
                                    AstroAspectType aspect_type,
                                    AstroTransit filtered[],
                                    int max_filtered,
                                    int *num_filtered);

/* ========================================================================
 * Transit Output
 * ======================================================================== */

/**
 * Print transit list
 *
 * @param transits Array of transits
 * @param num_transits Number of transits
 * @param fp File pointer
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_transits_print(const AstroTransit transits[],
                        int num_transits,
                        FILE *fp);

/**
 * Format transit as string
 *
 * @param transit Transit to format
 * @param buffer Buffer to store string
 * @param buffer_size Size of buffer
 * @return Formatted string (same as buffer)
 */
char* astro_transits_format(const AstroTransit *transit,
                            char *buffer,
                            int buffer_size);

#ifdef __cplusplus
}
#endif

#endif /* ASTRO_TRANSITS_H */
