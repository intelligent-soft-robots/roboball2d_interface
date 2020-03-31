import multiprocessing
import time
import sys

import real_time_tools_py
import shared_memory_py

import tennis2d

import o80_py as o80
import o80_tennis2d_py as driver_interface 

from tennis2d.physics import B2World
from tennis2d.robot import DefaultRobotState

import configuration
import run_support


def run_simulation(config,
                   switch,
                   interface_id_robot,
                   interface_id_ball_gun,
                   render=True):


    nb_balls = config.Balls.nb_balls
    goal = (config.Goal.x_min,config.Goal.x_max,config.Goal.color)
    
    sim_robot = run_support.Simulation(interface_id_robot,
                                       ball=nb_balls,
                                       robot=True,
                                       ball_gun=True,
                                       render=render,
                                       width=config.Window.width,
                                       height=config.Window.height)

    sim_ball_gun = run_support.Simulation(interface_id_ball_gun,
                                          ball=False,
                                          robot=False,
                                          ball_gun=False,
                                          render=False)

    time_start = time.time()
    previous_action_id = -1
    torques = [0,0,0]
    robot_state = None

    frequency_manager = real_time_tools_py.FrequencyManager(
        configuration.Interfaces.reality_frequency)

    while run_support.should_run(switch):

        # if ball gun requested to shoot, doing so.
        if sim_ball_gun.reader.read_ball_gun_shoot():
            # (sim_robot is managing the simulation)
            world_state = sim_robot.world.reset(None,sim_robot.ball_gun)
            sim_ball_gun.writer.write_ball_gun_shoot(False)

        # mirroring the robot based on action provided by the driver
        action = sim_robot.reader.read_action()
        should_use_action = all([a is True for a in (action.valid,
                                                     previous_action_id!=action.id,
                                                     action.is_mirroring_information_set())])

        if should_use_action:

            # mi = [ [angle,angular_velocity], ... ]
            mi = action.get_mirroring_information()
            angles = [m[0] for m in mi]
            angular_velocities = [m[1] for m in mi]
            robot_state = tennis2d.robot.DefaultRobotState(
                sim_robot.robot_config,
                generalized_coordinates=angles,
                generalized_velocities=angular_velocities)
            previous_action_id = action.id
            
            # running the physics (mirring robot + virtual balls dynamics)
            world_state = sim_robot.world.step(None,
                                               mirroring_robot_states=robot_state,
                                               current_time=time.time()-time_start)
            
            # converting world state to something the driver will
            # be able to use
            sm_world_state = run_support.convert(world_state)
            sim_robot.writer.write_world_state(sm_world_state)
        
            # rendering
            if sim_robot.renderer:
                sim_robot.renderer.render(world_state,
                                          [goal],
                                          time_step=1.0/30.0,
                                          wait=False)

        frequency_manager.wait()


        
if __name__ == '__main__':

    function = run_simulation
    switch = configuration.Interfaces.switch_simulation
    interface_ids = [configuration.Interfaces.sim_robot,
                     configuration.Interfaces.sim_ball_guns]

    run_support.execute(configuration,
                        function,
                        switch,
                        interface_ids,
                        sys.argv[1:])
