/**
 * @file astro_aspects.h
 * @brief Aspect calculation and analysis module
 *
 * This module provides functions for calculating and analyzing astrological
 * aspects between planets, including orb calculations, applying/separating
 * aspects, and aspect pattern recognition.
 */

#ifndef ASTRO_ASPECTS_H
#define ASTRO_ASPECTS_H

#include "astro_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================
 * Aspect Configuration
 * ======================================================================== */

/**
 * Aspect orb configuration
 */
typedef struct {
    AstroAspectType aspect_type;
    double default_orb;      /* Default orb in degrees */
    double tight_orb;        /* Tight orb for exact aspects */
    int is_major;            /* 1 for major aspects, 0 for minor */
} AstroAspectConfig;

/**
 * Set custom orb for an aspect type
 *
 * @param aspect_type Type of aspect
 * @param orb Orb in degrees
 */
void astro_aspects_set_orb(AstroAspectType aspect_type, double orb);

/**
 * Get orb for an aspect type
 *
 * @param aspect_type Type of aspect
 * @return Orb in degrees
 */
double astro_aspects_get_orb(AstroAspectType aspect_type);

/**
 * Reset all orbs to default values
 */
void astro_aspects_reset_orbs(void);

/* ========================================================================
 * Aspect Calculations
 * ======================================================================== */

/**
 * Calculate aspect between two planets
 *
 * @param pos1 First planet position
 * @param pos2 Second planet position
 * @param aspect Pointer to store aspect data
 * @return ASTRO_OK if aspect found, error code otherwise
 */
int astro_aspects_calc_aspect(const AstroPlanetPosition *pos1,
                              const AstroPlanetPosition *pos2,
                              AstroAspect *aspect);

/**
 * Calculate all aspects in a chart
 *
 * @param positions Array of planet positions
 * @param num_planets Number of planets
 * @param aspects Array to store aspects
 * @param max_aspects Maximum number of aspects to store
 * @param num_aspects Pointer to store number of aspects found
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_aspects_calc_all(const AstroPlanetPosition positions[],
                           int num_planets,
                           AstroAspect aspects[],
                           int max_aspects,
                           int *num_aspects);

/**
 * Calculate aspects between two sets of positions (e.g., natal and transit)
 *
 * @param natal_positions Natal planet positions
 * @param num_natal Number of natal planets
 * @param transit_positions Transit planet positions
 * @param num_transit Number of transit planets
 * @param aspects Array to store aspects
 * @param max_aspects Maximum number of aspects
 * @param num_aspects Pointer to store number of aspects found
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_aspects_calc_between_charts(const AstroPlanetPosition natal_positions[],
                                      int num_natal,
                                      const AstroPlanetPosition transit_positions[],
                                      int num_transit,
                                      AstroAspect aspects[],
                                      int max_aspects,
                                      int *num_aspects);

/**
 * Check if two longitudes form an aspect
 *
 * @param lon1 First longitude
 * @param lon2 Second longitude
 * @param aspect_type Pointer to store aspect type
 * @param orb Pointer to store orb value
 * @return 1 if aspect found, 0 otherwise
 */
int astro_aspects_check_aspect(double lon1, double lon2,
                               AstroAspectType *aspect_type,
                               double *orb);

/**
 * Determine if aspect is applying or separating
 *
 * @param pos1 First planet position (with speed)
 * @param pos2 Second planet position (with speed)
 * @param aspect Aspect data
 * @return 1 if applying, 0 if separating, -1 if stationary
 */
int astro_aspects_is_applying(const AstroPlanetPosition *pos1,
                              const AstroPlanetPosition *pos2,
                              const AstroAspect *aspect);

/* ========================================================================
 * Aspect Patterns
 * ======================================================================== */

/**
 * Aspect pattern types
 */
typedef enum {
    ASTRO_PATTERN_GRAND_TRINE,      /* Three planets in trine */
    ASTRO_PATTERN_T_SQUARE,         /* Three planets: two opposition, both square third */
    ASTRO_PATTERN_GRAND_CROSS,      /* Four planets in square and opposition */
    ASTRO_PATTERN_YOD,              /* Two planets sextile, both quincunx third */
    ASTRO_PATTERN_KITE,             /* Grand trine with fourth planet opposite one point */
    ASTRO_PATTERN_STELLIUM,         /* Three or more planets in same sign/house */
    ASTRO_PATTERN_MYSTIC_RECTANGLE  /* Two oppositions sextile/trine */
} AstroPatternType;

/**
 * Aspect pattern structure
 */
typedef struct {
    AstroPatternType pattern_type;
    AstroPlanet planets[6];          /* Planets involved */
    int num_planets;
    AstroElement element;            /* For elemental patterns */
    char description[256];
} AstroPattern;

/**
 * Find aspect patterns in a chart
 *
 * @param positions Planet positions
 * @param num_planets Number of planets
 * @param aspects Calculated aspects
 * @param num_aspects Number of aspects
 * @param patterns Array to store patterns
 * @param max_patterns Maximum patterns to find
 * @param num_patterns Pointer to store number of patterns found
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_aspects_find_patterns(const AstroPlanetPosition positions[],
                                int num_planets,
                                const AstroAspect aspects[],
                                int num_aspects,
                                AstroPattern patterns[],
                                int max_patterns,
                                int *num_patterns);

/* ========================================================================
 * Aspect Interpretation
 * ======================================================================== */

/**
 * Get aspect name
 *
 * @param aspect_type Aspect type
 * @return Aspect name (static, do not free)
 */
const char* astro_aspects_get_name(AstroAspectType aspect_type);

/**
 * Get aspect symbol
 *
 * @param aspect_type Aspect type
 * @return Aspect symbol (static, do not free)
 */
const char* astro_aspects_get_symbol(AstroAspectType aspect_type);

/**
 * Check if aspect is harmonious
 *
 * @param aspect_type Aspect type
 * @return 1 if harmonious, 0 if challenging, -1 if neutral
 */
int astro_aspects_is_harmonious(AstroAspectType aspect_type);

/**
 * Get aspect strength (based on orb)
 *
 * @param aspect Aspect data
 * @return Strength value (0.0 to 1.0)
 */
double astro_aspects_get_strength(const AstroAspect *aspect);

/**
 * Format aspect as string
 *
 * @param aspect Aspect data
 * @param positions Planet positions for names
 * @param buffer Buffer to store formatted string
 * @param buffer_size Size of buffer
 * @return Formatted string (same as buffer)
 */
char* astro_aspects_format(const AstroAspect *aspect,
                           const AstroPlanetPosition positions[],
                           char *buffer,
                           int buffer_size);

#ifdef __cplusplus
}
#endif

#endif /* ASTRO_ASPECTS_H */
