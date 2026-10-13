# Battle of the Flatland

Скелет client–server 2D-игры на C++20 + CMake + SFML + Protobuf.

## Раскладка папок

Зависимости и артефакты сборки лежат **рядом с `WorkTraining`**, не внутри проекта:

```
<parent>/
├── WorkTraining/
│   └── Junior/BattleOfTheFlatland/   ← исходники
├── LIB/                              ← SFML, googletest, protobuf
└── BUILD/                            ← cmake build (не в проекте!)
    └── BattleOfTheFlatland/
```

Если у тебя папка сборок называется иначе (например `BILD`) или уже задан CrashCourse-стиль — выставь:

```bat
set CMAKE_BUILDS_ROOT=C:\path\to\BILD
```

## Сторонние библиотеки (`LIB`)

```bat
setup.bat
```

Скрипт **сам находит** существующий `LIB` (canonical `../../../LIB` или flat `../LIB`, либо `BOTF_LIB_ROOT`).  
Если `SFML` / `sfml` / `SFML-*` уже есть — **клонирование пропускается**.

```bat
set BOTF_LIB_ROOT=C:\path\to\LIB
setup.bat
```

## Build (вне проекта)

Не используй `cmake -B build` внутри папки проекта.

```bat
build.bat
```

или вручную:

```bat
cmake -S . -B ..\..\..\BUILD\BattleOfTheFlatland -DBOTF_LIB_ROOT=..\..\..\LIB
cmake --build ..\..\..\BUILD\BattleOfTheFlatland
```

В Visual Studio: открой папку проекта и выбери preset **`vs-default`** / **`default`**  
(`binaryDir` = `../../../BUILD/BattleOfTheFlatland`).

Как в CrashCourse, можно:

```bat
set CMAKE_BUILDS_ROOT=C:\path\to\BUILD
```

и взять preset `debug` / `release`.

Бинарники: `botf_server`, `botf_client`.

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
│   ├── sources/include/common/   # общие типы namespace botf (ClientId, …)
│   └── protobuf/messages/        # proto package botf
├── net/             # SFML TCP: TcpServer + inbound/outbound очереди
├── server/          # botf_server
│   └── logic/       # DynamicObject + GameLogic (только сервер)
├── client/          # botf_client
│   └── app/         # SFML UI: AppRendering, InputHandler, SceneObject, Player
└── tests/
```

### net (серверная основа)

- `InboundQueue` — нотификации от клиентов (payload = protobuf Envelope)
- `OutboundQueue` — нотификации клиентам (`client_id` или broadcast)
- `TcpServer` — `listen` / `poll` на SFML `TcpListener` + `SocketSelector`

## Proto

- `envelope.proto` — `type + sequence + payload`
- `move.proto` — состояние движения на тик: `dx,dy ∈ {-1,0,1}` (диагонали ок, `0,0` = idle)
- `input.proto` — пока не трогаем (aim/fire позже)
- `world_state.proto` — снимок мира

Сервер: `botf_server` — долгий loop на 20 Hz; `botf_server --smoke` — самопроверка.
