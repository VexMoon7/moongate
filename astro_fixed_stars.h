/**
 * @file astro_fixed_stars.h
 * @brief Fixed star calculations and mythology
 *
 * This module provides access to major fixed stars and their conjunctions
 * with natal planets, adding deep archetypal meaning to chart interpretation.
 */

#ifndef ASTRO_FIXED_STARS_H
#define ASTRO_FIXED_STARS_H

#include "astro_types.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================
 * Major Fixed Stars
 * ======================================================================== */

/**
 * Fixed star identifiers
 */
typedef enum {
    /* Royal Stars (The Four Guardians) */
    FIXED_STAR_ALDEBARAN,    /* Watcher of the East - Archangel Michael */
    FIXED_STAR_REGULUS,      /* Watcher of the North - Archangel Raphael */
    FIXED_STAR_ANTARES,      /* Watcher of the West - Archangel Uriel */
    FIXED_STAR_FOMALHAUT,    /* Watcher of the South - Archangel Gabriel */

    /* First Magnitude Stars */
    FIXED_STAR_SIRIUS,       /* Brightest star - Dog Star */
    FIXED_STAR_CANOPUS,      /* Navigation star */
    FIXED_STAR_ARCTURUS,     /* Bear guardian */
    FIXED_STAR_VEGA,         /* Falling vulture */
    FIXED_STAR_CAPELLA,      /* Little she-goat */
    FIXED_STAR_RIGEL,        /* Orion's foot */
    FIXED_STAR_PROCYON,      /* Little dog */
    FIXED_STAR_BETELGEUSE,   /* Orion's shoulder */
    FIXED_STAR_ALTAIR,       /* Flying eagle */
    FIXED_STAR_SPICA,        /* Wheat shaft */
    FIXED_STAR_POLLUX,       /* Twin - immortal */
    FIXED_STAR_DENEB,        /* Swan's tail */

    /* Mythologically Significant */
    FIXED_STAR_ALGOL,        /* Demon star - Medusa's head */
    FIXED_STAR_ALCYONE,      /* Central Pleiades star */
    FIXED_STAR_PLEIONE,      /* Mother of the Pleiades */
    FIXED_STAR_ALCOR,        /* Test of vision */
    FIXED_STAR_MIZAR,        /* Horse's waist */
    FIXED_STAR_POLARIS,      /* North star */
    FIXED_STAR_ALPHERATZ,    /* Andromeda's head */
    FIXED_STAR_SCHEAT,       /* Fortunate */
    FIXED_STAR_MARKAB,       /* Saddle of Pegasus */
    FIXED_STAR_DENEB_ALGEDI, /* Goat's tail */
    FIXED_STAR_ACHERNAR,     /* River's end */
    FIXED_STAR_HAMAL,        /* Head of ram */
    FIXED_STAR_MENKAR,       /* Whale's jaw */
    FIXED_STAR_CASTOR,       /* Twin - mortal */
    FIXED_STAR_ALNILAM,      /* Orion's belt center */
    FIXED_STAR_ALNITAK,      /* Orion's belt */
    FIXED_STAR_MINTAKA,      /* Orion's belt */

    /* Galactic Center */
    FIXED_STAR_GALACTIC_CENTER, /* Center of Milky Way */

    FIXED_STAR_COUNT
} FixedStarID;

/**
 * Fixed star data
 */
typedef struct {
    FixedStarID id;
    const char *name;
    const char *traditional_name;
    const char *constellation;
    double longitude_2000;        /* Longitude at epoch 2000 */
    double latitude;
    double magnitude;             /* Visual magnitude */
    const char *mythology;        /* Mythological meaning */
    const char *keywords[6];      /* Symbolic keywords */
    const char *nature;          /* Planetary nature (e.g., "Mars-Jupiter") */
    int is_royal_star;           /* 1 if royal star */
    int is_malefic;             /* 1 if traditionally malefic */
    int is_benefic;             /* 1 if traditionally benefic */
} FixedStarData;

/**
 * Fixed star conjunction
 */
typedef struct {
    FixedStarID star_id;
    const char *star_name;
    AstroPlanet planet;
    const char *planet_name;
    double orb;                  /* Orb of conjunction */
    int is_exact;               /* 1 if within 1° */
    const char *interpretation;  /* Brief interpretation */
} FixedStarConjunction;

/* ========================================================================
 * Functions
 * ======================================================================== */

/**
 * Get fixed star data
 *
 * @param star_id Star identifier
 * @return Pointer to star data (static, do not free)
 */
const FixedStarData* astro_fixed_stars_get_data(FixedStarID star_id);

/**
 * Calculate fixed star position for a date
 *
 * @param star_id Star identifier
 * @param datetime Date for calculation
 * @param longitude Pointer to store longitude
 * @param latitude Pointer to store latitude
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_fixed_stars_calc_position(FixedStarID star_id,
                                   const AstroDateTime *datetime,
                                   double *longitude,
                                   double *latitude);

/**
 * Find fixed star conjunctions in a chart
 *
 * @param chart Chart to analyze
 * @param orb Maximum orb for conjunction (typically 1-2°)
 * @param conjunctions Array to store conjunctions
 * @param max_conjunctions Maximum conjunctions to find
 * @param num_conjunctions Pointer to store count
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_fixed_stars_find_conjunctions(const AstroChart *chart,
                                       double orb,
                                       FixedStarConjunction conjunctions[],
                                       int max_conjunctions,
                                       int *num_conjunctions);

/**
 * Find fixed star on a specific degree
 *
 * @param longitude Ecliptic longitude
 * @param datetime Date for precession correction
 * @param orb Maximum orb
 * @param star_id Pointer to store star ID
 * @return 1 if star found, 0 otherwise
 */
int astro_fixed_stars_find_at_degree(double longitude,
                                    const AstroDateTime *datetime,
                                    double orb,
                                    FixedStarID *star_id);

/**
 * Calculate current position of all major fixed stars
 *
 * @param datetime Date for calculation
 * @param positions Array to store positions (longitude/latitude pairs)
 * @param max_stars Maximum stars to calculate
 * @param num_stars Pointer to store count
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_fixed_stars_calc_all_positions(const AstroDateTime *datetime,
                                        double positions[][2],
                                        int max_stars,
                                        int *num_stars);

/**
 * Get fixed star by name
 *
 * @param name Star name (case-insensitive)
 * @return Star ID, or -1 if not found
 */
int astro_fixed_stars_get_by_name(const char *name);

/**
 * Get Royal Stars conjunctions
 *
 * @param chart Chart to analyze
 * @param orb Maximum orb
 * @param conjunctions Array to store conjunctions
 * @param max_conjunctions Maximum conjunctions
 * @param num_conjunctions Pointer to store count
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_fixed_stars_find_royal_stars(const AstroChart *chart,
                                       double orb,
                                       FixedStarConjunction conjunctions[],
                                       int max_conjunctions,
                                       int *num_conjunctions);

/**
 * Get interpretation for star-planet conjunction
 *
 * @param star_id Fixed star
 * @param planet Planet
 * @param buffer Buffer to store interpretation
 * @param buffer_size Size of buffer
 * @return Formatted interpretation
 */
char* astro_fixed_stars_interpret_conjunction(FixedStarID star_id,
                                              AstroPlanet planet,
                                              char *buffer,
                                              int buffer_size);

/**
 * Check if point is on Galactic Center
 *
 * The Galactic Center (26°+ Sagittarius) is spiritually significant.
 *
 * @param longitude Ecliptic longitude
 * @param datetime Date for calculation
 * @param orb Maximum orb (typically 1-2°)
 * @return 1 if conjunct Galactic Center, 0 otherwise
 */
int astro_fixed_stars_is_on_galactic_center(double longitude,
                                            const AstroDateTime *datetime,
                                            double orb);

/**
 * Find Pleiades conjunctions
 *
 * The Pleiades (Seven Sisters) are mythologically significant.
 *
 * @param chart Chart to analyze
 * @param orb Maximum orb
 * @param has_conjunction Pointer to store result
 * @param planet Pointer to store conjunct planet
 * @return ASTRO_OK on success
 */
int astro_fixed_stars_find_pleiades(const AstroChart *chart,
                                   double orb,
                                   int *has_conjunction,
                                   AstroPlanet *planet);

/* ========================================================================
 * Output Functions
 * ======================================================================== */

/**
 * Print fixed star conjunctions
 *
 * @param conjunctions Array of conjunctions
 * @param num_conjunctions Number of conjunctions
 * @param fp File pointer
 */
void astro_fixed_stars_print_conjunctions(const FixedStarConjunction conjunctions[],
                                         int num_conjunctions,
                                         FILE *fp);

/**
 * Print fixed star report for chart
 *
 * @param chart Chart to analyze
 * @param orb Maximum orb for conjunctions
 * @param fp File pointer
 * @return ASTRO_OK on success
 */
int astro_fixed_stars_print_report(const AstroChart *chart, double orb, FILE *fp);

/**
 * Format conjunction as string
 *
 * @param conjunction Conjunction to format
 * @param buffer Buffer to store string
 * @param buffer_size Size of buffer
 * @return Formatted string
 */
char* astro_fixed_stars_format_conjunction(const FixedStarConjunction *conjunction,
                                          char *buffer,
                                          int buffer_size);

#ifdef __cplusplus
}
#endif

#endif /* ASTRO_FIXED_STARS_H */
