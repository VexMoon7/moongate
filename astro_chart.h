/**
 * @file astro_chart.h
 * @brief Natal chart calculation and management module
 *
 * This module provides high-level functions for creating and analyzing
 * complete natal (birth) charts, including planet positions, houses,
 * aspects, and interpretive data.
 */

#ifndef ASTRO_CHART_H
#define ASTRO_CHART_H

#include "astro_types.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================
 * Chart Creation and Calculation
 * ======================================================================== */

/**
 * Create a new natal chart
 *
 * @param datetime Birth date/time
 * @param location Birth location
 * @param config Configuration (NULL for defaults)
 * @param chart Pointer to store chart data
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_chart_create(const AstroDateTime *datetime,
                       const AstroLocation *location,
                       const AstroConfig *config,
                       AstroChart *chart);

/**
 * Calculate all chart data (planets, houses, aspects)
 *
 * @param chart Chart to calculate
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_chart_calculate_all(AstroChart *chart);

/**
 * Update planet positions in chart
 *
 * @param chart Chart to update
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_chart_update_planets(AstroChart *chart);

/**
 * Update house cusps in chart
 *
 * @param chart Chart to update
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_chart_update_houses(AstroChart *chart);

/**
 * Update aspects in chart
 *
 * @param chart Chart to update
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_chart_update_aspects(AstroChart *chart);

/* ========================================================================
 * Chart Queries
 * ======================================================================== */

/**
 * Get planet position by planet type
 *
 * @param chart Chart to query
 * @param planet Planet to find
 * @return Pointer to planet position, NULL if not found
 */
const AstroPlanetPosition* astro_chart_get_planet(const AstroChart *chart,
                                                   AstroPlanet planet);

/**
 * Get all planets in a specific sign
 *
 * @param chart Chart to query
 * @param sign Zodiac sign
 * @param planets Array to store planet positions
 * @param max_planets Maximum planets to return
 * @param num_planets Pointer to store number found
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_chart_get_planets_in_sign(const AstroChart *chart,
                                    AstroSign sign,
                                    const AstroPlanetPosition *planets[],
                                    int max_planets,
                                    int *num_planets);

/**
 * Get all planets in a specific house
 *
 * @param chart Chart to query
 * @param house House number (1-12)
 * @param planets Array to store planet positions
 * @param max_planets Maximum planets to return
 * @param num_planets Pointer to store number found
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_chart_get_planets_in_house(const AstroChart *chart,
                                     int house,
                                     const AstroPlanetPosition *planets[],
                                     int max_planets,
                                     int *num_planets);

/**
 * Get all aspects involving a planet
 *
 * @param chart Chart to query
 * @param planet Planet to find aspects for
 * @param aspects Array to store aspects
 * @param max_aspects Maximum aspects to return
 * @param num_aspects Pointer to store number found
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_chart_get_planet_aspects(const AstroChart *chart,
                                   AstroPlanet planet,
                                   const AstroAspect *aspects[],
                                   int max_aspects,
                                   int *num_aspects);

/* ========================================================================
 * Chart Analysis
 * ======================================================================== */

/**
 * Element distribution in chart
 */
typedef struct {
    int fire_count;
    int earth_count;
    int air_count;
    int water_count;
    AstroElement dominant_element;
} AstroElementBalance;

/**
 * Modality distribution in chart
 */
typedef struct {
    int cardinal_count;
    int fixed_count;
    int mutable_count;
    AstroModality dominant_modality;
} AstroModalityBalance;

/**
 * Analyze element distribution
 *
 * @param chart Chart to analyze
 * @param balance Pointer to store balance data
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_chart_analyze_elements(const AstroChart *chart,
                                 AstroElementBalance *balance);

/**
 * Analyze modality distribution
 *
 * @param chart Chart to analyze
 * @param balance Pointer to store balance data
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_chart_analyze_modalities(const AstroChart *chart,
                                   AstroModalityBalance *balance);

/**
 * Get chart ruler (planet ruling ascendant sign)
 *
 * @param chart Chart to analyze
 * @return Chart ruler planet, or -1 if error
 */
AstroPlanet astro_chart_get_ruler(const AstroChart *chart);

/**
 * Calculate chart shape/pattern (bowl, bucket, bundle, etc.)
 */
typedef enum {
    ASTRO_CHART_SHAPE_SPLASH,      /* Evenly distributed */
    ASTRO_CHART_SHAPE_BUNDLE,      /* All within 120° */
    ASTRO_CHART_SHAPE_LOCOMOTIVE,  /* Empty trine */
    ASTRO_CHART_SHAPE_BOWL,        /* All within 180° */
    ASTRO_CHART_SHAPE_BUCKET,      /* Bowl with handle */
    ASTRO_CHART_SHAPE_SEESAW,      /* Two opposing groups */
    ASTRO_CHART_SHAPE_SPLAY        /* Irregular grouping */
} AstroChartShape;

/**
 * Determine chart shape
 *
 * @param chart Chart to analyze
 * @return Chart shape type
 */
AstroChartShape astro_chart_get_shape(const AstroChart *chart);

/* ========================================================================
 * Chart Comparison
 * ======================================================================== */

/**
 * Synastry data between two charts
 */
typedef struct {
    const AstroChart *chart1;
    const AstroChart *chart2;
    AstroAspect aspects[ASTRO_MAX_ASPECTS];
    int num_aspects;
    double compatibility_score;  /* 0-100 */
} AstroSynastry;

/**
 * Calculate synastry (relationship compatibility)
 *
 * @param chart1 First chart
 * @param chart2 Second chart
 * @param synastry Pointer to store synastry data
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_chart_calc_synastry(const AstroChart *chart1,
                              const AstroChart *chart2,
                              AstroSynastry *synastry);

/**
 * Calculate composite chart (midpoint chart)
 *
 * @param chart1 First chart
 * @param chart2 Second chart
 * @param composite Pointer to store composite chart
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_chart_calc_composite(const AstroChart *chart1,
                               const AstroChart *chart2,
                               AstroChart *composite);

/* ========================================================================
 * Chart Output
 * ======================================================================== */

/**
 * Print chart summary to file
 *
 * @param chart Chart to print
 * @param fp File pointer (stdout, stderr, or file)
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_chart_print(const AstroChart *chart, FILE *fp);

/**
 * Print chart positions table
 *
 * @param chart Chart to print
 * @param fp File pointer
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_chart_print_positions(const AstroChart *chart, FILE *fp);

/**
 * Print aspects table
 *
 * @param chart Chart to print
 * @param fp File pointer
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_chart_print_aspects(const AstroChart *chart, FILE *fp);

/**
 * Print house cusps
 *
 * @param chart Chart to print
 * @param fp File pointer
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_chart_print_houses(const AstroChart *chart, FILE *fp);

/**
 * Export chart to JSON format
 *
 * @param chart Chart to export
 * @param filename Output filename
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_chart_export_json(const AstroChart *chart, const char *filename);

/**
 * Export chart to CSV format
 *
 * @param chart Chart to export
 * @param filename Output filename
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_chart_export_csv(const AstroChart *chart, const char *filename);

#ifdef __cplusplus
}
#endif

#endif /* ASTRO_CHART_H */
