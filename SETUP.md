# Подготовка проекта к работе

Репозиторий: `D:\VS Projects\Logistics_Information_System`.
Проект собран и проверен: диалог создания компании, карта OpenStreetMap и
главное окно запускаются.

## Что установлено

| Компонент | Версия | Путь |
| --- | --- | --- |
| Qt | 5.15.2, MinGW 8.1, 64-bit | `D:\Qt\5.15.2\mingw81_64` |
| MinGW | 8.1.0 (x86_64, posix-seh) | `D:\Qt\Tools\mingw810_64` |
| OpenSSL | 1.1.1w (x64) | `D:\OpenSSL-Win64` |
| Python (для aqtinstall) | 3.12 | `%LOCALAPPDATA%\Programs\Python\Python312` |

Qt Location (`Qt5Location.dll`, плагин `osm`) входит в установленный пакет Qt.
Qt Creator не устанавливался — для сборки и запуска достаточно `.bat`-скриптов
или VS Code.

## Сборка и запуск

Из папки проекта:

```bat
build.bat            :: debug-сборка (по умолчанию)
build.bat release    :: release-сборка

run.bat              :: запуск debug-сборки
run.bat release      :: запуск release-сборки
```

Готовый файл: `build\debug\LogisticsInformationSystem.exe`
(или `build\release\...` для release).

Скрипты сами добавляют в `PATH` нужные каталоги Qt, MinGW и OpenSSL.
Если компоненты установлены в другие каталоги, поправьте переменные
`QT_DIR`, `MINGW_DIR`, `OPENSSL_DIR` в начале `build.bat` и `run.bat`.

### VS Code

В папке проекта есть `.vscode`:

* `Ctrl+Shift+B` — сборка (debug);
* `F5` — сборка и запуск под gdb;
* задача `run: debug` — просто запуск.

## Первый запуск

1. Запустите `run.bat`.
2. Появится диалог **Create Company**: введите название компании.
3. Добавьте склады и пункты назначения (кнопки под таблицами).
4. Нажмите **Create company** — откроется главное окно с картой.

Данные приложения хранятся в `%APPDATA%\TheKramarch\LogisticsInformationSystem`:

* `logistics.db` — база SQLite (схема v1);
* резервные копии `logistics.db.bak-*` создаются перед миграцией старой схемы.

Настройки (имя компании, путь к БД, локаль) хранятся в реестре Windows
через `QSettings`:
`HKEY_CURRENT_USER\Software\TheKramarch\LogisticsInformationSystem`.

Кэш тайлов карты: `%LOCALAPPDATA%\cache\QtLocation\5.8\tiles\osm`.

### DevTools и симуляция

Меню **DevTools** появляется только при запуске с флагом `--dev`:

```bat
build\debug\LogisticsInformationSystem.exe --dev
```

В меню доступны запуск/остановка симуляции, скорость (1x/2x/5x/10x) и сброс.
Симуляция использует `QTimer` и не блокирует интерфейс.

### Тесты

```bat
mkdir build\tests
cd build\tests
qmake ..\..\tests\tests.pro -spec win32-g++ "CONFIG+=debug"
mingw32-make
debug\tst_core.exe -o test-results.txt,txt
type test-results.txt
```

## Известные особенности (не блокируют работу)

* `QObject::connect(QQuickWindow, QDeclarativeGeoMap): invalid nullptr parameter` —
  безобидное предупреждение Qt Location при инициализации карты.
* Маршруты строятся через публичный OSRM (`router.project-osrm.org`) —
  при большом количестве запросов сервер может отвечать медленно.
* При активном использовании тайлов OSM стоит соблюдать
  [tile usage policy](https://operations.osmfoundation.org/policies/tiles/).

## Полезные команды

```bat
:: пересобрать с нуля
rmdir /s /q build
build.bat

:: проверить, что qmake видит установленный Qt
D:\Qt\5.15.2\mingw81_64\bin\qmake.exe -query QT_INSTALL_PREFIX
```
