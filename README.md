# Train-Planner FPGA Game

**Course:** Digital Logic & Computer Organization (COMP 541, UNC Chapel Hill)

**Technologies:** MIPS Assembly, SystemVerilog , FPGA (Nexys A7 Board), C (prototype)

**Demo:** Watch on YouTube

I designed and implemented a train track planning game on an FPGA circuit board (Nexys A7). The player’s goal is to connect a start point (S) and destination (D) by placing straight and curved track pieces. Once a valid path is built, a train animates along the track with sound and LED feedback. Movement on the 'game board' is controlled via the accelerometer built into the FPGA. 

The project integrates multiple hardware components — accelerometer, onboard buttons, LEDs, VGA display, and audio output. The program and software stack is written in MIPS assembly. All gameplay logic, path validation, and input/output handling run directly on the FPGA.

I implemented a full working CPU on the FPGA, using System Verilog and Vivado for program compilation. The CPU includes I/O integration with VGA, accelerometer, buttons, LEDs, and audio output, using traditional memory mapping strategies for incorporation.

For this final project, I made several custom additions to the provided procs.asm and template System Verilog files in order to incoporate the FGPA buttons. Specifically, I had to modify top.sv, memory_mapper.sv, and memIO.sv in order to incorpate the module. I additionally added a custom proc called "get_btns" to the procs.asm, to help me in my main assembly file.

## Features

### Path Validation Algorithm
- Implemented a Depth-First Search (DFS) in assembly to detect whether a complete and valid track connects the start and destination.

- Supports rules for vertical, horizontal, and curved pieces.

- Enforces placement constraints (e.g., curved pieces occupy 3 sprite cells and cannot overlap obstacles).
- total of around 1680 lines of MIPS assembly written for this program, much of which can be attributed to my implementation of DFS

### Custom Hardware Integration

- Accelerometer for smooth, analog-style cursor movement.
- Nexys Buttons (implemented and mapped into memory in System Verilog modules) for in-game controls. I was intially using output from the keyboard for these controls, but I later replaced this with the buttons as it made more realisitc sense in terms of making the game easy and fun to play. In my final demonstration, all game controls and movements can be made by simply holding the FPGA board.
- LEDs light up when a level is completed.
- Audio Output generates a train whistle sound during animation.
- Debouncing logic for reliable button input.

### Display & Interaction
- VGA monitor output for the game grid.
- Hover effects show placement previews.
- Smooth animations as the train moves across the track.

## Gameplay

**Objective:** Place track pieces between the start and destination so the train can complete the route.

**Controls:**

- Accelerometer: Tilt to move the cursor across the grid.

- Board Buttons:

  - BTNL: Place track at cursor.
  - BTNR: Rotate currently selected track.
  - BTNU: Switch 'currently selected track' to straight track.
  - BTND: Switch 'currently selected track' to curved track.

**Feedback:**
- Successful path → LEDs light up in sequence, train whistle sound plays.

- Invalid placements are rejected (e.g., overlap conflicts, wrong orientation).

