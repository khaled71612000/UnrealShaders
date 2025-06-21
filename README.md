![Presentation website](https://github.com/user-attachments/assets/d3669a3c-3c49-465f-abcb-fcd563b0f172)

# 🔆 Custom Shader Rendering in Unreal Engine
**Modular Global Shader Framework with Custom Passes, View Extensions, and Real-Time Effects**

![download](https://github.com/user-attachments/assets/a9168d64-82ca-45ae-93c1-36958c0ced3d)
---

## 🎯 Overview

This project explores a full custom rendering pipeline in Unreal Engine using C++, HLSL (`.usf`), and the global shader system.

Inspired by Froyok’s work on screen-space lens flares, this setup introduces a **modular way to inject your own render logic** into the engine — without overriding core systems.

You get control over:
- Custom post-process passes
- Custom vertex + pixel shaders
- Custom view extensions
- Scene capture and material injection

---

## 🧱 Components & Classes

### 📦 `MyCustomShader.usf`
The core shader logic — processes pixel color data, calculates glow, samples screen textures.

- Works like a mini post-process pass
- Supports view parameters and custom input

---

### 🛠 `MyCustomRenderingPass.cpp`
Defines a render graph pass with your custom pixel shader.  
Plugs into the post-processing chain using `AddPass()`.

---

### 🧠 `MyViewExtension.cpp`
Registers your shader with the view pipeline.  
Injects your rendering logic between UE's default passes.

---

### 🎮 `MyCharacter.cpp`
Actor used to test and trigger the rendering effect.  
Can toggle the shader live in-game.

---

### 🎮 `MyCustomRenderingModule.cpp`
Initializes shader classes, registers your rendering logic, and ensures module safety at startup/shutdown.

---

## 🔧 How It Works

1. Vertex/Pixel shaders (`MyCustomVS.cpp`, `MyCustomPS.cpp`) are compiled through the global shader system
2. A render graph pass is injected using `FRDGBuilder`
3. Your shader gets access to view data, screen color, and custom parameters
4. You can render overlays, effects, or post-process visuals

---

## 🧪 Use Cases

- 🔦 **Lens Flares** — draw soft bloom-style overlays from bright objects  
- 🧼 **Post-Processing Filters** — apply screen-space filters like blur, chromatic aberration  
- 🕶 **Stylized Rendering** — insert toon shading, outline passes, etc  
- 🧬 **Data Visualization** — debug visual buffers (normals, motion vectors, etc)

---

## 🚀 Getting Started

1. Clone this repo
2. Add `.usf` files to `/Shaders` folder in your Unreal project directory
3. Build the project in Visual Studio
4. In Unreal Editor:
   - Place `MyCharacter` into the scene
   - Press play and trigger the custom pass in runtime

✅ Works in packaged builds — no editor-only hacks.

---

## 📚 Credits & Resources

- 🎓 **Lens Flare Rendering in UE** by [Froyok](https://www.froyok.fr/blog/2021-09-ue4-custom-lens-flare/)
- 📘 Unreal Engine Shader Development Docs

![image](https://github.com/user-attachments/assets/54e59fbc-bce7-4369-8be7-58d9161348c3)
![image](https://github.com/user-attachments/assets/9b254bff-2229-4ec2-b425-2e2f5158a890)
