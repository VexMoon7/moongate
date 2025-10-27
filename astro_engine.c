/**
 * @file astro_engine.c
 * @brief Unified API implementation
 */

#include "astro_engine.h"
#include <stdio.h>
#include <string.h>

/* ========================================================================
 * Engine Configuration
 * ======================================================================== */

int astro_engine_get_default_config(AstroConfig *config) {
    if (config == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    config->house_system = ASTRO_HOUSE_PLACIDUS;
    config->coordinate_system = ASTRO_COORD_TROPICAL;
    config->include_asteroids = 1;
    config->include_nodes = 1;
    config->include_lilith = 1;
    config->aspect_orb_major = 8.0;
    config->aspect_orb_minor = 3.0;
    config->use_true_node = 0;
    strncpy(config->ephemeris_path, "./ephe", sizeof(config->ephemeris_path) - 1);
    config->ephemeris_path[sizeof(config->ephemeris_path) - 1] = '\0';

    return ASTRO_OK;
}

/* ========================================================================
 * Quick Access Functions
 * ======================================================================== */

int astro_engine_quick_chart(int year, int month, int day,
                             int hour, int minute, double second,
                             double timezone_offset,
                             double latitude, double longitude,
                             AstroChart *chart) {
    if (chart == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    AstroDateTime birth_time = {
        .year = year,
        .month = month,
        .day = day,
        .hour = hour,
        .minute = minute,
        .second = second,
        .timezone_offset = timezone_offset
    };
    birth_time.julian_day = astro_core_get_julian_day(&birth_time);

    AstroLocation birth_location = {
        .latitude = latitude,
        .longitude = longitude,
        .altitude = 0.0
    };
    strncpy(birth_location.location_name, "Location", ASTRO_MAX_NAME_LEN - 1);

    return astro_chart_create(&birth_time, &birth_location, NULL, chart);
}

int astro_engine_current_transits(const AstroChart *natal_chart,
                                  AstroTransit transits[],
                                  int max_transits,
                                  int *num_transits) {
    if (natal_chart == NULL || transits == NULL || num_transits == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    AstroDateTime current_time;
    int result = astro_core_get_current_datetime(&current_time);
    if (result != ASTRO_OK) {
        return result;
    }

    return astro_transits_calc_current(natal_chart, &current_time,
                                      transits, max_transits, num_transits);
}

int astro_engine_current_position(AstroPlanet planet,
                                  AstroPlanetPosition *position) {
    if (position == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    AstroDateTime current_time;
    int result = astro_core_get_current_datetime(&current_time);
    if (result != ASTRO_OK) {
        return result;
    }

    return astro_core_calc_planet(planet, &current_time, 0, position);
}

/* ========================================================================
 * Module Information
 * ======================================================================== */

int astro_engine_get_modules(AstroModuleInfo modules[],
                             int max_modules,
                             int *num_modules) {
    if (modules == NULL || num_modules == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    const AstroModuleInfo module_list[] = {
        {"astro_core", "Core ephemeris calculations and planet positions", "1.0.0"},
        {"astro_aspects", "Aspect calculation and pattern recognition", "1.0.0"},
        {"astro_chart", "Natal chart creation and analysis", "1.0.0"},
        {"astro_transits", "Transit and progression calculations", "1.0.0"}
    };

    int count = sizeof(module_list) / sizeof(module_list[0]);
    if (count > max_modules) {
        count = max_modules;
    }

    for (int i = 0; i < count; i++) {
        modules[i] = module_list[i];
    }

    *num_modules = count;
    return ASTRO_OK;
}

/* ========================================================================
 * Utility Functions
 * ======================================================================== */

int astro_engine_validate_datetime(const AstroDateTime *datetime) {
    if (datetime == NULL) {
        return 0;
    }

    if (datetime->year < -10000 || datetime->year > 10000) {
        return 0;
    }

    if (datetime->month < 1 || datetime->month > 12) {
        return 0;
    }

    if (datetime->day < 1 || datetime->day > 31) {
        return 0;
    }

    if (datetime->hour < 0 || datetime->hour > 23) {
        return 0;
    }

    if (datetime->minute < 0 || datetime->minute > 59) {
        return 0;
    }

    if (datetime->second < 0.0 || datetime->second >= 60.0) {
        return 0;
    }

    return 1;
}

int astro_engine_validate_location(const AstroLocation *location) {
    if (location == NULL) {
        return 0;
    }

    if (location->latitude < -90.0 || location->latitude > 90.0) {
        return 0;
    }

    if (location->longitude < -180.0 || location->longitude > 180.0) {
        return 0;
    }

    return 1;
}

const char* astro_engine_get_element_name(AstroElement element) {
    switch (element) {
        case ASTRO_ELEMENT_FIRE: return "Fire";
        case ASTRO_ELEMENT_EARTH: return "Earth";
        case ASTRO_ELEMENT_AIR: return "Air";
        case ASTRO_ELEMENT_WATER: return "Water";
        default: return "Unknown";
    }
}

const char* astro_engine_get_modality_name(AstroModality modality) {
    switch (modality) {
        case ASTRO_MODALITY_CARDINAL: return "Cardinal";
        case ASTRO_MODALITY_FIXED: return "Fixed";
        case ASTRO_MODALITY_MUTABLE: return "Mutable";
        default: return "Unknown";
    }
}

void astro_engine_print_info(FILE *fp) {
    if (fp == NULL) {
        return;
    }

    fprintf(fp, "\n");
    fprintf(fp, "================================================================\n");
    fprintf(fp, "  Astrological Data Analysis Engine\n");
    fprintf(fp, "  Version: %s\n", astro_engine_get_version());
    fprintf(fp, "================================================================\n");
    fprintf(fp, "\n");

    fprintf(fp, "A modular ephemeris and astrological data analysis engine\n");
    fprintf(fp, "built on the Swiss Ephemeris library.\n\n");

    fprintf(fp, "Modules:\n");
    AstroModuleInfo modules[10];
    int num_modules;

    if (astro_engine_get_modules(modules, 10, &num_modules) == ASTRO_OK) {
        for (int i = 0; i < num_modules; i++) {
            fprintf(fp, "  - %-20s : %s (v%s)\n",
                   modules[i].name,
                   modules[i].description,
                   modules[i].version);
        }
    }

    fprintf(fp, "\nFeatures:\n");
    fprintf(fp, "  - Core ephemeris calculations (planets, houses, angles)\n");
    fprintf(fp, "  - Aspect analysis with pattern recognition\n");
    fprintf(fp, "  - Complete natal chart calculation\n");
    fprintf(fp, "  - Transit and progression analysis\n");
    fprintf(fp, "  - Synastry (relationship compatibility)\n");
    fprintf(fp, "  - Multiple export formats (JSON, CSV, text)\n");

    fprintf(fp, "\nSupported Features:\n");
    fprintf(fp, "  - 23 celestial bodies (planets, asteroids, nodes)\n");
    fprintf(fp, "  - 9 house systems\n");
    fprintf(fp, "  - 11 aspect types\n");
    fprintf(fp, "  - Tropical and sidereal zodiacs\n");
    fprintf(fp, "  - Multiple progression types\n");
    fprintf(fp, "  - Solar, lunar, and planetary returns\n");

    fprintf(fp, "\n");
}
