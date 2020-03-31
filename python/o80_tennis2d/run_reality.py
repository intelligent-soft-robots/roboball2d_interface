import time
import sys

import real_time_tools_py
import shared_memory_py

import o80_py as o80
import o80_tennis2d_py as driver_interface 
from tennis2d.physics import WorldState

import run_support

import configuration

# reads from the memory torques to apply
# and moves the robot, only
# blind to anything else (including the ball)
def run_reality(configuration,
                switch,
                interface_id_robot,
                interface_id_ball_gun,
                render=True):

    
    frequency_manager = real_time_tools_py.FrequencyManager(
        configuration.Interfaces.reality_frequency)
    
    # note: rendering real robot but omitting ball (will be displayed
    #       by vision system) -> render_ball=False
    sim_robot = run_support.Simulation(interface_id_robot,
                                       ball=True,
                                       robot=True,
                                       ball_gun=True,
                                       render=render,
                                       render_ball=False,
                                       width=configuration.Window.width,
                                       height=configuration.Window.height)

    sim_ball_gun = run_support.Simulation(interface_id_ball_gun,
                                          ball=False,
                                          robot=False,
                                          ball_gun=False,
                                          render=False)

    time_start = time.time()
    previous_action_id = -1
    torques = [0,0,0]

    while run_support.should_run(switch):

        # -- Ball Gun -- #
        
        # ball gun requested to shoot. Doing so.
        if sim_ball_gun.reader.read_ball_gun_shoot():
            # (sim_robot is managing the simulation)
            sim_robot.world.reset(None,sim_robot.ball_gun)
            sim_ball_gun.writer.write_ball_gun_shoot(False)

        # -- Torque Control Robot -- #
            
        # reading from shared memory the current
        # active action (action : tennis2d_interface.Action).
        # note: the action is written in the shared memory
        # by tennis2d_interface.Driver
        action = sim_robot.reader.read_action()

        # the action is used if it is valid (i.e. not a dummy initial
        # object) and has torques defined
        should_use_action = all([a is True for a in (action.valid,
                                                     previous_action_id!=action.id,
                                                     action.are_torques_set())])
        if should_use_action:
            torques = action.get_torques()

        # note : the above implies : for as long there a no new action provided,
        #        the robot keeps applying torques of the last action
            
        # dynamics of the robot based on the
        # users input torques
        # (setting the current time : having the robot running at 'reality' time
        world_state = sim_robot.world.step(action.get_torques(),
                                           relative_torques=action.are_torques_relative(),
                                           current_time=time.time()-time_start)

        previous_action_id = action.id

        # sim.world returns world_state as defined in the tennis2d
        # python package, but driver_interfaces need an instance of
        # o80_tennis2d.tennis2d_interface.WorldState, which is similar but supports
        # serialization, i.e. can be written in shared_memory

        sm_world_state = run_support.convert(world_state)
        
        # sharing the observed world state with rest of the world
        sim_robot.writer.write_world_state(sm_world_state)

        # rendering robot
        if sim_robot.renderer:
            world_state.ball = None
            sim_robot.renderer.render(world_state,[],time_step=1.0/30.0,wait=False)

        frequency_manager.wait()



        
if __name__ == '__main__':

    function = run_reality
    switch = configuration.Interfaces.switch_reality
    interface_ids = [configuration.Interfaces.real_robot,
                     configuration.Interfaces.real_ball_gun]

    run_support.execute(configuration,
                        function,
                        switch,
                        interface_ids,
                        sys.argv[1:])
