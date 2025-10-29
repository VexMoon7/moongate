/**
 * @file astro_multi_synastry.h
 * @brief Multi-person synastry and relationship analysis
 *
 * This module provides comprehensive relationship analysis for 2-4 people,
 * including romantic, family, friendship, and professional relationships.
 * Integrates asteroids, Sabian symbols, numerology, and fixed stars.
 */

#ifndef ASTRO_MULTI_SYNASTRY_H
#define ASTRO_MULTI_SYNASTRY_H

#include "astro_types.h"
#include "astro_numerology.h"
#include "astro_sabian.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_SYNASTRY_PEOPLE 4
#define MAX_SYNASTRY_ASPECTS 200
#define MAX_SYNASTRY_PATTERNS 50

/* ========================================================================
 * Relationship Types
 * ======================================================================== */

typedef enum {
    RELATIONSHIP_ROMANTIC,      /* Romantic/sexual relationship */
    RELATIONSHIP_FRIENDSHIP,    /* Friendship */
    RELATIONSHIP_FAMILY,        /* Family (parent-child, siblings, etc.) */
    RELATIONSHIP_PROFESSIONAL,  /* Work/professional relationship */
    RELATIONSHIP_CREATIVE,      /* Creative collaboration */
    RELATIONSHIP_SPIRITUAL,     /* Spiritual/teaching relationship */
    RELATIONSHIP_KARMIC         /* Karmic/past-life connection */
} RelationshipType;

/**
 * Person in synastry analysis
 */
typedef struct {
    char name[ASTRO_MAX_NAME_LEN];
    AstroChart chart;
    NumerologyProfile numerology;
    SabianSymbol key_symbols[10];   /* Sun, Moon, ASC, MC, etc. */
    int num_key_symbols;
} SynastryPerson;

/**
 * Aspect between two people
 */
typedef struct {
    int person1_index;              /* Index in people array */
    int person2_index;
    AstroPlanet planet1;
    AstroPlanet planet2;
    AstroAspectType aspect_type;
    double orb;
    double strength;               /* 0-100 */
    int is_exact;
    int is_applying;
    const char *interpretation;    /* Mythopoetic interpretation */
} MultiSynastryAspect;

/**
 * Synastry pattern (between 2+ people)
 */
typedef struct {
    AstroPatternType pattern_type;
    int people_indices[MAX_SYNASTRY_PEOPLE];
    int num_people;
    char description[512];
    double significance;           /* 0-100 */
} MultiSynastryPattern;

/**
 * Multi-person synastry analysis
 */
typedef struct {
    SynastryPerson people[MAX_SYNASTRY_PEOPLE];
    int num_people;
    RelationshipType relationship_type;

    /* Inter-chart aspects */
    MultiSynastryAspect aspects[MAX_SYNASTRY_ASPECTS];
    int num_aspects;

    /* Patterns across charts */
    MultiSynastryPattern patterns[MAX_SYNASTRY_PATTERNS];
    int num_patterns;

    /* Compatibility scores */
    double compatibility_matrix[MAX_SYNASTRY_PEOPLE][MAX_SYNASTRY_PEOPLE];
    double overall_compatibility;   /* Average or weighted */

    /* Element/modality balance across group */
    int group_fire_count;
    int group_earth_count;
    int group_air_count;
    int group_water_count;

    int group_cardinal_count;
    int group_fixed_count;
    int group_mutable_count;

    /* Themes */
    char primary_themes[5][128];    /* Main relationship themes */
    int num_themes;

    char challenges[5][128];        /* Main challenges */
    int num_challenges;

    char strengths[5][128];         /* Main strengths */
    int num_strengths;

} MultiSynastry;

/* ========================================================================
 * Core Functions
 * ======================================================================== */

/**
 * Create multi-person synastry analysis
 *
 * @param people Array of people to analyze
 * @param num_people Number of people (2-4)
 * @param relationship_type Type of relationship
 * @param synastry Pointer to store synastry analysis
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_multi_synastry_create(const SynastryPerson people[],
                                int num_people,
                                RelationshipType relationship_type,
                                MultiSynastry *synastry);

/**
 * Add person to existing synastry
 *
 * @param synastry Synastry analysis
 * @param person Person to add
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_multi_synastry_add_person(MultiSynastry *synastry,
                                   const SynastryPerson *person);

/**
 * Calculate all inter-chart aspects
 *
 * @param synastry Synastry analysis
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_multi_synastry_calc_aspects(MultiSynastry *synastry);

/**
 * Find patterns across all charts
 *
 * @param synastry Synastry analysis
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_multi_synastry_find_patterns(MultiSynastry *synastry);

/**
 * Calculate compatibility matrix
 *
 * @param synastry Synastry analysis
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_multi_synastry_calc_compatibility(MultiSynastry *synastry);

/* ========================================================================
 * Specialized Analysis
 * ======================================================================== */

/**
 * Find double whammies (mutual aspects)
 *
 * A double whammy is when Person A's planet aspects Person B's planet,
 * AND Person B's same planet aspects Person A's same planet.
 *
 * @param synastry Synastry analysis
 * @param whammies Array to store double whammies
 * @param max_whammies Maximum whammies to find
 * @param num_whammies Pointer to store count
 * @return ASTRO_OK on success
 */
int astro_multi_synastry_find_double_whammies(const MultiSynastry *synastry,
                                              MultiSynastryAspect whammies[],
                                              int max_whammies,
                                              int *num_whammies);

/**
 * Find composite chart for group
 *
 * Creates a midpoint chart representing the relationship itself.
 *
 * @param synastry Synastry analysis
 * @param composite Pointer to store composite chart
 * @return ASTRO_OK on success
 */
int astro_multi_synastry_calc_composite(const MultiSynastry *synastry,
                                       AstroChart *composite);

/**
 * Find Davison relationship chart
 *
 * Uses time-space midpoint between people.
 *
 * @param synastry Synastry analysis (2 people only)
 * @param davison Pointer to store Davison chart
 * @return ASTRO_OK on success
 */
int astro_multi_synastry_calc_davison(const MultiSynastry *synastry,
                                     AstroChart *davison);

/**
 * Analyze house overlays
 *
 * Shows how one person's planets fall in another's houses.
 *
 * @param synastry Synastry analysis
 * @param person1_index First person
 * @param person2_index Second person
 * @param overlays Array to store overlay data
 * @param max_overlays Maximum overlays
 * @param num_overlays Pointer to store count
 * @return ASTRO_OK on success
 */
int astro_multi_synastry_house_overlays(const MultiSynastry *synastry,
                                       int person1_index,
                                       int person2_index,
                                       struct HouseOverlay overlays[],
                                       int max_overlays,
                                       int *num_overlays);

/**
 * Find vertex connections
 *
 * The Vertex represents fated encounters and karmic connections.
 *
 * @param synastry Synastry analysis
 * @param connections Array to store vertex aspects
 * @param max_connections Maximum connections
 * @param num_connections Pointer to store count
 * @return ASTRO_OK on success
 */
int astro_multi_synastry_vertex_connections(const MultiSynastry *synastry,
                                           MultiSynastryAspect connections[],
                                           int max_connections,
                                           int *num_connections);

/**
 * Find nodal connections (karmic indicators)
 *
 * @param synastry Synastry analysis
 * @param connections Array to store nodal aspects
 * @param max_connections Maximum connections
 * @param num_connections Pointer to store count
 * @return ASTRO_OK on success
 */
int astro_multi_synastry_nodal_connections(const MultiSynastry *synastry,
                                          MultiSynastryAspect connections[],
                                          int max_connections,
                                          int *num_connections);

/* ========================================================================
 * Asteroid & Fixed Star Synastry
 * ======================================================================== */

/**
 * Find asteroid connections between charts
 *
 * Analyzes love asteroids (Eros, Psyche, etc.) and other significant
 * asteroids in synastry.
 *
 * @param synastry Synastry analysis
 * @param category Asteroid category ("love", "destiny", "healing", etc.)
 * @param connections Array to store connections
 * @param max_connections Maximum connections
 * @param num_connections Pointer to store count
 * @return ASTRO_OK on success
 */
int astro_multi_synastry_asteroid_connections(const MultiSynastry *synastry,
                                             const char *category,
                                             MultiSynastryAspect connections[],
                                             int max_connections,
                                             int *num_connections);

/**
 * Find fixed star connections
 *
 * @param synastry Synastry analysis
 * @param connections Array to store connections
 * @param max_connections Maximum connections
 * @param num_connections Pointer to store count
 * @return ASTRO_OK on success
 */
int astro_multi_synastry_fixed_star_connections(const MultiSynastry *synastry,
                                               MultiSynastryAspect connections[],
                                               int max_connections,
                                               int *num_connections);

/* ========================================================================
 * Numerology Integration
 * ======================================================================== */

/**
 * Analyze numerology compatibility in group
 *
 * @param synastry Synastry analysis
 * @param scores Array to store pairwise scores
 * @param num_pairs Pointer to store number of pairs
 * @return Average numerology compatibility
 */
double astro_multi_synastry_numerology_compatibility(const MultiSynastry *synastry,
                                                    double scores[][2],
                                                    int *num_pairs);

/* ========================================================================
 * Sabian Symbol Integration
 * ======================================================================== */

/**
 * Find shared Sabian symbol themes
 *
 * @param synastry Synastry analysis
 * @param themes Array to store shared themes
 * @param max_themes Maximum themes
 * @param num_themes Pointer to store count
 * @return ASTRO_OK on success
 */
int astro_multi_synastry_sabian_themes(const MultiSynastry *synastry,
                                      char themes[][256],
                                      int max_themes,
                                      int *num_themes);

/* ========================================================================
 * Interpretation & Output
 * ======================================================================== */

/**
 * Generate mythopoetic interpretation
 *
 * Creates poetic, archetypal interpretation of the relationship.
 *
 * @param synastry Synastry analysis
 * @param buffer Buffer to store interpretation
 * @param buffer_size Size of buffer
 * @return Formatted interpretation
 */
char* astro_multi_synastry_mythopoetic_reading(const MultiSynastry *synastry,
                                               char *buffer,
                                               int buffer_size);

/**
 * Get relationship archetype
 *
 * Identifies the core archetypal pattern (e.g., "Teacher-Student",
 * "Twin Flames", "Soul Family", etc.)
 *
 * @param synastry Synastry analysis
 * @return Archetype name (static, do not free)
 */
const char* astro_multi_synastry_get_archetype(const MultiSynastry *synastry);

/**
 * Print comprehensive synastry report
 *
 * @param synastry Synastry analysis
 * @param fp File pointer
 * @return ASTRO_OK on success
 */
int astro_multi_synastry_print_report(const MultiSynastry *synastry, FILE *fp);

/**
 * Print compatibility matrix
 *
 * @param synastry Synastry analysis
 * @param fp File pointer
 */
void astro_multi_synastry_print_compatibility_matrix(const MultiSynastry *synastry,
                                                     FILE *fp);

/**
 * Print group dynamics analysis
 *
 * @param synastry Synastry analysis
 * @param fp File pointer
 */
void astro_multi_synastry_print_group_dynamics(const MultiSynastry *synastry,
                                              FILE *fp);

/**
 * Export synastry to JSON
 *
 * @param synastry Synastry analysis
 * @param filename Output filename
 * @return ASTRO_OK on success
 */
int astro_multi_synastry_export_json(const MultiSynastry *synastry,
                                    const char *filename);

/* ========================================================================
 * Helper Structures
 * ======================================================================== */

/**
 * House overlay data
 */
struct HouseOverlay {
    AstroPlanet planet;
    int house;                     /* House in other person's chart */
    const char *interpretation;
};

#ifdef __cplusplus
}
#endif

#endif /* ASTRO_MULTI_SYNASTRY_H */
