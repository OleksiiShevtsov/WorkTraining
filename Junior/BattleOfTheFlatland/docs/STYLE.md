# BOTF code style

## Naming

| Что | Стиль | Пример |
|-----|--------|--------|
| Члены класса | `m_` + camelCase | `m_position`, `m_latestWorld` |
| Локальные / параметры | camelCase без префикса | `clientId`, `tickStart` |
| Свободные функции / методы | camelCase | `sceneFromWorldState`, `buildWorldState` |
| Константы | `k` + PascalCase | `kTickHz`, `kPort` |
| Типы / классы | PascalCase | `GameLogic`, `TcpServer` |
| Namespaces | `botf::…` | `botf::net`, `botf::logic`, `botf::app` |

Не использовать trailing underscore (`foo_`) для members.

Исключения (оставляем как есть):

- идентификаторы STL / языка (`static_cast`, `push_back`, `std::unordered_map`, …)
- сгенерированный protobuf API (`set_tick`, `players_size`, `mutable_position`, …)

> `clang-format` правит только пробелы/переносы; переименование идентификаторов — вручную / рефакторингом по этой таблице.

## Форматирование

Корень проекта: `.clang-format` (Google-based, 2 spaces, column 100).  
При сборке target `botf_format` прогоняет `*.cpp` / `*.h` / `*.hpp` модулей проекта.

```bash
./format.sh
# или
cmake --build build --target botf_format
```

## Слои (ориентир)

- `net/` — транспорт (`TcpServer` / `TcpClient`, framing, queues), без геймплея  
- `logic/` — симуляция, без SFML Network / wire protobuf  
- `ServerEngine` — серверный loop (tick + broadcast)  
- `ClientApp` — клиентский loop (input + net + scene)
