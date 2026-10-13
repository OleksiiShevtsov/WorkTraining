#pragma once

#include "common/types.hpp"
#include "logic/dynamic_object.h"

#include <optional>
#include <unordered_map>
#include <vector>

namespace botf {

// Server-side world simulation. No SFML / no networking.
class GameLogic {
 public:
  GameLogic();

  GameLogic(const GameLogic&) = delete;
  GameLogic& operator=(const GameLogic&) = delete;

  // Spawn a controllable entity; returns its logic id.
  common::ID spawn_player(common::Point at, float speed_limit);

  // Bind a network client to a logic entity (one entity per client).
  void bind_client(ClientId client_id, common::ID entity_id);

  // Apply Move-state input for a network client (no-op if unbound).
  void set_client_input(ClientId client_id, int dx, int dy);

  // Apply input directly by entity id.
  void set_player_input(common::ID entity_id, int dx, int dy);

  // Advance the world by dt seconds.
  void tick(float dt);

  [[nodiscard]] const std::vector<DynamicObject>& objects() const;
  [[nodiscard]] std::optional<common::ID> entity_for_client(ClientId client_id) const;

 private:
  DynamicObject* find_mutable(common::ID entity_id);

  std::vector<DynamicObject> m_objects;
  std::unordered_map<ClientId, common::ID> m_client_to_entity;
  common::ID m_next_entity_id{1};
};

}  // namespace botf
