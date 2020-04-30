#pragma once

#include <mutex>

#include "robot_interfaces/robot_driver.hpp"
#include "roboball2d_interface/ball_gun_action.hpp"
#include "roboball2d_interface/writer.hpp"

namespace roboball2d_interface
{

    class VoidObservation
    {};

    class BallGunDriver
	: public robot_interfaces::RobotDriver<BallGunAction,
					       VoidObservation>
    {
    public:
	BallGunDriver(std::string interface_id);
	void initialize();
	BallGunAction apply_action(const BallGunAction &desired_action);
	VoidObservation get_latest_observation();
	void shutdown();
	std::string get_error();

    private:
	std::string interface_id_;
	Writer<BallGunAction> writer_;
	bool should_shoot_;

    private:
	static std::mutex mutex_;
    };

    std::mutex BallGunDriver::mutex_;

}
