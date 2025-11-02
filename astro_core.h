/**
 * @file astro_core.h
 * @brief Core ephemeris calculation module
 *
 * This module provides a simplified, type-safe interface to the Swiss Ephemeris
 * library for calculating planetary positions, house cusps, and other
 * fundamental astronomical data.
 */

#ifndef ASTRO_CORE_H
#define ASTRO_CORE_H

#include "astro_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================
 * Initialization and Configuration
 * ======================================================================== */

/**
 * Initialize the ephemeris engine with configuration
 *
 * @param config Configuration settings (can be NULL for defaults)
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_core_init(const AstroConfig *config);

/**
 * Set the ephemeris path
 *
 * @param path Path to ephemeris data files
 */
void astro_core_set_ephemeris_path(const char *path);

/**
 * Get the current configuration
 *
 * @param config Pointer to store configuration
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_core_get_config(AstroConfig *config);

/**
 * Clean up and free resources
 */
void astro_core_cleanup(void);

/* ========================================================================
 * Date and Time Calculations
 * ======================================================================== */

/**
 * Calculate Julian Day from calendar date
 *
 * @param datetime Date/time structure
 * @return Julian Day number (UT)
 */
double astro_core_get_julian_day(const AstroDateTime *datetime);

/**
 * Convert Julian Day to calendar date
 *
 * @param julian_day Julian Day number
 * @param datetime Pointer to store result
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_core_get_calendar_date(double julian_day, AstroDateTime *datetime);

/**
 * Get current date/time
 *
 * @param datetime Pointer to store current date/time
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_core_get_current_datetime(AstroDateTime *datetime);

/**
 * Calculate sidereal time
 *
 * @param datetime Date/time
 * @param longitude Geographic longitude
 * @return Sidereal time in degrees
 */
double astro_core_get_sidereal_time(const AstroDateTime *datetime, double longitude);

/* ========================================================================
 * Planet Position Calculations
 * ======================================================================== */

/**
 * Calculate position for a single planet
 *
 * @param planet Planet to calculate
 * @param datetime Date/time for calculation
 * @param flags Calculation flags (geocentric, heliocentric, etc.)
 * @param position Pointer to store planet position
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_core_calc_planet(AstroPlanet planet, const AstroDateTime *datetime,
                           int flags, AstroPlanetPosition *position);

/**
 * Calculate positions for all planets
 *
 * @param datetime Date/time for calculation
 * @param flags Calculation flags
 * @param positions Array to store positions (size ASTRO_MAX_PLANETS)
 * @param num_planets Pointer to store number of planets calculated
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_core_calc_planets(const AstroDateTime *datetime, int flags,
                            AstroPlanetPosition positions[], int *num_planets);

/**
 * Calculate planet position with topocentric correction
 *
 * @param planet Planet to calculate
 * @param datetime Date/time for calculation
 * @param location Geographic location
 * @param position Pointer to store planet position
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_core_calc_planet_topocentric(AstroPlanet planet,
                                       const AstroDateTime *datetime,
                                       const AstroLocation *location,
                                       AstroPlanetPosition *position);

/**
 * Get planet name
 *
 * @param planet Planet identifier
 * @return Planet name string (static, do not free)
 */
const char* astro_core_get_planet_name(AstroPlanet planet);

/**
 * Check if planet is retrograde
 *
 * @param position Planet position data
 * @return 1 if retrograde, 0 otherwise
 */
int astro_core_is_retrograde(const AstroPlanetPosition *position);

/* ========================================================================
 * House Calculations
 * ======================================================================== */

/**
 * Calculate house cusps
 *
 * @param datetime Date/time for calculation
 * @param location Geographic location
 * @param house_system House system to use
 * @param houses Pointer to store house data
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_core_calc_houses(const AstroDateTime *datetime,
                          const AstroLocation *location,
                          AstroHouseSystem house_system,
                          AstroHouses *houses);

/**
 * Determine which house a planet is in
 *
 * @param longitude Planet's ecliptic longitude
 * @param houses House cusp data
 * @return House number (1-12)
 */
int astro_core_get_house(double longitude, const AstroHouses *houses);

/**
 * Get house system name
 *
 * @param system House system identifier
 * @return House system name (static, do not free)
 */
const char* astro_core_get_house_system_name(AstroHouseSystem system);

/* ========================================================================
 * Zodiac Sign Functions
 * ======================================================================== */

/**
 * Get zodiac sign from longitude
 *
 * @param longitude Ecliptic longitude in degrees
 * @return Zodiac sign
 */
AstroSign astro_core_get_sign(double longitude);

/**
 * Get position within sign
 *
 * @param longitude Ecliptic longitude in degrees
 * @return Position within sign (0-30 degrees)
 */
double astro_core_get_sign_position(double longitude);

/**
 * Get sign name
 *
 * @param sign Zodiac sign
 * @return Sign name (static, do not free)
 */
const char* astro_core_get_sign_name(AstroSign sign);

/**
 * Get sign element
 *
 * @param sign Zodiac sign
 * @return Element (Fire, Earth, Air, Water)
 */
AstroElement astro_core_get_sign_element(AstroSign sign);

/**
 * Get sign modality
 *
 * @param sign Zodiac sign
 * @return Modality (Cardinal, Fixed, Mutable)
 */
AstroModality astro_core_get_sign_modality(AstroSign sign);

/* ========================================================================
 * Rising, Setting, and Transit Times
 * ======================================================================== */

/**
 * Calculate rise time for a planet
 *
 * @param planet Planet to calculate
 * @param datetime Date for calculation (time ignored)
 * @param location Geographic location
 * @param rise_time Pointer to store rise time
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_core_calc_rise(AstroPlanet planet, const AstroDateTime *datetime,
                        const AstroLocation *location, AstroDateTime *rise_time);

/**
 * Calculate set time for a planet
 *
 * @param planet Planet to calculate
 * @param datetime Date for calculation (time ignored)
 * @param location Geographic location
 * @param set_time Pointer to store set time
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_core_calc_set(AstroPlanet planet, const AstroDateTime *datetime,
                       const AstroLocation *location, AstroDateTime *set_time);

/**
 * Calculate transit time (culmination) for a planet
 *
 * @param planet Planet to calculate
 * @param datetime Date for calculation (time ignored)
 * @param location Geographic location
 * @param transit_time Pointer to store transit time
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_core_calc_transit(AstroPlanet planet, const AstroDateTime *datetime,
                           const AstroLocation *location, AstroDateTime *transit_time);

/* ========================================================================
 * Utility Functions
 * ======================================================================== */

/**
 * Normalize angle to 0-360 range
 *
 * @param angle Angle in degrees
 * @return Normalized angle
 */
double astro_core_normalize_angle(double angle);

/**
 * Calculate angular distance between two points
 *
 * @param lon1 First longitude
 * @param lon2 Second longitude
 * @return Angular distance (0-180 degrees)
 */
double astro_core_angular_distance(double lon1, double lon2);

/**
 * Format position as degrees, minutes, seconds
 *
 * @param degrees Position in decimal degrees
 * @param buffer Buffer to store formatted string
 * @param buffer_size Size of buffer
 * @return Formatted string (same as buffer)
 */
char* astro_core_format_dms(double degrees, char *buffer, int buffer_size);

/**
 * Get error message for error code
 *
 * @param error_code Error code
 * @return Error message string (static, do not free)
 */
const char* astro_core_get_error_message(AstroErrorCode error_code);

/**
 * Get library version information
 *
 * @return Version string (static, do not free)
 */
const char* astro_core_get_version(void);

#ifdef __cplusplus
}
#endif

#endif /* ASTRO_CORE_H */
