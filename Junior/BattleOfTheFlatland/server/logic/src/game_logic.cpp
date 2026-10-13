#include "logic/game_logic.h"

#include "common/global_settings.hpp"

namespace botf {

GameLogic::GameLogic() {
  const common::ID id = spawn_player(
      common::Point{common::GlobalSettings::defaultSpawnX, common::GlobalSettings::defaultSpawnY},
      common::GlobalSettings::defaultPlayerSpeedLimit);
  (void)id;
}

common::ID GameLogic::spawn_player(common::Point at, float speed_limit) {
  const common::ID id = m_next_entity_id++;
  m_objects.emplace_back(id, at, speed_limit);
  return id;
}

void GameLogic::bind_client(ClientId client_id, common::ID entity_id) {
  m_client_to_entity[client_id] = entity_id;
}

void GameLogic::set_client_input(ClientId client_id, int dx, int dy) {
  const auto it = m_client_to_entity.find(client_id);
  if (it == m_client_to_entity.end()) {
    return;
  }
  set_player_input(it->second, dx, dy);
}

void GameLogic::set_player_input(common::ID entity_id, int dx, int dy) {
  if (DynamicObject* obj = find_mutable(entity_id)) {
    obj->set_input(dx, dy);
  }
}

void GameLogic::tick(float dt) {
  for (auto& obj : m_objects) {
    obj.update(dt);
  }
}

const std::vector<DynamicObject>& GameLogic::objects() const {
  return m_objects;
}

std::optional<common::ID> GameLogic::entity_for_client(ClientId client_id) const {
  const auto it = m_client_to_entity.find(client_id);
  if (it == m_client_to_entity.end()) {
    return std::nullopt;
  }
  return it->second;
}

DynamicObject* GameLogic::find_mutable(common::ID entity_id) {
  for (auto& obj : m_objects) {
    if (obj.id() == entity_id) {
      return &obj;
    }
  }
  return nullptr;
}

}  // namespace botf
