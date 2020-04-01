import multiprocessing
import time
import sys

import shared_memory
import roboball2d_interface

from roboball2d.physics import B2World
from roboball2d.physics import WorldState
from roboball2d.rendering import PygletRenderer
from roboball2d.rendering import RenderingConfig
from roboball2d.robot import DefaultRobotConfig
from roboball2d.robot import DefaultRobotState
from roboball2d.ball import BallConfig
from roboball2d.ball_gun import DefaultBallGun
from roboball2d.robot import DefaultRobotState

def _item_to_item(item1,item2):
    if item1 is None:
        return
    for attr in ("position","angle",
                 "linear_velocity",
                 "angular_velocity",
                 "torque","desired_torque"):
        v = getattr(item1,attr)
        if v is not None:
            setattr(item2,attr,v)

def _array_to_array(a1,a2):
    list(map(_item_to_item,a1,a2))

def _robot_to_robot(r1,r2):
    if r1 is None:
        return
    if r2 is None:
        return
    for attr in ("joints","rods"):
        _array_to_array(getattr(r1,attr),
                        getattr(r2,attr))
    _item_to_item(r1.racket,r2.racket)
    
def _convert(w1,w2):
    _robot_to_robot(w1.robot,w2.robot)
    _item_to_item(w1.ball,w2.ball)
    if(hasattr(w1,"ball_hits_floor_position")):
        if w1.ball_hits_floor:
            w2.ball_hits_floor = w1.ball_hits_floor_position
        else :
            w2_ball_hits_floor = False
    else :
        if w1.ball_hits_floor:
            w2.ball_hits_floor = True
            w2.ball_hits_floor_position = w1.ball_hits_floor
        else:
            w2.ball_hits_floor = False
    if w1.t is not None:
        w2.t = w1.t

def convert(world_state):
    sm_world_state = roboball2d_interface.WorldState()
    _convert(world_state,sm_world_state)
    return sm_world_state

def reverse(sm_world_state,ball_config):
    world_state = WorldState([],[ball_config])
    _convert(sm_world_state,world_state)
    return world_state

def should_run(switch):
    value = shared_memory.get_bool("switch",switch)
    return value

def clean_memory(switch, interface_ids):
    shared_memory.clear_shared_memory(switch)
    for interface_id in interface_ids:
        shared_memory.clear_shared_memory(interface_id)
        
# expect a configuration.txt file in the same folder as this run_support.py
# file
def read_config():

    class _Config:
        def __init__(self): pass

    config = _Config()
        
    full_path = os.path.realpath(__file__)
    dir_path = os.path.dirname(full_path)
    config_path = dir_path+os.sep+"configuration.txt"
    if not os.path.isfile(config_path):
        raise Exception("failed to find",config_path)
    with open(config_path,"r") as f:
        for line in f.readline():
            line = line.strip()
            if not line.startswith('#'):
                try:
                    key,value = line.split("\t")
                except :
                    raise Exception("failed to read config line:",line)
                try :
                    value = float(value)
                except :
                    raise Exception("failed to evaluate config line:",line)
                setattr(config,key,value)

    setattr(config,"path",config_path)
            
    return config
    

def get_configuration_attribute(configuration,attr):
    try :
        return getattr(configuration,attr)
    except:
        raise Exception("failed to read "+attr+" from "+
                        configuration.path)
    

class Simulation:

    __slots__=[ "robot_config","ball_config",
                "world","renderer","robot_init",
                "ball_gun",
                "torques_reader","torques_writer",
                "mirror_reader","mirror_writer",
                "ball_gun_reader","ball_gun_writer" ]
    
    def __init__(self,interface_id,
                 ball=True,
                 robot=True,
                 ball_gun=False,
                 render=True,
                 render_ball=True,
                 width=None,
                 height=None,
                 location=[0,0]):
        
        for slot in self.__slots__:
            setattr(self,slot,None)

        if robot:
            self.robot_config = DefaultRobotConfig()

        if ball:
            if ball==True:
                self.ball_config = BallConfig()
            else :
                self.ball_config = [BallConfig() for _
                                   in range(ball)]

        if ball_gun:
            if ball==True:
                self.ball_gun = DefaultBallGun(self.ball_config)
            else :
                self.ball_gun = [DefaultBallGun(config)
                                for config in self.ball_config]

        visible_area_width = 6.0
        visual_height = 0.05

        if robot or ball:
            self.world = B2World(self.robot_config,
                                 self.ball_config,
                                 visible_area_width)

        # graphics renderer
        if render:
            renderer_config = RenderingConfig(visible_area_width,
                                              visual_height)
            if width is not None:
                renderer_config.window.width=width
            if height is not None:
                renderer_config.window.height=height
            if location is not None:
                renderer_config.window.location = location
            if render_ball:
                self.renderer = PygletRenderer(renderer_config,
                                               self.robot_config,
                                               self.ball_config)
            else :
                self.renderer = PygletRenderer(renderer_config,
                                               self.robot_config,
                                               [])
        if robot:
            self.robot_init = DefaultRobotState(self.robot_config)

        self.torques_reader = roboball2d_interface.TorquesReader(interface_id)
        self.torques_writer = roboball2d_interface.TorquesWriter(interface_id)

        self.mirror_reader = roboball2d_interface.MirrorReader(interface_id)
        self.mirror_writer = roboball2d_interface.MirrorWriter(interface_id)

        self.ball_gun_reader = roboball2d_interface.BallGunReader(interface_id)
        self.ball_gun_writer = roboball2d_interface.BallGunWriter(interface_id)
        

def _stop(switch):
    shared_memory.set_bool("switch",switch,False)

def execute(configuration,
            function,
            switch,
            interface_ids,
            args):

    rendering = True
    if "-no-render" in args:
        rendering = False
    stop = False
    if "-stop" in args:
        stop=True

    if stop :
        _stop(switch)
        return
    
    print()
    print("switch:",switch," | interfaces:",*interface_ids)
    print()

    shared_memory.set_bool("switch",switch,True)
    function(configuration,switch,*interface_ids,render=rendering)
    
    clean_memory(switch,interface_ids)
