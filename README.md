# FlyEngine

FlyEngine is a lightweight C++ graphics engine designed for 3D rendering applications, offering foundational features such as shader management, texture handling, framebuffer effects, and interactive camera control. Built as a flexible and efficient framework, FlyEngine is ideal for developers aiming to create custom 3D applications, games, or any graphics-intensive projects. This engine serves as both a practical tool and a learning project, developed alongside the comprehensive exploration of OpenGL through Joey de Vries's [LearnOpenGL](https://learnopengl.com/) book. Many techniques implemented in FlyEngine were inspired by concepts covered in this resource, making it a hands-on application of OpenGL fundamentals and advanced techniques. 

## Features

- **Shader Management**: Compile, manage, and use GLSL shaders for custom rendering.
- **Model and Texture Handling**: Seamlessly load and render models and textures in various formats.
- **Framebuffer Effects**: Implement advanced rendering techniques, including bloom and depth mapping.
- **Camera and Input Handling**: Navigate 3D scenes with customizable camera controls and action-based input handling.
- **Flexible Framework for Graphics Applications**: Use FlyEngine to build a wide range of applications, from interactive visualizations to games.

## Table of Contents

- [Project Structure](#project-structure)
- [Installation](#installation)
- [Getting Started](#getting-started)
- [Core Components](#core-components)
- [Using Custom Shaders](#using-custom-shaders)
- [Detailed Examples](#detailed-examples)
- [Contributing](#contributing)
- [License](#license)

## Project Structure

The FlyEngine repository contains three main components, each serving a unique purpose:

1. **FlyEngine**: The core graphics engine library, containing the essential classes and functions for rendering, shaders, textures, input handling, and more. This is the main component for users interested in building their own 3D applications or games. FlyEngine provides a flexible framework suitable for a wide range of graphics applications.

2. **Development Environment**: A set of development and testing tools that demonstrate FlyEngine’s capabilities. This environment includes multiple pre-configured scenes showcasing various rendering techniques, shader effects, and visual styles. It serves as an internal testing ground for features, helping developers explore and validate FlyEngine’s functionalities.

3. **Launcher**: An interface to easily run and interact with different scenes within the Development Environment. The Launcher allows for quick testing and experimentation by providing a user-friendly interface to load scenes, toggle developer logs, and execute basic commands.

> **Note**: **FlyEngine** is the primary library intended for distribution and use in user applications. Development Environment and Launcher are designed for showcasing FlyEngine’s features and for testing purposes. Users interested in building their own 3D applications should focus on integrating the FlyEngine library into their projects.

## Installation

### Prerequisites

- **C++ Compiler** (GCC, Clang, or Visual Studio)
- **OpenGL**-compatible graphics card
- **GLFW**: For window management and input handling
- **GLAD**: For OpenGL function loading
- **GLM**: For vector and matrix math operations
- **Assimp**: For model loading

### Building the Project

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/lata-michal/FlyEngine.git
   cd FlyEngine
   ```

2. **Build the Project**:
   Open `FlyEngine.sln` in Visual Studio and build the solution to generate the executable.

3. **Setting Up a New Project Using FlyEngine**:
   To create a new project in the solution that uses FlyEngine, follow these steps:
   - Add a reference to `FlyEngine.lib` in your project.
   - Add the following paths to your **Additional Include Directories** in the project properties:

     ```
     $(SolutionDir)FlyEngine\src\include;
     $(SolutionDir)FlyEngine\src\vendor;
     $(SolutionDir)Dependencies\GLFW\include\;
     $(SolutionDir)Dependencies\GLAD\include\;
     $(SolutionDir)Dependencies\ASSIMP\assimp-5.2.5\include;
     $(SolutionDir)Dependencies\ASSIMP\assimp-5.2.5\contrib
     ```

## Getting Started

FlyEngine provides core functionalities to create 3D applications and games. Start by initializing a window, loading shaders and models, setting up the camera, and handling input. This setup forms the basis for building interactive graphics applications.

### Core Initialization

- **Window**: Initialize an OpenGL window using `Window::Initialize`.
- **Shader**: Load and manage shaders with the `Shader` class.
- **Camera**: Control camera movement and orientation for exploring scenes.
- **Framebuffer**: Use framebuffer objects for rendering and post-processing effects.

## Core Components

- **`Window`**: Manages the application window and OpenGL context.
- **`Shader`**: Loads, compiles, and binds GLSL shaders, with methods for setting uniform variables.
- **`Texture`**: Loads textures from files and manages OpenGL texture states.
- **`Camera`**: Controls camera movements, view matrices, and orientation, with key and mouse bindings.
- **`Model` and `Mesh`**: Load and handle 3D models, supporting multiple meshes and textures.
- **`FrameBuffer`**: Manages framebuffers for off-screen rendering and post-processing.
- **`BloomEffect`**: Adds a bloom effect to the scene using upsampling and downsampling shaders for intensity control.
- **`Skybox`**: Renders a skybox for immersive environmental backgrounds.
- **`KeyboardInput` and `MouseInput`**: Capture and bind keyboard and mouse inputs using an action-based system.

## Using Custom Shaders

FlyEngine supports custom GLSL shaders, allowing users to create their own rendering techniques and visual effects. While FlyEngine itself doesn’t include predefined shaders, it provides robust shader management through the `Shader` class, allowing users to compile, bind, and set uniforms for custom shaders.

### Example Shaders in Launcher’s Resource Directory

In the Launcher’s `res/shaders` directory, a collection of example shaders is provided as part of the Development Environment. These shaders demonstrate various effects and techniques that FlyEngine supports. Users can refer to these shaders as examples to understand and implement custom rendering effects, such as:

- **Lighting Models**: Basic lighting effects, such as Phong and Blinn-Phong lighting.
- **Normal and Parallax Mapping**: Surface detail techniques, adding depth and realism.
- **Depth and Shadow Mapping**: Simulating depth and creating shadows for immersive scenes.
- **Post-Processing Effects**: Tone mapping, bloom, and other image adjustments.
- **Dynamic Effects**: Vertex manipulation effects like explosions or distortions.

> **Note**: These shaders are designed for demonstration and development purposes within the Launcher and Development Environment. They are not part of the core FlyEngine library but are provided as resources to help users explore the types of effects they can achieve.

### Loading and Binding Shaders

To load and bind shaders in FlyEngine, use the `Shader` class. Here’s a general outline:

```cpp
feng::Shader shader("path/to/shader.glsl");
shader.Bind();
shader.SetUniform("uniformName", value);  // Example for setting uniforms
```

## Detailed Examples

### 1. Basic Scene with Model and Shader

```cpp
#include "Window.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"

int main() {
    // Initialize window
    feng::Window::Initialize(1280, 720, "Basic Scene", false);

    // Load shader
    feng::Shader shader("shaders/basic.glsl");

    // Load model
    feng::Model model("models/cube.obj");
    model.Load();

    // Set up camera
    feng::Camera camera(glm::vec3(0.0f, 1.0f, 5.0f));

    // Main rendering loop
    while (!feng::Window::WindowShouldClose()) {
        camera.UpdateCamera();
        feng::Window::Clear(0.1f, 0.1f, 0.1f, 1.0f, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Bind shader and draw model
        shader.Bind();
        model.Draw(shader);

        feng::Window::SwapBuffers();
        feng::Window::PollEvents();
    }

    return 0;
}
```

### 2. Applying Bloom Post-Processing Effect

```cpp
#include "Window.h"
#include "Shader.h"
#include "Model.h"
#include "BloomEffect.h"

int main() {
    feng::Window::Initialize(1280, 720, "Bloom Effect Scene", false);

    // Load shaders
    feng::Shader mainShader("shaders/main.glsl");
    feng::Shader bloomShader("shaders/bloom.glsl");

    // Initialize bloom effect
    feng::BloomEffect bloomEffect("shaders/upscale.glsl", "shaders/downscale.glsl", 1.0f, 0.5f);

    // Load model
    feng::Model model("models/sphere.obj");
    model.Load();

    while (!feng::Window::WindowShouldClose()) {
        bloomEffect.BindSourceFBO();
        mainShader.Bind();
        model.Draw(mainShader);
        bloomEffect.UnbindSourceFBO();

        // Render with bloom
        bloomEffect.RenderSceneWBloom(bloomShader);

        feng::Window::SwapBuffers();
        feng::Window::PollEvents();
    }

    return 0;
}
```

### 3. Loading and Displaying a Skybox

```cpp
#include "Window.h"
#include "Shader.h"
#include "Skybox.h"

int main() {
    feng::Window::Initialize(1280, 720, "Skybox Demo", false);

    // Specify the paths to the cubemap textures
    std::vector<std::string> faces = {
        "textures/right.jpg", "textures/left.jpg",
        "textures/top.jpg", "textures/bottom.jpg",
        "textures/front.jpg", "textures/back.jpg"
    };

    // Initialize and load the skybox
    feng::Skybox skybox(faces);
    feng::Shader skyboxShader("shaders/skybox.glsl");

    // Main loop
    while (!feng::Window::WindowShouldClose()) {
        feng::Window::Clear(0.1f, 0.1f, 0.1f, 1.0f, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render the skybox
        skyboxShader.Bind();
        skybox.Draw(skyboxShader);

        feng::Window::SwapBuffers();
        feng::Window::PollEvents();
    }

    return 0;
}
```

### 4. Using Action Bindings for Interactive Camera Controls

FlyEngine’s action-based input system binds specific keys to actions, allowing easy control of the camera.

```cpp
#include "Window.h"
#include "Camera.h"

int main() {
    feng::Window::Initialize(1280, 720, "Input Handling Demo", false);

    // Set up camera with action bindings
    feng::Camera camera(glm::vec3(0.0f, 1.0f, 5.0f));
    camera.AddKeyBinding(GLFW_KEY_W, KeyActions::FORWARD);
    camera.AddKeyBinding(GLFW_KEY_S, KeyActions::BACKWARD);
    camera.AddKeyBinding(GLFW_KEY_A, KeyActions::LEFT);
    camera.AddKeyBinding(GLFW_KEY_D, KeyActions::RIGHT);
    camera.AddKeyBinding(GLFW_KEY_ESCAPE, KeyActions::EXIT);
    camera.AddMouseBinding(0, MouseActions::LOOKAROUND);
    camera.AddScrollBinding(0, ScrollActions::ZOOM);

    while (!feng::Window::WindowShouldClose()) {
        camera.UpdateCamera();
        feng::Window::Clear(0.1f, 0.1f, 0.1f, 1.0f, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render scene objects here

        feng::Window::SwapBuffers();
        feng::Window::PollEvents();
    }

    return 0;
}
```

## Contributing

Contributions are welcome! Please fork the repository, create a feature branch, and submit a pull request.

## License

This project is licensed under the MIT License.
