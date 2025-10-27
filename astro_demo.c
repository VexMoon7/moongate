/**
 * @file astro_demo.c
 * @brief Demonstration program for the Astrological Data Analysis Engine
 *
 * This program demonstrates the key features of the modular ephemeris
 * and astrological data analysis engine.
 */

#include "astro_engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ========================================================================
 * Demo Functions
 * ======================================================================== */

void demo_engine_info(void) {
    printf("\n========================================\n");
    printf("  ENGINE INFORMATION\n");
    printf("========================================\n");
    astro_engine_print_info(stdout);
}

void demo_natal_chart(void) {
    printf("\n========================================\n");
    printf("  NATAL CHART DEMONSTRATION\n");
    printf("========================================\n\n");

    /* Example: Chart for January 15, 1990, 14:30 in London */
    AstroChart chart;
    int result = astro_engine_quick_chart(
        1990, 1, 15,      /* Date: 1990-01-15 */
        14, 30, 0.0,      /* Time: 14:30:00 */
        0.0,              /* UTC timezone */
        51.5074,          /* London latitude */
        -0.1278,          /* London longitude */
        &chart
    );

    if (result != ASTRO_OK) {
        printf("Error creating chart: %s\n",
               astro_core_get_error_message(result));
        return;
    }

    strncpy(chart.chart_name, "Example Natal Chart", ASTRO_MAX_NAME_LEN);

    /* Print complete chart */
    astro_chart_print(&chart, stdout);

    /* Analyze elements */
    AstroElementBalance element_balance;
    astro_chart_analyze_elements(&chart, &element_balance);

    printf("Element Distribution:\n");
    printf("  Fire:  %d planets\n", element_balance.fire_count);
    printf("  Earth: %d planets\n", element_balance.earth_count);
    printf("  Air:   %d planets\n", element_balance.air_count);
    printf("  Water: %d planets\n", element_balance.water_count);
    printf("  Dominant: %s\n\n",
           astro_engine_get_element_name(element_balance.dominant_element));

    /* Analyze modalities */
    AstroModalityBalance modality_balance;
    astro_chart_analyze_modalities(&chart, &modality_balance);

    printf("Modality Distribution:\n");
    printf("  Cardinal: %d planets\n", modality_balance.cardinal_count);
    printf("  Fixed:    %d planets\n", modality_balance.fixed_count);
    printf("  Mutable:  %d planets\n", modality_balance.mutable_count);
    printf("  Dominant: %s\n\n",
           astro_engine_get_modality_name(modality_balance.dominant_modality));

    /* Export to JSON */
    result = astro_chart_export_json(&chart, "example_chart.json");
    if (result == ASTRO_OK) {
        printf("Chart exported to example_chart.json\n");
    }

    /* Export to CSV */
    result = astro_chart_export_csv(&chart, "example_chart.csv");
    if (result == ASTRO_OK) {
        printf("Chart exported to example_chart.csv\n");
    }
}

void demo_aspects(void) {
    printf("\n========================================\n");
    printf("  ASPECT ANALYSIS DEMONSTRATION\n");
    printf("========================================\n\n");

    /* Create chart */
    AstroChart chart;
    astro_engine_quick_chart(1990, 1, 15, 14, 30, 0.0, 0.0,
                            51.5074, -0.1278, &chart);

    /* Get Sun position */
    const AstroPlanetPosition *sun = astro_chart_get_planet(&chart, ASTRO_SUN);
    if (sun != NULL) {
        printf("Sun Position:\n");
        printf("  Longitude: %.2f°\n", sun->longitude);
        printf("  Sign: %s %.2f°\n",
               astro_core_get_sign_name(sun->sign),
               sun->sign_position);
        printf("  House: %d\n\n", sun->house);
    }

    /* Find aspect patterns */
    printf("Searching for aspect patterns...\n");
    AstroPattern patterns[20];
    int num_patterns;

    int result = astro_aspects_find_patterns(
        chart.planets, chart.num_planets,
        chart.aspects, chart.num_aspects,
        patterns, 20, &num_patterns
    );

    if (result == ASTRO_OK && num_patterns > 0) {
        printf("Found %d aspect pattern(s):\n", num_patterns);
        for (int i = 0; i < num_patterns; i++) {
            printf("  %d. %s\n", i + 1, patterns[i].description);
        }
    } else {
        printf("No major aspect patterns found.\n");
    }
    printf("\n");
}

void demo_transits(void) {
    printf("\n========================================\n");
    printf("  TRANSIT ANALYSIS DEMONSTRATION\n");
    printf("========================================\n\n");

    /* Create natal chart */
    AstroChart natal_chart;
    astro_engine_quick_chart(1990, 1, 15, 14, 30, 0.0, 0.0,
                            51.5074, -0.1278, &natal_chart);

    printf("Calculating current transits to natal chart...\n\n");

    /* Calculate current transits */
    AstroTransit transits[ASTRO_MAX_TRANSITS];
    int num_transits;

    int result = astro_engine_current_transits(&natal_chart, transits,
                                               ASTRO_MAX_TRANSITS, &num_transits);

    if (result == ASTRO_OK) {
        astro_transits_print(transits, num_transits, stdout);

        /* Analyze importance */
        AstroTransitScore scores[ASTRO_MAX_TRANSITS];
        int num_scores;

        result = astro_transits_analyze_importance(transits, num_transits,
                                                   scores, ASTRO_MAX_TRANSITS,
                                                   &num_scores);

        if (result == ASTRO_OK && num_scores > 0) {
            printf("Most Important Transits:\n");
            /* Sort by strength (simple bubble sort for demo) */
            for (int i = 0; i < num_scores - 1; i++) {
                for (int j = 0; j < num_scores - i - 1; j++) {
                    if (scores[j].strength < scores[j + 1].strength) {
                        AstroTransitScore temp = scores[j];
                        scores[j] = scores[j + 1];
                        scores[j + 1] = temp;
                    }
                }
            }

            /* Show top 5 */
            int show_count = (num_scores < 5) ? num_scores : 5;
            for (int i = 0; i < show_count; i++) {
                printf("  %d. %s\n", i + 1, scores[i].interpretation);
            }
        }
    }
}

void demo_progressions(void) {
    printf("\n========================================\n");
    printf("  PROGRESSION DEMONSTRATION\n");
    printf("========================================\n\n");

    /* Create natal chart */
    AstroChart natal_chart;
    astro_engine_quick_chart(1990, 1, 15, 14, 30, 0.0, 0.0,
                            51.5074, -0.1278, &natal_chart);

    /* Calculate progressed chart for current date */
    AstroDateTime target_date;
    astro_core_get_current_datetime(&target_date);

    AstroChart progressed_chart;
    int result = astro_transits_calc_progressed_chart(
        &natal_chart,
        ASTRO_PROGRESSION_SECONDARY,
        &target_date,
        &progressed_chart
    );

    if (result == ASTRO_OK) {
        printf("Secondary Progressed Chart\n");
        printf("Target Date: %04d-%02d-%02d\n\n",
               target_date.year, target_date.month, target_date.day);

        printf("Progressed Sun:\n");
        const AstroPlanetPosition *prog_sun =
            astro_chart_get_planet(&progressed_chart, ASTRO_SUN);
        const AstroPlanetPosition *natal_sun =
            astro_chart_get_planet(&natal_chart, ASTRO_SUN);

        if (prog_sun && natal_sun) {
            printf("  Natal:      %s %.2f°\n",
                   astro_core_get_sign_name(natal_sun->sign),
                   natal_sun->sign_position);
            printf("  Progressed: %s %.2f°\n",
                   astro_core_get_sign_name(prog_sun->sign),
                   prog_sun->sign_position);
            printf("  Movement:   %.2f°\n\n",
                   prog_sun->longitude - natal_sun->longitude);
        }
    }
}

void demo_synastry(void) {
    printf("\n========================================\n");
    printf("  SYNASTRY DEMONSTRATION\n");
    printf("========================================\n\n");

    /* Create two charts */
    AstroChart chart1, chart2;

    printf("Creating charts for compatibility analysis...\n\n");

    astro_engine_quick_chart(1990, 1, 15, 14, 30, 0.0, 0.0,
                            51.5074, -0.1278, &chart1);
    strncpy(chart1.chart_name, "Person 1", ASTRO_MAX_NAME_LEN);

    astro_engine_quick_chart(1988, 6, 22, 10, 15, 0.0, 0.0,
                            51.5074, -0.1278, &chart2);
    strncpy(chart2.chart_name, "Person 2", ASTRO_MAX_NAME_LEN);

    /* Calculate synastry */
    AstroSynastry synastry;
    int result = astro_chart_calc_synastry(&chart1, &chart2, &synastry);

    if (result == ASTRO_OK) {
        printf("Synastry Analysis\n");
        printf("Between: %s and %s\n\n", chart1.chart_name, chart2.chart_name);
        printf("Compatibility Score: %.1f/100\n\n", synastry.compatibility_score);

        printf("Inter-chart Aspects (%d found):\n", synastry.num_aspects);
        int show_count = (synastry.num_aspects < 10) ? synastry.num_aspects : 10;
        for (int i = 0; i < show_count; i++) {
            const AstroAspect *asp = &synastry.aspects[i];
            printf("  %s (%s) %s %s (%s) - %.2f°\n",
                   astro_core_get_planet_name(asp->planet1),
                   chart1.chart_name,
                   astro_aspects_get_name(asp->aspect_type),
                   astro_core_get_planet_name(asp->planet2),
                   chart2.chart_name,
                   asp->difference);
        }
        if (synastry.num_aspects > 10) {
            printf("  ... and %d more\n", synastry.num_aspects - 10);
        }
    }
}

void demo_current_sky(void) {
    printf("\n========================================\n");
    printf("  CURRENT SKY POSITIONS\n");
    printf("========================================\n\n");

    AstroDateTime current_time;
    astro_core_get_current_datetime(&current_time);

    printf("Date: %04d-%02d-%02d %02d:%02d UTC\n\n",
           current_time.year, current_time.month, current_time.day,
           current_time.hour, current_time.minute);

    printf("Current Planet Positions:\n");
    printf("%-12s %-12s %10s %8s\n", "Planet", "Sign", "Position", "Speed");
    printf("%-12s %-12s %10s %8s\n", "------", "----", "--------", "-----");

    AstroPlanet planets[] = {
        ASTRO_SUN, ASTRO_MOON, ASTRO_MERCURY, ASTRO_VENUS,
        ASTRO_MARS, ASTRO_JUPITER, ASTRO_SATURN, ASTRO_URANUS,
        ASTRO_NEPTUNE, ASTRO_PLUTO
    };

    for (int i = 0; i < 10; i++) {
        AstroPlanetPosition pos;
        if (astro_engine_current_position(planets[i], &pos) == ASTRO_OK) {
            printf("%-12s %-12s %3d°%02d'    %+6.3f%s\n",
                   pos.name,
                   astro_core_get_sign_name(pos.sign),
                   (int)pos.sign_position,
                   (int)((pos.sign_position - (int)pos.sign_position) * 60),
                   pos.longitude_speed,
                   pos.is_retrograde ? "R" : "");
        }
    }
    printf("\n");
}

/* ========================================================================
 * Main Menu
 * ======================================================================== */

void print_menu(void) {
    printf("\n========================================\n");
    printf("  ASTRO ENGINE DEMO MENU\n");
    printf("========================================\n");
    printf("1. Engine Information\n");
    printf("2. Natal Chart Demo\n");
    printf("3. Aspect Analysis Demo\n");
    printf("4. Transit Analysis Demo\n");
    printf("5. Progression Demo\n");
    printf("6. Synastry Demo\n");
    printf("7. Current Sky Positions\n");
    printf("8. Run All Demos\n");
    printf("0. Exit\n");
    printf("========================================\n");
    printf("Select option: ");
}

int main(int argc, char *argv[]) {
    printf("\n");
    printf("================================================================\n");
    printf("  Astrological Data Analysis Engine - Demonstration Program\n");
    printf("  %s\n", astro_engine_get_version());
    printf("================================================================\n");

    /* Initialize engine */
    printf("\nInitializing engine...\n");
    AstroConfig config;
    astro_engine_get_default_config(&config);

    int result = astro_engine_init(&config);
    if (result != ASTRO_OK) {
        printf("Error initializing engine: %s\n",
               astro_core_get_error_message(result));
        return 1;
    }
    printf("Engine initialized successfully.\n");

    /* Check for command line argument */
    if (argc > 1) {
        if (strcmp(argv[1], "--all") == 0 || strcmp(argv[1], "-a") == 0) {
            demo_engine_info();
            demo_current_sky();
            demo_natal_chart();
            demo_aspects();
            demo_transits();
            demo_progressions();
            demo_synastry();
            astro_engine_cleanup();
            return 0;
        }
    }

    /* Interactive menu */
    int choice;
    do {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            /* Clear input buffer */
            while (getchar() != '\n');
            choice = -1;
        }

        switch (choice) {
            case 1:
                demo_engine_info();
                break;
            case 2:
                demo_natal_chart();
                break;
            case 3:
                demo_aspects();
                break;
            case 4:
                demo_transits();
                break;
            case 5:
                demo_progressions();
                break;
            case 6:
                demo_synastry();
                break;
            case 7:
                demo_current_sky();
                break;
            case 8:
                demo_engine_info();
                demo_current_sky();
                demo_natal_chart();
                demo_aspects();
                demo_transits();
                demo_progressions();
                demo_synastry();
                break;
            case 0:
                printf("\nExiting...\n");
                break;
            default:
                printf("\nInvalid option. Please try again.\n");
        }
    } while (choice != 0);

    /* Cleanup */
    astro_engine_cleanup();

    printf("\n");
    printf("================================================================\n");
    printf("  Thank you for using the Astrological Data Analysis Engine!\n");
    printf("================================================================\n\n");

    return 0;
}
