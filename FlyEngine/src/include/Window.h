#pragma once

#include "Actions.h"
#include "FileSystem.h"
#include "KeyboardInput.h"
#include "MouseInput.h"

namespace feng {

    struct GLFWwindowDestroyer
    {
        void operator()(GLFWwindow* ptr)
        {
            glfwDestroyWindow(ptr);
        }
    };

    class Window
    {
    public:
        static std::unique_ptr<GLFWwindow, GLFWwindowDestroyer> m_Window;

        static KeyboardInput m_KeyboardInput;
        static MouseInput m_MouseInput;
        static std::unordered_set<uint32_t> m_ErrorSet;
    public:
        Window() = delete;
        Window(const Window& other) = delete;

        static void Initialize(int32_t width, int32_t height, const char* title, bool fullscreen, int32_t glfwVersionMaj = 4, int32_t glfwVersionMin = 3, 
            int32_t openGLCoreProfile = GLFW_OPENGL_CORE_PROFILE, bool vsync = true, float sensitivity = 0.03f, int32_t antialiasing = 4);

        static bool ValidateWindow();
        static void Destroy() { m_Window.reset(); }
        static void TerminateGLFW() { glfwTerminate(); }

        static void WindowProcessInput(Actions& action);
        static bool WindowShouldClose();

        static void Clear(uint32_t mask);
        static void Clear(float r, float g, float b, float a, uint32_t mask);
        static void SwapBuffers();
        static void PollEvents();

        static int32_t GetWidth() { return m_Width; }
        static int32_t GetHeight() { return m_Height; }
        static int32_t QueryAntiAliasing() { return m_Antialiasing; }
        static bool QueryResized() { if (m_Resized == true) { m_Resized = false; return true; } else return false; }

        static void SetWindowSize(int32_t width, int32_t height);
        static void SetWindowTitle(const std::string& title);
        static void SetWindowIcon(const std::string& iconPath);
        static void SetSensitivity(float sensitivity) { m_MouseInput.SetSensitivity(sensitivity); }
        static void SetDeltaTime(float deltaTime) { m_DeltaTime = deltaTime; }
        static void SetWindowShouldClose();
        static void SetFullscreen(bool fullscreen);
        static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) { glViewport(x, y, width, height); }

        static void EnableFaceCulling() { glEnable(GL_CULL_FACE); }
        static void SetCWFaceCulling() { glFrontFace(GL_CW); }
        static void SetCCWFaceCulling() { glFrontFace(GL_CCW); }
        static void DisableFaceCulling() { glDisable(GL_CULL_FACE); }

        static void EnableBlending() { glEnable(GL_BLEND); }
        static void SetBlendFunc(GLenum sfactor, GLenum dfactor) { glBlendFunc(sfactor, dfactor); }
        static void SetBlendEquation(GLenum mode) { glBlendEquation(mode); }
        static void DisableBlending() { glDisable(GL_BLEND); }

        static void EnableStencilBuffer() { glEnable(GL_STENCIL_TEST); glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE); }
        static void DisableStencilBufer() { glDisable(GL_STENCIL_TEST); }

        static void EnableDepthTesting() { glEnable(GL_DEPTH_TEST); }
        static void SetDepthFunc(GLenum func) { glDepthFunc(func); }
        static void DisableDepthTesting() { glDisable(GL_DEPTH_TEST); }
    private:
        static void WindowFrameBufferSizeCallback(GLFWwindow* window, int32_t width, int32_t height);
        static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
        static void MouseCursorPosCallback(GLFWwindow* window, double xpos, double ypos);
        static void KeyCallback(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods);
        static void MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
    private:
        static int32_t m_Width;
        static int32_t m_Height;
        static int32_t m_LastWidth;
        static int32_t m_LastHeight;
        static int32_t m_LastXpos;
        static int32_t m_LastYpos;
        static int32_t m_Antialiasing;
        static const char* m_Title;

        static float m_DeltaTime;
        static bool m_InitializedGLAD;
        static bool m_Fullscreen;
        static bool m_Resized;
    };

}