# Cyrus FPS Assignment

## Overview

This project implements a simple FPS-style box shooting system in **Unreal Engine 5 using C++**.
The game dynamically fetches box configuration data from a remote **JSON file**, spawns boxes in the level, and allows the player to destroy them by shooting. Each box type has its own **health, score value, and color**, which are defined in the JSON file.

The goal of the assignment is to demonstrate:

* Networking (HTTP requests)
* JSON parsing
* Data-driven gameplay
* Actor spawning
* Dynamic material usage
* Basic gameplay systems in C++

---

# Features Implemented

### JSON Data Fetching

The game downloads box configuration data from the following JSON file:

https://raw.githubusercontent.com/CyrusCHAU/Varadise-Technical-Test/refs/heads/main/data.json

The JSON contains:

* Box type definitions
* Health
* Score value
* Color
* Transform data for spawning

---

### JSON Parsing

The JSON is parsed in **CyrusFPSAssignmentGameMode** using Unreal's built-in JSON utilities.

Data from the JSON is stored in a structure:

```
FBoxTypeData
{
    Health
    Score
    Color
}
```

Each box type is stored in a `TMap` so it can be quickly accessed when spawning objects.

---

### Dynamic Box Spawning

After the JSON is parsed:

1. The system reads the **objects array** from the JSON.
2. Each object contains:

   * Type
   * Transform (location, rotation, scale)

Boxes are then spawned dynamically using:

```
GetWorld()->SpawnActor<ABoxActor>()
```

Each spawned box receives its configuration based on its type.

---

### Dynamic Material Colors

Each box uses a dynamic material instance.

The color defined in the JSON is applied at runtime using:

```
DynamicMaterial->SetVectorParameterValue("Color", Color)
```

This allows different box types to appear with different colors.

---

### Health System

Each box has a health value defined in the JSON.

When the player shoots a box:

* Health decreases
* When health reaches 0 the box is destroyed.

---

### Score System

Each box type has a score value defined in the JSON.

When a box is destroyed:

* The score value is added to the player's total score.
* The updated score is logged in the output log.

Example output:

```
Current Score: 50
Current Score: 70
Current Score: 170
```

---

### Player Shooting

The player can shoot boxes using the **left mouse button**.

A line trace detects whether the player hits a box actor.
If the trace hits a `BoxActor`, the `ApplyDamage()` function is triggered.

---

# Project Structure

Important C++ files:

```
Source/CyrusFPSAssignment/

BoxActor.h
BoxActor.cpp
    Handles box behavior:
    - Health
    - Damage
    - Dynamic materials

CyrusFPSAssignmentGameMode.h
CyrusFPSAssignmentGameMode.cpp
    Handles:
    - JSON fetching
    - JSON parsing
    - Box spawning
    - Score tracking
```

---

# Challenges Faced

### 1. JSON Parsing Structure

Initially, correctly mapping the JSON structure to Unreal data structures required careful parsing, especially for nested arrays like color and transform data.

### 2. Dynamic Materials

Applying colors dynamically required creating a **dynamic material instance** rather than modifying the base material.

### 3. Actor and Blueprint Integration

Ensuring the C++ actor components correctly matched the Blueprint setup was important to avoid duplicate meshes and material errors.

### 4. HTTP Request Handling

Handling asynchronous HTTP responses and safely parsing JSON data required careful validation of responses.

---

# How to Run / Test the Project

1. Clone the repository

```
git clone https://github.com/KarthikVeeranala/CyrusFPSAssignment.git
```

2. Open the project in **Unreal Engine 5**

3. Build the project (if needed) in **Visual Studio**

4. Press **Play**

5. The system will:

   * Download JSON data
   * Spawn boxes in the level
   * Apply colors and properties dynamically

6. Shoot boxes using **Left Mouse Button**

7. Watch the score update in the **Output Log**

---

# Technologies Used

* Unreal Engine 5
* C++
* Unreal HTTP Module
* Unreal JSON Utilities
* Unreal Gameplay Framework

---

# Notes

All gameplay logic was implemented using **native Unreal Engine C++ APIs** as required in the assignment instructions.
