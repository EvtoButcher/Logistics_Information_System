# Logistics Information System

**LogisticsInformationSystem** — десктопное приложение для планирования перевозок
между складами и пунктами назначения. Отображает маршруты на карте
OpenStreetMap, хранит заказы в SQLite и позволяет запускать симуляцию движения
грузовиков в режиме разработчика.

Статус: **v0.1.0** (стабилизация текущего функционала).

## Возможности v0.1.0

- создание компании, складов, грузов и пунктов назначения;
- создание заказов и построение маршрутов на карте OSM;
- цвет маршрута, расстояние и сохранение пути в базе данных;
- восстановление заказов, складов и пунктов назначения после перезапуска;
- таблица заказов: выбор, смена цвета, удаление;
- DevTools-меню с симуляцией движения грузовиков (запуск с `--dev`).

Пока **не реализовано** (в roadmap): автоматическое назначение грузовиков,
объединение заказов, геокодинг адресов, русский интерфейс по умолчанию.

## Требования

- Windows 10/11, 64-bit
- Qt 5.15.2 (MinGW 8.1)
- OpenSSL 1.1.1 (для HTTPS-тайлов карты)

Подробная настройка окружения — в [SETUP.md](SETUP.md).

## Сборка и запуск

```bat
build.bat            :: debug
build.bat release    :: release
run.bat              :: запуск debug-сборки
run.bat release      :: запуск release-сборки
```

Готовый файл: `build\debug\LogisticsInformationSystem.exe`.

Для VS Code в проекте есть `.vscode` (сборка по `Ctrl+Shift+B`, запуск по `F5`).

### DevTools

Симуляция грузовиков доступна только в dev-режиме:

```bat
build\debug\LogisticsInformationSystem.exe --dev
```

В меню **DevTools** есть запуск/остановка, скорость и сброс симуляции.

### Тесты

```bat
mkdir build\tests
cd build\tests
qmake ..\..\tests\tests.pro -spec win32-g++ "CONFIG+=debug"
mingw32-make
debug\tst_core.exe
```

### Данные

База и резервные копии: `%APPDATA%\TheKramarch\LogisticsInformationSystem`.
Настройки: `HKEY_CURRENT_USER\Software\TheKramarch\LogisticsInformationSystem`.

## Скриншоты

### Главное окно

![Главное окно](https://github.com/EvtoButcher/Logistics_Information_System/assets/52111046/609c4bc0-f91c-4bed-b171-cd2651b4108b)

### Создание компании

![Создание компании](https://github.com/EvtoButcher/Logistics_Information_System/assets/52111046/b923e4db-5437-4608-9246-42d6c5012c07)

### Симуляция движения

![Симуляция](https://github.com/EvtoButcher/Logistics_Information_System/assets/52111046/7dd0a65e-1b7a-4533-adbe-39b7c713ccce)

## Структура

- `Core/` — доменная модель, БД, модели для QML;
- `ui/` — виджеты и диалоги;
- `Core/Qml/` — QML-карта и вспомогательные компоненты;
- `Common/` — общие утилиты;
- `Resources/`, `locale/` — ресурсы и переводы.

## Документация

- [SETUP.md](SETUP.md) — окружение, сборка, запуск, известные особенности;
- [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) — слои, потоки и схема БД;
- [TODO.md](TODO.md) — план v0.1.0 и backlog;
- [CHANGELOG.md](CHANGELOG.md) — история версий.

## Лицензия

Код проекта распространяется по лицензии [MIT](LICENSE).
Сторонние компоненты (Qt, OpenSSL, данные OpenStreetMap) описаны в
[THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md).
