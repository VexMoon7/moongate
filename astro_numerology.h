/**
 * @file astro_numerology.h
 * @brief Numerology calculations integrated with astrology
 *
 * This module provides Pythagorean and Chaldean numerology calculations,
 * integrated with astrological birth data for comprehensive analysis.
 */

#ifndef ASTRO_NUMEROLOGY_H
#define ASTRO_NUMEROLOGY_H

#include "astro_types.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================
 * Numerology Data Structures
 * ======================================================================== */

/**
 * Numerology profile
 */
typedef struct {
    int life_path;           /* Life path number (1-9, 11, 22, 33) */
    int destiny;            /* Destiny/expression number */
    int soul_urge;          /* Soul urge/heart's desire number */
    int personality;        /* Personality number */
    int maturity;          /* Maturity number (Life Path + Destiny) */
    int birthday;          /* Birthday number (day of month) */
    int challenge[4];      /* Challenge numbers */
    int pinnacle[4];       /* Pinnacle numbers */
    int personal_year;     /* Current personal year */
    int personal_month;    /* Current personal month */
    int personal_day;      /* Current personal day */
} NumerologyProfile;

/**
 * Numerology system type
 */
typedef enum {
    NUMEROLOGY_PYTHAGOREAN,  /* Standard Western numerology */
    NUMEROLOGY_CHALDEAN      /* Ancient Chaldean system */
} NumerologySystem;

/* ========================================================================
 * Core Number Calculations
 * ======================================================================== */

/**
 * Calculate life path number from birth date
 *
 * @param datetime Birth date/time
 * @return Life path number (1-9, 11, 22, 33)
 */
int astro_numerology_life_path(const AstroDateTime *datetime);

/**
 * Calculate destiny number from full name
 *
 * @param full_name Full birth name
 * @param system Numerology system to use
 * @return Destiny number (1-9, 11, 22, 33)
 */
int astro_numerology_destiny(const char *full_name, NumerologySystem system);

/**
 * Calculate soul urge number from vowels in name
 *
 * @param full_name Full birth name
 * @param system Numerology system to use
 * @return Soul urge number (1-9, 11, 22, 33)
 */
int astro_numerology_soul_urge(const char *full_name, NumerologySystem system);

/**
 * Calculate personality number from consonants in name
 *
 * @param full_name Full birth name
 * @param system Numerology system to use
 * @return Personality number (1-9, 11, 22, 33)
 */
int astro_numerology_personality(const char *full_name, NumerologySystem system);

/**
 * Calculate birthday number
 *
 * @param datetime Birth date
 * @return Birthday number (day of month, reduced if > 31)
 */
int astro_numerology_birthday(const AstroDateTime *datetime);

/**
 * Calculate personal year number
 *
 * @param birth_date Birth date
 * @param current_date Current date
 * @return Personal year number (1-9)
 */
int astro_numerology_personal_year(const AstroDateTime *birth_date,
                                   const AstroDateTime *current_date);

/**
 * Calculate personal month number
 *
 * @param birth_date Birth date
 * @param current_date Current date
 * @return Personal month number (1-9)
 */
int astro_numerology_personal_month(const AstroDateTime *birth_date,
                                    const AstroDateTime *current_date);

/**
 * Calculate personal day number
 *
 * @param birth_date Birth date
 * @param current_date Current date
 * @return Personal day number (1-9)
 */
int astro_numerology_personal_day(const AstroDateTime *birth_date,
                                  const AstroDateTime *current_date);

/* ========================================================================
 * Advanced Numbers
 * ======================================================================== */

/**
 * Calculate maturity number (Life Path + Destiny)
 *
 * @param life_path Life path number
 * @param destiny Destiny number
 * @return Maturity number
 */
int astro_numerology_maturity(int life_path, int destiny);

/**
 * Calculate challenge numbers
 *
 * Challenge numbers represent lessons to learn at different life stages.
 *
 * @param datetime Birth date
 * @param challenges Array to store 4 challenge numbers
 * @return ASTRO_OK on success
 */
int astro_numerology_challenges(const AstroDateTime *datetime, int challenges[4]);

/**
 * Calculate pinnacle numbers
 *
 * Pinnacle numbers represent major life periods and their themes.
 *
 * @param datetime Birth date
 * @param pinnacles Array to store 4 pinnacle numbers
 * @return ASTRO_OK on success
 */
int astro_numerology_pinnacles(const AstroDateTime *datetime, int pinnacles[4]);

/* ========================================================================
 * Complete Profile
 * ======================================================================== */

/**
 * Calculate complete numerology profile
 *
 * @param full_name Full birth name
 * @param birth_date Birth date
 * @param current_date Current date (for personal year/month/day)
 * @param system Numerology system to use
 * @param profile Pointer to store profile
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_numerology_calc_profile(const char *full_name,
                                  const AstroDateTime *birth_date,
                                  const AstroDateTime *current_date,
                                  NumerologySystem system,
                                  NumerologyProfile *profile);

/* ========================================================================
 * Compatibility
 * ======================================================================== */

/**
 * Calculate numerology compatibility between two people
 *
 * @param profile1 First person's numerology profile
 * @param profile2 Second person's numerology profile
 * @return Compatibility score (0-100)
 */
double astro_numerology_compatibility(const NumerologyProfile *profile1,
                                     const NumerologyProfile *profile2);

/**
 * Analyze life path compatibility
 *
 * @param life_path1 First person's life path
 * @param life_path2 Second person's life path
 * @return Compatibility description
 */
const char* astro_numerology_life_path_compatibility(int life_path1, int life_path2);

/* ========================================================================
 * Interpretation
 * ======================================================================== */

/**
 * Get meaning of a number
 *
 * @param number Number to interpret (1-9, 11, 22, 33)
 * @return Meaning/description (static, do not free)
 */
const char* astro_numerology_get_meaning(int number);

/**
 * Get keywords for a number
 *
 * @param number Number to interpret
 * @param keywords Array to store keyword pointers
 * @param max_keywords Maximum keywords to return
 * @param num_keywords Pointer to store count
 * @return ASTRO_OK on success
 */
int astro_numerology_get_keywords(int number,
                                 const char *keywords[],
                                 int max_keywords,
                                 int *num_keywords);

/**
 * Check if number is a master number
 *
 * @param number Number to check
 * @return 1 if master number (11, 22, 33), 0 otherwise
 */
int astro_numerology_is_master_number(int number);

/* ========================================================================
 * Integration with Astrology
 * ======================================================================== */

/**
 * Find astrological correlations with numerology
 *
 * Maps numerology numbers to planetary energies and zodiac signs.
 *
 * @param number Numerology number
 * @param planet Pointer to store correlated planet
 * @param sign Pointer to store correlated sign
 * @return ASTRO_OK on success
 */
int astro_numerology_planetary_correlation(int number,
                                          AstroPlanet *planet,
                                          AstroSign *sign);

/**
 * Calculate combined astro-numerology score
 *
 * Integrates astrological chart with numerology profile for
 * deeper insight into life themes and compatibility.
 *
 * @param chart Astrological chart
 * @param profile Numerology profile
 * @param buffer Buffer to store interpretation
 * @param buffer_size Size of buffer
 * @return Formatted interpretation
 */
char* astro_numerology_integrated_reading(const AstroChart *chart,
                                         const NumerologyProfile *profile,
                                         char *buffer,
                                         int buffer_size);

/* ========================================================================
 * Output Functions
 * ======================================================================== */

/**
 * Print numerology profile
 *
 * @param profile Profile to print
 * @param fp File pointer
 */
void astro_numerology_print_profile(const NumerologyProfile *profile, FILE *fp);

/**
 * Print compatibility analysis
 *
 * @param profile1 First person's profile
 * @param profile2 Second person's profile
 * @param fp File pointer
 */
void astro_numerology_print_compatibility(const NumerologyProfile *profile1,
                                         const NumerologyProfile *profile2,
                                         FILE *fp);

#ifdef __cplusplus
}
#endif

#endif /* ASTRO_NUMEROLOGY_H */
