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


  // max time allowed for the robot to apply an action.
  double max_action_duration_s = std::numeric_limits<double>::infinity();
  // max time allowed for 2 successive actions
  double max_inter_action_duration_s = std::numeric_limits<double>::infinity();

  // should be the same value as the one set in /python/o80_tennis2d/configuration.py
  std::string interface_id_real_robot = "real-robot";
  std::string interface_id_ball_gun = "real-ball-gun";
  
  typedef robot_interfaces::Robot<Action,WorldState,Driver> Robot;
  Robot robot(max_action_duration_s,
	      max_inter_action_duration_s,
	      interface_id_real_robot);
  
  // the ball-gun uses same driver and action as the robot. Trick is that
  // an action can be set as a torque action that the robot will understand, or a ball-gun
  // action to that a ball-gun will understand
  Robot ball_gun(max_action_duration_s,
                 max_inter_action_duration_s,
		 interface_id_ball_gun);
  
  robot.initialize();
  ball_gun.initialize();
    
  double start_time = real_time_tools::Timer::get_current_time_sec();
  double current_time = start_time;

  // we will shoot a ball every 3 secs
  double ball_gun_time = start_time + 3.0;

  // action asking the ball gun to trigger
  Action shoot;
  Action stop_shoot;
  shoot.set_ball_gun(true);
  stop_shoot.set_ball_gun(false);

  // shooting a first time
  ball_gun.append_desired_action(shoot);
  // we shoot only once
  ball_gun.append_desired_action(stop_shoot);
    
  
  while(current_time-start_time<10.0)
    {

      if(current_time > ball_gun_time)
	{
	  ball_gun.append_desired_action(shoot);
	  ball_gun.append_desired_action(stop_shoot);
	  ball_gun_time = current_time+3.0;
	}
      
      current_time = real_time_tools::Timer::get_current_time_sec();
      
      double torque1 = 0.5*cos(current_time);
      double torque2 = 0.5*sin(current_time);
      double torque3 = torque1;

      bool relative_torques = true;
      
      Action action;
      action.set_torques(torque1,torque2,torque3,
			 relative_torques);

      robot_interfaces::TimeIndex index = robot.append_desired_action(action);
      WorldState world_state = robot.get_observation(index);

      std::cout << current_time-start_time << "\t|\t"
		<< world_state.robot.joints[0].angle << "\t"
		<< world_state.robot.joints[1].angle << "\t"
		<< world_state.robot.joints[2].angle << "\n";

      real_time_tools::Timer::sleep_ms(10);
      
    }
  
	      
}
