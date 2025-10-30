/**
 * @file test_moon_calculations.c
 * @brief Test program for planetary moon calculations
 */

#include "astro_engine.h"
#include "astro_planetary_moons.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    printf("\n");
    printf("════════════════════════════════════════════════════════════════\n");
    printf("  PLANETARY MOONS CALCULATION TEST\n");
    printf("════════════════════════════════════════════════════════════════\n\n");

    /* Initialize the engine */
    astro_core_init(NULL);  /* Use default config */
    astro_core_set_ephemeris_path("./ephe");

    /* Set test date: 2025-01-15 12:00 UTC */
    AstroDateTime test_date;
    test_date.year = 2025;
    test_date.month = 1;
    test_date.day = 15;
    test_date.hour = 12;
    test_date.minute = 0;
    test_date.second = 0;
    test_date.timezone_offset = 0.0;

    printf("Test Date: 2025-01-15 12:00 UTC\n\n");

    /* Test 1: Calculate single moon position */
    printf("════════════════════════════════════════════════════════════════\n");
    printf("TEST 1: Single Moon Position (Io)\n");
    printf("════════════════════════════════════════════════════════════════\n\n");

    PlanetaryMoonPosition io_pos;
    int result = astro_moons_calc_position(MOON_IO, &test_date, &io_pos);

    if (result == ASTRO_OK) {
        printf("✓ SUCCESS\n\n");
        astro_moons_print_position(&io_pos, stdout);
        printf("\n");
        printf("  Longitude: %.6f°\n", io_pos.position.longitude);
        printf("  Latitude:  %.6f°\n", io_pos.position.latitude);
        printf("  Distance:  %.6f AU\n", io_pos.position.distance);
        printf("  Speed:     %.6f°/day\n", io_pos.position.longitude_speed);
        printf("  Retrograde: %s\n", io_pos.position.is_retrograde ? "Yes" : "No");
    } else {
        printf("✗ FAILED: Error code %d\n", result);
    }

    /* Test 2: Calculate all moons */
    printf("\n════════════════════════════════════════════════════════════════\n");
    printf("TEST 2: All 27 Planetary Moons\n");
    printf("════════════════════════════════════════════════════════════════\n\n");

    PlanetaryMoonPosition all_moons[27];
    int num_moons;
    result = astro_moons_calc_all_positions(&test_date, all_moons, 27, &num_moons);

    if (result == ASTRO_OK) {
        printf("✓ SUCCESS: Calculated %d moons\n\n", num_moons);
        astro_moons_print_report(&test_date, stdout);
    } else {
        printf("✗ FAILED: Error code %d\n", result);
    }

    /* Test 3: Get Galilean moons */
    printf("\n════════════════════════════════════════════════════════════════\n");
    printf("TEST 3: Galilean Moons of Jupiter\n");
    printf("════════════════════════════════════════════════════════════════\n\n");

    PlanetaryMoonPosition galilean[4];
    result = astro_moons_get_galilean_moons(&test_date, galilean);

    if (result == ASTRO_OK) {
        printf("✓ SUCCESS\n\n");
        for (int i = 0; i < 4; i++) {
            astro_moons_print_position(&galilean[i], stdout);
        }
    } else {
        printf("✗ FAILED: Error code %d\n", result);
    }

    /* Test 4: Get Saturn system */
    printf("\n════════════════════════════════════════════════════════════════\n");
    printf("TEST 4: Saturn System Moons\n");
    printf("════════════════════════════════════════════════════════════════\n\n");

    result = astro_moons_print_system(ASTRO_SATURN, &test_date, stdout);

    if (result == ASTRO_OK) {
        printf("✓ SUCCESS\n");
    } else {
        printf("✗ FAILED: Error code %d\n", result);
    }

    /* Test 5: Orbital phase */
    printf("\n════════════════════════════════════════════════════════════════\n");
    printf("TEST 5: Orbital Phase (Titan)\n");
    printf("════════════════════════════════════════════════════════════════\n\n");

    double phase;
    const char *phase_name;
    result = astro_moons_orbital_phase(MOON_TITAN, &test_date, &phase, &phase_name);

    if (result == ASTRO_OK) {
        printf("✓ SUCCESS\n\n");
        printf("  Titan orbital phase: %.2f° (%s)\n", phase, phase_name);
    } else {
        printf("✗ FAILED: Error code %d\n", result);
    }

    /* Test 6: Search by keyword */
    printf("\n════════════════════════════════════════════════════════════════\n");
    printf("TEST 6: Search Moons by Keyword ('fear')\n");
    printf("════════════════════════════════════════════════════════════════\n\n");

    PlanetaryMoonID found_moons[10];
    int num_found;
    result = astro_moons_search_by_keyword("fear", found_moons, 10, &num_found);

    if (result == ASTRO_OK) {
        printf("✓ SUCCESS: Found %d moons\n\n", num_found);
        for (int i = 0; i < num_found; i++) {
            const PlanetaryMoonData *data = astro_moons_get_data(found_moons[i]);
            printf("  - %s: %s\n", data->name, data->archetype);
        }
    } else {
        printf("✗ FAILED: Error code %d\n", result);
    }

    /* Test 7: Get moons by parent */
    printf("\n════════════════════════════════════════════════════════════════\n");
    printf("TEST 7: Get Pluto's Moons\n");
    printf("════════════════════════════════════════════════════════════════\n\n");

    PlanetaryMoonID pluto_moon_ids[10];
    int num_pluto;
    result = astro_moons_get_for_parent(ASTRO_PLUTO, pluto_moon_ids, 10, &num_pluto);

    if (result == ASTRO_OK) {
        printf("✓ SUCCESS: Found %d moons\n\n", num_pluto);
        for (int i = 0; i < num_pluto; i++) {
            const PlanetaryMoonData *data = astro_moons_get_data(pluto_moon_ids[i]);
            printf("  - %s: %s\n", data->name, data->archetype);
        }
    } else {
        printf("✗ FAILED: Error code %d\n", result);
    }

    /* Test 8: Get by name */
    printf("\n════════════════════════════════════════════════════════════════\n");
    printf("TEST 8: Find Moon by Name ('Charon')\n");
    printf("════════════════════════════════════════════════════════════════\n\n");

    int moon_id = astro_moons_get_by_name("Charon");
    if (moon_id > 0) {
        printf("✓ SUCCESS\n\n");
        const PlanetaryMoonData *data = astro_moons_get_data(moon_id);
        printf("  Found: %s (ID: %d)\n", data->name, moon_id);
        printf("  Archetype: %s\n", data->archetype);
        printf("  Mythology: %s\n", data->mythology);
    } else {
        printf("✗ FAILED: Moon not found\n");
    }

    /* Test 9: Conjunctions with natal chart */
    printf("\n════════════════════════════════════════════════════════════════\n");
    printf("TEST 9: Moon Conjunctions with Natal Chart\n");
    printf("════════════════════════════════════════════════════════════════\n\n");

    /* Create a simple natal chart */
    AstroLocation birth_location;
    birth_location.latitude = 40.7128;
    birth_location.longitude = -74.0060;
    birth_location.altitude = 0.0;
    strncpy(birth_location.location_name, "New York", ASTRO_MAX_NAME_LEN - 1);
    birth_location.location_name[ASTRO_MAX_NAME_LEN - 1] = '\0';

    AstroDateTime birth_date = {1990, 1, 1, 12, 0, 0, 0.0, 0.0};

    AstroChart natal_chart;
    result = astro_chart_create(&birth_date, &birth_location, NULL, &natal_chart);

    if (result == ASTRO_OK) {
        result = astro_moons_print_conjunctions(&natal_chart, &test_date, 2.0, stdout);
        if (result == ASTRO_OK) {
            printf("✓ SUCCESS\n");
        }
    } else {
        printf("✗ FAILED: Could not create natal chart\n");
    }

    /* Test 10: Interpretation */
    printf("\n════════════════════════════════════════════════════════════════\n");
    printf("TEST 10: Moon-Planet Conjunction Interpretation\n");
    printf("════════════════════════════════════════════════════════════════\n\n");

    char interpretation[1024];
    astro_moons_interpret_conjunction(MOON_CHARON, ASTRO_SUN, interpretation, 1024);
    printf("✓ SUCCESS\n\n");
    printf("%s\n", interpretation);

    printf("\n════════════════════════════════════════════════════════════════\n");
    printf("  ALL TESTS COMPLETED\n");
    printf("════════════════════════════════════════════════════════════════\n\n");

    /* Cleanup */
    astro_core_cleanup();

    return 0;
}
