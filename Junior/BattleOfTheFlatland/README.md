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
Если SFML уже есть — **не клонирует**.

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
