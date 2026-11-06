# TODO — v0.1.0

Цель: стабильная версия текущего функционала под брендом **LogisticsInformationSystem**.
Формат: чек-лист по PR. Отмечаем по мере выполнения.

## PR 1 — `chore/bootstrap`

- [x] Зафиксировать локальные исправления сборки и карты
- [x] Добавить `build.bat` / `run.bat` и конфигурацию VS Code
- [x] Переименовать бренд: `OctopusLogistics` → `LogisticsInformationSystem`
- [x] Добавить `VERSION = 0.1.0` и отображение версии в About
- [x] Добавить `LICENSE` (MIT), `THIRD_PARTY_NOTICES.md`
- [x] Добавить `.editorconfig`, `.gitattributes`, `.clang-format`
- [x] Расширить `.gitignore` (Qt-артефакты, БД, локальные настройки)
- [x] Добавить CI (GitHub Actions, Qt 5.15.2 MinGW, debug-сборка)
- [x] Добавить шаблоны issue/PR
- [x] Добавить `CHANGELOG.md`
- [x] Обновить `SETUP.md` под бренд
- [x] Собрать, запустить, закоммитить, открыть PR, дождаться CI, squash-merge

## PR 2 — `fix/stability-p0`

- [x] `Company::~Company` — убрать двойное удаление складов
- [x] `CarSimulatorModel` — вернуть корректный `checkTravelStatus`, убрать UB
- [x] `common::createCode` — убрать `reinterpret_cast`/UB
- [x] Проверить пустые `path_`/`path_cache_` и границы индексов
- [x] `OrderTable` — layout warning, утечка `QColorDialog`, начальный цвет
- [x] `CreateWarehouseDialog` / `CreateDestinationDialog` — утечки объектов
- [x] `RouteModel::checkPathCacheStatus` — возвращать правильный статус
- [x] Убрать warning’и компилятора
- [ ] Тесты для `createCode` и парсинга путей (PR 6)

## PR 3 — `refactor/data-layer`

- [ ] `ApplicationSettings`: `QSettings` org/app, а не файл в CWD
- [ ] БД в `QStandardPaths::AppDataLocation`, имя `logistics.db`
- [ ] Проверять `name` и `name + ".db"`, закрывать соединение
- [ ] Уникальное имя SQLite-соединения, `QSqlQuery` без утечки
- [ ] Схема v1 + `PRAGMA user_version` + миграция с бэкапом
- [ ] Удаление пути по `main_code`, транзакции, `foreign_keys=ON`
- [ ] Хранить путь как JSON, добавить `Cargo_table`
- [ ] Тесты CRUD и миграции legacy-БД

## PR 4 — `refactor/models-threading`

- [ ] Убрать `QtConcurrent`/`QThread::msleep`/`processEvents` из restore/route
- [ ] Все операции с SQLite — в одном потоке
- [ ] Заменить статусные «рукопожатия» на сигналы
- [ ] Перевести карту на list-модели (или синхронное восстановление)
- [ ] Проверить отсутствие cross-thread warning’ов

## PR 5 — `refactor/ui-devtools`

- [ ] Меню DevTools за флагом `--dev`
- [ ] Симуляция на `QTimer` без busy-wait
- [ ] Start/Stop/Speed/Reset симуляции
- [ ] Удаление склада/назначения — синхронизация UI, компании, карты, БД
- [ ] Валидация координат, имени маршрута, веса/объёма и сброс полей
- [ ] Инфраструктура переводов (English по умолчанию, RU позже)
- [ ] Переименования: `Triffic`, `Colpleted`, `Wudget`, `inserInto`

## PR 6 — `test/release-0.1.0`

- [ ] QtTest: `common`, `RouteInfo`, БД, настройки, модели
- [ ] CI: тесты + сборка release
- [ ] README: реальные возможности, сборка, скриншоты, roadmap
- [ ] CHANGELOG, `docs/ARCHITECTURE.md`, `docs/BUILD.md`
- [ ] Portable zip: `windeployqt` + OpenSSL + exe
- [ ] Тег `v0.1.0` и GitHub Release

## Backlog v0.2.0

- [ ] Автоподбор грузовиков и объединение заказов
- [ ] Геокодинг адресов
- [ ] Русский UI по умолчанию
- [ ] Qt 6 / CMake, структура `src/`
- [ ] Установщик
