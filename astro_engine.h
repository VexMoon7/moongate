/**
 * @file astro_engine.h
 * @brief Unified API for the Astrological Data Analysis Engine
 *
 * This header provides a single unified interface to all functionality
 * of the modular ephemeris and astrological data analysis engine.
 * Include only this file to access all features.
 */

#ifndef ASTRO_ENGINE_H
#define ASTRO_ENGINE_H

/* Include all module headers */
#include "astro_types.h"
#include "astro_core.h"
#include "astro_aspects.h"
#include "astro_chart.h"
#include "astro_transits.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @mainpage Astrological Data Analysis Engine
 *
 * A modular, professional-grade ephemeris and astrological data analysis
 * engine built on the Swiss Ephemeris library.
 *
 * @section features Features
 *
 * - **Core Ephemeris Calculations**: Planet positions, houses, rising/setting times
 * - **Aspect Analysis**: Major and minor aspects with pattern recognition
 * - **Natal Charts**: Complete birth chart calculation and analysis
 * - **Transits**: Current and future transits to natal positions
 * - **Progressions**: Secondary, tertiary, and solar arc progressions
 * - **Synastry**: Relationship compatibility analysis
 * - **Multiple Output Formats**: JSON, CSV, formatted text
 *
 * @section modules Architecture
 *
 * The engine is organized into modular components:
 *
 * - **astro_core**: Low-level ephemeris calculations
 * - **astro_aspects**: Aspect calculation and pattern detection
 * - **astro_chart**: Natal chart creation and analysis
 * - **astro_transits**: Transit and progression calculations
 *
 * @section usage Quick Start
 *
 * @code
 * #include "astro_engine.h"
 *
 * int main() {
 *     // Initialize engine
 *     astro_engine_init(NULL);
 *
 *     // Create birth data
 *     AstroDateTime birth_time = {
 *         .year = 1990, .month = 1, .day = 15,
 *         .hour = 14, .minute = 30, .second = 0,
 *         .timezone_offset = 0.0
 *     };
 *
 *     AstroLocation birth_location = {
 *         .latitude = 51.5074,  // London
 *         .longitude = -0.1278,
 *         .altitude = 0.0
 *     };
 *
 *     // Create natal chart
 *     AstroChart chart;
 *     astro_chart_create(&birth_time, &birth_location, NULL, &chart);
 *
 *     // Print chart
 *     astro_chart_print(&chart, stdout);
 *
 *     // Cleanup
 *     astro_engine_cleanup();
 *
 *     return 0;
 * }
 * @endcode
 */

/* ========================================================================
 * Engine Initialization and Configuration
 * ======================================================================== */

/**
 * Initialize the astrological engine
 *
 * This is a convenience function that initializes the core module.
 * Must be called before using any engine functions.
 *
 * @param config Configuration (NULL for defaults)
 * @return ASTRO_OK on success, error code otherwise
 */
static inline int astro_engine_init(const AstroConfig *config) {
    return astro_core_init(config);
}

/**
 * Get default configuration
 *
 * @param config Pointer to store default configuration
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_engine_get_default_config(AstroConfig *config);

/**
 * Cleanup and free resources
 *
 * Should be called when done using the engine.
 */
static inline void astro_engine_cleanup(void) {
    astro_core_cleanup();
}

/**
 * Get engine version string
 *
 * @return Version string (static, do not free)
 */
static inline const char* astro_engine_get_version(void) {
    return astro_core_get_version();
}

/* ========================================================================
 * Quick Access Functions
 * ======================================================================== */

/**
 * Quick create and calculate natal chart
 *
 * Convenience function that creates a chart with default settings.
 *
 * @param year Birth year
 * @param month Birth month (1-12)
 * @param day Birth day (1-31)
 * @param hour Birth hour (0-23)
 * @param minute Birth minute (0-59)
 * @param second Birth second (0-59)
 * @param timezone_offset Timezone offset from UTC (hours)
 * @param latitude Birth latitude (degrees)
 * @param longitude Birth longitude (degrees)
 * @param chart Pointer to store chart
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_engine_quick_chart(int year, int month, int day,
                             int hour, int minute, double second,
                             double timezone_offset,
                             double latitude, double longitude,
                             AstroChart *chart);

/**
 * Calculate current transits for a natal chart
 *
 * Convenience function to get current transits.
 *
 * @param natal_chart Natal chart
 * @param transits Array to store transits
 * @param max_transits Maximum transits
 * @param num_transits Pointer to store count
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_engine_current_transits(const AstroChart *natal_chart,
                                  AstroTransit transits[],
                                  int max_transits,
                                  int *num_transits);

/**
 * Calculate planet position for current time
 *
 * @param planet Planet to calculate
 * @param position Pointer to store position
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_engine_current_position(AstroPlanet planet,
                                  AstroPlanetPosition *position);

/* ========================================================================
 * Module Information
 * ======================================================================== */

/**
 * Module information
 */
typedef struct {
    const char *name;
    const char *description;
    const char *version;
} AstroModuleInfo;

/**
 * Get list of available modules
 *
 * @param modules Array to store module info
 * @param max_modules Maximum modules
 * @param num_modules Pointer to store count
 * @return ASTRO_OK on success, error code otherwise
 */
int astro_engine_get_modules(AstroModuleInfo modules[],
                             int max_modules,
                             int *num_modules);

/* ========================================================================
 * Utility Functions
 * ======================================================================== */

/**
 * Validate date/time
 *
 * @param datetime Date/time to validate
 * @return 1 if valid, 0 otherwise
 */
int astro_engine_validate_datetime(const AstroDateTime *datetime);

/**
 * Validate location
 *
 * @param location Location to validate
 * @return 1 if valid, 0 otherwise
 */
int astro_engine_validate_location(const AstroLocation *location);

/**
 * Get element name
 *
 * @param element Element type
 * @return Element name (static, do not free)
 */
const char* astro_engine_get_element_name(AstroElement element);

/**
 * Get modality name
 *
 * @param modality Modality type
 * @return Modality name (static, do not free)
 */
const char* astro_engine_get_modality_name(AstroModality modality);

/**
 * Print engine information
 *
 * @param fp File pointer
 */
void astro_engine_print_info(FILE *fp);

#ifdef __cplusplus
}
#endif

#endif /* ASTRO_ENGINE_H */
