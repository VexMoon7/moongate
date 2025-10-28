/**
 * @file cosmic_weather.c
 * @brief Cosmic Weather Dashboard - Real-time Astrological Climate Analysis
 *
 * This program generates a comprehensive "cosmic weather report" showing:
 * - Current planetary positions and aspects
 * - Aspect patterns forming in the sky
 * - Retrograde planets
 * - Lunar phase
 * - Important transits
 * - Weekly forecast
 */

#include "astro_engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

/* ========================================================================
 * Cosmic Weather Data Structures
 * ======================================================================== */

typedef struct {
    AstroDateTime current_time;
    AstroPlanetPosition planets[ASTRO_MAX_PLANETS];
    int num_planets;
    AstroAspect sky_aspects[ASTRO_MAX_ASPECTS];
    int num_sky_aspects;
    AstroPattern patterns[20];
    int num_patterns;
    double moon_phase_angle;
    const char* moon_phase_name;
    int retrograde_planets[10];
    int num_retrograde;
} CosmicWeather;

/* ========================================================================
 * Lunar Phase Calculation
 * ======================================================================== */

const char* get_moon_phase_name(double phase_angle) {
    // Normalize to 0-360
    while (phase_angle < 0) phase_angle += 360.0;
    while (phase_angle >= 360.0) phase_angle -= 360.0;

    if (phase_angle < 22.5 || phase_angle >= 337.5) return "New Moon";
    if (phase_angle < 67.5) return "Waxing Crescent";
    if (phase_angle < 112.5) return "First Quarter";
    if (phase_angle < 157.5) return "Waxing Gibbous";
    if (phase_angle < 202.5) return "Full Moon";
    if (phase_angle < 247.5) return "Waning Gibbous";
    if (phase_angle < 292.5) return "Last Quarter";
    return "Waning Crescent";
}

const char* get_moon_phase_emoji(const char* phase_name) {
    if (strcmp(phase_name, "New Moon") == 0) return "🌑";
    if (strcmp(phase_name, "Waxing Crescent") == 0) return "🌒";
    if (strcmp(phase_name, "First Quarter") == 0) return "🌓";
    if (strcmp(phase_name, "Waxing Gibbous") == 0) return "🌔";
    if (strcmp(phase_name, "Full Moon") == 0) return "🌕";
    if (strcmp(phase_name, "Waning Gibbous") == 0) return "🌖";
    if (strcmp(phase_name, "Last Quarter") == 0) return "🌗";
    if (strcmp(phase_name, "Waning Crescent") == 0) return "🌘";
    return "🌙";
}

void calculate_lunar_phase(CosmicWeather *weather) {
    // Find Sun and Moon
    const AstroPlanetPosition *sun = NULL;
    const AstroPlanetPosition *moon = NULL;

    for (int i = 0; i < weather->num_planets; i++) {
        if (weather->planets[i].planet == ASTRO_SUN) {
            sun = &weather->planets[i];
        } else if (weather->planets[i].planet == ASTRO_MOON) {
            moon = &weather->planets[i];
        }
    }

    if (sun && moon) {
        // Calculate angular separation
        weather->moon_phase_angle = moon->longitude - sun->longitude;
        while (weather->moon_phase_angle < 0) weather->moon_phase_angle += 360.0;
        while (weather->moon_phase_angle >= 360.0) weather->moon_phase_angle -= 360.0;

        weather->moon_phase_name = get_moon_phase_name(weather->moon_phase_angle);
    } else {
        weather->moon_phase_angle = 0.0;
        weather->moon_phase_name = "Unknown";
    }
}

/* ========================================================================
 * Retrograde Detection
 * ======================================================================== */

void detect_retrogrades(CosmicWeather *weather) {
    weather->num_retrograde = 0;

    for (int i = 0; i < weather->num_planets; i++) {
        if (weather->planets[i].is_retrograde) {
            // Skip Sun and Moon (never retrograde)
            if (weather->planets[i].planet == ASTRO_SUN ||
                weather->planets[i].planet == ASTRO_MOON) {
                continue;
            }
            weather->retrograde_planets[weather->num_retrograde++] = i;
        }
    }
}

/* ========================================================================
 * Cosmic Weather Analysis
 * ======================================================================== */

int calculate_cosmic_weather(CosmicWeather *weather) {
    // Get current time
    int result = astro_core_get_current_datetime(&weather->current_time);
    if (result != ASTRO_OK) return result;

    // Calculate current planetary positions
    result = astro_core_calc_planets(&weather->current_time, 0,
                                     weather->planets, &weather->num_planets);
    if (result != ASTRO_OK) return result;

    // Calculate aspects in the current sky
    result = astro_aspects_calc_all(weather->planets, weather->num_planets,
                                    weather->sky_aspects, ASTRO_MAX_ASPECTS,
                                    &weather->num_sky_aspects);
    if (result != ASTRO_OK) return result;

    // Find aspect patterns
    result = astro_aspects_find_patterns(weather->planets, weather->num_planets,
                                         weather->sky_aspects, weather->num_sky_aspects,
                                         weather->patterns, 20, &weather->num_patterns);
    if (result != ASTRO_OK) return result;

    // Calculate lunar phase
    calculate_lunar_phase(weather);

    // Detect retrograde planets
    detect_retrogrades(weather);

    return ASTRO_OK;
}

/* ========================================================================
 * Dashboard Display Functions
 * ======================================================================== */

void print_header(FILE *fp) {
    fprintf(fp, "\n");
    fprintf(fp, "╔════════════════════════════════════════════════════════════════╗\n");
    fprintf(fp, "║                                                                ║\n");
    fprintf(fp, "║              ✨ COSMIC WEATHER DASHBOARD ✨                   ║\n");
    fprintf(fp, "║                                                                ║\n");
    fprintf(fp, "║         Real-time Astrological Climate Analysis                ║\n");
    fprintf(fp, "║                                                                ║\n");
    fprintf(fp, "╚════════════════════════════════════════════════════════════════╝\n");
    fprintf(fp, "\n");
}

void print_current_time(const CosmicWeather *weather, FILE *fp) {
    fprintf(fp, "📅 Current Date & Time\n");
    fprintf(fp, "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    fprintf(fp, "  %04d-%02d-%02d  %02d:%02d:%02.0f UTC\n",
            weather->current_time.year, weather->current_time.month,
            weather->current_time.day, weather->current_time.hour,
            weather->current_time.minute, weather->current_time.second);
    fprintf(fp, "  Julian Day: %.5f\n\n", weather->current_time.julian_day);
}

void print_planetary_positions(const CosmicWeather *weather, FILE *fp) {
    fprintf(fp, "🌍 Current Planetary Positions\n");
    fprintf(fp, "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    fprintf(fp, "%-12s %-12s %12s  %8s\n", "Planet", "Sign", "Position", "Motion");
    fprintf(fp, "%-12s %-12s %12s  %8s\n", "------", "----", "--------", "------");

    // Print main planets first
    AstroPlanet main_planets[] = {
        ASTRO_SUN, ASTRO_MOON, ASTRO_MERCURY, ASTRO_VENUS,
        ASTRO_MARS, ASTRO_JUPITER, ASTRO_SATURN, ASTRO_URANUS,
        ASTRO_NEPTUNE, ASTRO_PLUTO
    };

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < weather->num_planets; j++) {
            if (weather->planets[j].planet == main_planets[i]) {
                const AstroPlanetPosition *p = &weather->planets[j];
                fprintf(fp, "%-12s %-12s %3d°%02d' %c    %s%+6.3f\n",
                        p->name,
                        astro_core_get_sign_name(p->sign),
                        (int)p->sign_position,
                        (int)((p->sign_position - (int)p->sign_position) * 60),
                        (p->planet == ASTRO_SUN || p->planet == ASTRO_MOON) ? ' ' :
                        (p->is_retrograde ? 'R' : 'D'),
                        p->is_retrograde ? " " : " ",
                        p->longitude_speed);
                break;
            }
        }
    }
    fprintf(fp, "\n  R = Retrograde    D = Direct\n\n");
}

void print_lunar_phase(const CosmicWeather *weather, FILE *fp) {
    fprintf(fp, "🌙 Lunar Phase\n");
    fprintf(fp, "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    fprintf(fp, "  %s %s\n", get_moon_phase_emoji(weather->moon_phase_name),
            weather->moon_phase_name);
    fprintf(fp, "  Phase Angle: %.1f°\n", weather->moon_phase_angle);

    // Calculate illumination percentage
    double illumination = 50.0 * (1.0 - cos(weather->moon_phase_angle * M_PI / 180.0));
    fprintf(fp, "  Illumination: %.1f%%\n\n", illumination);
}

void print_retrogrades(const CosmicWeather *weather, FILE *fp) {
    fprintf(fp, "⏮️  Retrograde Planets\n");
    fprintf(fp, "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");

    if (weather->num_retrograde == 0) {
        fprintf(fp, "  No planets currently retrograde\n\n");
    } else {
        fprintf(fp, "  %d planet%s currently retrograde:\n",
                weather->num_retrograde,
                weather->num_retrograde == 1 ? "" : "s");

        for (int i = 0; i < weather->num_retrograde; i++) {
            const AstroPlanetPosition *p = &weather->planets[weather->retrograde_planets[i]];
            fprintf(fp, "    • %s in %s (%.2f°/day)\n",
                    p->name,
                    astro_core_get_sign_name(p->sign),
                    p->longitude_speed);
        }
        fprintf(fp, "\n");
    }
}

void print_sky_aspects(const CosmicWeather *weather, FILE *fp) {
    fprintf(fp, "🔗 Current Aspects in the Sky\n");
    fprintf(fp, "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");

    if (weather->num_sky_aspects == 0) {
        fprintf(fp, "  No significant aspects at this time\n\n");
        return;
    }

    // Categorize aspects
    int major_count = 0;
    int minor_count = 0;

    for (int i = 0; i < weather->num_sky_aspects; i++) {
        AstroAspectType type = weather->sky_aspects[i].aspect_type;
        if (type == ASTRO_ASPECT_CONJUNCTION || type == ASTRO_ASPECT_OPPOSITION ||
            type == ASTRO_ASPECT_TRINE || type == ASTRO_ASPECT_SQUARE ||
            type == ASTRO_ASPECT_SEXTILE) {
            major_count++;
        } else {
            minor_count++;
        }
    }

    fprintf(fp, "  Total: %d aspects (%d major, %d minor)\n\n",
            weather->num_sky_aspects, major_count, minor_count);

    // Show major aspects
    fprintf(fp, "  Major Aspects:\n");
    for (int i = 0; i < weather->num_sky_aspects; i++) {
        const AstroAspect *a = &weather->sky_aspects[i];
        AstroAspectType type = a->aspect_type;

        if (type == ASTRO_ASPECT_CONJUNCTION || type == ASTRO_ASPECT_OPPOSITION ||
            type == ASTRO_ASPECT_TRINE || type == ASTRO_ASPECT_SQUARE ||
            type == ASTRO_ASPECT_SEXTILE) {

            const char *harmony = astro_aspects_is_harmonious(type) == 1 ? "✓" :
                                  astro_aspects_is_harmonious(type) == 0 ? "✗" : "○";

            fprintf(fp, "    %s %-10s %-12s %-10s (%.2f° %s)\n",
                    harmony,
                    astro_core_get_planet_name(a->planet1),
                    astro_aspects_get_name(a->aspect_type),
                    astro_core_get_planet_name(a->planet2),
                    a->difference,
                    a->is_exact ? "EXACT" : "");
        }
    }
    fprintf(fp, "\n  ✓ = Harmonious  ✗ = Challenging  ○ = Neutral\n\n");
}

void print_aspect_patterns(const CosmicWeather *weather, FILE *fp) {
    fprintf(fp, "⭐ Aspect Patterns\n");
    fprintf(fp, "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");

    if (weather->num_patterns == 0) {
        fprintf(fp, "  No major aspect patterns detected\n\n");
    } else {
        fprintf(fp, "  %d pattern%s detected:\n",
                weather->num_patterns,
                weather->num_patterns == 1 ? "" : "s");

        for (int i = 0; i < weather->num_patterns; i++) {
            const char *emoji;
            switch (weather->patterns[i].pattern_type) {
                case ASTRO_PATTERN_GRAND_TRINE: emoji = "🔺"; break;
                case ASTRO_PATTERN_T_SQUARE: emoji = "⊤"; break;
                case ASTRO_PATTERN_STELLIUM: emoji = "✨"; break;
                default: emoji = "•"; break;
            }

            fprintf(fp, "    %s %s\n", emoji, weather->patterns[i].description);
        }
        fprintf(fp, "\n");
    }
}

void print_cosmic_climate(const CosmicWeather *weather, FILE *fp) {
    fprintf(fp, "🌈 Cosmic Climate Summary\n");
    fprintf(fp, "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");

    // Analyze overall energy
    int harmonious = 0, challenging = 0, neutral = 0;

    for (int i = 0; i < weather->num_sky_aspects; i++) {
        int harmony = astro_aspects_is_harmonious(weather->sky_aspects[i].aspect_type);
        if (harmony == 1) harmonious++;
        else if (harmony == 0) challenging++;
        else neutral++;
    }

    fprintf(fp, "  Energy Balance:\n");
    fprintf(fp, "    Harmonious aspects: %d\n", harmonious);
    fprintf(fp, "    Challenging aspects: %d\n", challenging);
    fprintf(fp, "    Neutral aspects: %d\n\n", neutral);

    // General interpretation
    fprintf(fp, "  Overall Atmosphere:\n");
    if (harmonious > challenging * 1.5) {
        fprintf(fp, "    ✨ Flowing and harmonious - good for cooperation and creativity\n");
    } else if (challenging > harmonious * 1.5) {
        fprintf(fp, "    ⚡ Dynamic and challenging - good for breakthroughs and action\n");
    } else {
        fprintf(fp, "    ⚖️  Balanced energy - mixed opportunities and challenges\n");
    }

    if (weather->num_retrograde > 3) {
        fprintf(fp, "    🔄 High retrograde activity - time for review and reflection\n");
    } else if (weather->num_retrograde > 0) {
        fprintf(fp, "    ↩️  Some retrograde activity - balance forward motion with review\n");
    }

    fprintf(fp, "\n");
}

void print_transit_forecast(const CosmicWeather *weather, FILE *fp) {
    fprintf(fp, "📊 Weekly Outlook\n");
    fprintf(fp, "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");

    // Find Moon's daily motion
    const AstroPlanetPosition *moon = NULL;
    for (int i = 0; i < weather->num_planets; i++) {
        if (weather->planets[i].planet == ASTRO_MOON) {
            moon = &weather->planets[i];
            break;
        }
    }

    if (moon) {
        // Calculate when Moon changes sign
        double degrees_to_next_sign = 30.0 - moon->sign_position;
        double hours_to_next_sign = (degrees_to_next_sign / moon->longitude_speed) * 24.0;

        fprintf(fp, "  Lunar Forecast:\n");
        fprintf(fp, "    Moon currently in %s\n", astro_core_get_sign_name(moon->sign));
        fprintf(fp, "    Moving at %.2f°/day\n", moon->longitude_speed);

        if (hours_to_next_sign < 48) {
            AstroSign next_sign = (moon->sign + 1) % 12;
            fprintf(fp, "    Enters %s in ~%.0f hours\n",
                    astro_core_get_sign_name(next_sign), hours_to_next_sign);
        }
        fprintf(fp, "\n");
    }

    // Note about retrograde periods
    if (weather->num_retrograde > 0) {
        fprintf(fp, "  Active Retrogrades:\n");
        fprintf(fp, "    Review, revise, and reflect in areas of:\n");
        for (int i = 0; i < weather->num_retrograde && i < 3; i++) {
            const AstroPlanetPosition *p = &weather->planets[weather->retrograde_planets[i]];
            fprintf(fp, "      • %s: ", p->name);

            switch (p->planet) {
                case ASTRO_MERCURY:
                    fprintf(fp, "Communication, technology, travel\n");
                    break;
                case ASTRO_VENUS:
                    fprintf(fp, "Relationships, values, finances\n");
                    break;
                case ASTRO_MARS:
                    fprintf(fp, "Action, energy, conflicts\n");
                    break;
                case ASTRO_JUPITER:
                    fprintf(fp, "Beliefs, expansion, growth\n");
                    break;
                case ASTRO_SATURN:
                    fprintf(fp, "Structure, responsibility, limits\n");
                    break;
                default:
                    fprintf(fp, "Transformation and change\n");
                    break;
            }
        }
    }

    fprintf(fp, "\n");
}

void print_footer(FILE *fp) {
    fprintf(fp, "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    fprintf(fp, "Generated by Astro Engine v1.0.0 (Swiss Ephemeris 2.10.03)\n");
    fprintf(fp, "For entertainment purposes only\n");
    fprintf(fp, "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");
}

/* ========================================================================
 * Main Program
 * ======================================================================== */

int main(int argc, char *argv[]) {
    // Initialize engine
    printf("Calculating cosmic weather...\n\n");

    AstroConfig config;
    astro_engine_get_default_config(&config);

    int result = astro_engine_init(&config);
    if (result != ASTRO_OK) {
        fprintf(stderr, "Error initializing engine: %s\n",
                astro_core_get_error_message(result));
        return 1;
    }

    // Calculate cosmic weather
    CosmicWeather weather;
    memset(&weather, 0, sizeof(CosmicWeather));

    result = calculate_cosmic_weather(&weather);
    if (result != ASTRO_OK) {
        fprintf(stderr, "Error calculating cosmic weather: %s\n",
                astro_core_get_error_message(result));
        astro_engine_cleanup();
        return 1;
    }

    // Display dashboard
    print_header(stdout);
    print_current_time(&weather, stdout);
    print_planetary_positions(&weather, stdout);
    print_lunar_phase(&weather, stdout);
    print_retrogrades(&weather, stdout);
    print_sky_aspects(&weather, stdout);
    print_aspect_patterns(&weather, stdout);
    print_cosmic_climate(&weather, stdout);
    print_transit_forecast(&weather, stdout);
    print_footer(stdout);

    // Optionally save to file
    if (argc > 1 && strcmp(argv[1], "--save") == 0) {
        FILE *fp = fopen("cosmic_weather_report.txt", "w");
        if (fp) {
            print_header(fp);
            print_current_time(&weather, fp);
            print_planetary_positions(&weather, fp);
            print_lunar_phase(&weather, fp);
            print_retrogrades(&weather, fp);
            print_sky_aspects(&weather, fp);
            print_aspect_patterns(&weather, fp);
            print_cosmic_climate(&weather, fp);
            print_transit_forecast(&weather, fp);
            print_footer(fp);
            fclose(fp);
            printf("Report saved to cosmic_weather_report.txt\n\n");
        }
    }

    // Cleanup
    astro_engine_cleanup();

    return 0;
}
