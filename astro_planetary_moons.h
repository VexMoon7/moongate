/**
 * @file astro_planetary_moons.h
 * @brief Planetary satellite (moon) calculations with mythological associations
 *
 * This module provides comprehensive support for 27 planetary moons across
 * Mars, Jupiter, Saturn, Uranus, Neptune, and Pluto. Each moon carries
 * mythological significance and archetypal meaning for astrological interpretation.
 *
 * UNIQUE FEATURE: One of the few astrological libraries to include planetary
 * moons with full mythological and archetypal associations!
 */

#ifndef ASTRO_PLANETARY_MOONS_H
#define ASTRO_PLANETARY_MOONS_H

#include "astro_types.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================
 * Planetary Moon Identifiers
 * ======================================================================== */

/**
 * Planetary moon identifiers (Swiss Ephemeris ID numbers)
 */
typedef enum {
    /* Mars Moons - Fear & Terror */
    MOON_PHOBOS = 9401,        /* Fear, panic, primal terror */
    MOON_DEIMOS = 9402,        /* Dread, slow-building anxiety */

    /* Jupiter Moons - Divine Drama & Transformation */
    MOON_IO = 9501,            /* Volcanic passion, suffering, transformation */
    MOON_EUROPA = 9502,        /* Hidden depths, forbidden love, mystery */
    MOON_GANYMEDE = 9503,      /* Divine service, elevation, beauty */
    MOON_CALLISTO = 9504,      /* Wilderness, independence, transformation */

    /* Saturn Moons - Structure & Boundaries */
    MOON_MIMAS = 9601,         /* Impact, survival, resilience */
    MOON_ENCELADUS = 9602,     /* Hidden life, underground currents */
    MOON_TETHYS = 9603,        /* Ocean goddess, nourishment, flow */
    MOON_DIONE = 9604,         /* Divine feminine, oracular wisdom */
    MOON_RHEA = 9605,          /* Mother goddess, fertility, earth */
    MOON_TITAN = 9606,         /* Primordial power, atmospheric depth */
    MOON_HYPERION = 9607,      /* Light, watching, observation */
    MOON_IAPETUS = 9608,       /* Duality, piercing, mortality */

    /* Uranus Moons - Shakespearean Archetypes */
    MOON_ARIEL = 9701,         /* Air spirit, mischief, magic */
    MOON_UMBRIEL = 9702,       /* Shadow, melancholy, darkness */
    MOON_TITANIA = 9703,       /* Fairy queen, dreams, enchantment */
    MOON_OBERON = 9704,        /* Fairy king, authority, testing */
    MOON_MIRANDA = 9705,       /* Wonder, suffering, transformation */

    /* Neptune Moons - Oceanic Depths */
    MOON_TRITON = 9801,        /* Messenger of the sea, trumpeter */
    MOON_NEREID = 9802,        /* Sea nymph, dancing, play */
    MOON_PROTEUS = 9808,       /* Shape-shifter, evasion, prophecy */

    /* Pluto Moons - Underworld Mysteries */
    MOON_CHARON = 9901,        /* Ferryman, transition, threshold */
    MOON_NIX = 9902,           /* Night, darkness, mother of Charon */
    MOON_HYDRA = 9903,         /* Multi-headed, regeneration, guardian */
    MOON_KERBEROS = 9904,      /* Guardian, threshold, three-headed */
    MOON_STYX = 9905,          /* River of oath, binding, invincibility */

    /* System Centers (Center of Body) */
    MOON_JUPITER_COB = 9599,
    MOON_SATURN_COB = 9699,
    MOON_URANUS_COB = 9799,
    MOON_NEPTUNE_COB = 9899,
    MOON_PLUTO_COB = 9999
} PlanetaryMoonID;

/**
 * Planetary moon data with mythological associations
 */
typedef struct {
    PlanetaryMoonID id;
    const char *name;
    AstroPlanet parent_planet;
    const char *parent_name;

    /* Mythological Information */
    const char *mythology;           /* Full mythological story */
    const char *archetype;          /* Core archetypal meaning */
    const char *keywords[8];        /* Symbolic keywords */

    /* Astrological Significance */
    const char *astrological_meaning;
    const char *psychological_theme;
    const char *shadow_expression;
    const char *gift_expression;

    /* Physical Characteristics (for context) */
    double orbital_period_days;     /* Orbital period around parent */
    int discovery_year;
    const char *discoverer;
} PlanetaryMoonData;

/**
 * Planetary moon position
 */
typedef struct {
    PlanetaryMoonID moon_id;
    const char *moon_name;
    AstroPlanetPosition position;
    double orbital_phase;           /* 0-360° in orbit around parent */
    int is_eclipsed;               /* 1 if in planet's shadow */
    const PlanetaryMoonData *data;
} PlanetaryMoonPosition;

/**
 * Moon-planet conjunction
 */
typedef struct {
    PlanetaryMoonID moon_id;
    const char *moon_name;
    AstroPlanet planet;
    const char *planet_name;
    double orb;
    int is_exact;                  /* Within 1° */
    const char *interpretation;
} MoonPlanetConjunction;

/* ========================================================================
 * Core Functions
 * ======================================================================== */

/**
 * Get planetary moon data
 *
 * @param moon_id Moon identifier
 * @return Pointer to moon data (static, do not free)
 */
const PlanetaryMoonData* astro_moons_get_data(PlanetaryMoonID moon_id);

/**
 * Calculate planetary moon position
 *
 * @param moon_id Moon identifier
 * @param datetime Date/time for calculation
 * @param position Pointer to store moon position
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_moons_calc_position(PlanetaryMoonID moon_id,
                              const AstroDateTime *datetime,
                              PlanetaryMoonPosition *position);

/**
 * Calculate all planetary moon positions
 *
 * @param datetime Date/time for calculation
 * @param positions Array to store positions
 * @param max_moons Maximum moons to calculate
 * @param num_moons Pointer to store count
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_moons_calc_all_positions(const AstroDateTime *datetime,
                                   PlanetaryMoonPosition positions[],
                                   int max_moons,
                                   int *num_moons);

/**
 * Get moons for a specific planetary system
 *
 * @param parent_planet Parent planet (Mars, Jupiter, Saturn, etc.)
 * @param datetime Date/time for calculation
 * @param positions Array to store moon positions
 * @param max_moons Maximum moons to calculate
 * @param num_moons Pointer to store count
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_moons_get_system_moons(AstroPlanet parent_planet,
                                 const AstroDateTime *datetime,
                                 PlanetaryMoonPosition positions[],
                                 int max_moons,
                                 int *num_moons);

/* ========================================================================
 * Conjunctions & Aspects
 * ======================================================================== */

/**
 * Find conjunctions between planetary moons and natal planets
 *
 * @param chart Natal chart
 * @param datetime Date for moon positions
 * @param orb Maximum orb (typically 1-2°)
 * @param conjunctions Array to store conjunctions
 * @param max_conjunctions Maximum conjunctions
 * @param num_conjunctions Pointer to store count
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_moons_find_conjunctions(const AstroChart *chart,
                                  const AstroDateTime *datetime,
                                  double orb,
                                  MoonPlanetConjunction conjunctions[],
                                  int max_conjunctions,
                                  int *num_conjunctions);

/**
 * Find significant moon transits
 *
 * Looks for planetary moons aspecting natal chart positions.
 *
 * @param chart Natal chart
 * @param datetime Transit date
 * @param orb Maximum orb
 * @param transits Array to store transit aspects
 * @param max_transits Maximum transits
 * @param num_transits Pointer to store count
 * @return ASTRO_OK on success
 */
int astro_moons_find_transits(const AstroChart *chart,
                              const AstroDateTime *datetime,
                              double orb,
                              MoonPlanetConjunction transits[],
                              int max_transits,
                              int *num_transits);

/**
 * Find moon-moon aspects in chart
 *
 * Analyzes relationships between different planetary moons.
 *
 * @param datetime Date for calculation
 * @param aspects Array to store aspects
 * @param max_aspects Maximum aspects
 * @param num_aspects Pointer to store count
 * @return ASTRO_OK on success
 */
int astro_moons_find_moon_aspects(const AstroDateTime *datetime,
                                  AstroAspect aspects[],
                                  int max_aspects,
                                  int *num_aspects);

/* ========================================================================
 * Specialized Analysis
 * ======================================================================== */

/**
 * Get Jupiter's Galilean moons (Io, Europa, Ganymede, Callisto)
 *
 * These four moons are especially significant mythologically.
 *
 * @param datetime Date for calculation
 * @param positions Array to store 4 moon positions
 * @return ASTRO_OK on success
 */
int astro_moons_get_galilean_moons(const AstroDateTime *datetime,
                                   PlanetaryMoonPosition positions[4]);

/**
 * Get Saturn's major moons (Titan, Rhea, Iapetus, Dione, Tethys, Enceladus)
 *
 * @param datetime Date for calculation
 * @param positions Array to store moon positions
 * @param max_moons Maximum moons (should be at least 6)
 * @param num_moons Pointer to store count
 * @return ASTRO_OK on success
 */
int astro_moons_get_saturn_major_moons(const AstroDateTime *datetime,
                                       PlanetaryMoonPosition positions[],
                                       int max_moons,
                                       int *num_moons);

/**
 * Get Pluto's moons (all related to underworld mythology)
 *
 * @param datetime Date for calculation
 * @param positions Array to store moon positions
 * @param max_moons Maximum moons (should be at least 5)
 * @param num_moons Pointer to store count
 * @return ASTRO_OK on success
 */
int astro_moons_get_pluto_moons(const AstroDateTime *datetime,
                                PlanetaryMoonPosition positions[],
                                int max_moons,
                                int *num_moons);

/**
 * Analyze moon orbital phase
 *
 * Returns the moon's position in its orbit around parent planet.
 * Useful for timing and phase analysis.
 *
 * @param moon_id Moon identifier
 * @param datetime Date for calculation
 * @param phase Pointer to store orbital phase (0-360°)
 * @param phase_name Pointer to store phase name (New, Waxing, Full, Waning)
 * @return ASTRO_OK on success
 */
int astro_moons_orbital_phase(PlanetaryMoonID moon_id,
                              const AstroDateTime *datetime,
                              double *phase,
                              const char **phase_name);

/* ========================================================================
 * Moon Mythology & Interpretation
 * ======================================================================== */

/**
 * Get mythological story for moon
 *
 * @param moon_id Moon identifier
 * @return Full mythological story (static, do not free)
 */
const char* astro_moons_get_mythology(PlanetaryMoonID moon_id);

/**
 * Get archetypal interpretation
 *
 * @param moon_id Moon identifier
 * @return Core archetypal meaning (static, do not free)
 */
const char* astro_moons_get_archetype(PlanetaryMoonID moon_id);

/**
 * Interpret moon conjunction with natal planet
 *
 * @param moon_id Planetary moon
 * @param planet Natal planet
 * @param buffer Buffer to store interpretation
 * @param buffer_size Size of buffer
 * @return Formatted interpretation
 */
char* astro_moons_interpret_conjunction(PlanetaryMoonID moon_id,
                                        AstroPlanet planet,
                                        char *buffer,
                                        int buffer_size);

/**
 * Get shadow expression of moon
 *
 * Describes the challenging or difficult manifestation of the moon's energy.
 *
 * @param moon_id Moon identifier
 * @return Shadow expression (static, do not free)
 */
const char* astro_moons_get_shadow(PlanetaryMoonID moon_id);

/**
 * Get gift expression of moon
 *
 * Describes the positive or integrated manifestation of the moon's energy.
 *
 * @param moon_id Moon identifier
 * @return Gift expression (static, do not free)
 */
const char* astro_moons_get_gift(PlanetaryMoonID moon_id);

/* ========================================================================
 * Search & Query Functions
 * ======================================================================== */

/**
 * Get moon by name (case-insensitive)
 *
 * @param name Moon name
 * @return Moon ID, or -1 if not found
 */
int astro_moons_get_by_name(const char *name);

/**
 * Get all moons for parent planet
 *
 * @param parent_planet Parent planet
 * @param moon_ids Array to store moon IDs
 * @param max_moons Maximum moons
 * @param num_moons Pointer to store count
 * @return ASTRO_OK on success
 */
int astro_moons_get_for_parent(AstroPlanet parent_planet,
                               PlanetaryMoonID moon_ids[],
                               int max_moons,
                               int *num_moons);

/**
 * Search moons by keyword
 *
 * Searches through keywords and mythological associations.
 *
 * @param keyword Keyword to search for
 * @param moon_ids Array to store matching moon IDs
 * @param max_moons Maximum moons
 * @param num_moons Pointer to store count
 * @return ASTRO_OK on success
 */
int astro_moons_search_by_keyword(const char *keyword,
                                  PlanetaryMoonID moon_ids[],
                                  int max_moons,
                                  int *num_moons);

/* ========================================================================
 * Output Functions
 * ======================================================================== */

/**
 * Print planetary moon position
 *
 * @param position Moon position
 * @param fp File pointer
 */
void astro_moons_print_position(const PlanetaryMoonPosition *position, FILE *fp);

/**
 * Print all planetary moons report
 *
 * @param datetime Date for calculation
 * @param fp File pointer
 * @return ASTRO_OK on success
 */
int astro_moons_print_report(const AstroDateTime *datetime, FILE *fp);

/**
 * Print moons for specific planetary system
 *
 * @param parent_planet Parent planet
 * @param datetime Date for calculation
 * @param fp File pointer
 * @return ASTRO_OK on success
 */
int astro_moons_print_system(AstroPlanet parent_planet,
                             const AstroDateTime *datetime,
                             FILE *fp);

/**
 * Print moon conjunctions report
 *
 * @param chart Natal chart
 * @param datetime Date for moon positions
 * @param orb Maximum orb
 * @param fp File pointer
 * @return ASTRO_OK on success
 */
int astro_moons_print_conjunctions(const AstroChart *chart,
                                   const AstroDateTime *datetime,
                                   double orb,
                                   FILE *fp);

/**
 * Format moon position as string
 *
 * @param position Moon position
 * @param buffer Buffer to store string
 * @param buffer_size Size of buffer
 * @return Formatted string
 */
char* astro_moons_format_position(const PlanetaryMoonPosition *position,
                                  char *buffer,
                                  int buffer_size);

#ifdef __cplusplus
}
#endif

#endif /* ASTRO_PLANETARY_MOONS_H */
