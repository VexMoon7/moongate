/**
 * @file astro_core.c
 * @brief Core ephemeris calculation module implementation
 */

#include "astro_core.h"
#include "swephexp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

/* ========================================================================
 * Module-level variables
 * ======================================================================== */

static AstroConfig g_config = {
    .house_system = ASTRO_HOUSE_PLACIDUS,
    .coordinate_system = ASTRO_COORD_TROPICAL,
    .include_asteroids = 1,
    .include_nodes = 1,
    .include_lilith = 1,
    .aspect_orb_major = 8.0,
    .aspect_orb_minor = 3.0,
    .use_true_node = 0,
    .ephemeris_path = "./ephe"
};

static int g_initialized = 0;

/* Planet names lookup table */
static const char* planet_names[] = {
    "Sun", "Moon", "Mercury", "Venus", "Mars", "Jupiter", "Saturn",
    "Uranus", "Neptune", "Pluto", "Mean Node", "True Node",
    "Mean Apogee", "Osc. Apogee", "Earth", "Chiron", "Pholus",
    "Ceres", "Pallas", "Juno", "Vesta", "Intp. Apogee", "Intp. Perigee"
};

/* Sign names lookup table */
static const char* sign_names[] = {
    "Aries", "Taurus", "Gemini", "Cancer", "Leo", "Virgo",
    "Libra", "Scorpio", "Sagittarius", "Capricorn", "Aquarius", "Pisces"
};

/* ========================================================================
 * Initialization and Configuration
 * ======================================================================== */

int astro_core_init(const AstroConfig *config) {
    if (config != NULL) {
        memcpy(&g_config, config, sizeof(AstroConfig));
    }

    /* Set ephemeris path */
    swe_set_ephe_path(g_config.ephemeris_path);

    g_initialized = 1;
    return ASTRO_OK;
}

void astro_core_set_ephemeris_path(const char *path) {
    if (path != NULL) {
        strncpy(g_config.ephemeris_path, path, sizeof(g_config.ephemeris_path) - 1);
        g_config.ephemeris_path[sizeof(g_config.ephemeris_path) - 1] = '\0';
        swe_set_ephe_path(g_config.ephemeris_path);
    }
}

int astro_core_get_config(AstroConfig *config) {
    if (config == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }
    memcpy(config, &g_config, sizeof(AstroConfig));
    return ASTRO_OK;
}

void astro_core_cleanup(void) {
    swe_close();
    g_initialized = 0;
}

/* ========================================================================
 * Date and Time Calculations
 * ======================================================================== */

double astro_core_get_julian_day(const AstroDateTime *datetime) {
    if (datetime == NULL) {
        return 0.0;
    }

    double hour = datetime->hour + datetime->minute / 60.0 +
                  datetime->second / 3600.0 - datetime->timezone_offset;

    return swe_julday(datetime->year, datetime->month, datetime->day,
                      hour, SE_GREG_CAL);
}

int astro_core_get_calendar_date(double julian_day, AstroDateTime *datetime) {
    if (datetime == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    double hour;
    swe_revjul(julian_day, SE_GREG_CAL, &datetime->year, &datetime->month,
               &datetime->day, &hour);

    datetime->hour = (int)hour;
    double minute_fraction = (hour - datetime->hour) * 60.0;
    datetime->minute = (int)minute_fraction;
    datetime->second = (minute_fraction - datetime->minute) * 60.0;
    datetime->timezone_offset = 0.0; /* UTC */
    datetime->julian_day = julian_day;

    return ASTRO_OK;
}

int astro_core_get_current_datetime(AstroDateTime *datetime) {
    if (datetime == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    time_t now = time(NULL);
    struct tm *tm_info = gmtime(&now);

    datetime->year = tm_info->tm_year + 1900;
    datetime->month = tm_info->tm_mon + 1;
    datetime->day = tm_info->tm_mday;
    datetime->hour = tm_info->tm_hour;
    datetime->minute = tm_info->tm_min;
    datetime->second = tm_info->tm_sec;
    datetime->timezone_offset = 0.0; /* UTC */
    datetime->julian_day = astro_core_get_julian_day(datetime);

    return ASTRO_OK;
}

double astro_core_get_sidereal_time(const AstroDateTime *datetime, double longitude) {
    if (datetime == NULL) {
        return 0.0;
    }

    double jd = astro_core_get_julian_day(datetime);
    return swe_sidtime(jd) * 15.0 + longitude; /* Convert hours to degrees */
}

/* ========================================================================
 * Planet Position Calculations
 * ======================================================================== */

int astro_core_calc_planet(AstroPlanet planet, const AstroDateTime *datetime,
                           int flags, AstroPlanetPosition *position) {
    if (datetime == NULL || position == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    if (planet < 0 || planet >= ASTRO_MAX_PLANETS) {
        return ASTRO_ERROR_INVALID_PLANET;
    }

    double jd = astro_core_get_julian_day(datetime);
    double xx[6];
    char serr[256];

    /* Calculate planet position */
    int result = swe_calc_ut(jd, planet, flags | SEFLG_SPEED, xx, serr);
    if (result < 0) {
        return ASTRO_ERROR_CALCULATION;
    }

    /* Fill in position structure */
    position->planet = planet;
    position->longitude = xx[0];
    position->latitude = xx[1];
    position->distance = xx[2];
    position->longitude_speed = xx[3];
    position->latitude_speed = xx[4];
    position->distance_speed = xx[5];

    /* Calculate sign and position within sign */
    position->sign = astro_core_get_sign(position->longitude);
    position->sign_position = astro_core_get_sign_position(position->longitude);

    /* Check if retrograde */
    position->is_retrograde = (position->longitude_speed < 0.0) ? 1 : 0;

    /* Copy planet name */
    strncpy(position->name, astro_core_get_planet_name(planet), ASTRO_MAX_NAME_LEN - 1);
    position->name[ASTRO_MAX_NAME_LEN - 1] = '\0';

    /* House will be filled in later when houses are calculated */
    position->house = 0;

    return ASTRO_OK;
}

int astro_core_calc_planets(const AstroDateTime *datetime, int flags,
                            AstroPlanetPosition positions[], int *num_planets) {
    if (datetime == NULL || positions == NULL || num_planets == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    int count = 0;
    int max_planet = g_config.include_asteroids ? ASTRO_VESTA : ASTRO_PLUTO;

    /* Calculate main planets */
    for (int i = ASTRO_SUN; i <= max_planet; i++) {
        if (i == ASTRO_EARTH) continue; /* Skip Earth for geocentric */

        int result = astro_core_calc_planet(i, datetime, flags, &positions[count]);
        if (result == ASTRO_OK) {
            count++;
        }
    }

    /* Add nodes if configured */
    if (g_config.include_nodes) {
        AstroPlanet node = g_config.use_true_node ? ASTRO_TRUE_NODE : ASTRO_MEAN_NODE;
        int result = astro_core_calc_planet(node, datetime, flags, &positions[count]);
        if (result == ASTRO_OK) {
            count++;
        }
    }

    /* Add Lilith if configured */
    if (g_config.include_lilith) {
        int result = astro_core_calc_planet(ASTRO_MEAN_APOGEE, datetime, flags, &positions[count]);
        if (result == ASTRO_OK) {
            count++;
        }
    }

    *num_planets = count;
    return ASTRO_OK;
}

int astro_core_calc_planet_topocentric(AstroPlanet planet,
                                       const AstroDateTime *datetime,
                                       const AstroLocation *location,
                                       AstroPlanetPosition *position) {
    if (datetime == NULL || location == NULL || position == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    /* Set topocentric position */
    swe_set_topo(location->longitude, location->latitude, location->altitude);

    /* Calculate with topocentric flag */
    int result = astro_core_calc_planet(planet, datetime, SEFLG_TOPOCTR, position);

    return result;
}

const char* astro_core_get_planet_name(AstroPlanet planet) {
    if (planet >= 0 && planet < ASTRO_MAX_PLANETS) {
        return planet_names[planet];
    }
    return "Unknown";
}

int astro_core_is_retrograde(const AstroPlanetPosition *position) {
    if (position == NULL) {
        return 0;
    }
    return position->is_retrograde;
}

/* ========================================================================
 * House Calculations
 * ======================================================================== */

int astro_core_calc_houses(const AstroDateTime *datetime,
                          const AstroLocation *location,
                          AstroHouseSystem house_system,
                          AstroHouses *houses) {
    if (datetime == NULL || location == NULL || houses == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    double jd = astro_core_get_julian_day(datetime);
    double cusps[13];  /* House cusps 1-12 (0 unused) */
    double ascmc[10];  /* Ascendant, MC, etc. */

    int result = swe_houses(jd, location->latitude, location->longitude,
                           house_system, cusps, ascmc);

    if (result < 0) {
        return ASTRO_ERROR_CALCULATION;
    }

    /* Copy house cusps */
    for (int i = 0; i <= ASTRO_MAX_HOUSES; i++) {
        houses->cusp[i] = cusps[i];
    }

    /* Copy additional points */
    houses->ascendant = ascmc[0];
    houses->mc = ascmc[1];
    houses->armc = ascmc[2];
    houses->vertex = ascmc[3];
    houses->equatorial_ascendant = ascmc[4];
    houses->co_ascendant_koch = ascmc[5];
    houses->co_ascendant_munkasey = ascmc[6];
    houses->polar_ascendant = ascmc[7];
    houses->system = house_system;

    return ASTRO_OK;
}

int astro_core_get_house(double longitude, const AstroHouses *houses) {
    if (houses == NULL) {
        return 0;
    }

    /* Normalize longitude */
    longitude = astro_core_normalize_angle(longitude);

    /* Find which house the longitude falls into */
    for (int i = 1; i <= ASTRO_MAX_HOUSES; i++) {
        double cusp1 = houses->cusp[i];
        double cusp2 = houses->cusp[(i % ASTRO_MAX_HOUSES) + 1];

        /* Handle wrapping around 0° */
        if (cusp2 < cusp1) {
            cusp2 += 360.0;
            if (longitude < cusp1) {
                longitude += 360.0;
            }
        }

        if (longitude >= cusp1 && longitude < cusp2) {
            return i;
        }
    }

    return 1; /* Default to first house */
}

const char* astro_core_get_house_system_name(AstroHouseSystem system) {
    switch (system) {
        case ASTRO_HOUSE_PLACIDUS: return "Placidus";
        case ASTRO_HOUSE_KOCH: return "Koch";
        case ASTRO_HOUSE_PORPHYRIUS: return "Porphyrius";
        case ASTRO_HOUSE_REGIOMONTANUS: return "Regiomontanus";
        case ASTRO_HOUSE_CAMPANUS: return "Campanus";
        case ASTRO_HOUSE_EQUAL: return "Equal";
        case ASTRO_HOUSE_WHOLE_SIGN: return "Whole Sign";
        case ASTRO_HOUSE_MERIDIAN: return "Meridian";
        case ASTRO_HOUSE_MORINUS: return "Morinus";
        default: return "Unknown";
    }
}

/* ========================================================================
 * Zodiac Sign Functions
 * ======================================================================== */

AstroSign astro_core_get_sign(double longitude) {
    longitude = astro_core_normalize_angle(longitude);
    return (AstroSign)((int)(longitude / 30.0));
}

double astro_core_get_sign_position(double longitude) {
    longitude = astro_core_normalize_angle(longitude);
    return fmod(longitude, 30.0);
}

const char* astro_core_get_sign_name(AstroSign sign) {
    if (sign >= 0 && sign < 12) {
        return sign_names[sign];
    }
    return "Unknown";
}

AstroElement astro_core_get_sign_element(AstroSign sign) {
    switch (sign) {
        case ASTRO_ARIES:
        case ASTRO_LEO:
        case ASTRO_SAGITTARIUS:
            return ASTRO_ELEMENT_FIRE;
        case ASTRO_TAURUS:
        case ASTRO_VIRGO:
        case ASTRO_CAPRICORN:
            return ASTRO_ELEMENT_EARTH;
        case ASTRO_GEMINI:
        case ASTRO_LIBRA:
        case ASTRO_AQUARIUS:
            return ASTRO_ELEMENT_AIR;
        case ASTRO_CANCER:
        case ASTRO_SCORPIO:
        case ASTRO_PISCES:
            return ASTRO_ELEMENT_WATER;
        default:
            return ASTRO_ELEMENT_FIRE;
    }
}

AstroModality astro_core_get_sign_modality(AstroSign sign) {
    switch (sign) {
        case ASTRO_ARIES:
        case ASTRO_CANCER:
        case ASTRO_LIBRA:
        case ASTRO_CAPRICORN:
            return ASTRO_MODALITY_CARDINAL;
        case ASTRO_TAURUS:
        case ASTRO_LEO:
        case ASTRO_SCORPIO:
        case ASTRO_AQUARIUS:
            return ASTRO_MODALITY_FIXED;
        case ASTRO_GEMINI:
        case ASTRO_VIRGO:
        case ASTRO_SAGITTARIUS:
        case ASTRO_PISCES:
            return ASTRO_MODALITY_MUTABLE;
        default:
            return ASTRO_MODALITY_CARDINAL;
    }
}

/* ========================================================================
 * Rising, Setting, and Transit Times
 * ======================================================================== */

int astro_core_calc_rise(AstroPlanet planet, const AstroDateTime *datetime,
                        const AstroLocation *location, AstroDateTime *rise_time) {
    if (datetime == NULL || location == NULL || rise_time == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    double jd_start = astro_core_get_julian_day(datetime);
    double jd_rise;
    char serr[256];
    double geopos[3];

    geopos[0] = location->longitude;
    geopos[1] = location->latitude;
    geopos[2] = location->altitude;

    int result = swe_rise_trans(jd_start, planet, NULL, SEFLG_SWIEPH,
                                SE_CALC_RISE, geopos,
                                0.0, 0.0, &jd_rise, serr);

    if (result < 0) {
        return ASTRO_ERROR_CALCULATION;
    }

    return astro_core_get_calendar_date(jd_rise, rise_time);
}

int astro_core_calc_set(AstroPlanet planet, const AstroDateTime *datetime,
                       const AstroLocation *location, AstroDateTime *set_time) {
    if (datetime == NULL || location == NULL || set_time == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    double jd_start = astro_core_get_julian_day(datetime);
    double jd_set;
    char serr[256];
    double geopos[3];

    geopos[0] = location->longitude;
    geopos[1] = location->latitude;
    geopos[2] = location->altitude;

    int result = swe_rise_trans(jd_start, planet, NULL, SEFLG_SWIEPH,
                                SE_CALC_SET, geopos,
                                0.0, 0.0, &jd_set, serr);

    if (result < 0) {
        return ASTRO_ERROR_CALCULATION;
    }

    return astro_core_get_calendar_date(jd_set, set_time);
}

int astro_core_calc_transit(AstroPlanet planet, const AstroDateTime *datetime,
                           const AstroLocation *location, AstroDateTime *transit_time) {
    if (datetime == NULL || location == NULL || transit_time == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    double jd_start = astro_core_get_julian_day(datetime);
    double jd_transit;
    char serr[256];
    double geopos[3];

    geopos[0] = location->longitude;
    geopos[1] = location->latitude;
    geopos[2] = location->altitude;

    int result = swe_rise_trans(jd_start, planet, NULL, SEFLG_SWIEPH,
                                SE_CALC_MTRANSIT, geopos,
                                0.0, 0.0, &jd_transit, serr);

    if (result < 0) {
        return ASTRO_ERROR_CALCULATION;
    }

    return astro_core_get_calendar_date(jd_transit, transit_time);
}

/* ========================================================================
 * Utility Functions
 * ======================================================================== */

double astro_core_normalize_angle(double angle) {
    angle = fmod(angle, 360.0);
    if (angle < 0.0) {
        angle += 360.0;
    }
    return angle;
}

double astro_core_angular_distance(double lon1, double lon2) {
    double diff = fabs(lon2 - lon1);
    if (diff > 180.0) {
        diff = 360.0 - diff;
    }
    return diff;
}

char* astro_core_format_dms(double degrees, char *buffer, int buffer_size) {
    if (buffer == NULL || buffer_size < 16) {
        return NULL;
    }

    int d = (int)degrees;
    double minutes_frac = (degrees - d) * 60.0;
    int m = (int)minutes_frac;
    int s = (int)((minutes_frac - m) * 60.0);

    snprintf(buffer, buffer_size, "%d°%02d'%02d\"", d, m, s);
    return buffer;
}

const char* astro_core_get_error_message(AstroErrorCode error_code) {
    switch (error_code) {
        case ASTRO_OK:
            return "Success";
        case ASTRO_ERROR_INVALID_DATE:
            return "Invalid date";
        case ASTRO_ERROR_INVALID_LOCATION:
            return "Invalid location";
        case ASTRO_ERROR_EPHEMERIS_FILE:
            return "Ephemeris file error";
        case ASTRO_ERROR_CALCULATION:
            return "Calculation error";
        case ASTRO_ERROR_INVALID_PLANET:
            return "Invalid planet";
        case ASTRO_ERROR_INVALID_HOUSE_SYSTEM:
            return "Invalid house system";
        case ASTRO_ERROR_NULL_POINTER:
            return "Null pointer";
        case ASTRO_ERROR_OUT_OF_RANGE:
            return "Value out of range";
        default:
            return "Unknown error";
    }
}

const char* astro_core_get_version(void) {
    static char version[128];
    snprintf(version, sizeof(version), "Astro Engine v1.0.0 (Swiss Ephemeris %s)", swe_version(version));
    /* swe_version writes to the buffer, so get it first */
    char swe_ver[64];
    swe_version(swe_ver);
    snprintf(version, sizeof(version), "Astro Engine v1.0.0 (Swiss Ephemeris %s)", swe_ver);
    return version;
}
