.. raw:: html

   <div align="center" style="margin-bottom: 2em;">
     <div style="display: flex; align-items: center; justify-content: center; gap: 1.5em; margin-bottom: 0.8em;">
       <img src="_static/tesollo_logo.png" alt="Tesollo" style="height: 80px;" />
       <img class="title-banner" src="_static/ROS2_Color.svg" alt="ROS 2" style="height: 60px;" />
     </div>
     <p style="font-size: 1.2em; color: #666; margin-top: 0.5em;">
       Delto Gripper ROS 2 Packages for Tesollo's Robotic Grippers
     </p>
     <p>
       <a href="https://github.com/tesollodelto/tesollo_ros2">
         <img src="https://img.shields.io/badge/GitHub-tesollo__ros2-181717?logo=github" alt="GitHub" />
       </a>
       <img src="https://img.shields.io/badge/ROS_2-Humble-blue?logo=ros" alt="ROS 2 Humble" />
       <img src="https://img.shields.io/badge/Ubuntu-22.04-E95420?logo=ubuntu&logoColor=white" alt="Ubuntu 22.04" />
       <img src="https://img.shields.io/badge/License-BSD--3--Clause-green" alt="License" />
     </p>
   </div>

Product Lineup
--------------

.. raw:: html

   <div style="display: flex; flex-wrap: wrap; justify-content: center; gap: 1.5em; margin: 2em 0;">

     <div style="text-align: center; width: 180px;">
       <a href="grippers/dg5f.html">
         <img src="_static/DG-5F-M.png" alt="DG-5F" style="width: 160px; height: 160px; object-fit: contain; border: 1px solid #ddd; border-radius: 12px; padding: 8px;" />
       </a>
       <p style="margin-top: 0.5em; font-weight: bold;">DG-5F</p>
       <p style="font-size: 0.85em; color: #888;">5-Finger Hand</p>
     </div>

     <div style="text-align: center; width: 180px;">
       <a href="grippers/dg5f.html">
         <img src="_static/DG-5F-S.webp" alt="DG-5F-S" style="width: 160px; height: 160px; object-fit: contain; border: 1px solid #ddd; border-radius: 12px; padding: 8px;" />
       </a>
       <p style="margin-top: 0.5em; font-weight: bold;">DG-5F-S</p>
       <p style="font-size: 0.85em; color: #888;">5-Finger Short</p>
     </div>

     <div style="text-align: center; width: 180px;">
       <a href="grippers/dg4f.html">
         <img src="_static/DG-4F.webp" alt="DG-4F" style="width: 160px; height: 160px; object-fit: contain; border: 1px solid #ddd; border-radius: 12px; padding: 8px;" />
       </a>
       <p style="margin-top: 0.5em; font-weight: bold;">DG-4F</p>
       <p style="font-size: 0.85em; color: #888;">4-Finger Gripper</p>
     </div>

     <div style="text-align: center; width: 180px;">
       <a href="grippers/dg3f_m.html">
         <img src="_static/DG-3F-M.webp" alt="DG-3F-M" style="width: 160px; height: 160px; object-fit: contain; border: 1px solid #ddd; border-radius: 12px; padding: 8px;" />
       </a>
       <p style="margin-top: 0.5em; font-weight: bold;">DG-3F-M</p>
       <p style="font-size: 0.85em; color: #888;">3-Finger w/ F/T Sensor</p>
     </div>

     <div style="text-align: center; width: 180px;">
       <img src="_static/DG2F.webp" alt="DG-2F" style="width: 160px; height: 160px; object-fit: contain; border: 1px solid #ddd; border-radius: 12px; padding: 8px; opacity: 0.5;" />
       <p style="margin-top: 0.5em; font-weight: bold; color: #aaa;">DG-2F</p>
       <p style="font-size: 0.85em; color: #aaa;">Coming Soon</p>
     </div>

     <div style="text-align: center; width: 180px;">
       <img src="_static/DG-1F.webp" alt="DG-1F" style="width: 160px; height: 160px; object-fit: contain; border: 1px solid #ddd; border-radius: 12px; padding: 8px; opacity: 0.5;" />
       <p style="margin-top: 0.5em; font-weight: bold; color: #aaa;">DG-1F</p>
       <p style="font-size: 0.85em; color: #aaa;">Coming Soon</p>
     </div>

   </div>

Quick Start
-----------

.. code-block:: bash

   # Clone (example: DG-5F left hand)
   git clone https://github.com/tesollodelto/tesollo_ros2.git
   cd tesollo_ros2
   git submodule update --init dg5f_ros2 dg_sdk_ros2_bridge dg_common/dg_tcp_comm dg_common/dg_hardware

   # Build
   cd ~/ros2_ws && colcon build --symlink-install && source install/setup.bash

   # Launch
   ros2 launch dg5f_driver dg5f_left_driver.launch.py

.. raw:: html

   <div style="display: flex; flex-wrap: wrap; justify-content: center; gap: 1.5em; margin: 2em 0;">
     <a href="getting_started/installation.html" style="text-decoration: none;">
       <div style="border: 1px solid #ddd; border-radius: 12px; padding: 1.2em 2em; width: 200px; text-align: center;">
         <div style="font-size: 2em;">📦</div>
         <div style="font-weight: bold; margin-top: 0.3em;">Installation</div>
         <div style="font-size: 0.85em; color: #888;">Setup guide</div>
       </div>
     </a>
     <a href="getting_started/quickstart.html" style="text-decoration: none;">
       <div style="border: 1px solid #ddd; border-radius: 12px; padding: 1.2em 2em; width: 200px; text-align: center;">
         <div style="font-size: 2em;">🚀</div>
         <div style="font-weight: bold; margin-top: 0.3em;">Quick Start</div>
         <div style="font-size: 0.85em; color: #888;">Get running fast</div>
       </div>
     </a>
     <a href="architecture/overview.html" style="text-decoration: none;">
       <div style="border: 1px solid #ddd; border-radius: 12px; padding: 1.2em 2em; width: 200px; text-align: center;">
         <div style="font-size: 2em;">🏗️</div>
         <div style="font-weight: bold; margin-top: 0.3em;">Architecture</div>
         <div style="font-size: 0.85em; color: #888;">How it works</div>
       </div>
     </a>
     <a href="reference/services.html" style="text-decoration: none;">
       <div style="border: 1px solid #ddd; border-radius: 12px; padding: 1.2em 2em; width: 200px; text-align: center;">
         <div style="font-size: 2em;">📖</div>
         <div style="font-weight: bold; margin-top: 0.3em;">API Reference</div>
         <div style="font-size: 0.85em; color: #888;">Messages & Services</div>
       </div>
     </a>
   </div>

.. toctree::
   :maxdepth: 2
   :caption: Getting Started
   :hidden:

   getting_started/installation
   getting_started/quickstart

.. toctree::
   :maxdepth: 2
   :caption: Gripper Models
   :hidden:

   grippers/dg3f_m
   grippers/dg4f
   grippers/dg5f
   grippers/dg3f_b

.. toctree::
   :maxdepth: 2
   :caption: Packages
   :hidden:

   packages/delto_hardware
   packages/dg_tcp_comm
   packages/dg_sdk_ros2_bridge

.. toctree::
   :maxdepth: 2
   :caption: Architecture
   :hidden:

   architecture/overview
   architecture/hardware_interface
   architecture/communication

.. toctree::
   :maxdepth: 1
   :caption: Reference
   :hidden:

   reference/messages
   reference/services
