#include <limits>

#include "real_time_tools/timer.hpp"

#include "robot_interfaces/robot.hpp"

#include "o80_tennis2d/tennis2d_interface/driver.hpp"
#include "o80_tennis2d/tennis2d_interface/action.hpp"
#include "o80_tennis2d/tennis2d_interface/world_state.hpp"

using namespace o80_tennis2d;
using namespace o80_tennis2d::tennis2d_interface;

int main()
{
  Writer writer("A");
  Reader reader("A");

  WorldState ws;
  ws.ball.position[0]=1.0;
  ws.ball.position[1]=1.0;

  WorldState ws1 = reader.read_world_state();

  writer.write_world_state(ws);

  WorldState ws2 = reader.read_world_state();

}
