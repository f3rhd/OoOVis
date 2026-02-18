# OoOVisual: Out-of-Order CPU Architecture Visualizer

## 1. Overview

**OoOVisual** is an architectural-level visualization tool for an Out-of-Order (OoO) CPU core.

It models modern superscalar mechanisms such as:

* Reorder Buffer (ROB)
* Register Renaming
* Distributed Reservation Stations
* Out-of-Order Execution
* Branch Prediction & Recovery
* Load/Store Unit

The project’s goal is to make dynamic scheduling, speculative execution, and pipeline behavior observable in real time, providing clear insights into microarchitectural operation.

---

## 2. Features

### Architectural Simulation

* Tomasulo-style dynamic scheduling
* Physical register allocation and renaming
* In-order commit via Reorder Buffer
* Speculative execution with branch prediction
* Pipeline flush and recovery on misprediction
* Distributed reservation stations
* Execution units and load/store handling

### Visual Architecture Layout

* Units placed in 2D space, connected with animated, orthogonal wires
* Wires pulse when control or data signals propagate
* Pulse colors indicate event types:

| Color   | Context                                   | Meaning                                                                 |
|---------|-------------------------------------------|-------------------------------------------------------------------------|
| Green   | Any Execution Unit → ROB                  | Instruction committed successfully                                      |
| Green   | ROB → Register Manager                    | Operation completed; updates the register alias table                  |
| Green   | Fetch Unit → Fetch Group → Dispatcher     | Fetch successful; fetch group is on its way to be dispatched           |
| Red     | Dispatcher → Any Reservation Station      | Dispatch failed due to structural hazards                               |
| Orange  | Dispatcher                               | Dispatcher is actively dispatching instructions                         |
| Purple  | ROB → Register Manager                    | Flushing ROB entries and restoring the alias table due to branch misprediction |


### Interactive Inspection

* Hover over units to view current state
* Double left click → open detailed view
* Double right click → close detailed view

Clickable units include:

* Fetch Unit
* Reorder Buffer
* Register Manager
* Load/Store Execution Unit

### Camera Controls

* `W / A / S / D` → Move camera
* `+ / -` → Zoom in / Zoom out

---

## 3. GUI Demonstration


![OoOVisual Demo](assets/demo.gif)

---

## 4. Dependencies

OoOVisual is built using:

* C++20
* Dear ImGui
* GLFW
* OpenGL
* CMake

### Linux (Debian/Ubuntu)

```bash
sudo apt update
sudo apt install build-essential cmake libglfw3-dev libgl1-mesa-dev
```

### Windows (vcpkg recommended)

```bash
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
vcpkg install glfw3
```

### macOS (Homebrew)

```bash
brew install glfw
```

---

## 5. Building the Project

### Linux / macOS

```bash
git clone https://github.com/f3rhd/OoOVisual.git
cd OoOVisual
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### Windows (Visual Studio)

```bash
cmake -S . -B build -G "Visual Studio 18 2026" -A x64 -DCMAKE_TOOLCHAIN_FILE="C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake"
```

---

## 6. Usage

Run the visualizer:

```bash
./OoOVisual <input.s>
```

When launched:

* Architectural layout is displayed
* Wires pulse each simulation tick when signals propagate
* Hover over units to inspect state
* Double left click → open detailed window
* Double right click → close detailed window
* WASD to navigate, `+/-` to zoom

---

## 7. Repository Structure

```
OoOVisual/
├─ OoOVisual/Core/                    # OoO core implementation 
├─ OoOVisual/Frontend/                # Frontend needed for assembly parsing
├─ OoOVisual/Visualizer/              # ImGui rendering, interactions, camera control
├─ OoOVisual/tests/                   # RISC-V Assembly tests
├─ OoOVisual/vendor/                  # Third-party libraries (GLFW, ImGui)
├─ OoOVisual/main.cpp                 # Entry point
├─ CMakeLists.txt                     # Build configuration
├─ assets/                            # Assets
└─ README.md                          # This file
```

---

## 8. Purpose

OoOVisual is intended for:

* Studying Out-of-Order execution
* Understanding Tomasulo-style scheduling
* Visualizing speculative execution and recovery
* Teaching modern CPU microarchitecture concepts

---

## 9. License

This project is licensed under the MIT License.
