# Moongate

**Professional-grade astrological computation engine built on Swiss Ephemeris**  
Modular C framework for natal charts, transits, progressions, synastry, and the exclusive **Planetary Moons Module**.

---

## Highlights

- Modular astro_architecture (`astro_core`, `astro_chart`, `astro_aspects`, `astro_transits`)
- Scientific precision + mythic interpretation
- 27 planetary moons with archetypal depth
- Multiple house systems, aspect types, and chart patterns
- JSON, CSV, and formatted text export
- Extensible and ready for research, apps, or cosmic analytics

---

## Architecture

Moongate is a set of modular C libraries that wrap Swiss Ephemeris with a clean, type-safe API.

| Module | Purpose |
|-------|---------|
| **astro_core** | Low-level ephemeris and planetary position calculations |
| **astro_aspects** | Aspect detection, pattern recognition, strength analysis |
| **astro_chart** | Complete chart generation, synastry, and element distribution |
| **astro_transits** | Transits, progressions, and solar-arc directions |
| **astro_moons** | 27 planetary moons with mythic interpretation (exclusive) |

---

## Quick Start

```bash
# Build the entire engine
make astro

# Run the demo
./astro_demo
```

### Minimal example

```c
#include "astro_engine.h"

int main() {
    astro_engine_init(NULL);

    AstroChart chart;
    astro_engine_quick_chart(
        1991, 3, 20, 22, 24, 27.0,   // UTC
        32.4333, -106.6000,          // Organ, New Mexico
        &chart
    );

    astro_chart_print(&chart, stdout);
    astro_engine_cleanup();
    return 0;
}
```

---

## Planetary Moons Module

Adds 27 moons across Mars, Jupiter, Saturn, Uranus, Neptune, and Pluto with real-time calculation and archetypal interpretation.

Example themes:
- **Phobos / Deimos** — fear and anxiety patterns
- **Io / Europa / Ganymede / Callisto** — transformational mythic currents
- **Titan / Rhea / Iapetus** — primordial power and duality

Moons integrate into natal and transit charts with configurable orbs and symbolic metadata.

---

## Documentation

For full API usage, examples, and module breakdowns, see:  
**[ASTRO_ENGINE_README.md](./ASTRO_ENGINE_README.md)**

---

## Roadmap

- Fixed Stars module (Royal Stars, Pleiades, Galactic Center)
- Extended Asteroids (70+ blueprinted)
- Numerology integration
- Multi-person synastry (2–4 charts)
- REST API wrapper for web and mobile

---

## Credits & Licensing

Moongate builds upon the **Swiss Ephemeris** by Astrodienst AG, used under its dual **AGPL / commercial** license.

All original Moongate extensions, modules, and documentation © 2025 **VexMoon7**.

Swiss Ephemeris licensing info: https://www.astro.com/swisseph/swephinfo_e.htm

---

<p align="center"><em>The Null Meridian Initiative — Metrics for the Mythic Mind.</em></p>
