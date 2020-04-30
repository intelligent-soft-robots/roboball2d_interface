import time
import sys

import real_time_tools
import shared_memory

import o80
import roboball2d_interface 
from roboball2d.physics import WorldState

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

    # ensures "reality" loops at the desired frequency
    frequency_manager = real_time_tools.FrequencyManager(
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
    previous_ball_gun_id = -1
    previous_action_id = -1
    torques = [0,0,0]

    while run_support.should_run(switch):

        # -- Ball Gun -- #
        
        # ball gun requested to shoot. Doing so.
        ball_gun_action = sim_ball_gun.ball_gun_reader.read_action()
        if all([ball_gun_action.is_valid(),
                ball_gun_action.should_shoot(),
                ball_gun_action.id!=previous_ball_gun_id]):
            previous_ball_gun_id=ball_gun_action.id
            sim_robot.world.reset(None,sim_robot.ball_gun)

        # -- Torque Control Robot -- #
            
        # reading from shared memory the current
        # active action (action : roboball2d_interface.Action).
        # note: the action is written in the shared memory
        # by roboball2d_interface.Driver
        action = sim_robot.torques_reader.read_action()

        # the action is used if it is valid (i.e. not a dummy initial
        # object) and has torques defined
        should_use_action = previous_action_id!=action.id
        should_use_action = should_use_action and action.is_valid()
                                 
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

        # sim.world returns world_state as defined in the roboball2d
        # python package, but roboball2d_interfaces need an instance of
        # o80_roboball2d.roboball2d_interface.WorldState, which is similar but supports
        # serialization, i.e. can be written in shared_memory

        sm_world_state = run_support.convert(world_state)
        
        # sharing the observed world state with rest of the world
        sim_robot.torques_writer.write_world_state(sm_world_state)

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
