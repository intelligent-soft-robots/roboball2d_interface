#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include "roboball2d_interface/ball_gun_action.hpp"
#include "roboball2d_interface/driver.hpp"
#include "roboball2d_interface/item.hpp"
#include "roboball2d_interface/mirror_action.hpp"
#include "roboball2d_interface/reader.hpp"
#include "roboball2d_interface/torques_action.hpp"
#include "roboball2d_interface/world_state.hpp"
#include "roboball2d_interface/writer.hpp"

using namespace roboball2d_interface;

template<int NB_ROBOTS,int NB_BALLS,int TYPE>
void add_world_state(pybind11::module &m,
		     std::string classname_prefix)
{
    pybind11::class_<roboball2d_interface::WorldState<NB_ROBOTS,NB_BALLS,TYPE>>(m,(classname_prefix+"WorldState").c_str())
        .def(pybind11::init<>())
        .def_readonly("id", &WorldState<NB_ROBOTS,NB_BALLS,TYPE>::id)
        .def_readonly("valid", &WorldState<NB_ROBOTS,NB_BALLS,TYPE>::valid)
        .def_readwrite("t", &WorldState<NB_ROBOTS,NB_BALLS,TYPE>::t)
        .def_readwrite("ball", &WorldState<NB_ROBOTS,NB_BALLS,TYPE>::ball)
        .def_readwrite("robot", &WorldState<NB_ROBOTS,NB_BALLS,TYPE>::robot)
	.def_readwrite("balls", &WorldState<NB_ROBOTS,NB_BALLS,TYPE>::balls)
        .def_readwrite("robots", &WorldState<NB_ROBOTS,NB_BALLS,TYPE>::robots)
        .def_readwrite("ball_hits_floor_position",
                       &WorldState<NB_ROBOTS,NB_BALLS,TYPE>::ball_hits_floor_position)
        .def_readwrite("ball_hits_floor", &WorldState<NB_ROBOTS,NB_BALLS,TYPE>::ball_hits_floor)
        .def_readwrite("ball_hits_racket", &WorldState<NB_ROBOTS,NB_BALLS,TYPE>::ball_hits_racket)
	.def_readwrite("balls_hits_floor_position",
                       &WorldState<NB_ROBOTS,NB_BALLS,TYPE>::balls_hits_floor_position)
        .def_readwrite("balls_hits_floor", &WorldState<NB_ROBOTS,NB_BALLS,TYPE>::balls_hits_floor)
        .def_readwrite("balls_hits_racket", &WorldState<NB_ROBOTS,NB_BALLS,TYPE>::balls_hits_racket)
        .def("console", &WorldState<NB_ROBOTS,NB_BALLS,TYPE>::console);

    
    pybind11::class_<roboball2d_interface::Reader<TorquesAction,NB_ROBOTS,NB_BALLS,TYPE>>(
										     m, (classname_prefix+"TorquesReader").c_str())
        .def(pybind11::init<std::string>())
        .def("read_action", &Reader<TorquesAction,NB_ROBOTS,NB_BALLS,TYPE>::read_action)
        .def("read_world_state", &Reader<TorquesAction,NB_ROBOTS,NB_BALLS,TYPE>::read_world_state);

    pybind11::class_<roboball2d_interface::Writer<TorquesAction,NB_ROBOTS,NB_BALLS,TYPE>>(
										     m, (classname_prefix+"TorquesWriter").c_str())
        .def(pybind11::init<std::string>())
        .def("write_world_state", &Writer<TorquesAction,NB_ROBOTS,NB_BALLS,TYPE>::write_world_state)
        .def("write_action", &Writer<TorquesAction,NB_ROBOTS,NB_BALLS,TYPE>::write_action);

    pybind11::class_<roboball2d_interface::Reader<BallGunAction,NB_ROBOTS,NB_BALLS,TYPE>>(
										     m, (classname_prefix+"BallGunReader").c_str())
        .def(pybind11::init<std::string>())
        .def("read_action", &Reader<BallGunAction,NB_ROBOTS,NB_BALLS,TYPE>::read_action)
        .def("read_world_state", &Reader<BallGunAction,NB_ROBOTS,NB_BALLS,TYPE>::read_world_state);

    pybind11::class_<roboball2d_interface::Writer<BallGunAction,NB_ROBOTS,NB_BALLS,TYPE>>(
										     m, (classname_prefix+"BallGunWriter").c_str())
        .def(pybind11::init<std::string>())
        .def("write_world_state", &Writer<BallGunAction,NB_ROBOTS,NB_BALLS,TYPE>::write_world_state)
        .def("write_action", &Writer<BallGunAction,NB_ROBOTS,NB_BALLS,TYPE>::write_action);

    pybind11::class_<roboball2d_interface::Reader<MirrorAction,NB_ROBOTS,NB_BALLS,TYPE>>(m,
										    (classname_prefix+"MirrorReader").c_str())
        .def(pybind11::init<std::string>())
        .def("read_action", &Reader<MirrorAction,NB_ROBOTS,NB_BALLS,TYPE>::read_action)
        .def("read_world_state", &Reader<MirrorAction,NB_ROBOTS,NB_BALLS,TYPE>::read_world_state);

    pybind11::class_<roboball2d_interface::Writer<MirrorAction,NB_ROBOTS,NB_BALLS,TYPE>>(m,
										    (classname_prefix+"MirrorWriter").c_str())
        .def(pybind11::init<std::string>())
        .def("write_world_state", &Writer<MirrorAction,NB_ROBOTS,NB_BALLS,TYPE>::write_world_state)
        .def("write_action", &Writer<MirrorAction,NB_ROBOTS,NB_BALLS,TYPE>::write_action);

    
}


PYBIND11_MODULE(roboball2d_interface, m)
{
    pybind11::class_<roboball2d_interface::Item>(m, "Item")
        .def(pybind11::init<>())
        .def_readwrite("position", &Item::position)
        .def_readwrite("angle", &Item::angle)
        .def_readwrite("linear_velocity", &Item::linear_velocity)
        .def_readwrite("angular_velocity", &Item::angular_velocity)
        .def_readwrite("torque", &Item::torque)
        .def_readwrite("desired_torque", &Item::desired_torque)
        .def("console", &Item::console);

    pybind11::class_<roboball2d_interface::Robot>(m, "Robot")
        .def(pybind11::init<>())
        .def_readwrite("joints", &Robot::joints)
        .def_readwrite("rods", &Robot::rods)
        .def_readwrite("racket", &Robot::racket)
        .def("console", &Robot::console);

    // world state 1 robot 1 ball, no class name prefix
    add_world_state<1,1>(m,"");

    // world state 1 robot 5 balls
    // classname prefix: FiveBalls (WorldState -> FiveBallsWorldState, etc)
    add_world_state<1,5>(m,
			 "FiveBalls");

    pybind11::class_<roboball2d_interface::TorquesAction>(m, "TorquesAction")
        .def(pybind11::init<>())
        .def_readonly("id", &TorquesAction::id)
	.def("is_valid",&TorquesAction::is_valid)
        .def("set_torques", &TorquesAction::set_torques)
        .def("get_torques", &TorquesAction::get_torques)
        .def("are_torques_relative", &TorquesAction::are_torques_relative);

    pybind11::class_<roboball2d_interface::BallGunAction>(m, "BallGunAction")
        .def(pybind11::init<bool>())
        .def_readonly("id", &BallGunAction::id)
	.def("is_valid",&BallGunAction::is_valid)
        .def("set_ball_gun", &BallGunAction::set_ball_gun)
        .def("should_shoot", &BallGunAction::should_shoot);

    pybind11::class_<roboball2d_interface::MirrorAction>(m, "MirrorAction")
        .def(pybind11::init<>())
        .def_readonly("id", &MirrorAction::id)
	.def("is_valid",&MirrorAction::is_valid)
        .def("set_mirroring_information", &MirrorAction::set_mirroring_information)
        .def("get_mirroring_information", &MirrorAction::get_mirroring_information);
}
