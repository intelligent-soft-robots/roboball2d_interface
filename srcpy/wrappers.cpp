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

    pybind11::class_<roboball2d_interface::WorldState>(m, "WorldState")
        .def(pybind11::init<>())
        .def_readonly("id", &WorldState::id)
        .def_readonly("valid", &WorldState::valid)
        .def_readwrite("t", &WorldState::t)
        .def_readwrite("ball", &WorldState::ball)
        .def_readwrite("robot", &WorldState::robot)
        .def_readwrite("ball_hits_floor_position",
                       &WorldState::ball_hits_floor_position)
        .def_readwrite("ball_hits_floor", &WorldState::ball_hits_floor)
        .def_readwrite("ball_hits_racket", &WorldState::ball_hits_racket)
        .def("console", &WorldState::console);

    pybind11::class_<roboball2d_interface::Reader<TorquesAction>>(
        m, "TorquesReader")
        .def(pybind11::init<std::string>())
        .def("read_action", &Reader<TorquesAction>::read_action)
        .def("read_world_state", &Reader<TorquesAction>::read_world_state);

    pybind11::class_<roboball2d_interface::Writer<TorquesAction>>(
        m, "TorquesWriter")
        .def(pybind11::init<std::string>())
        .def("write_world_state", &Writer<TorquesAction>::write_world_state)
        .def("write_action", &Writer<TorquesAction>::write_action);

    pybind11::class_<roboball2d_interface::Reader<BallGunAction>>(
        m, "BallGunReader")
        .def(pybind11::init<std::string>())
        .def("read_action", &Reader<BallGunAction>::read_action)
        .def("read_world_state", &Reader<BallGunAction>::read_world_state);

    pybind11::class_<roboball2d_interface::Writer<BallGunAction>>(
        m, "BallGunWriter")
        .def(pybind11::init<std::string>())
        .def("write_world_state", &Writer<BallGunAction>::write_world_state)
        .def("write_action", &Writer<BallGunAction>::write_action);

    pybind11::class_<roboball2d_interface::Reader<MirrorAction>>(m,
                                                                 "MirrorReader")
        .def(pybind11::init<std::string>())
        .def("read_action", &Reader<MirrorAction>::read_action)
        .def("read_world_state", &Reader<MirrorAction>::read_world_state);

    pybind11::class_<roboball2d_interface::Writer<MirrorAction>>(m,
                                                                 "MirrorWriter")
        .def(pybind11::init<std::string>())
        .def("write_world_state", &Writer<MirrorAction>::write_world_state)
        .def("write_action", &Writer<MirrorAction>::write_action);

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
