/**
 * @file chart_and_synastry_example.c
 * @brief Complete example: Create charts, add moons, perform synastry
 *
 * This program demonstrates:
 * 1. Creating natal charts for two people
 * 2. Adding planetary moon analysis
 * 3. Performing relationship synastry
 * 4. Complete reading with mythology
 */

#include "astro_engine.h"
#include "astro_planetary_moons.h"
#include "astro_sabian.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* ========================================================================
 * Helper function to set up a location
 * ======================================================================== */

void setup_location(AstroLocation *loc, double lat, double lon, const char *name) {
    loc->latitude = lat;
    loc->longitude = lon;
    loc->altitude = 0.0;
    strncpy(loc->location_name, name, ASTRO_MAX_NAME_LEN - 1);
    loc->location_name[ASTRO_MAX_NAME_LEN - 1] = '\0';
}

/* ========================================================================
 * Create a natal chart with full analysis
 * ======================================================================== */

int create_chart_with_analysis(const char *name,
                               int year, int month, int day,
                               int hour, int minute,
                               double lat, double lon,
                               const char *location_name,
                               AstroChart *chart) {

    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  Creating Chart for %s\n", name);
    printf("╚════════════════════════════════════════════════════════════════╝\n\n");

    /* Set up birth data */
    AstroDateTime birth_time = {
        .year = year,
        .month = month,
        .day = day,
        .hour = hour,
        .minute = minute,
        .second = 0,
        .timezone_offset = 0.0  // UTC
    };

    AstroLocation birth_location;
    setup_location(&birth_location, lat, lon, location_name);

    /* Create the chart */
    int result = astro_chart_create(&birth_time, &birth_location, NULL, chart);
    if (result != ASTRO_OK) {
        printf("Error creating chart: %d\n", result);
        return result;
    }

    /* Save name */
    strncpy(chart->chart_name, name, ASTRO_MAX_NAME_LEN - 1);
    chart->chart_name[ASTRO_MAX_NAME_LEN - 1] = '\0';

    printf("✓ Chart created successfully\n");
    printf("  Date: %04d-%02d-%02d %02d:%02d UTC\n",
           year, month, day, hour, minute);
    printf("  Location: %s (%.2f°, %.2f°)\n\n",
           location_name, lat, lon);

    /* Print basic planetary positions */
    printf("PLANETARY POSITIONS\n");
    printf("────────────────────────────────────────────────────────────────\n");
    for (int i = 0; i < chart->num_planets && i < 10; i++) {
        const AstroPlanetPosition *p = &chart->planets[i];
        printf("%-10s %02.0f°%s%02d' %s %s\n",
               p->name,
               floor(p->sign_position),
               astro_core_get_sign_symbol(p->sign),
               (int)((p->sign_position - floor(p->sign_position)) * 60),
               astro_core_get_sign_name(p->sign),
               p->is_retrograde ? "(R)" : "");
    }
    printf("\n");

    return ASTRO_OK;
}

/* ========================================================================
 * Add planetary moon analysis to a chart
 * ======================================================================== */

void add_moon_analysis(const AstroChart *chart) {
    printf("PLANETARY MOON CONJUNCTIONS\n");
    printf("────────────────────────────────────────────────────────────────\n\n");

    /* Find moon conjunctions with natal planets */
    MoonPlanetConjunction conjunctions[50];
    int num_conj;

    int result = astro_moons_find_conjunctions(chart, &chart->birth_time, 2.0,
                                              conjunctions, 50, &num_conj);

    if (result != ASTRO_OK || num_conj == 0) {
        printf("No significant moon conjunctions found (within 2° orb)\n\n");
        return;
    }

    printf("Found %d conjunction(s):\n\n", num_conj);

    for (int i = 0; i < num_conj; i++) {
        MoonPlanetConjunction *conj = &conjunctions[i];
        const PlanetaryMoonData *moon_data = astro_moons_get_data(conj->moon_id);

        printf("%s %s (%s) conjunct natal %s (orb: %.2f°)%s\n",
               conj->is_exact ? "★" : "○",
               conj->moon_name,
               moon_data->parent_name,
               conj->planet_name,
               conj->orb,
               conj->is_exact ? " [EXACT]" : "");

        printf("   Archetype: %s\n", moon_data->archetype);
        printf("   Theme: %s\n", moon_data->psychological_theme);
        printf("   Shadow: %s\n", moon_data->shadow_expression);
        printf("   Gift: %s\n\n", moon_data->gift_expression);
    }
}

/* ========================================================================
 * Perform synastry analysis between two charts
 * ======================================================================== */

void perform_synastry(const AstroChart *chart1, const AstroChart *chart2) {
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  SYNASTRY ANALYSIS\n");
    printf("║  %s & %s\n", chart1->chart_name, chart2->chart_name);
    printf("╚════════════════════════════════════════════════════════════════╝\n\n");

    /* Calculate synastry */
    AstroSynastry synastry;
    int result = astro_chart_calc_synastry(chart1, chart2, &synastry);

    if (result != ASTRO_OK) {
        printf("Error calculating synastry: %d\n", result);
        return;
    }

    /* Print compatibility score */
    printf("COMPATIBILITY SCORE: %.1f/100\n", synastry.compatibility_score);
    printf("────────────────────────────────────────────────────────────────\n\n");

    if (synastry.compatibility_score > 75) {
        printf("✨ Excellent compatibility - Strong harmonious connection\n\n");
    } else if (synastry.compatibility_score > 50) {
        printf("💫 Good compatibility - Balanced dynamic\n\n");
    } else if (synastry.compatibility_score > 25) {
        printf("⚡ Moderate compatibility - Growth through challenge\n\n");
    } else {
        printf("🔥 Challenging compatibility - Intense learning curve\n\n");
    }

    /* Print inter-chart aspects */
    printf("INTER-CHART ASPECTS (%d found)\n", synastry.num_aspects);
    printf("────────────────────────────────────────────────────────────────\n\n");

    /* Group aspects by type */
    int harmonious = 0, challenging = 0, neutral = 0;

    for (int i = 0; i < synastry.num_aspects; i++) {
        int harmony = astro_aspects_is_harmonious(synastry.aspects[i].aspect_type);
        if (harmony == 1) harmonious++;
        else if (harmony == 0) challenging++;
        else neutral++;
    }

    printf("Summary:\n");
    printf("  Harmonious aspects: %d (trine, sextile, conjunction)\n", harmonious);
    printf("  Challenging aspects: %d (square, opposition)\n", challenging);
    printf("  Neutral aspects: %d (other)\n\n", neutral);

    /* Print top aspects (up to 15) */
    int show_count = (synastry.num_aspects < 15) ? synastry.num_aspects : 15;

    printf("Key Aspects:\n\n");
    for (int i = 0; i < show_count; i++) {
        const AstroAspect *asp = &synastry.aspects[i];
        int harmony = astro_aspects_is_harmonious(asp->aspect_type);
        const char *symbol = (harmony == 1) ? "💚" :
                           (harmony == 0) ? "💥" : "○";

        printf("%s %s's %s %s %s's %s (orb: %.1f°)\n",
               symbol,
               chart1->chart_name,
               astro_core_get_planet_name(asp->planet1),
               astro_aspects_get_name(asp->aspect_type),
               chart2->chart_name,
               astro_core_get_planet_name(asp->planet2),
               asp->orb);
    }

    if (synastry.num_aspects > 15) {
        printf("\n... and %d more aspects\n", synastry.num_aspects - 15);
    }
    printf("\n");
}

/* ========================================================================
 * Add planetary moons to synastry analysis
 * ======================================================================== */

void add_moons_to_synastry(const AstroChart *chart1, const AstroChart *chart2) {
    printf("PLANETARY MOONS IN SYNASTRY\n");
    printf("────────────────────────────────────────────────────────────────\n\n");

    /* Get current moon positions */
    PlanetaryMoonPosition moons[27];
    int num_moons;

    /* Use chart1's birth time to get moon positions for that moment */
    int result = astro_moons_calc_all_positions(&chart1->birth_time,
                                               moons, 27, &num_moons);

    if (result != ASTRO_OK) {
        printf("Error calculating moon positions\n\n");
        return;
    }

    /* Check moons against chart2's planets */
    printf("Checking %s's birth-time moons against %s's natal planets:\n\n",
           chart1->chart_name, chart2->chart_name);

    int found = 0;
    for (int m = 0; m < num_moons; m++) {
        for (int p = 0; p < chart2->num_planets; p++) {
            double diff = fabs(moons[m].position.longitude -
                              chart2->planets[p].longitude);

            /* Normalize */
            while (diff > 360.0) diff -= 360.0;
            if (diff > 180.0) diff = 360.0 - diff;

            /* Check if within 2° orb */
            if (diff <= 2.0) {
                printf("★ %s's %s conjunct %s's %s (%.2f°)\n",
                       chart1->chart_name,
                       moons[m].moon_name,
                       chart2->chart_name,
                       chart2->planets[p].name,
                       diff);
                printf("   Archetype: %s\n", moons[m].data->archetype);
                printf("   Theme: %s brings %s to %s's %s\n\n",
                       chart1->chart_name,
                       moons[m].data->psychological_theme,
                       chart2->chart_name,
                       chart2->planets[p].name);
                found++;
            }
        }
    }

    if (found == 0) {
        printf("No significant moon-planet connections found (within 2° orb)\n\n");
    } else {
        printf("Found %d moon-planet connection(s) - this adds mythological\n", found);
        printf("depth and archetypal themes to the relationship dynamic.\n\n");
    }
}

/* ========================================================================
 * Main Program
 * ======================================================================== */

int main(int argc, char *argv[]) {
    printf("\n");
    printf("════════════════════════════════════════════════════════════════\n");
    printf("  NATAL CHART & SYNASTRY ANALYSIS WITH PLANETARY MOONS\n");
    printf("════════════════════════════════════════════════════════════════\n");

    /* Initialize the engine */
    astro_core_init(NULL);
    astro_core_set_ephemeris_path("./ephe");

    /* ===================================================================
     * EXAMPLE 1: Create two natal charts
     * =================================================================== */

    AstroChart person1_chart, person2_chart;

    /* Person 1: Sample natal chart */
    create_chart_with_analysis(
        "Alice",
        1990, 3, 15,    // March 15, 1990
        14, 30,         // 14:30 UTC
        51.5074, -0.1278,  // London
        "London, UK",
        &person1_chart
    );

    /* Add planetary moon analysis for Person 1 */
    add_moon_analysis(&person1_chart);

    /* Person 2: Sample natal chart */
    create_chart_with_analysis(
        "Bob",
        1988, 11, 22,   // November 22, 1988
        9, 15,          // 09:15 UTC
        40.7128, -74.0060,  // New York
        "New York, USA",
        &person2_chart
    );

    /* Add planetary moon analysis for Person 2 */
    add_moon_analysis(&person2_chart);

    /* ===================================================================
     * EXAMPLE 2: Perform synastry analysis
     * =================================================================== */

    perform_synastry(&person1_chart, &person2_chart);

    /* Add planetary moons layer to synastry */
    add_moons_to_synastry(&person1_chart, &person2_chart);

    /* ===================================================================
     * EXAMPLE 3: Show how to customize
     * =================================================================== */

    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║  HOW TO USE THIS FOR YOUR OWN CHARTS\n");
    printf("╚════════════════════════════════════════════════════════════════╝\n\n");

    printf("To create your own charts, modify the birth data:\n\n");

    printf("1. CHANGE THE DATE/TIME:\n");
    printf("   create_chart_with_analysis(\n");
    printf("       \"Your Name\",\n");
    printf("       1995, 7, 4,        // Year, Month, Day\n");
    printf("       12, 0,             // Hour, Minute (24-hour, UTC)\n");
    printf("       ...\n");
    printf("   );\n\n");

    printf("2. CHANGE THE LOCATION:\n");
    printf("   Coordinates are in decimal degrees:\n");
    printf("   - North latitude = positive, South = negative\n");
    printf("   - East longitude = positive, West = negative\n\n");

    printf("   Examples:\n");
    printf("   London:      51.5074, -0.1278\n");
    printf("   New York:    40.7128, -74.0060\n");
    printf("   Los Angeles: 34.0522, -118.2437\n");
    printf("   Tokyo:       35.6762, 139.6503\n");
    printf("   Sydney:     -33.8688, 151.2093\n\n");

    printf("3. ADJUST THE ORB:\n");
    printf("   The moon conjunction orb is set to 2.0 degrees.\n");
    printf("   For tighter/looser connections, change this in:\n");
    printf("   astro_moons_find_conjunctions(chart, &time, 2.0, ...)\n\n");

    printf("4. RECOMPILE AND RUN:\n");
    printf("   make chart_and_synastry_example\n");
    printf("   ./chart_and_synastry_example\n\n");

    printf("════════════════════════════════════════════════════════════════\n\n");

    /* Cleanup */
    astro_core_cleanup();

    return 0;
}
