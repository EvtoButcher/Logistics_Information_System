# Changelog

All notable changes to this project are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- Repository bootstrap: build/run scripts, VS Code configuration, CI.
- MIT license and third-party notices.
- Project TODO and v0.1.0 roadmap.

### Changed

- Project branding: `OctopusLogistics` → `LogisticsInformationSystem`.
- Application version is now `0.1.0`.

## [0.1.0] - TBD

### Added

- Company creation with warehouses, cargo and destinations.
- Order creation, OSM map routes, route colors and distance.
- SQLite persistence and map state restoration.
- DevTools truck simulation (behind `--dev`).

### Fixed

- Build failure caused by a Cyrillic character in `OrderDB.cpp`.
- Map did not render because the `mapboxgl` provider requires an API key;
  switched to the OpenStreetMap provider.

[Unreleased]: https://github.com/EvtoButcher/Logistics_Information_System/compare/v0.1.0...HEAD
[0.1.0]: https://github.com/EvtoButcher/Logistics_Information_System/releases/tag/v0.1.0
