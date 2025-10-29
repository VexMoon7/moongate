/**
 * @file astro_sabian.h
 * @brief Sabian Symbol system for degree-based interpretation
 *
 * The Sabian Symbols are a set of 360 symbolic images, one for each
 * degree of the zodiac. Created by Marc Edmund Jones and Elsie Wheeler
 * in 1925, they provide poetic, archetypal imagery for chart interpretation.
 */

#ifndef ASTRO_SABIAN_H
#define ASTRO_SABIAN_H

#include "astro_types.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================
 * Sabian Symbol Structure
 * ======================================================================== */

/**
 * Sabian symbol for a specific degree
 */
typedef struct {
    int degree;                    /* 1-360 */
    AstroSign sign;               /* Zodiac sign */
    int degree_in_sign;           /* 1-30 */
    const char *symbol;           /* The symbolic image */
    const char *keyword;          /* Key theme */
    const char *interpretation;   /* Brief interpretation */
    const char *archetype;        /* Archetypal meaning */
} SabianSymbol;

/* ========================================================================
 * Functions
 * ======================================================================== */

/**
 * Get Sabian symbol for a degree
 *
 * @param longitude Ecliptic longitude (0-360)
 * @return Pointer to Sabian symbol (static, do not free)
 */
const SabianSymbol* astro_sabian_get_symbol(double longitude);

/**
 * Get Sabian symbol for a specific sign and degree
 *
 * @param sign Zodiac sign
 * @param degree Degree within sign (1-30)
 * @return Pointer to Sabian symbol (static, do not free)
 */
const SabianSymbol* astro_sabian_get_by_sign(AstroSign sign, int degree);

/**
 * Get Sabian symbol for an absolute degree (1-360)
 *
 * @param degree Absolute degree (1-360)
 * @return Pointer to Sabian symbol (static, do not free)
 */
const SabianSymbol* astro_sabian_get_by_degree(int degree);

/**
 * Get Sabian symbols for all planets in a chart
 *
 * @param chart Chart to analyze
 * @param symbols Array to store symbols
 * @param max_symbols Maximum symbols to return
 * @param num_symbols Pointer to store count
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_sabian_get_chart_symbols(const AstroChart *chart,
                                   SabianSymbol symbols[],
                                   int max_symbols,
                                   int *num_symbols);

/**
 * Get Sabian symbol for planet position
 *
 * @param position Planet position
 * @return Pointer to Sabian symbol (static, do not free)
 */
const SabianSymbol* astro_sabian_get_for_planet(const AstroPlanetPosition *position);

/**
 * Format Sabian symbol as string
 *
 * @param symbol Sabian symbol
 * @param buffer Buffer to store formatted string
 * @param buffer_size Size of buffer
 * @return Formatted string (same as buffer)
 */
char* astro_sabian_format(const SabianSymbol *symbol, char *buffer, int buffer_size);

/**
 * Print Sabian symbol
 *
 * @param symbol Symbol to print
 * @param fp File pointer
 */
void astro_sabian_print(const SabianSymbol *symbol, FILE *fp);

/**
 * Search Sabian symbols by keyword
 *
 * @param keyword Keyword to search for
 * @param symbols Array to store matching symbols
 * @param max_symbols Maximum symbols to return
 * @param num_symbols Pointer to store count
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_sabian_search(const char *keyword,
                       SabianSymbol symbols[],
                       int max_symbols,
                       int *num_symbols);

/**
 * Get Sabian symbol interpretation for a chart point
 *
 * Provides detailed interpretation of the Sabian symbol for a specific
 * chart point, considering the planet and house placement.
 *
 * @param position Planet position
 * @param house House number (1-12)
 * @param buffer Buffer to store interpretation
 * @param buffer_size Size of buffer
 * @return Formatted interpretation
 */
char* astro_sabian_interpret_point(const AstroPlanetPosition *position,
                                   int house,
                                   char *buffer,
                                   int buffer_size);

/* ========================================================================
 * Sabian Symbol Groups
 * ======================================================================== */

/**
 * Get symbols for critical degrees (0°, 13°, 26° of cardinal signs)
 *
 * @param chart Chart to analyze
 * @param symbols Array to store symbols
 * @param max_symbols Maximum symbols
 * @param num_symbols Pointer to store count
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_sabian_get_critical_degrees(const AstroChart *chart,
                                      SabianSymbol symbols[],
                                      int max_symbols,
                                      int *num_symbols);

/**
 * Get symbols for important chart angles (ASC, MC, DSC, IC)
 *
 * @param houses House data
 * @param symbols Array to store symbols
 * @param max_symbols Maximum symbols (should be at least 4)
 * @param num_symbols Pointer to store count
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_sabian_get_angles(const AstroHouses *houses,
                            SabianSymbol symbols[],
                            int max_symbols,
                            int *num_symbols);

#ifdef __cplusplus
}
#endif

#endif /* ASTRO_SABIAN_H */
