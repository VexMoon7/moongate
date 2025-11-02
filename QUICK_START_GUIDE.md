# Quick Start Guide - Creating Charts & Synastry

This guide shows you how to create natal charts and perform synastry analysis with planetary moons.

## 🚀 Quick Demo

```bash
# Build everything
make astro

# Run the complete example
./chart_and_synastry_example
```

## 📖 Basic Usage

### 1. Creating a Natal Chart

```c
#include "astro_engine.h"

int main() {
    // Initialize
    astro_core_init(NULL);
    astro_core_set_ephemeris_path("./ephe");

    // Set up birth data
    AstroDateTime birth_time = {
        .year = 1990,
        .month = 3,
        .day = 15,
        .hour = 14,
        .minute = 30,
        .second = 0,
        .timezone_offset = 0.0  // UTC
    };

    AstroLocation birth_location;
    birth_location.latitude = 51.5074;   // London
    birth_location.longitude = -0.1278;
    birth_location.altitude = 0.0;
    strcpy(birth_location.location_name, "London, UK");

    // Create chart
    AstroChart chart;
    astro_chart_create(&birth_time, &birth_location, NULL, &chart);

    // Print chart
    astro_chart_print(&chart, stdout);

    // Cleanup
    astro_core_cleanup();
    return 0;
}
```

### 2. Adding Planetary Moons Analysis

```c
#include "astro_planetary_moons.h"

// After creating a chart, add moon analysis:

MoonPlanetConjunction conjunctions[50];
int num_conj;

astro_moons_find_conjunctions(&chart, &birth_time, 2.0,
                              conjunctions, 50, &num_conj);

// Print results
for (int i = 0; i < num_conj; i++) {
    printf("%s conjunct %s (%.2f°)\n",
           conjunctions[i].moon_name,
           conjunctions[i].planet_name,
           conjunctions[i].orb);
}
```

### 3. Performing Synastry Analysis

```c
// Create two charts
AstroChart chart1, chart2;
astro_chart_create(&person1_birth, &person1_location, NULL, &chart1);
astro_chart_create(&person2_birth, &person2_location, NULL, &chart2);

// Calculate synastry
AstroSynastry synastry;
astro_chart_calc_synastry(&chart1, &chart2, &synastry);

// Print results
printf("Compatibility: %.1f/100\n", synastry.compatibility_score);
printf("Aspects found: %d\n", synastry.num_aspects);

// Print aspects
for (int i = 0; i < synastry.num_aspects; i++) {
    printf("%s %s %s (%.1f°)\n",
           astro_core_get_planet_name(synastry.aspects[i].planet1),
           astro_aspects_get_name(synastry.aspects[i].aspect_type),
           astro_core_get_planet_name(synastry.aspects[i].planet2),
           synastry.aspects[i].orb);
}
```

## 🌍 Common Locations (Lat/Long)

```
London:      51.5074, -0.1278
New York:    40.7128, -74.0060
Los Angeles: 34.0522, -118.2437
Chicago:     41.8781, -87.6298
Tokyo:       35.6762, 139.6503
Sydney:     -33.8688, 151.2093
Paris:       48.8566, 2.3522
Berlin:      52.5200, 13.4050
Mumbai:      19.0760, 72.8777
São Paulo:  -23.5505, -46.6333
```

**Note:**
- North latitude = positive, South = negative
- East longitude = positive, West = negative

## ⏰ Time Format

All times should be in **24-hour format, UTC**:

```c
AstroDateTime birth = {
    .year = 1990,
    .month = 3,         // 1-12 (January = 1)
    .day = 15,          // 1-31
    .hour = 14,         // 0-23 (14 = 2 PM)
    .minute = 30,       // 0-59
    .second = 0,        // 0-59
    .timezone_offset = 0.0  // 0.0 = UTC
};
```

**Converting local time to UTC:**
- PDT (UTC-7): Add 7 hours
- EDT (UTC-4): Add 4 hours
- BST (UTC+1): Subtract 1 hour
- JST (UTC+9): Subtract 9 hours

Example: 2:30 PM PDT = 14:30 + 7 = 21:30 UTC

## 🔧 Complete Example Program

See `chart_and_synastry_example.c` for a complete working example that shows:

1. Creating natal charts for two people
2. Adding planetary moon analysis to each chart
3. Performing synastry analysis
4. Adding planetary moons to synastry
5. Complete formatted output

**To compile and run:**

```bash
make chart_and_synastry_example
./chart_and_synastry_example
```

## 📊 Orb Settings

**For Natal Charts:**
- Major aspects: 8° (default)
- Minor aspects: 3° (default)

**For Planetary Moons:**
- Conjunctions: 2° (recommended)
- Tight orb for subtle influences

**For Synastry:**
- Use same orbs as natal charts
- Or tighten for most significant connections

## 🎨 Customizing Your Analysis

### Adjust Orbs

```c
// For moons, use tighter orbs (1-2 degrees)
astro_moons_find_conjunctions(&chart, &birth_time, 1.5, ...);

// For synastry, standard orbs work well
astro_chart_calc_synastry(&chart1, &chart2, &synastry);
```

### Filter Results

```c
// Show only exact conjunctions
for (int i = 0; i < num_conj; i++) {
    if (conjunctions[i].is_exact) {
        // Print only exact conjunctions
    }
}
```

### Focus on Specific Moons

```c
// Get just Jupiter's moons
PlanetaryMoonPosition galilean[4];
astro_moons_get_galilean_moons(&birth_time, galilean);

// Or Saturn's moons
PlanetaryMoonPosition saturn_moons[8];
int num_saturn;
astro_moons_get_system_moons(ASTRO_SATURN, &birth_time,
                             saturn_moons, 8, &num_saturn);
```

## 💡 Tips

### For Beginners:
1. Start with `./chart_and_synastry_example` to see output format
2. Modify the birth data in the code
3. Recompile and run to see your own chart
4. Gradually add more features as you learn

### For Intermediate Users:
1. Use `astro_chart_export_json()` to save charts
2. Build custom interpretation functions
3. Combine with Sabian symbols for deeper meaning
4. Experiment with different house systems

### For Advanced Users:
1. Integrate with databases to store charts
2. Build web APIs using the engine
3. Create custom moon interpretation algorithms
4. Explore composite and Davison charts (planned features)

## 🐛 Common Issues

**Problem:** "Error calculating chart"
- **Solution:** Check that ephemeris files are in `./ephe/` directory

**Problem:** "No aspects found"
- **Solution:** Increase orb or check that both charts were created successfully

**Problem:** "Segmentation fault"
- **Solution:** Always initialize with `astro_core_init()` before calculating

**Problem:** "Wrong positions"
- **Solution:** Verify date/time is in UTC, not local time

## 📚 Next Steps

1. **Read the full docs:**
   - `ASTRO_ENGINE_README.md` - Complete API reference
   - `MYTHIC_ASTROLOGY_MODULES.md` - Advanced features
   - `COMPREHENSIVE_FEATURES.md` - Full feature list

2. **Explore example programs:**
   - `astro_demo.c` - All core features
   - `cosmic_weather.c` - Real-time astrology
   - `mythic_transits.c` - Deep transit analysis
   - `planetary_moons_demo.c` - Moon mythology

3. **Experiment:**
   - Modify `chart_and_synastry_example.c` with your own data
   - Try different orbs and configurations
   - Build custom interpretation functions

## 🎯 Quick Reference: Key Functions

### Chart Creation
```c
astro_chart_create()           // Create natal chart
astro_chart_print()            // Print chart to console
astro_chart_export_json()      // Export to JSON
astro_chart_export_csv()       // Export to CSV
```

### Planetary Moons
```c
astro_moons_calc_position()          // Single moon position
astro_moons_calc_all_positions()     // All 27 moons
astro_moons_find_conjunctions()      // Find natal conjunctions
astro_moons_get_galilean_moons()     // Jupiter's 4 moons
astro_moons_get_system_moons()       // All moons for a planet
```

### Synastry
```c
astro_chart_calc_synastry()    // Calculate compatibility
// Returns AstroSynastry with:
//   - compatibility_score (0-100)
//   - num_aspects
//   - aspects[] array
```

### Transits
```c
astro_transits_calc_current()  // Current transits
astro_transits_calc_period()   // Transits over time
astro_transits_find_exact()    // Exact transit timing
```

## 🌟 Example: Quick Chart Creation

Minimal working example (20 lines):

```c
#include "astro_engine.h"
#include <stdio.h>

int main() {
    astro_core_init(NULL);
    astro_core_set_ephemeris_path("./ephe");

    AstroChart chart;
    astro_engine_quick_chart(
        1990, 3, 15,      // Date: March 15, 1990
        14, 30, 0.0,      // Time: 14:30 UTC
        0.0,              // Timezone offset
        51.5074, -0.1278, // London coordinates
        &chart
    );

    astro_chart_print(&chart, stdout);
    astro_core_cleanup();
    return 0;
}
```

Compile and run:
```bash
cc -o quick_chart quick_chart.c -L. -lastro -lm -ldl
./quick_chart
```

---

**For more details, see:**
- Full API documentation: `ASTRO_ENGINE_README.md`
- Complete features: `COMPREHENSIVE_FEATURES.md`
- Source code examples: `chart_and_synastry_example.c`
