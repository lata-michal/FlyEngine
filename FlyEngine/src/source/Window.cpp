#include "Window.h"

#include "Log.h"

using namespace feng;

std::unique_ptr<GLFWwindow, GLFWwindowDestroyer> Window::m_Window;
std::unordered_set<uint32_t> Window::m_ErrorSet;
KeyboardInput Window::m_KeyboardInput;
MouseInput Window::m_MouseInput;

int32_t Window::m_Width;
int32_t Window::m_Height;
int32_t Window::m_LastWidth;
int32_t Window::m_LastHeight;
int32_t Window::m_LastXpos;
int32_t Window::m_LastYpos;
const char* Window::m_Title;
float Window::m_DeltaTime;
bool Window::m_InitializedGLAD = false;
int32_t Window::m_Antialiasing = 0;
bool Window::m_Fullscreen = false;
bool Window::m_Resized = false;

void Window::Initialize(int32_t width, int32_t height, const char* title, bool fullscreen, int32_t glfwVersionMaj, int32_t glfwVersionMin, 
    int32_t openGLCoreProfile, bool vsync, float sensitivity, int32_t antialiasing)
{
    if (!glfwInit())
    {
        Log::Print("[GLFW ERROR] Failed to initialize GLFW\n");
    }
    else
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glfwVersionMaj);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glfwVersionMin);
        glfwWindowHint(GLFW_OPENGL_PROFILE, openGLCoreProfile);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

        if (antialiasing)
            glfwWindowHint(GLFW_SAMPLES, antialiasing);

        m_Window.reset(glfwCreateWindow(width, height, title, nullptr, nullptr));

        if (!m_Window)
        {
            Log::Print("[GLFW ERROR] Failed to create GLFW window\n");
            glfwTerminate();
        }
        else
        {
            m_Width = width;
            m_Height = height;
            m_Title = title;

            glfwSwapInterval(static_cast<int32_t>(vsync));

            glfwMakeContextCurrent(m_Window.get());

            if (!m_InitializedGLAD)
            {
                if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
                {
                    Log::Print("[GLAD ERROR] Failed to initialize OpenGL\n");
                    return;
                }
            }

            m_KeyboardInput = KeyboardInput();
            m_MouseInput = MouseInput();

            m_InitializedGLAD = true;
            m_Antialiasing = antialiasing;

            Log::Print("\n");
            Log::Print(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
            Log::Print("\n");

            m_LastXpos = 100;
            m_LastYpos = 100;

            glfwSetWindowPos(m_Window.get(), m_LastXpos, m_LastYpos);
            glfwSetFramebufferSizeCallback(m_Window.get(), WindowFrameBufferSizeCallback);
            glfwSetScrollCallback(m_Window.get(), MouseScrollCallback);
            glfwSetCursorPosCallback(m_Window.get(), MouseCursorPosCallback);
            glfwSetKeyCallback(m_Window.get(), KeyCallback);
            glfwSetInputMode(m_Window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            glDebugMessageCallback(MessageCallback, 0);
            glEnable(GL_DEBUG_OUTPUT);
            glViewport(0, 0, m_Width, m_Height);
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);
            glEnable(GL_FRAMEBUFFER_SRGB);

            if (fullscreen)
                SetFullscreen(true);
            if (antialiasing)
                glEnable(GL_MULTISAMPLE);
        }
    }
}

bool Window::ValidateWindow()
{
    return m_Window != nullptr;
}

void Window::SetWindowSize(int32_t width, int32_t height)
{
    m_Width = width;
    m_Height = height;
    glfwSetWindowSize(m_Window.get(), m_Width, m_Height);
}

void Window::SetWindowTitle(const std::string& title)
{
    glfwSetWindowTitle(m_Window.get(), title.c_str());
}

void feng::Window::SetWindowIcon(const std::string& iconPath)
{
    int32_t iconWidth, iconHeight, iconChannels;
    unsigned char* buffer = stbi_load(iconPath.c_str(), &iconWidth, &iconHeight, &iconChannels, 0);

    if (buffer)
    {
        GLFWimage icon;
        icon.width = iconWidth;
        icon.height = iconHeight;
        icon.pixels = buffer;

        glfwSetWindowIcon(m_Window.get(), 1, &icon);
        stbi_image_free(buffer);
    }
    else
    {
        Log::Print("FAILED TO LOAD ICON AT PATH " + iconPath + '\n');
        stbi_image_free(buffer);
    }
}

void Window::SetWindowShouldClose()
{
    glfwSetWindowShouldClose(m_Window.get(), true);
}

void Window::SetFullscreen(bool fullscreen)
{
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);

    if (fullscreen)
    {
        m_Fullscreen = true;
        m_LastWidth = m_Width;
        m_LastHeight = m_Height;
        glfwGetWindowPos(m_Window.get(), &m_LastXpos, &m_LastYpos);

        glfwSetWindowMonitor(m_Window.get(), monitor, 0, 0, videoMode->width, videoMode->height, videoMode->refreshRate);
    }
    else if (!fullscreen)
    {
        m_Fullscreen = false;

        glfwSetWindowMonitor(m_Window.get(), nullptr, m_LastXpos, m_LastYpos, m_LastWidth, m_LastHeight, GLFW_DONT_CARE);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
}

void Window::WindowProcessInput(Actions& actionObject) 
{
    /*mouse movement*/
    if (m_MouseInput.QueryMovedMouse())
    {
        m_MouseInput.SetMovedMouse(false);
        MouseMove move = m_MouseInput.GetMouseMove();

        for (const auto& [key, assignedAction] : actionObject.GetMouseBindings())
            if (assignedAction == MouseActions::LOOKAROUND)
                actionObject.LookAround(move.offsetX, move.offsetY);
    }
    /*process scroll*/
    if (m_MouseInput.QueryScrolled())
    {
        m_MouseInput.SetScrolled(false);
        float scroll = m_MouseInput.GetScroll();

        for (const auto& [key, assignedAction] : actionObject.GetScrollBindings())
            if (assignedAction == ScrollActions::ZOOM)
                actionObject.Zoom(scroll);
    }
    /*process keyboard*/
    if (m_KeyboardInput.QueryState())
    {
        for (const auto& [key, assignedAction] : actionObject.GetKeyBindings())
        {
            if (m_KeyboardInput.m_Keys[key])
            {
                if (static_cast<bool>(assignedAction & 0xF)) // 0xF - flag reserved for move actions
                {
                    actionObject.Move(assignedAction, m_DeltaTime);
                }
                if (static_cast<bool>(assignedAction & KeyActions::FULLSCREEN))
                {
                    if (!m_Fullscreen)
                        SetFullscreen(true);
                    else if (m_Fullscreen)
                        SetFullscreen(false);
                }
                if (static_cast<bool>(assignedAction & KeyActions::EXIT))
                {
                    SetWindowShouldClose();
                }

                actionObject.OtherAction(assignedAction, m_DeltaTime);
            }
        }
    }

}

bool Window::WindowShouldClose()
{
    return glfwWindowShouldClose(m_Window.get());
}

void feng::Window::Clear(uint32_t mask)
{
    glClear(mask);
}

void Window::Clear(float r, float g, float b, float a, uint32_t mask)
{
    glClearColor(r, g, b, a);
    glClear(mask);
}

void Window::SwapBuffers()
{
    glfwSwapBuffers(m_Window.get());
}

void Window::PollEvents()
{
    glfwPollEvents();
}

void Window::WindowFrameBufferSizeCallback(GLFWwindow* window, int32_t width, int32_t height)
{
    glViewport(0, 0, width, height);
    m_Width = width;
    m_Height = height;
    m_Resized = true;
}

void Window::MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    m_MouseInput.SetScrolled(true);
    m_MouseInput.Scrolled(static_cast<float>(yoffset));
}

void Window::MouseCursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    m_MouseInput.SetMovedMouse(true);
    m_MouseInput.MouseMoved(static_cast<float>(xpos), static_cast<float>(ypos));
}

void Window::KeyCallback(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
{
    if (key != GLFW_KEY_UNKNOWN)
    {
        if (action == GLFW_PRESS)
        {
            m_KeyboardInput.m_Keys[key] = true;
            m_KeyboardInput.m_Pressed.insert(key);

            m_KeyboardInput.SetState(true);
        }
        if (action == GLFW_RELEASE)
        {
            m_KeyboardInput.m_Keys[key] = false;
            m_KeyboardInput.m_Pressed.erase(key);

            if (m_KeyboardInput.m_Pressed.empty())
                m_KeyboardInput.SetState(false);
            else
                m_KeyboardInput.SetState(true);
        }
    }
}

void GLAPIENTRY Window::MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    if (type != GL_DEBUG_TYPE_OTHER && severity != GL_DEBUG_SEVERITY_LOW)
    {
        if (m_ErrorSet.find(id) == m_ErrorSet.end())
        {
            Log::Print("GL CALLBACK: TYPE: " + std::to_string(type) + " SEVERITY: " + std::to_string(severity) 
                + " MESSAGE: " + std::string(message) + '\n');
            m_ErrorSet.insert(id);
        }
    }
}
