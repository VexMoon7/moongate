/**
 * @file astro_asteroids.h
 * @brief Extended asteroid and celestial body support
 *
 * This module provides access to 100+ asteroids, TNOs (Trans-Neptunian Objects),
 * centaurs, and other minor bodies for deep astrological analysis.
 */

#ifndef ASTRO_ASTEROIDS_H
#define ASTRO_ASTEROIDS_H

#include "astro_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================
 * Extended Asteroid Catalog
 * ======================================================================== */

/**
 * Extended asteroid catalog with mythological associations
 */
typedef enum {
    /* Major Asteroids (already in core) */
    ASTROID_CERES = 1,
    ASTROID_PALLAS = 2,
    ASTROID_JUNO = 3,
    ASTROID_VESTA = 4,
    ASTROID_CHIRON = 2060,
    ASTROID_PHOLUS = 5145,

    /* Love & Relationship Asteroids */
    ASTROID_EROS = 433,        /* Erotic love, passion */
    ASTROID_PSYCHE = 16,       /* Soul, deep connection */
    ASTROID_AMOR = 1221,       /* Romantic love */
    ASTROID_CUPIDO = 763,      /* Desire, attraction */
    ASTROID_SAPPHO = 80,       /* Poetic love, same-sex love */
    ASTROID_VALENTINE = 447,   /* Devoted love */

    /* Destiny & Fate Asteroids */
    ASTROID_KARMA = 3811,      /* Karmic patterns */
    ASTROID_MOIRA = 638,       /* Fate, destiny */
    ASTROID_NEMESIS = 128,     /* Divine retribution */
    ASTROID_FORTUNA = 19,      /* Fortune, luck */
    ASTROID_TYCHE = 258,       /* Chance, fortune */

    /* Wisdom & Knowledge */
    ASTROID_SOPHIA = 251,      /* Divine wisdom */
    ASTROID_ATHENE = 881,      /* Strategic wisdom */
    ASTROID_PANDORA = 55,      /* Curiosity, revelation */
    ASTROID_URANIA = 30,       /* Astronomy, higher knowledge */

    /* Healing & Transformation */
    ASTROID_HYGEIA = 10,       /* Health, healing */
    ASTROID_AESCULAPIA = 1027, /* Medical healing */
    ASTROID_PHOENIX = 4544,    /* Rebirth, transformation */
    ASTROID_ISIS = 42,         /* Magic, resurrection */

    /* Shadow & Underworld */
    ASTROID_PERSEPHONE = 399,  /* Underworld journey */
    ASTROID_HADES = 1587,      /* Underworld, hidden realm */
    ASTROID_HECATE = 100,      /* Witchcraft, crossroads */
    ASTROID_MEDUSA = 149,      /* Shadow feminine */
    ASTROID_LILITH_ASTEROID = 1181, /* Dark feminine (different from Black Moon) */

    /* Centaurs (Shamanic Bridges) */
    ASTROID_NESSUS = 7066,     /* Abuse patterns, karma */
    ASTROID_CHARIKLO = 10199,  /* Healing grace */
    ASTROID_HYLONOME = 10370,  /* Grief, soul loss */
    ASTROID_ASBOLUS = 8405,    /* Divination, intuition */

    /* TNOs (Outer Solar System) */
    ASTROID_ERIS = 136199,     /* Discord, revelation */
    ASTROID_SEDNA = 90377,     /* Deep ancestral trauma */
    ASTROID_QUAOAR = 50000,    /* Creation mythology */
    ASTROID_MAKEMAKE = 136472, /* Creator deity */
    ASTROID_HAUMEA = 136108,   /* Birth, fertility */
    ASTROID_ORCUS = 90482,     /* Oaths, consequences */
    ASTROID_IXION = 28978,     /* Transgression, redemption */

    /* Muses & Creativity */
    ASTROID_TERPSICHORE = 81,  /* Dance */
    ASTROID_ERATO = 62,        /* Erotic poetry */
    ASTROID_EUTERPE = 27,      /* Music, lyric poetry */
    ASTROID_THALIA = 23,       /* Comedy */
    ASTROID_MELPOMENE = 18,    /* Tragedy */
    ASTROID_CALLIOPE = 22,     /* Epic poetry */

    /* Nature & Elements */
    ASTROID_GAIA = 1184,       /* Earth mother */
    ASTROID_DEMETER = 1108,    /* Agriculture, nurturing */
    ASTROID_FLORA = 8,         /* Flowers, spring */
    ASTROID_POMONA = 32,       /* Fruit trees, abundance */

    /* Justice & Truth */
    ASTROID_JUSTITIA = 269,    /* Justice */
    ASTROID_VERITAS = 490,     /* Truth */
    ASTROID_ASTRAEA = 5,       /* Innocence, purity */

    /* Communication & Trickster */
    ASTROID_HERMES = 69230,    /* Messages, trickster */
    ASTROID_IRIS = 7,          /* Rainbow, messages */

    /* Power & War */
    ASTROID_BELLONA = 28,      /* War goddess */
    ASTROID_ARES = 2174,       /* War god */

    /* Additional Important Asteroids */
    ASTROID_APOLLO = 1862,     /* Sun, prophecy, arts */
    ASTROID_DIANA = 78,        /* Moon, hunting, independence */
    ASTROID_PROSERPINA = 26,   /* Seasonal cycles */
    ASTROID_ORPHEUS = 3361,    /* Music, descent to underworld */
    ASTROID_EURYDICE = 75,     /* Lost love */

    ASTROID_COUNT = 70         /* Number of extended asteroids */
} AstroidExtended;

/**
 * Asteroid information structure
 */
typedef struct {
    int number;                    /* Swiss Ephemeris number */
    const char *name;              /* Asteroid name */
    const char *mythology;         /* Brief mythological meaning */
    const char *keywords[5];       /* Symbolic keywords */
    const char *archetype;         /* Archetypal theme */
} AstroidInfo;

/* ========================================================================
 * Functions
 * ======================================================================== */

/**
 * Get asteroid information
 *
 * @param asteroid Asteroid identifier
 * @return Pointer to asteroid info (static, do not free)
 */
const AstroidInfo* astro_asteroids_get_info(AstroidExtended asteroid);

/**
 * Calculate asteroid position
 *
 * @param asteroid Asteroid to calculate
 * @param datetime Date/time for calculation
 * @param position Pointer to store position
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_asteroids_calc_position(AstroidExtended asteroid,
                                  const AstroDateTime *datetime,
                                  AstroPlanetPosition *position);

/**
 * Calculate positions for multiple asteroids
 *
 * @param asteroids Array of asteroid identifiers
 * @param num_asteroids Number of asteroids to calculate
 * @param datetime Date/time for calculation
 * @param positions Array to store positions
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_asteroids_calc_multiple(const AstroidExtended asteroids[],
                                  int num_asteroids,
                                  const AstroDateTime *datetime,
                                  AstroPlanetPosition positions[]);

/**
 * Get asteroid by name
 *
 * @param name Asteroid name (case-insensitive)
 * @return Asteroid identifier, or -1 if not found
 */
int astro_asteroids_get_by_name(const char *name);

/**
 * Get all asteroids in a category
 *
 * @param category Category name ("love", "healing", "destiny", etc.)
 * @param asteroids Array to store asteroid identifiers
 * @param max_asteroids Maximum asteroids to return
 * @param num_asteroids Pointer to store count
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_asteroids_get_by_category(const char *category,
                                    AstroidExtended asteroids[],
                                    int max_asteroids,
                                    int *num_asteroids);

/**
 * Check if asteroid is conjunct a planet
 *
 * @param asteroid_pos Asteroid position
 * @param planet_pos Planet position
 * @param orb Maximum orb in degrees
 * @return 1 if conjunct, 0 otherwise
 */
int astro_asteroids_is_conjunct(const AstroPlanetPosition *asteroid_pos,
                                const AstroPlanetPosition *planet_pos,
                                double orb);

/**
 * Find all asteroid-planet contacts in chart
 *
 * @param asteroid_positions Array of asteroid positions
 * @param num_asteroids Number of asteroids
 * @param planet_positions Array of planet positions
 * @param num_planets Number of planets
 * @param orb Maximum orb for conjunction
 * @param contacts Array to store aspects
 * @param max_contacts Maximum contacts to find
 * @param num_contacts Pointer to store count
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_asteroids_find_contacts(const AstroPlanetPosition asteroid_positions[],
                                  int num_asteroids,
                                  const AstroPlanetPosition planet_positions[],
                                  int num_planets,
                                  double orb,
                                  AstroAspect contacts[],
                                  int max_contacts,
                                  int *num_contacts);

#ifdef __cplusplus
}
#endif

#endif /* ASTRO_ASTEROIDS_H */
