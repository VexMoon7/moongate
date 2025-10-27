/**
 * @file astro_types.h
 * @brief Common data types for the astrological analysis engine
 *
 * This file defines all common data structures, enumerations, and constants
 * used throughout the astrological analysis engine modules.
 */

#ifndef ASTRO_TYPES_H
#define ASTRO_TYPES_H

#include <time.h>

/* ========================================================================
 * Constants and Limits
 * ======================================================================== */

#define ASTRO_MAX_PLANETS 23
#define ASTRO_MAX_HOUSES 12
#define ASTRO_MAX_ASPECTS 50
#define ASTRO_MAX_NAME_LEN 64
#define ASTRO_MAX_TRANSITS 100

/* ========================================================================
 * Planet Enumeration
 * ======================================================================== */

typedef enum {
    ASTRO_SUN = 0,
    ASTRO_MOON = 1,
    ASTRO_MERCURY = 2,
    ASTRO_VENUS = 3,
    ASTRO_MARS = 4,
    ASTRO_JUPITER = 5,
    ASTRO_SATURN = 6,
    ASTRO_URANUS = 7,
    ASTRO_NEPTUNE = 8,
    ASTRO_PLUTO = 9,
    ASTRO_MEAN_NODE = 10,
    ASTRO_TRUE_NODE = 11,
    ASTRO_MEAN_APOGEE = 12,
    ASTRO_OSC_APOGEE = 13,
    ASTRO_EARTH = 14,
    ASTRO_CHIRON = 15,
    ASTRO_PHOLUS = 16,
    ASTRO_CERES = 17,
    ASTRO_PALLAS = 18,
    ASTRO_JUNO = 19,
    ASTRO_VESTA = 20,
    ASTRO_INTP_APOGEE = 21,
    ASTRO_INTP_PERIGEE = 22
} AstroPlanet;

/* ========================================================================
 * Zodiac Signs
 * ======================================================================== */

typedef enum {
    ASTRO_ARIES = 0,
    ASTRO_TAURUS = 1,
    ASTRO_GEMINI = 2,
    ASTRO_CANCER = 3,
    ASTRO_LEO = 4,
    ASTRO_VIRGO = 5,
    ASTRO_LIBRA = 6,
    ASTRO_SCORPIO = 7,
    ASTRO_SAGITTARIUS = 8,
    ASTRO_CAPRICORN = 9,
    ASTRO_AQUARIUS = 10,
    ASTRO_PISCES = 11
} AstroSign;

/* ========================================================================
 * Elements and Modalities
 * ======================================================================== */

typedef enum {
    ASTRO_ELEMENT_FIRE = 0,
    ASTRO_ELEMENT_EARTH = 1,
    ASTRO_ELEMENT_AIR = 2,
    ASTRO_ELEMENT_WATER = 3
} AstroElement;

typedef enum {
    ASTRO_MODALITY_CARDINAL = 0,
    ASTRO_MODALITY_FIXED = 1,
    ASTRO_MODALITY_MUTABLE = 2
} AstroModality;

/* ========================================================================
 * Aspect Types
 * ======================================================================== */

typedef enum {
    ASTRO_ASPECT_CONJUNCTION = 0,    /* 0° */
    ASTRO_ASPECT_OPPOSITION = 180,   /* 180° */
    ASTRO_ASPECT_TRINE = 120,        /* 120° */
    ASTRO_ASPECT_SQUARE = 90,        /* 90° */
    ASTRO_ASPECT_SEXTILE = 60,       /* 60° */
    ASTRO_ASPECT_QUINCUNX = 150,     /* 150° */
    ASTRO_ASPECT_SEMISEXTILE = 30,   /* 30° */
    ASTRO_ASPECT_SEMISQUARE = 45,    /* 45° */
    ASTRO_ASPECT_SESQUIQUADRATE = 135, /* 135° */
    ASTRO_ASPECT_QUINTILE = 72,      /* 72° */
    ASTRO_ASPECT_BIQUINTILE = 144    /* 144° */
} AstroAspectType;

/* ========================================================================
 * Dignity Types
 * ======================================================================== */

typedef enum {
    ASTRO_DIGNITY_RULERSHIP = 0,
    ASTRO_DIGNITY_EXALTATION = 1,
    ASTRO_DIGNITY_DETRIMENT = 2,
    ASTRO_DIGNITY_FALL = 3,
    ASTRO_DIGNITY_NONE = 4
} AstroDignity;

/* ========================================================================
 * House Systems
 * ======================================================================== */

typedef enum {
    ASTRO_HOUSE_PLACIDUS = 'P',
    ASTRO_HOUSE_KOCH = 'K',
    ASTRO_HOUSE_PORPHYRIUS = 'O',
    ASTRO_HOUSE_REGIOMONTANUS = 'R',
    ASTRO_HOUSE_CAMPANUS = 'C',
    ASTRO_HOUSE_EQUAL = 'E',
    ASTRO_HOUSE_WHOLE_SIGN = 'W',
    ASTRO_HOUSE_MERIDIAN = 'X',
    ASTRO_HOUSE_MORINUS = 'M'
} AstroHouseSystem;

/* ========================================================================
 * Coordinate Types
 * ======================================================================== */

typedef enum {
    ASTRO_COORD_TROPICAL = 0,
    ASTRO_COORD_SIDEREAL = 1
} AstroCoordinateSystem;

/* ========================================================================
 * Data Structures
 * ======================================================================== */

/**
 * Geographic location
 */
typedef struct {
    double latitude;   /* Degrees, positive = North */
    double longitude;  /* Degrees, positive = East */
    double altitude;   /* Meters above sea level */
    char location_name[ASTRO_MAX_NAME_LEN];
} AstroLocation;

/**
 * Date and time
 */
typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    double second;
    double timezone_offset; /* Hours from UTC */
    double julian_day;      /* Calculated Julian Day (UT) */
} AstroDateTime;

/**
 * Planet position data
 */
typedef struct {
    AstroPlanet planet;
    double longitude;        /* Ecliptic longitude in degrees */
    double latitude;         /* Ecliptic latitude in degrees */
    double distance;         /* Distance from Earth in AU */
    double longitude_speed;  /* Daily motion in degrees */
    double latitude_speed;
    double distance_speed;
    AstroSign sign;         /* Zodiac sign */
    double sign_position;   /* Position within sign (0-30°) */
    int house;              /* House number (1-12) */
    int is_retrograde;      /* 1 if retrograde, 0 otherwise */
    char name[ASTRO_MAX_NAME_LEN];
} AstroPlanetPosition;

/**
 * Aspect between two planets
 */
typedef struct {
    AstroPlanet planet1;
    AstroPlanet planet2;
    AstroAspectType aspect_type;
    double orb;              /* Actual angle */
    double difference;       /* Difference from exact aspect */
    int is_applying;         /* 1 if applying, 0 if separating */
    int is_exact;            /* 1 if within tight orb */
    char name[ASTRO_MAX_NAME_LEN];
} AstroAspect;

/**
 * House cusp data
 */
typedef struct {
    double cusp[ASTRO_MAX_HOUSES + 1]; /* House cusps (1-12, 0 is unused) */
    double ascendant;                   /* Ascendant (1st house cusp) */
    double mc;                          /* Midheaven (10th house cusp) */
    double armc;                        /* ARMC (sidereal time) */
    double vertex;                      /* Vertex */
    double equatorial_ascendant;        /* Equatorial ascendant */
    double co_ascendant_koch;           /* Co-ascendant (Koch) */
    double co_ascendant_munkasey;       /* Co-ascendant (Munkasey) */
    double polar_ascendant;             /* Polar ascendant */
    AstroHouseSystem system;
} AstroHouses;

/**
 * Complete natal chart
 */
typedef struct {
    AstroDateTime birth_time;
    AstroLocation birth_location;
    AstroPlanetPosition planets[ASTRO_MAX_PLANETS];
    int num_planets;
    AstroHouses houses;
    AstroAspect aspects[ASTRO_MAX_ASPECTS];
    int num_aspects;
    char chart_name[ASTRO_MAX_NAME_LEN];
} AstroChart;

/**
 * Transit event
 */
typedef struct {
    AstroPlanet transiting_planet;
    AstroPlanet natal_planet;
    AstroAspectType aspect_type;
    AstroDateTime event_time;
    double orb;
    int is_exact;
    char description[256];
} AstroTransit;

/**
 * Configuration for chart calculations
 */
typedef struct {
    AstroHouseSystem house_system;
    AstroCoordinateSystem coordinate_system;
    int include_asteroids;           /* Include Chiron, Ceres, etc. */
    int include_nodes;               /* Include lunar nodes */
    int include_lilith;              /* Include Black Moon Lilith */
    double aspect_orb_major;         /* Orb for major aspects (degrees) */
    double aspect_orb_minor;         /* Orb for minor aspects (degrees) */
    int use_true_node;               /* Use true node vs mean node */
    char ephemeris_path[256];        /* Path to ephemeris files */
} AstroConfig;

/**
 * Error codes
 */
typedef enum {
    ASTRO_OK = 0,
    ASTRO_ERROR_INVALID_DATE = -1,
    ASTRO_ERROR_INVALID_LOCATION = -2,
    ASTRO_ERROR_EPHEMERIS_FILE = -3,
    ASTRO_ERROR_CALCULATION = -4,
    ASTRO_ERROR_INVALID_PLANET = -5,
    ASTRO_ERROR_INVALID_HOUSE_SYSTEM = -6,
    ASTRO_ERROR_NULL_POINTER = -7,
    ASTRO_ERROR_OUT_OF_RANGE = -8
} AstroErrorCode;

#endif /* ASTRO_TYPES_H */
