# Battle of the Flatland

Скелет client–server 2D-игры на C++20 + CMake + SFML + Protobuf.

## Сторонние библиотеки (`LIB`)

Зависимости лежат **рядом с репозиторием `WorkTraining`**, не внутри него:

```
<parent>/
├── WorkTraining/
│   └── Junior/BattleOfTheFlatland/
└── LIB/
    ├── SFML/
    ├── googletest/
    └── protobuf/
```

Установка:

```bat
setup.bat
```

CMake сам резолвит путь в `BOTF_LIB_ROOT` (см. `cmake/BotfLibRoot.cmake`). При необходимости:

```powershell
cmake -S . -B build -DBOTF_LIB_ROOT=C:\path\to\LIB
```

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

## Build

```powershell
cmake -S . -B build
cmake --build build
```

Бинарники: `botf_server`, `botf_client`.
