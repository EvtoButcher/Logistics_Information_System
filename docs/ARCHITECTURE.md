# Architecture

## Layers

- `Core/` — domain types, SQLite access and Qt models exposed to QML.
- `ui/` — widgets, dialogs and the main window.
- `Core/Qml/` — map views and map helpers.
- `Common/` — small shared utilities.
- `Resources/`, `locale/` — icons and translation assets.

## Main flow

1. `MainWindow` creates `MapItemEngine` and opens the database at
   `ApplicationSettings::dbPath()`.
2. `Company` restores warehouses and destinations from `OrderDB`.
3. `MapItemEngine` restores routes, warehouse and destination markers into
   QML models on the main thread.
4. User actions call `MapItemEngine`, which writes to SQLite and emits model
   signals consumed by QML.

## Threading rule

All SQLite access and all QML model updates happen on the main thread.
Do not run database or model code inside `QtConcurrent`/worker threads.
Long-running work, if added later, must return results through signals.

## Database

Schema version 1 (`PRAGMA user_version = 1`):

- `Orders` — order header, route code/color/distance;
- `PathTable` — one JSON-encoded route path per order;
- `Warehouse_table`, `Destination_table` — 64-bit code and position;
- `Cargo_table` — cargo belonging to a warehouse.

Legacy databases are backed up to `*.bak-YYYYMMDD-HHmmss-zzz` and migrated
inside a transaction.

## Simulation

`CarSimulatorModel` is a `QTimer`-driven model. It is controlled only through
the **DevTools** menu, which is visible when the application is started with
`--dev`. There is no simulation on normal startup.

## Limitations

- Route path cache is stored per order; adding routes quickly can overwrite the
  model state used by the path-cache callback.
- The public OSRM endpoint is used for routing.
- Qt 5.15.2 requires OpenSSL 1.1.x; the release archive bundles OpenSSL 1.1.1w.
