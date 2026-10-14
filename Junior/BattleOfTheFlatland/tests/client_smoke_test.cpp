#include "app/client_app.h"
#include "app/input_handler.h"
#include "app/player.h"
#include "envelope.pb.h"
#include "net/packet_framing.hpp"
#include "net/protocol.hpp"
#include "world_state.pb.h"

#include <vector>

#include <gtest/gtest.h>

TEST(ClientSmoke, WorldStateMapsToSceneObject) {
  botf::app::InputHandler input;
  const auto sample = input.readMoveInput();
  const auto movePacket =
      botf::net::framePayload(botf::net::makeMoveEnvelope(sample.dx, sample.dy, 1, 1));
  EXPECT_FALSE(movePacket.empty());

  botf::WorldState state;
  state.set_tick(42);
  auto* player = state.add_players();
  player->set_id(7);
  player->mutable_position()->set_x(100.f);
  player->mutable_position()->set_y(300.f);
  player->set_aim_angle(1.25f);

  const auto framed = botf::net::framePayload(botf::net::makeWorldStateEnvelope(state, 1));

  std::vector<std::uint8_t> buffer(framed.begin(), framed.end());
  auto consumed = botf::net::tryConsumeFrame(buffer);
  ASSERT_TRUE(consumed.has_value());

  auto envelope = botf::net::parseEnvelope(*consumed);
  ASSERT_TRUE(envelope.has_value());
  ASSERT_EQ(envelope->type(), botf::MESSAGE_TYPE_WORLD_STATE);

  auto parsed = botf::net::parseWorldStatePayload(envelope->payload());
  ASSERT_TRUE(parsed.has_value());
  ASSERT_EQ(parsed->players_size(), 1);

  const auto scene = botf::app::sceneFromWorldState(*parsed);
  ASSERT_EQ(scene.size(), 1u);
  EXPECT_EQ(scene[0].id, 7u);
  EXPECT_FLOAT_EQ(scene[0].position.x, 100.f);
  EXPECT_FLOAT_EQ(scene[0].position.y, 300.f);
  EXPECT_FLOAT_EQ(scene[0].angle, 1.25f);

  botf::app::Player localPlayer;
  localPlayer.setId(1);
  EXPECT_EQ(localPlayer.id(), 1u);

  EXPECT_EQ(parsed->tick(), 42u);
  EXPECT_FALSE(framed.empty());
}
