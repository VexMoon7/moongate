# Astrological Data Analysis Engine

A modular, professional-grade ephemeris and astrological data analysis engine built on the Swiss Ephemeris library.

## Overview

This engine provides a comprehensive, modular framework for astrological calculations and analysis. It wraps the powerful Swiss Ephemeris library with a clean, intuitive API suitable for building astrological applications.

## Features

### Core Capabilities

- **Ephemeris Calculations**: Accurate planetary positions using Swiss Ephemeris data
- **Multiple House Systems**: Placidus, Koch, Equal, Whole Sign, and 5 more
- **Aspect Analysis**: Major and minor aspects with configurable orbs
- **Pattern Recognition**: Grand trines, T-squares, stelliums, and more
- **Natal Charts**: Complete birth chart calculation and analysis
- **Transits**: Current and future transits to natal positions
- **Progressions**: Secondary, tertiary, and minor progressions
- **Solar Arc Directions**: Advanced predictive technique
- **Synastry**: Relationship compatibility analysis
- **Returns**: Solar, lunar, and planetary returns
- **Multiple Formats**: Export to JSON, CSV, or formatted text

### Technical Features

- Modular architecture with clean separation of concerns
- Type-safe API with comprehensive error handling
- Efficient calculations using Swiss Ephemeris
- Support for 23 celestial bodies (planets, asteroids, nodes)
- Tropical and sidereal zodiac systems
- Topocentric (location-specific) calculations
- Retrograde detection
- Element and modality analysis
- Chart shape/pattern detection

## Architecture

The engine is organized into four main modules:

### 1. astro_core - Core Ephemeris Module
Low-level ephemeris calculations and fundamental astronomical data.

**Key Functions:**
- `astro_core_init()` - Initialize the engine
- `astro_core_calc_planet()` - Calculate single planet position
- `astro_core_calc_planets()` - Calculate all planets
- `astro_core_calc_houses()` - Calculate house cusps
- `astro_core_get_sign()` - Get zodiac sign from longitude

### 2. astro_aspects - Aspect Analysis Module
Aspect calculation, pattern recognition, and interpretation.

**Key Functions:**
- `astro_aspects_calc_aspect()` - Calculate aspect between two planets
- `astro_aspects_calc_all()` - Calculate all aspects in chart
- `astro_aspects_find_patterns()` - Find aspect patterns
- `astro_aspects_is_harmonious()` - Determine aspect nature
- `astro_aspects_get_strength()` - Calculate aspect strength

### 3. astro_chart - Natal Chart Module
Complete natal chart creation, analysis, and output.

**Key Functions:**
- `astro_chart_create()` - Create complete natal chart
- `astro_chart_get_planet()` - Query planet by type
- `astro_chart_analyze_elements()` - Element distribution
- `astro_chart_calc_synastry()` - Relationship compatibility
- `astro_chart_export_json()` - Export to JSON

### 4. astro_transits - Transit & Progression Module
Transit calculations, progressions, and predictive techniques.

**Key Functions:**
- `astro_transits_calc_current()` - Current transits
- `astro_transits_calc_period()` - Transits over time period
- `astro_transits_find_exact()` - Find exact transit time
- `astro_transits_calc_progressed_chart()` - Secondary progressions
- `astro_transits_calc_solar_arc()` - Solar arc directions

## Quick Start

### Basic Example

```c
#include "astro_engine.h"

int main() {
    // Initialize engine
    astro_engine_init(NULL);

    // Create birth data
    AstroDateTime birth_time = {
        .year = 1990, .month = 1, .day = 15,
        .hour = 14, .minute = 30, .second = 0,
        .timezone_offset = 0.0
    };

    AstroLocation birth_location = {
        .latitude = 51.5074,  // London
        .longitude = -0.1278,
        .altitude = 0.0
    };

    // Create natal chart
    AstroChart chart;
    astro_chart_create(&birth_time, &birth_location, NULL, &chart);

    // Print chart
    astro_chart_print(&chart, stdout);

    // Export to JSON
    astro_chart_export_json(&chart, "chart.json");

    // Cleanup
    astro_engine_cleanup();

    return 0;
}
```

### Quick Chart Creation

```c
#include "astro_engine.h"

int main() {
    astro_engine_init(NULL);

    AstroChart chart;
    astro_engine_quick_chart(
        1990, 1, 15,      // Date
        14, 30, 0.0,      // Time
        0.0,              // UTC
        51.5074, -0.1278, // London coordinates
        &chart
    );

    astro_chart_print(&chart, stdout);
    astro_engine_cleanup();

    return 0;
}
```

### Transit Analysis

```c
#include "astro_engine.h"

int main() {
    astro_engine_init(NULL);

    // Create natal chart
    AstroChart natal_chart;
    astro_engine_quick_chart(1990, 1, 15, 14, 30, 0.0, 0.0,
                            51.5074, -0.1278, &natal_chart);

    // Calculate current transits
    AstroTransit transits[ASTRO_MAX_TRANSITS];
    int num_transits;

    astro_engine_current_transits(&natal_chart, transits,
                                 ASTRO_MAX_TRANSITS, &num_transits);

    // Print transits
    astro_transits_print(transits, num_transits, stdout);

    astro_engine_cleanup();
    return 0;
}
```

## Building

### Prerequisites
- C compiler (GCC, Clang, or compatible)
- Swiss Ephemeris data files (in `./ephe/` directory)
- Make

### Compilation

```bash
# Build library and demo
make astro

# Or build individual components
make astro_demo
make libastro.a
```

### Running the Demo

```bash
# Interactive demo
./astro_demo

# Run all demos
./astro_demo --all
```

## Configuration

### Default Configuration

```c
AstroConfig config = {
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

astro_engine_init(&config);
```

### Custom Aspect Orbs

```c
// Set custom orb for specific aspect
astro_aspects_set_orb(ASTRO_ASPECT_TRINE, 6.0);

// Reset all orbs to defaults
astro_aspects_reset_orbs();
```

## Data Structures

### Core Types

```c
// Date and time
typedef struct {
    int year, month, day;
    int hour, minute;
    double second;
    double timezone_offset;
    double julian_day;
} AstroDateTime;

// Geographic location
typedef struct {
    double latitude;
    double longitude;
    double altitude;
    char location_name[64];
} AstroLocation;

// Planet position
typedef struct {
    AstroPlanet planet;
    double longitude, latitude, distance;
    double longitude_speed;
    AstroSign sign;
    double sign_position;
    int house;
    int is_retrograde;
    char name[64];
} AstroPlanetPosition;

// Complete natal chart
typedef struct {
    AstroDateTime birth_time;
    AstroLocation birth_location;
    AstroPlanetPosition planets[23];
    int num_planets;
    AstroHouses houses;
    AstroAspect aspects[50];
    int num_aspects;
    char chart_name[64];
} AstroChart;
```

## Supported Bodies

- **Planets**: Sun, Moon, Mercury, Venus, Mars, Jupiter, Saturn, Uranus, Neptune, Pluto
- **Lunar Nodes**: Mean Node, True Node
- **Apogee/Perigee**: Mean Apogee (Black Moon Lilith), Osculating Apogee
- **Asteroids**: Chiron, Pholus, Ceres, Pallas, Juno, Vesta
- **Special**: Earth (for heliocentric calculations)

## House Systems

- Placidus
- Koch
- Porphyrius
- Regiomontanus
- Campanus
- Equal
- Whole Sign
- Meridian
- Morinus

## Aspect Types

### Major Aspects
- Conjunction (0°)
- Opposition (180°)
- Trine (120°)
- Square (90°)
- Sextile (60°)

### Minor Aspects
- Quincunx (150°)
- Semi-sextile (30°)
- Semi-square (45°)
- Sesquiquadrate (135°)
- Quintile (72°)
- Biquintile (144°)

## Advanced Features

### Progression Types

```c
// Secondary progressions (1 day = 1 year)
astro_transits_calc_progressed_chart(natal_chart,
    ASTRO_PROGRESSION_SECONDARY, target_date, &prog_chart);

// Tertiary progressions (1 day = 1 lunar month)
astro_transits_calc_progressed_chart(natal_chart,
    ASTRO_PROGRESSION_TERTIARY, target_date, &prog_chart);

// Solar arc directions
astro_transits_calc_solar_arc(natal_chart, target_date, &directed_chart);
```

### Synastry Analysis

```c
AstroChart chart1, chart2;
// ... create charts ...

AstroSynastry synastry;
astro_chart_calc_synastry(&chart1, &chart2, &synastry);

printf("Compatibility: %.1f/100\n", synastry.compatibility_score);
printf("Aspects: %d\n", synastry.num_aspects);
```

### Pattern Recognition

The engine automatically detects:
- Grand Trines
- T-Squares
- Grand Crosses
- Yods
- Stelliums
- Kites
- Mystic Rectangles

```c
AstroPattern patterns[20];
int num_patterns;

astro_aspects_find_patterns(positions, num_planets,
    aspects, num_aspects, patterns, 20, &num_patterns);

for (int i = 0; i < num_patterns; i++) {
    printf("%s\n", patterns[i].description);
}
```

## Output Formats

### Text Output
```c
astro_chart_print(&chart, stdout);
```

### JSON Export
```c
astro_chart_export_json(&chart, "chart.json");
```

### CSV Export
```c
astro_chart_export_csv(&chart, "chart.csv");
```

## Error Handling

All functions return `AstroErrorCode`:

```c
int result = astro_chart_create(&time, &location, NULL, &chart);
if (result != ASTRO_OK) {
    printf("Error: %s\n", astro_core_get_error_message(result));
    return 1;
}
```

Error codes:
- `ASTRO_OK` - Success
- `ASTRO_ERROR_INVALID_DATE` - Invalid date/time
- `ASTRO_ERROR_INVALID_LOCATION` - Invalid coordinates
- `ASTRO_ERROR_EPHEMERIS_FILE` - Ephemeris file error
- `ASTRO_ERROR_CALCULATION` - Calculation error
- `ASTRO_ERROR_NULL_POINTER` - Null pointer argument
- And more...

## Performance

- Planet calculations: ~0.1ms per planet
- Complete chart: ~5ms
- Transit search (1 year): ~100ms
- Pattern detection: ~1ms

Times measured on modern CPU, may vary based on configuration.

## Limitations

- Date range: ~10,000 BCE to 10,000 CE (Swiss Ephemeris dependent)
- Requires ephemeris data files
- Thread safety: Not guaranteed (use separate instances per thread)
- Memory: Static allocation for predictable behavior

## License

This engine interfaces with the Swiss Ephemeris library which is dual-licensed:
- AGPL (free for open-source projects)
- Commercial license (required for proprietary software)

See Swiss Ephemeris documentation for complete licensing details.

## Credits

Built on the Swiss Ephemeris library by Astrodienst AG:
- Authors: Dieter Koch, Alois Treindl
- Website: https://www.astro.com/swisseph/

## Version History

### Version 1.0.0 (2025)
- Initial release
- Complete modular architecture
- Core ephemeris calculations
- Aspect analysis with patterns
- Natal chart creation
- Transit calculations
- Progression support
- Synastry analysis
- Multiple output formats

## Support

For issues with this engine wrapper, see the source code and examples.

For Swiss Ephemeris issues, consult the official documentation at:
https://www.astro.com/swisseph/

## Examples

See `astro_demo.c` for comprehensive examples of all features.

Run the demo to explore:
```bash
./astro_demo --all
```
