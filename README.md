# Train-Planner FPGA Game

**Course:** Digital Logic & Computer Organization (COMP 541, UNC Chapel Hill)

**Technologies:** MIPS Assembly, SystemVerilog , FPGA (Nexys A7 Board), C (prototype)

**Demo:**  [Watch on YouTube!](https://www.youtube.com/watch?v=HUmOf4YsyzI&ab_channel=NaviKumar)

I designed and implemented a train track planning game on an FPGA circuit board (Nexys A7). The player’s goal is to connect a start point (S) and destination (D) by placing straight and curved track pieces. Once a valid path is built, a train animates along the track with sound and LED feedback. Movement on the 'game board' is controlled via the accelerometer built into the FPGA. 

The game integrates multiple hardware components, discussed below. The program and software stack is written in MIPS assembly. All gameplay logic, path validation, and input/output handling run directly on the FPGA, with a full woring CPU that I implemented using System Verilog (and Vivado for program compilation). The CPU design includes I/O integration with VGA, accelerometer, buttons, LEDs, and audio output, using traditional memory mapping strategies for incorporation.

For this final project, I made several custom additions to the provided procs.asm and template System Verilog files in order to incoporate the FGPA buttons. Specifically, I had to modify top.sv, memory_mapper.sv, and memIO.sv in order to incorpate the module. I additionally added a custom proc called "get_btns" to the procs.asm, to help me in my main assembly file.

**My source code (.asm, .sv, .c files, etc.) is available for viewing upon request. Please reach out to me at avikumar2048@gmail.com if you are interested.**

## Features

#### Path Validation Algorithm
- Implemented a Depth-First Search (DFS) in assembly to detect whether a complete and valid track connects the start and destination.

- Supports rules for vertical, horizontal, and curved pieces.

- Enforces placement constraints (e.g., curved pieces occupy 3 sprite cells and cannot overlap obstacles).
- total of around 1680 lines of MIPS assembly written for this program, much of which can be attributed to my implementation of DFS

#### Custom Hardware Integration

- Accelerometer for smooth, analog-style cursor movement.
- Nexys Buttons (implemented and mapped into memory in System Verilog modules) for in-game controls. I was intially using output from the keyboard for these controls, but I later replaced this with the buttons as it made more realisitc sense in terms of making the game easy and fun to play. In my final demonstration, all game controls and movements can be made by simply holding the FPGA board.
- LEDs light up when a level is completed.
- Audio Output generates a train whistle sound during animation.
- Debouncing logic for reliable button input.

#### Display & Interaction
- VGA monitor output for the game grid.
- Hover effects show placement previews.
- Smooth animations as the train moves across the track.

## Technical Implementation
#### System Verilog Hardware Modules
below is a diagram of the module structure of my implementation, where ```top.sv``` is my top-level module. All ```.sv``` files provided by the professor are indicated. All files marked as "custom" were written by me, or are modifications made to templates provided by the professor.
```

top.sv  (board-level integration, clocking, IO domains)
├─ clockdivider_Nexys4         // 100 MHz → 12.5 MHz (clk12) + gated 100 MHz (clk100)
├─ keyboard                    // PS/2 → keyb_char   [prof-provided]
├─ accelerometer               // SPI → accelX/Y/tmp [prof-provided]
│   └─ packs accelX/accelY into accel_val[31:0]
├─ mips                        // single-cycle MIPS core (controller + datapath) [custom]
│   ├─ controller              // op/func decode → control signals [custom]
│   └─ datapath                // regfile, ALU, PC logic, imm ext, branch/jump [custom]
│       ├─ register_file       // Nreg-param reg file (rd/wr ports) [custom]
│       └─ ALU                 // arithmetic/logic + Z flag [custom]
├─ imem : rom_module           // instruction ROM (init file via parameter) [prof-provided]
├─ memIO                       // memory-mapped I/O crossbar + RAMs [custom]
│   ├─ memory_mapper           // address decode (read/write enables + read mux) [custom]
│   ├─ screenmem : ram2port_module   // dual-port screen memory (CPU + VGA) [prof-provided]
│   └─ datamem   : ram_module        // data memory for .data + stack [custom]
├─ vgadisplaydriver            // VGA timing + tile renderer [custom]
│   ├─ vgatimer                // x/y pixel counters, sync, activevideo [custom]
│   └─ bitmapmem : rom_module  // character bitmaps; addr = {charcode,yoff,xoff} [custom]
├─ sound : montek_sound_generator     // PWM audio; driven by period register [prof-provided]
├─ display8digit               // 8-digit 7-segment scanner (for debug) [custom]
└─ (wires)
   ├─ charcode <— memIO (VGA reads screenmem)
   ├─ smem_addr —> memIO (VGA requests tile at row/col)
   └─ LED, audPWM/audEn, hsync/vsync, RGB

```
- top.sv: Top-level integration of peripherals. Namely, ```mips.sv```, ```rom_module.sv```, ```memIO.sv```,```vgadisplaydriver.sv```,```display8digit.sv```, and all I/O module files provided by the professor.
- memory_mapper.sv: Defines memory-mapped I/O space.
- memIO.sv: Handles communication between CPU and devices.
- debouncer.sv: Ensures stable button input.

#### Software

- ```main.asm```: Main game loop; Manages cursor updates, button inputs, track placement, DFS path search, and train animation. ~1700 lines
- ```procs_board.asm```: Procedures and helper functions for handling I/O in ```main.asm```.

  - get_btns → button inputs. Custom addition.


#### Development Flow

- Initial prototype in C to sketch out game logic.

- Manual translation into MIPS assembly for FPGA execution.

- Hardware bring-up in Vivado with custom SystemVerilog modules.

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


## Challenges & What I learned
- Debugging across environments: Some bugs appeared only in the Java emulator, others only in Mars MMIO, and still others only on the FPGA — requiring patient testing at each level.
- Assembly scale: Final program exceeded 1,600 assembly instructions, reinforcing lessons in optimization and structured low-level programming. I ended up writing a lot more assembly than the suggested range, which was between 500-1,000. But I thoroughly enjoyed this experience and found the process of translating C into MIPS to be quite satisfying.
- Full-stack hardware/software experience: Implementing buttons from Verilog all the way through to gameplay gave me deep insight into how software and hardware layers interact.
