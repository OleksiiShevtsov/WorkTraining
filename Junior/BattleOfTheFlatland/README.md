# Battle of the Flatland

Скелет client–server 2D-игры на C++20 + CMake + SFML + Protobuf.

## Раскладка папок

```
<parent>/
├── WorkTraining/
│   └── Junior/BattleOfTheFlatland/   ← исходники
├── LIB/                              ← SFML, googletest, protobuf
└── BUILD/                            ← CMAKE_BUILDS_ROOT указывает сюда
```

## Окружение

Глобально в системе (как в CrashCourse):

- `CMAKE_BUILDS_ROOT` → папка `BUILD` (рядом с `LIB` и `WorkTraining`)

Опционально:

- `BOTF_LIB_ROOT` → папка `LIB` (если CMake/setup не находят её сами)

## CMakePresets

Как в остальных уроках — только через env, без абсолютных путей:

```json
{
  "version": 3,
  "configurePresets": [
    {
      "name": "debug",
      "binaryDir": "$env{CMAKE_BUILDS_ROOT}/${sourceDirName}/debug"
    },
    {
      "name": "release",
      "binaryDir": "$env{CMAKE_BUILDS_ROOT}/${sourceDirName}/release"
    }
  ]
}
```

В Visual Studio выбери preset **debug** или **release**.

## LIB

```bat
setup.bat
```

Ищет `LIB` рядом с `WorkTraining` (или `BOTF_LIB_ROOT`).  
Если SFML / googletest / protobuf уже есть — **не клонирует**.  
Protobuf: тег `v21.12` в `%LIB%\protobuf` (битую папку без `CMakeLists.txt` переклонирует).  
CMake на Windows/LIB собирает protobuf из исходников (`protobuf_BUILD_TESTS=OFF`), без системного `find_package`.

## Format

В корне лежит `.clang-format` (стиль Google, 2 пробела, колонка 100).  
Visual Studio подхватывает его для **Format Document**.

При обычной сборке CMake сначала гоняет `clang-format` (target `botf_format`), потом компилирует.  
Нужен `clang-format` в PATH. Отключить:

```bat
cmake --preset debug -DBOTF_FORMAT_ON_BUILD=OFF
```

Вручную:

```bat
format.bat
cmake --build --preset debug --target botf_format
```

## Build

```bat
build.bat
build.bat release
```

или:

```bat
cmake --preset debug
cmake --build --preset debug
```

Бинарники: `botf_server`, `botf_client` в `%CMAKE_BUILDS_ROOT%\BattleOfTheFlatland\debug` (или `release`).

## Транспорт

```
[uint32 LE length][serialized botf.Envelope]
```

- **SFML Network** — доставка байтов  
- **Protobuf** — тело пакета  
- framing: `net/include/net/packet_framing.hpp`

## Модули

```
BattleOfTheFlatland/
├── cmake/
├── common/
│   ├── sources/include/common/
│   └── protobuf/messages/
├── net/
├── server/
│   └── logic/
├── client/
│   └── app/
└── tests/
```

Сервер: `botf_server` — loop 20 Hz; `botf_server --smoke` — самопроверка.
