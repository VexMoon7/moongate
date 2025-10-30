/**
 * @file astro_chart.c
 * @brief Natal chart calculation and management implementation
 */

#include "astro_chart.h"
#include "astro_core.h"
#include "astro_aspects.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* ========================================================================
 * Chart Creation and Calculation
 * ======================================================================== */

int astro_chart_create(const AstroDateTime *datetime,
                       const AstroLocation *location,
                       const AstroConfig *config,
                       AstroChart *chart) {
    if (datetime == NULL || location == NULL || chart == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    /* Initialize chart */
    memset(chart, 0, sizeof(AstroChart));

    /* Copy birth data */
    memcpy(&chart->birth_time, datetime, sizeof(AstroDateTime));
    memcpy(&chart->birth_location, location, sizeof(AstroLocation));

    /* Initialize engine with config if provided */
    if (config != NULL) {
        astro_core_init(config);
    }

    /* Calculate all chart data */
    return astro_chart_calculate_all(chart);
}

int astro_chart_calculate_all(AstroChart *chart) {
    if (chart == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    int result;

    /* Calculate planets */
    result = astro_chart_update_planets(chart);
    if (result != ASTRO_OK) {
        return result;
    }

    /* Calculate houses */
    result = astro_chart_update_houses(chart);
    if (result != ASTRO_OK) {
        return result;
    }

    /* Update planet house positions */
    for (int i = 0; i < chart->num_planets; i++) {
        chart->planets[i].house = astro_core_get_house(
            chart->planets[i].longitude, &chart->houses);
    }

    /* Calculate aspects */
    result = astro_chart_update_aspects(chart);
    if (result != ASTRO_OK) {
        return result;
    }

    return ASTRO_OK;
}

int astro_chart_update_planets(AstroChart *chart) {
    if (chart == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    return astro_core_calc_planets(&chart->birth_time, 0,
                                   chart->planets, &chart->num_planets);
}

int astro_chart_update_houses(AstroChart *chart) {
    if (chart == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    AstroConfig config;
    astro_core_get_config(&config);

    return astro_core_calc_houses(&chart->birth_time,
                                  &chart->birth_location,
                                  config.house_system,
                                  &chart->houses);
}

int astro_chart_update_aspects(AstroChart *chart) {
    if (chart == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    return astro_aspects_calc_all(chart->planets, chart->num_planets,
                                  chart->aspects, ASTRO_MAX_ASPECTS,
                                  &chart->num_aspects);
}

/* ========================================================================
 * Chart Queries
 * ======================================================================== */

const AstroPlanetPosition* astro_chart_get_planet(const AstroChart *chart,
                                                   AstroPlanet planet) {
    if (chart == NULL) {
        return NULL;
    }

    for (int i = 0; i < chart->num_planets; i++) {
        if (chart->planets[i].planet == planet) {
            return &chart->planets[i];
        }
    }

    return NULL;
}

int astro_chart_get_planets_in_sign(const AstroChart *chart,
                                    AstroSign sign,
                                    const AstroPlanetPosition *planets[],
                                    int max_planets,
                                    int *num_planets) {
    if (chart == NULL || planets == NULL || num_planets == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    int count = 0;
    for (int i = 0; i < chart->num_planets && count < max_planets; i++) {
        if (chart->planets[i].sign == sign) {
            planets[count++] = &chart->planets[i];
        }
    }

    *num_planets = count;
    return ASTRO_OK;
}

int astro_chart_get_planets_in_house(const AstroChart *chart,
                                     int house,
                                     const AstroPlanetPosition *planets[],
                                     int max_planets,
                                     int *num_planets) {
    if (chart == NULL || planets == NULL || num_planets == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    if (house < 1 || house > 12) {
        return ASTRO_ERROR_OUT_OF_RANGE;
    }

    int count = 0;
    for (int i = 0; i < chart->num_planets && count < max_planets; i++) {
        if (chart->planets[i].house == house) {
            planets[count++] = &chart->planets[i];
        }
    }

    *num_planets = count;
    return ASTRO_OK;
}

int astro_chart_get_planet_aspects(const AstroChart *chart,
                                   AstroPlanet planet,
                                   const AstroAspect *aspects[],
                                   int max_aspects,
                                   int *num_aspects) {
    if (chart == NULL || aspects == NULL || num_aspects == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    int count = 0;
    for (int i = 0; i < chart->num_aspects && count < max_aspects; i++) {
        if (chart->aspects[i].planet1 == planet ||
            chart->aspects[i].planet2 == planet) {
            aspects[count++] = &chart->aspects[i];
        }
    }

    *num_aspects = count;
    return ASTRO_OK;
}

/* ========================================================================
 * Chart Analysis
 * ======================================================================== */

int astro_chart_analyze_elements(const AstroChart *chart,
                                 AstroElementBalance *balance) {
    if (chart == NULL || balance == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    memset(balance, 0, sizeof(AstroElementBalance));

    /* Count planets by element */
    for (int i = 0; i < chart->num_planets; i++) {
        AstroElement element = astro_core_get_sign_element(chart->planets[i].sign);
        switch (element) {
            case ASTRO_ELEMENT_FIRE:
                balance->fire_count++;
                break;
            case ASTRO_ELEMENT_EARTH:
                balance->earth_count++;
                break;
            case ASTRO_ELEMENT_AIR:
                balance->air_count++;
                break;
            case ASTRO_ELEMENT_WATER:
                balance->water_count++;
                break;
        }
    }

    /* Determine dominant element */
    int max_count = balance->fire_count;
    balance->dominant_element = ASTRO_ELEMENT_FIRE;

    if (balance->earth_count > max_count) {
        max_count = balance->earth_count;
        balance->dominant_element = ASTRO_ELEMENT_EARTH;
    }
    if (balance->air_count > max_count) {
        max_count = balance->air_count;
        balance->dominant_element = ASTRO_ELEMENT_AIR;
    }
    if (balance->water_count > max_count) {
        max_count = balance->water_count;
        balance->dominant_element = ASTRO_ELEMENT_WATER;
    }

    return ASTRO_OK;
}

int astro_chart_analyze_modalities(const AstroChart *chart,
                                   AstroModalityBalance *balance) {
    if (chart == NULL || balance == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    memset(balance, 0, sizeof(AstroModalityBalance));

    /* Count planets by modality */
    for (int i = 0; i < chart->num_planets; i++) {
        AstroModality modality = astro_core_get_sign_modality(chart->planets[i].sign);
        switch (modality) {
            case ASTRO_MODALITY_CARDINAL:
                balance->cardinal_count++;
                break;
            case ASTRO_MODALITY_FIXED:
                balance->fixed_count++;
                break;
            case ASTRO_MODALITY_MUTABLE:
                balance->mutable_count++;
                break;
        }
    }

    /* Determine dominant modality */
    int max_count = balance->cardinal_count;
    balance->dominant_modality = ASTRO_MODALITY_CARDINAL;

    if (balance->fixed_count > max_count) {
        max_count = balance->fixed_count;
        balance->dominant_modality = ASTRO_MODALITY_FIXED;
    }
    if (balance->mutable_count > max_count) {
        balance->dominant_modality = ASTRO_MODALITY_MUTABLE;
    }

    return ASTRO_OK;
}

AstroPlanet astro_chart_get_ruler(const AstroChart *chart) {
    if (chart == NULL) {
        return -1;
    }

    AstroSign asc_sign = astro_core_get_sign(chart->houses.ascendant);

    /* Traditional rulers */
    switch (asc_sign) {
        case ASTRO_ARIES: return ASTRO_MARS;
        case ASTRO_TAURUS: return ASTRO_VENUS;
        case ASTRO_GEMINI: return ASTRO_MERCURY;
        case ASTRO_CANCER: return ASTRO_MOON;
        case ASTRO_LEO: return ASTRO_SUN;
        case ASTRO_VIRGO: return ASTRO_MERCURY;
        case ASTRO_LIBRA: return ASTRO_VENUS;
        case ASTRO_SCORPIO: return ASTRO_MARS; /* Traditional, modern: Pluto */
        case ASTRO_SAGITTARIUS: return ASTRO_JUPITER;
        case ASTRO_CAPRICORN: return ASTRO_SATURN;
        case ASTRO_AQUARIUS: return ASTRO_SATURN; /* Traditional, modern: Uranus */
        case ASTRO_PISCES: return ASTRO_JUPITER; /* Traditional, modern: Neptune */
        default: return -1;
    }
}

AstroChartShape astro_chart_get_shape(const AstroChart *chart) {
    if (chart == NULL || chart->num_planets < 3) {
        return ASTRO_CHART_SHAPE_SPLASH;
    }

    /* Find angular spread */
    double min_lon = 360.0;
    double max_lon = 0.0;
    double total_span = 0.0;

    for (int i = 0; i < chart->num_planets; i++) {
        double lon = chart->planets[i].longitude;
        if (lon < min_lon) min_lon = lon;
        if (lon > max_lon) max_lon = lon;
    }

    total_span = max_lon - min_lon;

    /* Bundle: all within 120° */
    if (total_span <= 120.0) {
        return ASTRO_CHART_SHAPE_BUNDLE;
    }

    /* Bowl: all within 180° */
    if (total_span <= 180.0) {
        return ASTRO_CHART_SHAPE_BOWL;
    }

    /* Check for locomotive (empty trine) */
    int max_gap = 0;
    for (int i = 0; i < chart->num_planets; i++) {
        int next = (i + 1) % chart->num_planets;
        double gap = chart->planets[next].longitude - chart->planets[i].longitude;
        if (gap < 0) gap += 360.0;
        if (gap > max_gap) max_gap = (int)gap;
    }

    if (max_gap >= 120) {
        return ASTRO_CHART_SHAPE_LOCOMOTIVE;
    }

    /* Default to splash */
    return ASTRO_CHART_SHAPE_SPLASH;
}

/* ========================================================================
 * Chart Comparison
 * ======================================================================== */

int astro_chart_calc_synastry(const AstroChart *chart1,
                              const AstroChart *chart2,
                              AstroSynastry *synastry) {
    if (chart1 == NULL || chart2 == NULL || synastry == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    memset(synastry, 0, sizeof(AstroSynastry));
    synastry->chart1 = chart1;
    synastry->chart2 = chart2;

    /* Calculate inter-chart aspects */
    int result = astro_aspects_calc_between_charts(
        chart1->planets, chart1->num_planets,
        chart2->planets, chart2->num_planets,
        synastry->aspects, ASTRO_MAX_ASPECTS,
        &synastry->num_aspects);

    if (result != ASTRO_OK) {
        return result;
    }

    /* Calculate compatibility score */
    double score = 50.0; /* Base score */

    for (int i = 0; i < synastry->num_aspects; i++) {
        if (astro_aspects_is_harmonious(synastry->aspects[i].aspect_type) == 1) {
            score += 5.0 * astro_aspects_get_strength(&synastry->aspects[i]);
        } else if (astro_aspects_is_harmonious(synastry->aspects[i].aspect_type) == 0) {
            score -= 3.0 * astro_aspects_get_strength(&synastry->aspects[i]);
        }
    }

    /* Clamp to 0-100 */
    if (score < 0) score = 0;
    if (score > 100) score = 100;

    synastry->compatibility_score = score;

    return ASTRO_OK;
}

int astro_chart_calc_composite(const AstroChart *chart1,
                               const AstroChart *chart2,
                               AstroChart *composite) {
    if (chart1 == NULL || chart2 == NULL || composite == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    memset(composite, 0, sizeof(AstroChart));

    /* Calculate midpoint time */
    double jd1 = chart1->birth_time.julian_day;
    double jd2 = chart2->birth_time.julian_day;
    double jd_mid = (jd1 + jd2) / 2.0;

    astro_core_get_calendar_date(jd_mid, &composite->birth_time);

    /* Calculate midpoint location */
    composite->birth_location.latitude =
        (chart1->birth_location.latitude + chart2->birth_location.latitude) / 2.0;
    composite->birth_location.longitude =
        (chart1->birth_location.longitude + chart2->birth_location.longitude) / 2.0;
    composite->birth_location.altitude =
        (chart1->birth_location.altitude + chart2->birth_location.altitude) / 2.0;

    strncpy(composite->chart_name, "Composite Chart", ASTRO_MAX_NAME_LEN);

    /* Calculate composite chart */
    return astro_chart_calculate_all(composite);
}

/* ========================================================================
 * Chart Output
 * ======================================================================== */

int astro_chart_print(const AstroChart *chart, FILE *fp) {
    if (chart == NULL || fp == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    fprintf(fp, "\n========================================\n");
    fprintf(fp, "  NATAL CHART: %s\n", chart->chart_name[0] ? chart->chart_name : "Unnamed");
    fprintf(fp, "========================================\n\n");

    fprintf(fp, "Birth Data:\n");
    fprintf(fp, "  Date: %04d-%02d-%02d %02d:%02d:%02.0f (UTC%+.1f)\n",
            chart->birth_time.year, chart->birth_time.month, chart->birth_time.day,
            chart->birth_time.hour, chart->birth_time.minute, chart->birth_time.second,
            chart->birth_time.timezone_offset);
    fprintf(fp, "  Location: %.4f°%c, %.4f°%c\n",
            fabs(chart->birth_location.latitude),
            chart->birth_location.latitude >= 0 ? 'N' : 'S',
            fabs(chart->birth_location.longitude),
            chart->birth_location.longitude >= 0 ? 'E' : 'W');
    fprintf(fp, "  Julian Day: %.5f\n\n", chart->birth_time.julian_day);

    astro_chart_print_positions(chart, fp);
    astro_chart_print_houses(chart, fp);
    astro_chart_print_aspects(chart, fp);

    return ASTRO_OK;
}

int astro_chart_print_positions(const AstroChart *chart, FILE *fp) {
    if (chart == NULL || fp == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    fprintf(fp, "Planet Positions:\n");
    fprintf(fp, "%-12s %-12s %8s %5s %6s\n",
            "Planet", "Sign", "Position", "House", "Speed");
    fprintf(fp, "%-12s %-12s %8s %5s %6s\n",
            "------", "----", "--------", "-----", "-----");

    for (int i = 0; i < chart->num_planets; i++) {
        const AstroPlanetPosition *pos = &chart->planets[i];
        fprintf(fp, "%-12s %-12s %3d°%02d' %5d  %6.3f%s\n",
                pos->name,
                astro_core_get_sign_name(pos->sign),
                (int)pos->sign_position,
                (int)((pos->sign_position - (int)pos->sign_position) * 60),
                pos->house,
                fabs(pos->longitude_speed),
                pos->is_retrograde ? "R" : "");
    }
    fprintf(fp, "\n");

    return ASTRO_OK;
}

int astro_chart_print_aspects(const AstroChart *chart, FILE *fp) {
    if (chart == NULL || fp == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    fprintf(fp, "Aspects (%d found):\n", chart->num_aspects);

    for (int i = 0; i < chart->num_aspects; i++) {
        const AstroAspect *asp = &chart->aspects[i];
        fprintf(fp, "  %-10s %-12s %-10s (orb: %.2f°) %s\n",
                astro_core_get_planet_name(asp->planet1),
                astro_aspects_get_name(asp->aspect_type),
                astro_core_get_planet_name(asp->planet2),
                asp->difference,
                asp->is_applying == 1 ? "applying" :
                asp->is_applying == 0 ? "separating" : "stationary");
    }
    fprintf(fp, "\n");

    return ASTRO_OK;
}

int astro_chart_print_houses(const AstroChart *chart, FILE *fp) {
    if (chart == NULL || fp == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    fprintf(fp, "House Cusps:\n");
    for (int i = 1; i <= 12; i++) {
        double cusp = chart->houses.cusp[i];
        AstroSign sign = astro_core_get_sign(cusp);
        double sign_pos = astro_core_get_sign_position(cusp);

        fprintf(fp, "  House %2d: %3d°%02d' %-12s\n",
                i,
                (int)sign_pos,
                (int)((sign_pos - (int)sign_pos) * 60),
                astro_core_get_sign_name(sign));
    }

    fprintf(fp, "\n  Ascendant: %.2f°\n", chart->houses.ascendant);
    fprintf(fp, "  MC:        %.2f°\n\n", chart->houses.mc);

    return ASTRO_OK;
}

int astro_chart_export_json(const AstroChart *chart, const char *filename) {
    if (chart == NULL || filename == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        return ASTRO_ERROR_EPHEMERIS_FILE;
    }

    fprintf(fp, "{\n");
    fprintf(fp, "  \"chart_name\": \"%s\",\n", chart->chart_name);
    fprintf(fp, "  \"birth_time\": {\n");
    fprintf(fp, "    \"year\": %d,\n", chart->birth_time.year);
    fprintf(fp, "    \"month\": %d,\n", chart->birth_time.month);
    fprintf(fp, "    \"day\": %d,\n", chart->birth_time.day);
    fprintf(fp, "    \"hour\": %d,\n", chart->birth_time.hour);
    fprintf(fp, "    \"minute\": %d,\n", chart->birth_time.minute);
    fprintf(fp, "    \"julian_day\": %.6f\n", chart->birth_time.julian_day);
    fprintf(fp, "  },\n");

    fprintf(fp, "  \"planets\": [\n");
    for (int i = 0; i < chart->num_planets; i++) {
        const AstroPlanetPosition *pos = &chart->planets[i];
        fprintf(fp, "    {\"name\": \"%s\", \"longitude\": %.6f, \"sign\": \"%s\", \"house\": %d, \"retrograde\": %s}%s\n",
                pos->name, pos->longitude, astro_core_get_sign_name(pos->sign),
                pos->house, pos->is_retrograde ? "true" : "false",
                i < chart->num_planets - 1 ? "," : "");
    }
    fprintf(fp, "  ]\n");

    fprintf(fp, "}\n");

    fclose(fp);
    return ASTRO_OK;
}

int astro_chart_export_csv(const AstroChart *chart, const char *filename) {
    if (chart == NULL || filename == NULL) {
        return ASTRO_ERROR_NULL_POINTER;
    }

    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        return ASTRO_ERROR_EPHEMERIS_FILE;
    }

    fprintf(fp, "Planet,Longitude,Latitude,Sign,House,Retrograde,Speed\n");
    for (int i = 0; i < chart->num_planets; i++) {
        const AstroPlanetPosition *pos = &chart->planets[i];
        fprintf(fp, "%s,%.6f,%.6f,%s,%d,%d,%.6f\n",
                pos->name, pos->longitude, pos->latitude,
                astro_core_get_sign_name(pos->sign), pos->house,
                pos->is_retrograde, pos->longitude_speed);
    }

    fclose(fp);
    return ASTRO_OK;
}
