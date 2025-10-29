## Quick orientation for AI coding agents

This repository is the Swiss Ephemeris (C) codebase. Below are the minimal, targeted facts and examples you need to be productive here.

### Big picture (what this project is)
- Native C library that computes planetary/fixed-star ephemerides and a small set of CLI tools (e.g. `swetest`, `swevents`, `swemini`).
- Numerical engine and higher-level astrology modules live side-by-side: core ephemeris code (`swe*.c`, `sweph*.c`) vs. higher-level engine modules (`astro_*.c`).
- Ephemeris binary/data files live in `ephe/` and are required for many features; the code reads them at runtime.

### Key files and places to look
- `Makefile` (root): canonical build targets. Typical commands: `make` (builds default binaries), `make swetest`, `make libswe.a`, `make test` (delegates to `setest/`).
- `swetest.c`: primary CLI that demonstrates library usage, ephemeris-path discovery, and flags such as `-edir` and `-ejpl`.
- `sweph.h`, `swephexp.h`, `swephlib.c`: core API, constants, return codes (OK/ERR/NOT_AVAILABLE/BEYOND_EPH_LIMITS), and state struct `swed` (TLS `struct swe_data swed`).
- `swejpl.c` / `ephe/`: JPL ephemeris integration and binary ephemeris files.
- `astro_*.c` and `astro_engine.c`: higher-level astrology features that depend on the core library.

### Build / test / run (concrete commands)
- Build everything (linux/macOS): `make` in repo root. On Linux a fully static `swetests` target is also available (`make swetests`).
- Build just the CLI: `make swetest` (links against `libswe.a`).
- Create library artifacts: `make libswe.a` (static) or `make libswe.so` / macOS `libswe.dylib` (shared target depends on OS flags in `Makefile`).
- Run basic smoke test: after `make`, run `./swetest -h` (this program also shows how ephemeris paths are found and how to set `-edir`).
- Unit/integration tests: `make test` changes into `setest/` and runs its test harness (requires the `setest` subdir to be present and its Makefile).

### Data and licensing notes (important)
- Ephemeris data files are stored under `ephe/` in this repo; many examples and automated runs assume those files are present locally.
- Licensing is dual: AGPL or a commercial Swiss Ephemeris Professional license (see header in `sweph.h`). Do NOT remove or alter existing copyright/license headers when editing code.

### Project-specific code patterns and conventions
- C-only codebase. Naming conventions:
  - core library functions and symbols start with `swe_` (public API) and `swi_`/`swem`/`swel` etc. for internal helpers.
  - higher-level astrology modules use `astro_` prefix.
  - main headers: `sweph.h` (constants/types), `swephlib.h` (library externs), `swephexp.h` (exported API).
- Stateful global: `swed` (TLS) holds ephemeris path, flags and cached data. Many functions read/update this state. Be cautious about concurrent access and initialization.
- Error handling: many APIs return integer status and take a `char *serr` buffer for textual errors. Prefer returning/propagating both the numeric code and serr text when modifying callers.

### Integration points and external dependencies
- JPL ephemeris files & formats: code paths in `swejpl.c` expect specific filenames/DE numbers. Tests or runs using `-ejpl` require the JPL file names to be available.
- Fixed star list `sefstars.txt` and other data files in `ephe/` are read at startup by library routines; ensure correct paths (`swe_set_ephe_path()` called from `swetest.c`, or pass `-edir` on CLI).
- Runtime links: math (`-lm`) and `-ldl` on Linux are used; platform flags are in the `Makefile`.

### Concrete examples for edits and PRs
- When adding a new public function, add a declaration to `swephexp.h` and the matching header comment with the correct copyright header.
- If your work touches ephemeris file handling, add tests or a `swetest` invocation that demonstrates correct ephe path resolution (see `swetest.c`'s `make_ephemeris_path()` usage).
- Keep the C style consistent with existing files: K&R/C89-compatible style, short comments at top that must not be removed.

### Quick checklist for any automated code change
1. Preserve licensing and copyright headers in modified files.
2. Add API declarations to `swephexp.h` for exported functions.
3. Update `Makefile` only if new build targets are needed; prefer adding to existing patterns (see `SWEOBJ`/`ASTROOBJ`).
4. Run `make` and `./swetest -h` (or a minimal `swetest` command) to smoke-test.
5. If changes affect ephemeris/data handling, validate against files in `ephe/`.

If anything here is unclear or you need more detail in a specific area (CI, a particular subsystem, or sample test commands), tell me which part to expand and I will iterate. 
