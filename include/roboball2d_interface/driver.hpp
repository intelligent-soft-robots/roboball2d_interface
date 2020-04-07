#pragma once

#include <mutex>

#include "robot_interfaces/robot_driver.hpp"

#include "roboball2d_interface/ball_gun_action.hpp"
#include "roboball2d_interface/mirror_action.hpp"
#include "roboball2d_interface/reader.hpp"
#include "roboball2d_interface/torques_action.hpp"
#include "roboball2d_interface/world_state.hpp"
#include "roboball2d_interface/writer.hpp"

namespace roboball2d_interface
{
    template <class Action,int NB_ROBOTS,int NB_BALLS,int TYPE>
    class Driver : public robot_interfaces::RobotDriver< Action,
							 WorldState<NB_ROBOTS,NB_BALLS,TYPE > >

{
public:
    Driver(std::string interface_id);
    void initialize();
    Action apply_action(const Action &desired_action);
    WorldState<NB_ROBOTS,NB_BALLS,TYPE> get_latest_observation();
    void shutdown();
    std::string get_error();

private:
    std::string interface_id_;
    Writer<Action,NB_ROBOTS,NB_BALLS,TYPE> writer_;
    Reader<Action,NB_ROBOTS,NB_BALLS,TYPE> reader_;
    bool should_shoot_;

private:
    static std::mutex mutex_;
};

    template <class Action,int NB_ROBOTS,int NB_BALLS,int TYPE>
    std::mutex Driver<Action,NB_ROBOTS,NB_BALLS,TYPE>::mutex_;

#include "driver.hxx"
}
