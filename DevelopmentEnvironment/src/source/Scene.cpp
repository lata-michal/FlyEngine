#include "Scene.h"

bool Scene::m_ForcedEnd;

bool Scene::RenderScene1(HWND hText)
{
    using namespace feng;

    m_ForcedEnd = false;
    Log::SetHandleTextBox(hText);

    Window::Initialize(1600, 1100, "FlyEngine :-)", false, 4, 3, GLFW_OPENGL_CORE_PROFILE, true, 0.03f, 4);

    if (!Window::ValidateWindow())
        return false;

    FileSystem fileSys;

    Shader shaderBackpack(fileSys.GetExecutableDirPath() + "\\res\\shaders\\backpack.shader");
    shaderBackpack.SetUniform1f("uMaterial.shininess", 96.0f);
    
    Shader shaderBackpackExplosion(fileSys.GetExecutableDirPath() + "\\res\\shaders\\backpackexploding.shader");
    shaderBackpackExplosion.SetUniform1f("uMaterial.shininess", 96.0f);

    Shader shaderBackpackNormals(fileSys.GetExecutableDirPath() + "\\res\\shaders\\backpacknormals.shader");

    Shader shaderFlower(fileSys.GetExecutableDirPath() + "\\res\\shaders\\flowers.shader");
    shaderFlower.SetUniform1f("uMaterial.shininess", 32.0f);

    Shader shaderFloor(fileSys.GetExecutableDirPath() + "\\res\\shaders\\floor.shader");
    Shader shaderGrass(fileSys.GetExecutableDirPath() + "\\res\\shaders\\grass.shader");
    Shader shaderWindow(fileSys.GetExecutableDirPath() + "\\res\\shaders\\window.shader");
    Shader shaderSkybox(fileSys.GetExecutableDirPath() + "\\res\\shaders\\skybox.shader");
    Shader shaderReflection(fileSys.GetExecutableDirPath() + "\\res\\shaders\\reflection.shader");
    Shader shaderRefraction(fileSys.GetExecutableDirPath() + "\\res\\shaders\\refraction.shader");
    Shader shaderCube(fileSys.GetExecutableDirPath() + "\\res\\shaders\\cubelight.shader");
    Shader postProcessingShader(fileSys.GetExecutableDirPath() + "\\res\\shaders\\postprocessing.shader");

    postProcessingShader.SetUniform1i("uScreenTexture", 0);
    postProcessingShader.SetUniform1f("uKernel[0]", 1.0f / 16.0f);
    postProcessingShader.SetUniform1f("uKernel[1]", 2.0f / 16.0f);
    postProcessingShader.SetUniform1f("uKernel[2]", 1.0f / 16.0f);
    postProcessingShader.SetUniform1f("uKernel[3]", 2.0f / 16.0f);
    postProcessingShader.SetUniform1f("uKernel[4]", 8.0f / 16.0f);
    postProcessingShader.SetUniform1f("uKernel[5]", 2.0f / 16.0f);
    postProcessingShader.SetUniform1f("uKernel[6]", 1.0f / 16.0f);
    postProcessingShader.SetUniform1f("uKernel[7]", 2.0f / 16.0f);
    postProcessingShader.SetUniform1f("uKernel[8]", 1.0f / 16.0f);
    postProcessingShader.SetUniform1f("uOffsetX", 32.0f / Window::GetWidth());
    postProcessingShader.SetUniform1f("uOffsetY", 32.0f / Window::GetHeight());


    Model modelFlowers(fileSys.GetExecutableDirPath() + "\\res\\models\\flower\\flower.obj");
    Model modelBackpack(fileSys.GetExecutableDirPath() + "\\res\\models\\backpack\\backpack.obj");
    modelFlowers.Load();
    modelBackpack.Load();

    std::vector<float> floorVertices = {
        1.0f, 0.0f, -1.0f, 0.0f, 3.0f,
        1.0f, 0.0f, 1.0f, 3.0f, 3.0f,
        -1.0f, 0.0f, 1.0f, 3.0f, 0.0f,
        -1.0f, 0.0f, -1.0f, 0.0f, 0.0f
    };

    VertexLayout planeVerticesLayout;
    planeVerticesLayout.AddVec3(0);
    planeVerticesLayout.AddVec2(1);

    std::vector<uint32_t> floorIndices = { 2, 1, 0, 3, 2, 0 };

    Texture floorTex(fileSys.GetExecutableDirPath() + "\\res\\textures\\f.jpg", "texture_diffuse", GL_REPEAT);
    std::vector<Texture> floorTexture = { floorTex };

    Mesh floor(floorVertices, floorIndices, floorTexture, planeVerticesLayout);

    srand(static_cast<uint32_t>(time(nullptr)));

    Texture grassTex(fileSys.GetExecutableDirPath() + "\\res\\textures\\grass.png", "texture_diffuse", GL_CLAMP_TO_EDGE);
    std::vector<Texture> grassTexture = { grassTex };

    std::vector<float> grassVertices = {
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f
    };

    std::vector<uint32_t> grassIndices = { 0, 1, 2, 0, 2, 3 };

    std::unordered_map<float, float> grassRotations;

    std::vector<float> instanceGrassModel(1681 * 16);

    uint32_t it = 0.0f;

    for (float i = -10.0f; i <= 10.0f; i += 0.5f)
    {
        for (float j = -10.0f; j <= 10.0f; j += 0.5f)
        {
            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, glm::vec3(i, 0.7f, j));
            model = glm::rotate(model, glm::radians(static_cast<float>(rand())), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.7f));

            instanceGrassModel[it++] = model[0][0];
            instanceGrassModel[it++] = model[0][1];
            instanceGrassModel[it++] = model[0][2];
            instanceGrassModel[it++] = model[0][3];

            instanceGrassModel[it++] = model[1][0];
            instanceGrassModel[it++] = model[1][1];
            instanceGrassModel[it++] = model[1][2];
            instanceGrassModel[it++] = model[1][3];

            instanceGrassModel[it++] = model[2][0];
            instanceGrassModel[it++] = model[2][1];
            instanceGrassModel[it++] = model[2][2];
            instanceGrassModel[it++] = model[2][3];

            instanceGrassModel[it++] = model[3][0];
            instanceGrassModel[it++] = model[3][1];
            instanceGrassModel[it++] = model[3][2];
            instanceGrassModel[it++] = model[3][3];
        }
    }

    Mesh grass(grassVertices, grassIndices, grassTexture, planeVerticesLayout);

    VBO instanceGrassVBO(instanceGrassModel);
    instanceGrassVBO.SetupInstanceVBOMat4(grass.GetVAO(), 2, 1);

    std::vector<glm::vec3> windowLocations = {
        glm::vec3(-1.5f, 2.0f, 7.52f),
        glm::vec3(1.5f, 2.0f, 8.51f),
        glm::vec3(0.0f, 2.0f, 5.7f),
        glm::vec3(0.5f, 2.0f, 7.4f)
    };

    Texture windowTex(fileSys.GetExecutableDirPath() + "\\res\\textures\\blending_transparent_window.png", "texture_diffuse", GL_CLAMP_TO_EDGE);
    std::vector<Texture> windowTexture = { windowTex };

    Mesh window(grassVertices, grassIndices, windowTexture, planeVerticesLayout);

    VertexLayout rPlaneLayout;
    rPlaneLayout.AddVec3(0);
    rPlaneLayout.AddVec3(1);

    std::vector<float> rPlaneVertices = {
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    std::vector<uint32_t> rPlaneIndices = { 0, 1, 2, 0, 2, 3 };

    Mesh reflectionPlane(rPlaneVertices, rPlaneIndices, rPlaneLayout);

    std::vector<float> cubeVertices = {
        -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f, 1.0f,
         1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f,1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,1.0f,  1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f, -1.0f,  1.0f, -1.0f, -1.0f, 1.0f,
        -1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, -1.0f,  1.0f
    };


    VertexLayout cubeVerticesLayout;
    cubeVerticesLayout.AddVec3(0);

    Mesh cube(cubeVertices, cubeVerticesLayout);

    VertexLayout screenRectangleVerticesLayout;
    screenRectangleVerticesLayout.AddVec2(0);
    screenRectangleVerticesLayout.AddVec2(1);

    std::vector<float> screenRectangleVertices = {
        1.0f, 1.0f, 1.0f, 1.0f, // top right
        -1.0f, 1.0f, 0.0f, 1.0f, //top left
        -1.0f, -1.0f, 0.0f, 0.0f, //bottom left
        1.0f, -1.0f, 1.0f, 0.0f //bottom right
    };

    std::vector<uint32_t> screenRectangleIndices = {
        0, 1, 2,
        0, 2, 3
    };

    VBO screenVBO(screenRectangleVertices);
    IBO screenIBO(screenRectangleIndices);
    FrameBuffer screenFB(screenVBO, screenIBO, screenRectangleVerticesLayout);

    std::vector<float> screenMirrorVertices = {
        0.3f, 0.8f, 1.0f, 1.0f, // top right
        -0.3f, 0.8f, 0.0f, 1.0f, //top left
        -0.3f, 0.5f, 0.0f, 0.0f, //bottom left
        0.3f, 0.5f, 1.0f, 0.0f //bottom right
    };

    std::vector<uint32_t> screenMirrorIndices = {
        0, 1, 2,
        0, 2, 3
    };

    VBO mirrorVBO(screenMirrorVertices);
    IBO mirrorIBO(screenMirrorIndices);

    FrameBuffer mirrorFB(mirrorVBO, mirrorIBO, screenRectangleVerticesLayout);

    std::vector<std::string> skyboxTextures = {
        fileSys.GetExecutableDirPath() + "\\res\\textures\\skybox\\right.jpg",
        fileSys.GetExecutableDirPath() + "\\res\\textures\\skybox\\left.jpg",
        fileSys.GetExecutableDirPath() + "\\res\\textures\\skybox\\top.jpg",
        fileSys.GetExecutableDirPath() + "\\res\\textures\\skybox\\bottom.jpg",
        fileSys.GetExecutableDirPath() + "\\res\\textures\\skybox\\front.jpg",
        fileSys.GetExecutableDirPath() + "\\res\\textures\\skybox\\back.jpg"
    };

    CubeMap skybox(skyboxTextures);

    glm::vec3 ambientDirlightColor(0.2f, 0.2f, 0.2f);
    glm::vec3 diffuseDirlightColor(0.5f, 0.5f, 0.5f);
    glm::vec3 specularDirlightColor(0.8f, 0.8f, 0.8f);

    glm::vec3 ambientPointColor(0.0f, 0.0f, 0.0f);
    glm::vec3 diffusePointColor(1.0f, 0.3f, 0.3f);
    glm::vec3 specularPointColor(1.0f, 0.3f, 0.3f);

    Timer timer;

    Camera camera(0.0f, 5.0f, 20.0f, glm::vec3(0.0f, 1.0f, 0.0f), -10.0f, -100.0f, 3.0f, 45.0f);

    camera.AddKeyBinding(GLFW_KEY_ESCAPE, KeyActions::EXIT);
    camera.AddKeyBinding(GLFW_KEY_W, KeyActions::FORWARD);
    camera.AddKeyBinding(GLFW_KEY_A, KeyActions::LEFT);
    camera.AddKeyBinding(GLFW_KEY_S, KeyActions::BACKWARD);
    camera.AddKeyBinding(GLFW_KEY_D, KeyActions::RIGHT);
    camera.AddKeyBinding(GLFW_KEY_F11, KeyActions::FULLSCREEN);
    camera.AddMouseBinding(0, MouseActions::LOOKAROUND);
    camera.AddScrollBinding(0, ScrollActions::ZOOM);

    Window::EnableFaceCulling();
    Window::SetCCWFaceCulling();
    Window::EnableDepthTesting();

    float iterator_effect = 32.0f;

    while (!Window::WindowShouldClose() && !m_ForcedEnd)
    {
        if (iterator_effect > 0.0f)
        {
            postProcessingShader.Bind();
            postProcessingShader.SetUniform1f("uOffsetX", iterator_effect * static_cast<float>(sin(glfwGetTime())) / Window::GetWidth());
            postProcessingShader.SetUniform1f("uOffsetY", iterator_effect * static_cast<float>(sin(glfwGetTime())) / Window::GetHeight());
            iterator_effect -= 0.08f;
        }

        if (Window::QueryResized())
        {
            screenFB.Update();
            mirrorFB.Update();
        }

        Window::WindowProcessInput(camera);

        timer.UpdateDelta();
        timer.UpdateFPS();

        Window::SetDeltaTime(timer.GetDeltaTime());
        Window::SetWindowTitle("FlyEngine : -)    FPS: " + timer.GetStringFPS());

        {
            screenFB.Bind();

            Window::Clear(1.0f, 1.0f, 1.0f, 1.0f, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glm::mat4 projection = glm::mat4(1);
            projection = glm::perspective(glm::radians(camera.GetZoom()), static_cast<float>(Window::GetWidth()) / static_cast<float>(Window::GetHeight()), 0.1f, 100.0f);

            glm::mat4 view = camera.GetViewMatrix();

            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, glm::vec3(2.0f, 3.5f, 1.0f));
            model = glm::scale(model, glm::vec3(0.1f));

            shaderCube.Bind();
            shaderCube.SetUniformMat4f("uMVP", projection * view * model);

            cube.Draw(shaderCube);

            model = glm::mat4(1);
            model = glm::translate(model, glm::vec3(0.0f, 1.2f, -5.0f));
            model = glm::scale(model, glm::vec3(0.7f));

            shaderBackpack.Bind();
            shaderBackpack.SetUniformMat4f("uMVP", projection * view * model);
            shaderBackpack.SetUniformMat4f("uModel", model);
            shaderBackpack.SetUniformMat3f("uTranInvModel", glm::transpose(glm::inverse(glm::mat3(model))));
            shaderBackpack.SetUniformVec3f("uViewPos", camera.GetCameraPosition());
            shaderBackpack.SetUniform3f("uDirLight.direction", 0.0f, -1.0f, -0.3f);
            shaderBackpack.SetUniformVec3f("uDirLight.ambient", ambientDirlightColor);
            shaderBackpack.SetUniformVec3f("uDirLight.diffuse", diffuseDirlightColor);
            shaderBackpack.SetUniformVec3f("uDirLight.specular", specularDirlightColor);

            shaderBackpack.SetUniformVec3f("uPointLight.position", glm::vec3(2.0f, 3.5f, 1.0f));
            shaderBackpack.SetUniform1f("uPointLight.constant", 1.0f);
            shaderBackpack.SetUniform1f("uPointLight.linear", 0.009f);
            shaderBackpack.SetUniform1f("uPointLight.quadratic", 0.0032f);
            shaderBackpack.SetUniformVec3f("uPointLight.ambient", ambientPointColor);
            shaderBackpack.SetUniformVec3f("uPointLight.diffuse", diffusePointColor);
            shaderBackpack.SetUniformVec3f("uPointLight.specular", specularPointColor);

            modelBackpack.Draw(shaderBackpack);

            shaderBackpackNormals.Bind();
            shaderBackpackNormals.SetUniformMat4f("uModel", model);
            shaderBackpackNormals.SetUniformMat4f("uView", view);
            shaderBackpackNormals.SetUniformMat3f("uTranInvModel", glm::transpose(glm::inverse(glm::mat3(model))));
            shaderBackpackNormals.SetUniformMat4f("uProjection", projection);

            modelBackpack.Draw(shaderBackpackNormals);

            model = glm::mat4(1);
            model = glm::translate(model, glm::vec3(-3.0f, 1.2f, 0.0f));
            model = glm::scale(model, glm::vec3(0.7f));

            shaderBackpackExplosion.Bind();
            shaderBackpackExplosion.SetUniformMat4f("uMVP", projection * view * model);
            shaderBackpackExplosion.SetUniformMat4f("uModel", model);
            shaderBackpackExplosion.SetUniformMat3f("uTranInvModel", glm::transpose(glm::inverse(glm::mat3(model))));

            if ((camera.GetCameraPosition().x > -9.0f && camera.GetCameraPosition().x < 3.0f)
                && (camera.GetCameraPosition().y > 0.0f && camera.GetCameraPosition().y < 2.5f)
                 && (camera.GetCameraPosition().z > -4.0f && camera.GetCameraPosition().z < 4.0f))
                shaderBackpackExplosion.SetUniform1f("uMagnitude", 10.0f);
            else
                shaderBackpackExplosion.SetUniform1f("uMagnitude", 0.0f);

            shaderBackpackExplosion.SetUniformMat4f("uMVP", projection * view * model);
            shaderBackpackExplosion.SetUniformMat4f("uModel", model);
            shaderBackpackExplosion.SetUniformMat3f("uTranInvModel", glm::transpose(glm::inverse(glm::mat3(model))));
            shaderBackpackExplosion.SetUniform1f("uTime", static_cast<float>(glfwGetTime()));
            shaderBackpackExplosion.SetUniformVec3f("uViewPos", camera.GetCameraPosition());
            shaderBackpackExplosion.SetUniform3f("uDirLight.direction", 0.0f, -1.0f, -0.3f);
            shaderBackpackExplosion.SetUniformVec3f("uDirLight.ambient", ambientDirlightColor);
            shaderBackpackExplosion.SetUniformVec3f("uDirLight.diffuse", diffuseDirlightColor);
            shaderBackpackExplosion.SetUniformVec3f("uDirLight.specular", specularDirlightColor);
            
            shaderBackpackExplosion.SetUniformVec3f("uPointLight.position", glm::vec3(2.0f, 3.5f, 1.0f));
            shaderBackpackExplosion.SetUniform1f("uPointLight.constant", 1.0f);
            shaderBackpackExplosion.SetUniform1f("uPointLight.linear", 0.009f);
            shaderBackpackExplosion.SetUniform1f("uPointLight.quadratic", 0.0032f);
            shaderBackpackExplosion.SetUniformVec3f("uPointLight.ambient", ambientPointColor);
            shaderBackpackExplosion.SetUniformVec3f("uPointLight.diffuse", diffusePointColor);
            shaderBackpackExplosion.SetUniformVec3f("uPointLight.specular", specularPointColor);

            modelBackpack.Draw(shaderBackpackExplosion);

            model = glm::mat4(1);
            model = glm::translate(model, glm::vec3(-15.0f, 1.2f, 0.0f));
            model = glm::scale(model, glm::vec3(0.7f));

            shaderRefraction.Bind();
            shaderRefraction.SetUniformMat4f("uMVP", projection * view * model);
            shaderRefraction.SetUniformMat4f("uModel", model);
            shaderRefraction.SetUniformMat3f("uTranInvModel", glm::transpose(glm::inverse(glm::mat3(model))));
            shaderRefraction.SetUniform1i("uSkybox", 0);
            shaderRefraction.SetUniformVec3f("uCameraPos", camera.GetCameraPosition());

            modelBackpack.Draw(shaderRefraction);

            model = glm::mat4(1);
            model = glm::translate(model, glm::vec3(2.5f, -0.01f, 1.0f));
            model = glm::scale(model, glm::vec3(0.001f));

            shaderFlower.Bind();
            shaderFlower.SetUniformMat4f("uMVP", projection * view * model);
            shaderFlower.SetUniformMat4f("uModel", model);
            shaderFlower.SetUniformMat3f("uTranInvModel", glm::transpose(glm::inverse(glm::mat3(model))));
            shaderFlower.SetUniformVec3f("uViewPos", camera.GetCameraPosition());
            shaderFlower.SetUniform3f("uDirLight.direction", -0.2f, -1.0f, -0.3f);
            shaderFlower.SetUniformVec3f("uDirLight.ambient", ambientDirlightColor);
            shaderFlower.SetUniformVec3f("uDirLight.diffuse", diffuseDirlightColor);
            shaderFlower.SetUniformVec3f("uDirLight.specular", specularDirlightColor);

            shaderFlower.SetUniformVec3f("uPointLight.position", glm::vec3(0.0f, 0.0f, 3.0f));
            shaderFlower.SetUniform1f("uPointLight.constant", 1.0f);
            shaderFlower.SetUniform1f("uPointLight.linear", 0.09f);
            shaderFlower.SetUniform1f("uPointLight.quadratic", 0.032f);
            shaderFlower.SetUniformVec3f("uPointLight.ambient", ambientPointColor);
            shaderFlower.SetUniformVec3f("uPointLight.diffuse", diffusePointColor);
            shaderFlower.SetUniformVec3f("uPointLight.specular", specularPointColor);

            Window::DisableFaceCulling();
            modelFlowers.Draw(shaderFlower);
            Window::EnableFaceCulling();
            
            model = glm::mat4(1);
            model = glm::translate(model, glm::vec3(3.5f, -0.01f, -1.0f));
            model = glm::scale(model, glm::vec3(0.001f));

            shaderFlower.Bind();
            shaderFlower.SetUniformMat4f("uMVP", projection * view * model);
            shaderFlower.SetUniformMat4f("uModel", model);
            shaderFlower.SetUniformMat3f("uTranInvModel", glm::transpose(glm::inverse(glm::mat3(model))));

            Window::DisableFaceCulling();
            modelFlowers.Draw(shaderFlower);
            Window::EnableFaceCulling();

            model = glm::mat4(1);
            model = glm::scale(model, glm::vec3(10.0f));

            shaderFloor.Bind();
            shaderFloor.SetUniformMat4f("uMVP", projection * view * model);

            Window::DisableFaceCulling();

            floor.Draw(shaderFloor);

            shaderGrass.Bind();
            shaderGrass.SetUniformMat4f("uVP", projection * view);

            grass.Draw(shaderGrass, 1681);

            model = glm::mat4(1);
            model = glm::translate(model, glm::vec3(-20.0f, 1.0f, 0.0f));

            shaderReflection.Bind();
            shaderReflection.SetUniformMat4f("uMVP", projection * view * model);
            shaderReflection.SetUniformMat3f("uTranInvModel", glm::transpose(glm::inverse(glm::mat3(model))));
            shaderReflection.SetUniformMat4f("uModel", model);
            shaderReflection.SetUniform1i("uSkybox", 0);
            shaderReflection.SetUniformVec3f("uCameraPos", camera.GetCameraPosition());

            reflectionPlane.Draw(shaderReflection);

            Window::EnableFaceCulling();

            glm::mat4 skyboxView = glm::mat4(glm::mat3(camera.GetViewMatrix()));

            shaderSkybox.Bind();
            shaderSkybox.SetUniformMat4f("uVP", projection * skyboxView);
            shaderSkybox.SetUniform1i("uSkybox", 0);

            skybox.Draw(shaderSkybox);

            Window::DisableFaceCulling();
            Window::EnableBlending();
            Window::SetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            std::map<float, glm::vec3, std::greater<float>> sortedWindowLocations;
            for (auto& location : windowLocations)
            {
                float distance = glm::length(camera.GetCameraPosition() - location);
                sortedWindowLocations[distance] = location;
            }

            for (auto& [key, location] : sortedWindowLocations)
            {
                model = glm::mat4(1);
                model = glm::translate(model, location);
                model = glm::scale(model, glm::vec3(0.7f));

                shaderWindow.Bind();
                shaderWindow.SetUniformMat4f("uMVP", projection * view * model);

                window.Draw(shaderWindow);
            }

            Window::DisableBlending();
            Window::EnableFaceCulling();

            screenFB.Draw(postProcessingShader);
        }

        {
            mirrorFB.Bind();

            Window::Clear(1.0f, 1.0f, 1.0f, 1.0f, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            glm::mat4 projection = glm::mat4(1);
            projection = glm::perspective(glm::radians(camera.GetZoom()), static_cast<float>(Window::GetWidth()) / static_cast<float>(Window::GetHeight()), 0.1f, 100.0f);

            glm::mat4 view = camera.GetViewMatrixMirror();


            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, glm::vec3(2.0f, 3.5f, 1.0f));
            model = glm::scale(model, glm::vec3(0.1f));

            shaderCube.Bind();
            shaderCube.SetUniformMat4f("uMVP", projection * view * model);

            cube.Draw(shaderCube);

            model = glm::mat4(1);
            model = glm::translate(model, glm::vec3(0.0f, 1.2f, -5.0f));
            model = glm::scale(model, glm::vec3(0.7f));

            shaderBackpack.Bind();
            shaderBackpack.SetUniformMat4f("uMVP", projection * view * model);
            shaderBackpack.SetUniformMat4f("uModel", model);
            shaderBackpack.SetUniformMat3f("uTranInvModel", glm::transpose(glm::inverse(glm::mat3(model))));
            shaderBackpack.SetUniformVec3f("uViewPos", camera.GetCameraPosition());
            shaderBackpack.SetUniform3f("uDirLight.direction", 0.0f, -1.0f, -0.3f);
            shaderBackpack.SetUniformVec3f("uDirLight.ambient", ambientDirlightColor);
            shaderBackpack.SetUniformVec3f("uDirLight.diffuse", diffuseDirlightColor);
            shaderBackpack.SetUniformVec3f("uDirLight.specular", specularDirlightColor);

            shaderBackpack.SetUniformVec3f("uPointLight.position", glm::vec3(2.0f, 3.5f, 1.0f));
            shaderBackpack.SetUniform1f("uPointLight.constant", 1.0f);
            shaderBackpack.SetUniform1f("uPointLight.linear", 0.009f);
            shaderBackpack.SetUniform1f("uPointLight.quadratic", 0.0032f);
            shaderBackpack.SetUniformVec3f("uPointLight.ambient", ambientPointColor);
            shaderBackpack.SetUniformVec3f("uPointLight.diffuse", diffusePointColor);
            shaderBackpack.SetUniformVec3f("uPointLight.specular", specularPointColor);

            modelBackpack.Draw(shaderBackpack);

            shaderBackpackNormals.Bind();
            shaderBackpackNormals.SetUniformMat4f("uModel", model);
            shaderBackpackNormals.SetUniformMat4f("uView", view);
            shaderBackpackNormals.SetUniformMat3f("uTranInvModel", glm::transpose(glm::inverse(glm::mat3(model))));
            shaderBackpackNormals.SetUniformMat4f("uProjection", projection);

            modelBackpack.Draw(shaderBackpackNormals);

            model = glm::mat4(1);
            model = glm::translate(model, glm::vec3(-3.0f, 1.2f, 0.0f));
            model = glm::scale(model, glm::vec3(0.7f));

            shaderBackpackExplosion.Bind();
            shaderBackpackExplosion.SetUniformMat4f("uMVP", projection * view * model);
            shaderBackpackExplosion.SetUniformMat4f("uModel", model);
            shaderBackpackExplosion.SetUniformMat3f("uTranInvModel", glm::transpose(glm::inverse(glm::mat3(model))));
            shaderBackpackExplosion.SetUniformMat4f("uMVP", projection* view* model);
            shaderBackpackExplosion.SetUniformMat4f("uModel", model);
            shaderBackpackExplosion.SetUniformMat3f("uTranInvModel", glm::transpose(glm::inverse(glm::mat3(model))));
            shaderBackpackExplosion.SetUniform1f("uTime", static_cast<float>(glfwGetTime()));
            shaderBackpackExplosion.SetUniformVec3f("uViewPos", camera.GetCameraPosition());
            shaderBackpackExplosion.SetUniform3f("uDirLight.direction", 0.0f, -1.0f, -0.3f);
            shaderBackpackExplosion.SetUniformVec3f("uDirLight.ambient", ambientDirlightColor);
            shaderBackpackExplosion.SetUniformVec3f("uDirLight.diffuse", diffuseDirlightColor);
            shaderBackpackExplosion.SetUniformVec3f("uDirLight.specular", specularDirlightColor);

            shaderBackpackExplosion.SetUniformVec3f("uPointLight.position", glm::vec3(2.0f, 3.5f, 1.0f));
            shaderBackpackExplosion.SetUniform1f("uPointLight.constant", 1.0f);
            shaderBackpackExplosion.SetUniform1f("uPointLight.linear", 0.009f);
            shaderBackpackExplosion.SetUniform1f("uPointLight.quadratic", 0.0032f);
            shaderBackpackExplosion.SetUniformVec3f("uPointLight.ambient", ambientPointColor);
            shaderBackpackExplosion.SetUniformVec3f("uPointLight.diffuse", diffusePointColor);
            shaderBackpackExplosion.SetUniformVec3f("uPointLight.specular", specularPointColor);

            modelBackpack.Draw(shaderBackpackExplosion);

            model = glm::mat4(1);
            model = glm::translate(model, glm::vec3(-15.0f, 1.2f, 0.0f));
            model = glm::scale(model, glm::vec3(0.7f));

            shaderRefraction.Bind();
            shaderRefraction.SetUniformMat4f("uMVP", projection* view* model);
            shaderRefraction.SetUniformMat4f("uModel", model);
            shaderRefraction.SetUniformMat3f("uTranInvModel", glm::transpose(glm::inverse(glm::mat3(model))));
            shaderRefraction.SetUniform1i("uSkybox", 0);
            shaderRefraction.SetUniformVec3f("uCameraPos", camera.GetCameraPosition());

            modelBackpack.Draw(shaderRefraction);

            model = glm::mat4(1);
            model = glm::translate(model, glm::vec3(2.5f, -0.01f, 1.0f));
            model = glm::scale(model, glm::vec3(0.001f));

            shaderFlower.Bind();
            shaderFlower.SetUniformMat4f("uMVP", projection * view * model);
            shaderFlower.SetUniformMat4f("uModel", model);
            shaderFlower.SetUniformMat3f("uTranInvModel", glm::transpose(glm::inverse(glm::mat3(model))));
            shaderFlower.SetUniformVec3f("uViewPos", camera.GetCameraPosition());
            shaderFlower.SetUniform3f("uDirLight.direction", -0.2f, -1.0f, -0.3f);
            shaderFlower.SetUniformVec3f("uDirLight.ambient", ambientDirlightColor);
            shaderFlower.SetUniformVec3f("uDirLight.diffuse", diffuseDirlightColor);
            shaderFlower.SetUniformVec3f("uDirLight.specular", specularDirlightColor);

            shaderFlower.SetUniformVec3f("uPointLight.position", glm::vec3(0.0f, 0.0f, 3.0f));
            shaderFlower.SetUniform1f("uPointLight.constant", 1.0f);
            shaderFlower.SetUniform1f("uPointLight.linear", 0.09f);
            shaderFlower.SetUniform1f("uPointLight.quadratic", 0.032f);
            shaderFlower.SetUniformVec3f("uPointLight.ambient", ambientPointColor);
            shaderFlower.SetUniformVec3f("uPointLight.diffuse", diffusePointColor);
            shaderFlower.SetUniformVec3f("uPointLight.specular", specularPointColor);

            Window::DisableFaceCulling();
            modelFlowers.Draw(shaderFlower);
            Window::EnableFaceCulling();

            model = glm::mat4(1);
            model = glm::translate(model, glm::vec3(3.5f, -0.01f, -1.0f));
            model = glm::scale(model, glm::vec3(0.001f));

            shaderFlower.Bind();
            shaderFlower.SetUniformMat4f("uMVP", projection* view* model);
            shaderFlower.SetUniformMat4f("uModel", model);
            shaderFlower.SetUniformMat3f("uTranInvModel", glm::transpose(glm::inverse(glm::mat3(model))));

            Window::DisableFaceCulling();
            modelFlowers.Draw(shaderFlower);
            Window::EnableFaceCulling();

            model = glm::mat4(1);
            model = glm::scale(model, glm::vec3(10.0f));

            shaderFloor.Bind();
            shaderFloor.SetUniformMat4f("uMVP", projection * view* model);

            Window::DisableFaceCulling();

            floor.Draw(shaderFloor);

            shaderGrass.Bind();
            shaderGrass.SetUniformMat4f("uVP", projection * view);

            grass.Draw(shaderGrass, 1681);

            model = glm::mat4(1);
            model = glm::translate(model, glm::vec3(-20.0f, 1.0f, 0.0f));

            shaderReflection.Bind();
            shaderReflection.SetUniformMat4f("uMVP", projection * view* model);
            shaderReflection.SetUniformMat3f("uTranInvModel", glm::transpose(glm::inverse(glm::mat3(model))));
            shaderReflection.SetUniformMat4f("uModel", model);
            shaderReflection.SetUniform1i("uSkybox", 0);
            shaderReflection.SetUniformVec3f("uCameraPos", camera.GetCameraPosition());

            reflectionPlane.Draw(shaderReflection);

            Window::EnableFaceCulling();

            glm::mat4 skyboxView = glm::mat4(glm::mat3(camera.GetViewMatrixMirror()));

            shaderSkybox.Bind();
            shaderSkybox.SetUniformMat4f("uVP", projection* skyboxView);
            shaderSkybox.SetUniform1i("uSkybox", 0);

            skybox.Draw(shaderSkybox);

            Window::DisableFaceCulling();
            Window::EnableBlending();
            Window::SetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            std::map<float, glm::vec3, std::greater<float>> sortedWindowLocations;
            for (auto& location : windowLocations)
            {
                float distance = glm::length(camera.GetCameraPosition() - location);
                sortedWindowLocations[distance] = location;
            }

            for (auto& [key, location] : sortedWindowLocations)
            {
                model = glm::mat4(1);
                model = glm::translate(model, location);
                model = glm::scale(model, glm::vec3(0.7f));

                shaderWindow.Bind();
                shaderWindow.SetUniformMat4f("uMVP", projection * view * model);

                window.Draw(shaderWindow);
            }

            Window::DisableBlending();
            Window::EnableFaceCulling();

            mirrorFB.Draw(postProcessingShader);
        }
        Window::SwapBuffers();
        Window::PollEvents();
    }

    Window::Destroy();
    Window::TerminateGLFW();

    return true;
}

bool Scene::RenderScene2(HWND hText)
{
    using namespace feng;

    m_ForcedEnd = false;
    Log::SetHandleTextBox(hText);

    Window::Initialize(1600, 1100, "FlyEngine :-)", false, 4, 3, GLFW_OPENGL_CORE_PROFILE, true, 0.03f);

    if (!Window::ValidateWindow())
        return false;

    FileSystem fileSys;

    Timer timer;

    Camera camera(0.0f, 0.0f, 5.0f);
    
    camera.AddKeyBinding(GLFW_KEY_ESCAPE, KeyActions::EXIT);
    camera.AddKeyBinding(GLFW_KEY_W, KeyActions::FORWARD);
    camera.AddKeyBinding(GLFW_KEY_A, KeyActions::LEFT);
    camera.AddKeyBinding(GLFW_KEY_S, KeyActions::BACKWARD);
    camera.AddKeyBinding(GLFW_KEY_D, KeyActions::RIGHT);
    camera.AddKeyBinding(GLFW_KEY_F11, KeyActions::FULLSCREEN);
    camera.AddMouseBinding(0, MouseActions::LOOKAROUND);
   
    std::vector<glm::vec3> cubePositions = {
        glm::vec3(-1.5f, 1.5f, 0.0f),
        glm::vec3(1.5f, 1.5f, 0.0f),
        glm::vec3(-1.5f, -1.5f, 0.0f),
        glm::vec3(1.5f, -1.5f, 0.0f)
    };

    std::vector<float> cubeVertices = {
        -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f, 1.0f,
         1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f,1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,1.0f,  1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f, -1.0f,  1.0f, -1.0f, -1.0f, 1.0f,
        -1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, -1.0f,  1.0f
    };


    VertexLayout cubeVerticesLayout;
    cubeVerticesLayout.AddVec3(0);

    Mesh cube(cubeVertices, cubeVerticesLayout);

    Shader cubeShader1(fileSys.GetExecutableDirPath() + "\\res\\shaders\\cube.shader");
    cubeShader1.BindUniformBlock("uMatrices", 0);
    Shader cubeShader2(fileSys.GetExecutableDirPath() + "\\res\\shaders\\cube2.shader");
    cubeShader2.BindUniformBlock("uMatrices", 0);
    Shader cubeShader3(fileSys.GetExecutableDirPath() + "\\res\\shaders\\cube3.shader");
    cubeShader3.BindUniformBlock("uMatrices", 0);
    Shader cubeShader4(fileSys.GetExecutableDirPath() + "\\res\\shaders\\cube4.shader");
    cubeShader4.BindUniformBlock("uMatrices", 0);

    Window::EnableFaceCulling();
    Window::SetCCWFaceCulling();

    UBO uniformBuffer(2 * sizeof(glm::mat4));
    uniformBuffer.BindBufferRange(0, 2 * sizeof(glm::mat4), 0);

    while (!Window::WindowShouldClose() && !m_ForcedEnd)
    {
        Window::WindowProcessInput(camera);

        timer.UpdateDelta();
        timer.UpdateFPS();

        Window::SetDeltaTime(timer.GetDeltaTime());
        Window::SetWindowTitle("FlyEngine : -)    FPS: " + timer.GetStringFPS());

        Window::Clear(0.0f, 0.0f, 0.0f, 1.0f, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::mat4(1);
        projection = glm::perspective(glm::radians(60.0f), static_cast<float>(Window::GetWidth()) / static_cast<float>(Window::GetHeight()), 0.1f, 100.0f);

        uniformBuffer.BufferSubData(0, sizeof(glm::mat4), 0, glm::value_ptr(projection));

        glm::mat4 view = camera.GetViewMatrix();
        uniformBuffer.BufferSubData(0, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[0]);
        cubeShader1.Bind();
        cubeShader1.SetUniformMat4f("uModel", model);

        model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[1]);
        cubeShader2.Bind();
        cubeShader2.SetUniformMat4f("uModel", model);
        
        model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[2]);
        cubeShader3.Bind();
        cubeShader3.SetUniformMat4f("uModel", model);

        model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[3]);
        cubeShader4.Bind();
        cubeShader4.SetUniformMat4f("uModel", model);

        cube.Draw(cubeShader1);
        cube.Draw(cubeShader2);
        cube.Draw(cubeShader3);
        cube.Draw(cubeShader4);

        Window::SwapBuffers();
        Window::PollEvents();
    }

    Window::Destroy();
    Window::TerminateGLFW();

    return true;
}

bool Scene::RenderScene3(HWND hText)
{
    using namespace feng;

    m_ForcedEnd = false;
    Log::SetHandleTextBox(hText);

    Window::Initialize(1600, 1100, "FlyEngine :-)", false, 4, 3, GLFW_OPENGL_CORE_PROFILE, true, 0.03f);

    if (!Window::ValidateWindow())
        return false;

    FileSystem fileSys;

    Timer timer;

    Camera camera(0.0f, 0.0f, 3.0f);

    camera.AddKeyBinding(GLFW_KEY_ESCAPE, KeyActions::EXIT);
    camera.AddKeyBinding(GLFW_KEY_W, KeyActions::FORWARD);
    camera.AddKeyBinding(GLFW_KEY_A, KeyActions::LEFT);
    camera.AddKeyBinding(GLFW_KEY_S, KeyActions::BACKWARD);
    camera.AddKeyBinding(GLFW_KEY_D, KeyActions::RIGHT);
    camera.AddKeyBinding(GLFW_KEY_F11, KeyActions::FULLSCREEN);
    camera.AddMouseBinding(0, MouseActions::LOOKAROUND);
    camera.AddScrollBinding(0, ScrollActions::ZOOM);

    std::vector<float> points = { 
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f
    };

    VBO pointsVBO(points);
    VertexLayout pointsLayout;
    pointsLayout.AddVec2(0);
    pointsLayout.AddVec3(1);
    VAO pointsVAO;
    pointsVAO.Setup(pointsVBO, pointsLayout);
    Shader shaderPoints(fileSys.GetExecutableDirPath() + "\\res\\shaders\\points.shader");

    Window::EnableFaceCulling();
    Window::SetCCWFaceCulling();


    while (!Window::WindowShouldClose() && !m_ForcedEnd)
    {
        Window::WindowProcessInput(camera);

        timer.UpdateDelta();
        timer.UpdateFPS();

        Window::SetDeltaTime(timer.GetDeltaTime());
        Window::SetWindowTitle("FlyEngine : -)    FPS: " + timer.GetStringFPS());

        Window::Clear(0.0f, 0.0f, 0.0f, 1.0f, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderPoints.Bind();
        pointsVAO.Bind();
        glDrawArrays(GL_POINTS, 0, pointsVAO.GetVerticesCount());

        Window::SwapBuffers();
        Window::PollEvents();
    }

    Window::Destroy();
    Window::TerminateGLFW();

    return true;
}

bool Scene::RenderScene4(HWND hText)
{
    using namespace feng;

    m_ForcedEnd = false;
    Log::SetHandleTextBox(hText);

    Window::Initialize(1600, 1100, "FlyEngine :-)", false, 4, 3, GLFW_OPENGL_CORE_PROFILE, true, 0.03f);

    if (!Window::ValidateWindow())
        return false;

    FileSystem fileSys;

    Timer timer;

    std::vector <float> vertices = {
        // position         // normals          //texCords;
               -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
               0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
               0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
               -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

               -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
               0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
               0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
               -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

               -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
               -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
               -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
               -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

               0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
               0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
               0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
               0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

               -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
               0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
               0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
               -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,

               -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
               0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
               0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
               -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f
    };

    std::vector<uint32_t> indices = {
         0, 1, 2,
        2, 3, 0,

        4, 5, 6,
        6, 7, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20,
    };

    VertexLayout layout;
    layout.AddVec3(0);
    layout.AddVec3(1);
    layout.AddVec2(2);

    Texture texDiff(fileSys.GetExecutableDirPath() + "\\res\\textures\\container2.png", "texture_diffuse", GL_CLAMP_TO_EDGE);
    Texture texSpecular(fileSys.GetExecutableDirPath() + "\\res\\textures\\container2_specular.png", "texture_specular", GL_CLAMP_TO_EDGE);

    std::vector<Texture> textures = { texDiff, texSpecular };

    Mesh cube(vertices, indices, textures, layout);

    std::vector<float> floorVertices = {
        1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 4.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 4.0f, 4.0f,
        -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 4.0f, 0.0f,
        -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f
    };

    VertexLayout planeVerticesLayout;
    planeVerticesLayout.AddVec3(0);
    planeVerticesLayout.AddVec3(1);
    planeVerticesLayout.AddVec2(2);

    std::vector<uint32_t> floorIndices = { 2, 1, 0, 3, 2, 0 };

    Texture floorTex(fileSys.GetExecutableDirPath() + "\\res\\textures\\floor.jpg", "texture_diffuse", GL_REPEAT);
    Texture floorTexSpecular(fileSys.GetExecutableDirPath() + "\\res\\textures\\floor_specular.jpg", "texture_specular", GL_REPEAT);
    std::vector<Texture> floorTexture = { floorTex, floorTexSpecular };

    Mesh floor(floorVertices, floorIndices, floorTexture, planeVerticesLayout);

    std::vector<std::string> skyboxTextures = {
    fileSys.GetExecutableDirPath() + "\\res\\textures\\skybox2\\px.png",
    fileSys.GetExecutableDirPath() + "\\res\\textures\\skybox2\\nx.png",
    fileSys.GetExecutableDirPath() + "\\res\\textures\\skybox2\\py.png",
    fileSys.GetExecutableDirPath() + "\\res\\textures\\skybox2\\ny.png",
    fileSys.GetExecutableDirPath() + "\\res\\textures\\skybox2\\pz.png",
    fileSys.GetExecutableDirPath() + "\\res\\textures\\skybox2\\nz.png"
    };

    CubeMap skybox(skyboxTextures);

    Shader shader(fileSys.GetExecutableDirPath() + "\\res\\shaders\\cubespecular.shader");
    shader.SetUniform1f("uMaterial.shininess", 64.0f);

    Shader shaderLight(fileSys.GetExecutableDirPath() + "\\res\\shaders\\cubelight2.shader");

    Shader shaderFloor(fileSys.GetExecutableDirPath() + "\\res\\shaders\\floorlight.shader");
    shaderFloor.SetUniform1f("uMaterial.shininess", 32.0f);

    Shader shaderSkybox(fileSys.GetExecutableDirPath() + "\\res\\shaders\\skybox.shader");

    glm::vec3 ambientDirlightColor(0.2f, 0.2f, 0.2f);
    glm::vec3 diffuseDirlightColor(0.8f, 0.4f, 0.4f);
    glm::vec3 specularDirlightColor(0.8f, 0.6f, 0.6f);

    glm::vec3 ambientPointColor(0.0f, 0.0f, 0.0f);
    glm::vec3 diffusePointColor(0.4f, 0.2f, 0.2f);
    glm::vec3 specularPointColor(0.8f, 0.8f, 0.8f);

    glm::vec3 ambientSpotlightColor(0.0f, 0.0f, 0.0f);
    glm::vec3 diffuseSpotlightColor(0.0f);
    glm::vec3 specularSpotlightColor(0.0f);

    glm::vec3 lightPosition(1.0f, 1.0f, -2.0f);

    Camera camera(0.0f, 1.0f, 5.0f, glm::vec3(0.0f, 1.0f, 0.0f), -10.0f, -90.0f, 3.0f, 45.0f);

    camera.AddKeyBinding(GLFW_KEY_ESCAPE, KeyActions::EXIT);
    camera.AddKeyBinding(GLFW_KEY_W, KeyActions::FORWARD);
    camera.AddKeyBinding(GLFW_KEY_A, KeyActions::LEFT);
    camera.AddKeyBinding(GLFW_KEY_S, KeyActions::BACKWARD);
    camera.AddKeyBinding(GLFW_KEY_D, KeyActions::RIGHT);
    camera.AddKeyBinding(GLFW_KEY_F11, KeyActions::FULLSCREEN);
    camera.AddMouseBinding(0, MouseActions::LOOKAROUND);
    camera.AddScrollBinding(0, ScrollActions::ZOOM);

    while (!Window::WindowShouldClose() && !m_ForcedEnd)
    {
        Window::WindowProcessInput(camera);

        timer.UpdateDelta();
        timer.UpdateFPS();

        Window::SetDeltaTime(timer.GetDeltaTime());
        Window::SetWindowTitle("FlyEngine : -)    FPS: " + timer.GetStringFPS());

        Window::Clear(0.0f, 0.0f, 0.0f, 1.0f, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), static_cast<float>(Window::GetWidth()) / static_cast<float>(Window::GetHeight()), 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1);

        shader.Bind();
        shader.SetUniformMat4f("uMVP", projection * view * model);
        shader.SetUniformMat4f("uModel", model);
        shader.SetUniformMat3f("uTranInvModel", glm::transpose(glm::inverse(glm::mat3(model))));
        shader.SetUniformVec3f("uViewPos", camera.GetCameraPosition());
        shader.SetUniform3f("uDirLight.direction", -0.2f, 0.1f, -1.0f);
        shader.SetUniformVec3f("uDirLight.ambient", ambientDirlightColor);
        shader.SetUniformVec3f("uDirLight.diffuse", diffuseDirlightColor);
        shader.SetUniformVec3f("uDirLight.specular", specularDirlightColor);

        shader.SetUniformVec3f("uPointLight.position", lightPosition);
        shader.SetUniform1f("uPointLight.constant", 1.0f);
        shader.SetUniform1f("uPointLight.linear", 0.009f);
        shader.SetUniform1f("uPointLight.quadratic", 0.0032f);
        shader.SetUniformVec3f("uPointLight.ambient", ambientPointColor);
        shader.SetUniformVec3f("uPointLight.diffuse", diffusePointColor);
        shader.SetUniformVec3f("uPointLight.specular", specularPointColor);

        shader.SetUniformVec3f("uSpotLight.position", camera.GetCameraPosition());
        shader.SetUniformVec3f("uSpotLight.direction", camera.GetCameraFront());
        shader.SetUniform1f("uSpotLight.cutOff", glm::cos(glm::radians(5.0f)));
        shader.SetUniform1f("uSpotLight.outerCutOff", glm::cos(glm::radians(7.5f)));
        shader.SetUniformVec3f("uSpotLight.ambient", ambientSpotlightColor);
        shader.SetUniformVec3f("uSpotLight.diffuse", diffuseSpotlightColor);
        shader.SetUniformVec3f("uSpotLight.specular", specularSpotlightColor);
        shader.SetUniform1f("uSpotLight.constant", 1.0f);
        shader.SetUniform1f("uSpotLight.linear", 0.09f);
        shader.SetUniform1f("uSpotLight.quadratic", 0.009f);

        cube.Draw(shader);

        model = glm::mat4(1);
        model = glm::translate(model, lightPosition);
        model = glm::scale(model, glm::vec3(0.1f));

        shaderLight.Bind();
        shaderLight.SetUniformMat4f("uMVP", projection* view* model);

        cube.Draw(shaderLight);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, -0.51f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f));

        shaderFloor.Bind();
        shaderFloor.SetUniformMat4f("uMVP", projection * view * model);
        shaderFloor.SetUniformMat4f("uModel", model);
        shaderFloor.SetUniformMat3f("uTranInvModel", glm::transpose(glm::inverse(glm::mat3(model))));
        shaderFloor.SetUniformVec3f("uViewPos", camera.GetCameraPosition());
        shaderFloor.SetUniform3f("uDirLight.direction", -0.2f, 0.1f, -1.0f);
        shaderFloor.SetUniformVec3f("uDirLight.ambient", ambientDirlightColor);
        shaderFloor.SetUniformVec3f("uDirLight.diffuse", diffuseDirlightColor);
        shaderFloor.SetUniformVec3f("uDirLight.specular", specularDirlightColor);
        
        shaderFloor.SetUniformVec3f("uPointLight.position", lightPosition);
        shaderFloor.SetUniform1f("uPointLight.constant", 1.0f);
        shaderFloor.SetUniform1f("uPointLight.linear", 0.009f);
        shaderFloor.SetUniform1f("uPointLight.quadratic", 0.0032f);
        shaderFloor.SetUniformVec3f("uPointLight.ambient", ambientPointColor);
        shaderFloor.SetUniformVec3f("uPointLight.diffuse", diffusePointColor);
        shaderFloor.SetUniformVec3f("uPointLight.specular", specularPointColor);
        
        shaderFloor.SetUniformVec3f("uSpotLight.position", camera.GetCameraPosition());
        shaderFloor.SetUniformVec3f("uSpotLight.direction", camera.GetCameraFront());
        shaderFloor.SetUniform1f("uSpotLight.cutOff", glm::cos(glm::radians(5.0f)));
        shaderFloor.SetUniform1f("uSpotLight.outerCutOff", glm::cos(glm::radians(7.5f)));
        shaderFloor.SetUniformVec3f("uSpotLight.ambient", ambientSpotlightColor);
        shaderFloor.SetUniformVec3f("uSpotLight.diffuse", diffuseSpotlightColor);
        shaderFloor.SetUniformVec3f("uSpotLight.specular", specularSpotlightColor);
        shaderFloor.SetUniform1f("uSpotLight.constant", 1.0f);
        shaderFloor.SetUniform1f("uSpotLight.linear", 0.09f);
        shaderFloor.SetUniform1f("uSpotLight.quadratic", 0.009f);

        floor.Draw(shaderFloor);

        glm::mat4 skyboxView = glm::mat4(glm::mat3(camera.GetViewMatrix()));

        shaderSkybox.Bind();
        shaderSkybox.SetUniformMat4f("uVP", projection * skyboxView);
        shaderSkybox.SetUniform1i("uSkybox", 0);

        skybox.Draw(shaderSkybox);

        Window::SwapBuffers();
        Window::PollEvents();
    }

    Window::Destroy();
    Window::TerminateGLFW();

    return true;
}

bool Scene::RenderScene5(HWND hText)
{
    using namespace feng;

    m_ForcedEnd = false;
    Log::SetHandleTextBox(hText);

    Window::Initialize(1600, 1100, "FlyEngine :-)", false, 4, 3, GLFW_OPENGL_CORE_PROFILE, true, 0.03f);

    if (!Window::ValidateWindow())
        return false;

    FileSystem fileSys;

    Timer timer;

    Camera camera(0.0f, 2.5f, -22.0f, glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 90.0f, 50.0f, 45.0f);

    camera.AddKeyBinding(GLFW_KEY_ESCAPE, KeyActions::EXIT);
    camera.AddKeyBinding(GLFW_KEY_W, KeyActions::FORWARD);
    camera.AddKeyBinding(GLFW_KEY_A, KeyActions::LEFT);
    camera.AddKeyBinding(GLFW_KEY_S, KeyActions::BACKWARD);
    camera.AddKeyBinding(GLFW_KEY_D, KeyActions::RIGHT);
    camera.AddKeyBinding(GLFW_KEY_F11, KeyActions::FULLSCREEN);
    camera.AddMouseBinding(0, MouseActions::LOOKAROUND);
    camera.AddScrollBinding(0, ScrollActions::ZOOM);

    std::vector<std::string> skyboxTextures = {
        fileSys.GetExecutableDirPath() + "\\res\\textures\\skybox2\\px.png",
        fileSys.GetExecutableDirPath() + "\\res\\textures\\skybox2\\nx.png",
        fileSys.GetExecutableDirPath() + "\\res\\textures\\skybox2\\py.png",
        fileSys.GetExecutableDirPath() + "\\res\\textures\\skybox2\\ny.png",
        fileSys.GetExecutableDirPath() + "\\res\\textures\\skybox2\\pz.png",
        fileSys.GetExecutableDirPath() + "\\res\\textures\\skybox2\\nz.png"
    };

    CubeMap skybox(skyboxTextures);

    Model modelPlane(fileSys.GetExecutableDirPath() + "\\res\\models\\plane\\raf_voyager_aeroplane_private_plane\\plane.obj", false);
    modelPlane.Load();

    Shader shaderSkybox(fileSys.GetExecutableDirPath() + "\\res\\shaders\\skybox.shader");
    Shader shaderPlane(fileSys.GetExecutableDirPath() + "\\res\\shaders\\plane.shader");
    shaderPlane.SetUniform1f("uMaterial.shininess", 16.0f);

    glm::vec3 ambientDirlightColor(0.15f, 0.07f, 0.07f);
    glm::vec3 diffuseDirlightColor(0.6f, 0.3f, 0.3f);
    glm::vec3 specularDirlightColor(0.8f, 0.6f, 0.6f);

    Window::SetFullscreen(true);

    Window::EnableFaceCulling();
    Window::SetCCWFaceCulling();

    float posX = 0.0f;
    float posY = 0.0f;
    float posZ = 0.0f;
    float rotation = -90.0f;
    float direction = 1.0f;

    while (!Window::WindowShouldClose() && !m_ForcedEnd)
    {
        Window::WindowProcessInput(camera);

        timer.UpdateDelta();
        timer.UpdateFPS();

        Window::SetDeltaTime(timer.GetDeltaTime());
        Window::SetWindowTitle("FlyEngine : -)    FPS: " + timer.GetStringFPS());

        Window::Clear(0.0f, 0.0f, 0.0f, 1.0f, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), static_cast<float>(Window::GetWidth()) / static_cast<float>(Window::GetHeight()), 0.1f, 10000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1);

        posZ += (timer.GetDeltaTime() * 50.0f);
        posX = static_cast<float>(cos(glfwGetTime())) * 4.0f;
        posY = static_cast<float>(sin(glfwGetTime())) * 2.0f;

        if (roundf(rotation) == 283.0f)
            direction *= (-1.0f);
        if (roundf(rotation) == -360.0f)
            direction *= (-1.0f);

        rotation += timer.GetDeltaTime() * 30.0f * direction;

        model = glm::translate(model, glm::vec3(posX, posY, posZ));
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.1f));

        shaderPlane.Bind();
        shaderPlane.SetUniformMat4f("uMVP", projection * view * model);
        shaderPlane.SetUniformMat4f("uModel", model);
        shaderPlane.SetUniformMat3f("uTranInvModel", glm::transpose(glm::inverse(glm::mat3(model))));
        shaderPlane.SetUniformVec3f("uViewPos", camera.GetCameraPosition());
        shaderPlane.SetUniform3f("uDirLight.direction", -0.2f, 0.1f, -1.0f);
        shaderPlane.SetUniformVec3f("uDirLight.ambient", ambientDirlightColor);
        shaderPlane.SetUniformVec3f("uDirLight.diffuse", diffuseDirlightColor);
        shaderPlane.SetUniformVec3f("uDirLight.specular", specularDirlightColor);

        modelPlane.Draw(shaderPlane);
        camera.ForceMove(1, timer.GetDeltaTime(), 50.0f);

        glm::mat4 skyboxView = glm::mat4(glm::mat3(camera.GetViewMatrix()));

        shaderSkybox.Bind();
        shaderSkybox.SetUniformMat4f("uVP", projection * skyboxView);
        shaderSkybox.SetUniform1i("uSkybox", 0);

        skybox.Draw(shaderSkybox);

        Window::SwapBuffers();
        Window::PollEvents();
    }

    Window::Destroy();
    Window::TerminateGLFW();

    return true;
}

bool Scene::RenderScene6(HWND hText)
{
    using namespace feng;

    m_ForcedEnd = false;
    Log::SetHandleTextBox(hText);

    Window::Initialize(1600, 1100, "FlyEngine :-)", false, 4, 3, GLFW_OPENGL_CORE_PROFILE, true, 0.03f);

    if (!Window::ValidateWindow())
        return false;

    FileSystem fileSys;

    Timer timer;

    Camera exit(0.0f, 0.0f, 0.0f);
    exit.AddKeyBinding(GLFW_KEY_ESCAPE, KeyActions::EXIT);

    std::vector<float> quadVertices = {
        -0.05f, 0.05f, 1.0f, 0.0f, 0.0f,
        0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
        -0.05f, -0.05f, 0.0f, 0.0f, 1.0f,
        0.05f, 0.05f, 0.0f, 1.0f, 1.0f
    };

    std::vector<uint32_t> quadIndices = {
        0, 1, 2,
        0, 1, 3
    };

    VertexLayout layout;
    layout.AddVec2(0);
    layout.AddVec3(1);

    std::vector<float> translations(200);
    uint32_t index = 0;
    float offset = 0.1f;

    for (int32_t y = -10; y < 10; y += 2)
    {
        for (int32_t x = -10; x < 10; x += 2)
        {
            translations[index++] = ((float)x / 10.0f + offset);
            translations[index++] = ((float)y / 10.0f + offset);
        }
    }

    Mesh quad(quadVertices, quadIndices, layout);

    VBO instanceQuadVBO(translations);
    instanceQuadVBO.SetupInstanceVBO(quad.GetVAO(), 2, 2, 1);

    Shader quadShader(fileSys.GetExecutableDirPath() + "\\res\\shaders\\quad.shader");

    while (!Window::WindowShouldClose() && !m_ForcedEnd)
    {
        Window::WindowProcessInput(exit);

        timer.UpdateDelta();
        timer.UpdateFPS();

        Window::SetDeltaTime(timer.GetDeltaTime());
        Window::SetWindowTitle("FlyEngine : -)    FPS: " + timer.GetStringFPS());

        Window::Clear(0.0f, 0.0f, 0.0f, 1.0f, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        quad.Draw(quadShader, 100);

        Window::SwapBuffers();
        Window::PollEvents();
    }

    Window::Destroy();
    Window::TerminateGLFW();

    return true;
}

bool Scene::RenderScene7(HWND hText)
{
    using namespace feng;

    m_ForcedEnd = false;
    Log::SetHandleTextBox(hText);

    Window::Initialize(1600, 1100, "FlyEngine :-)", false, 4, 3, GLFW_OPENGL_CORE_PROFILE, true, 0.03f);

    if (!Window::ValidateWindow())
        return false;

    FileSystem fileSys;

    Timer timer;

    Camera camera(0.0f, 100.0f, 400.0f, glm::vec3(0.0f, 1.0f, 0.0f), -15.0f, -90.0f, 30.0f, 80.0f);

    camera.AddKeyBinding(GLFW_KEY_ESCAPE, KeyActions::EXIT);
    camera.AddKeyBinding(GLFW_KEY_W, KeyActions::FORWARD);
    camera.AddKeyBinding(GLFW_KEY_A, KeyActions::LEFT);
    camera.AddKeyBinding(GLFW_KEY_S, KeyActions::BACKWARD);
    camera.AddKeyBinding(GLFW_KEY_D, KeyActions::RIGHT);
    camera.AddKeyBinding(GLFW_KEY_F11, KeyActions::FULLSCREEN);
    camera.AddMouseBinding(0, MouseActions::LOOKAROUND);
    camera.AddScrollBinding(0, ScrollActions::ZOOM);

    Window::EnableFaceCulling();
    Window::SetCCWFaceCulling();
    Window::SetFullscreen(true);

    Shader shaderSkybox(fileSys.GetExecutableDirPath() + "\\res\\shaders\\skybox.shader");
    Shader shaderPlanet(fileSys.GetExecutableDirPath() + "\\res\\shaders\\planet.shader");
    Shader shaderAsteroid(fileSys.GetExecutableDirPath() + "\\res\\shaders\\asteroid.shader");

    std::vector<std::string> skyboxTextures = {
        fileSys.GetExecutableDirPath() + "\\res\\textures\\skybox3\\right.png",
        fileSys.GetExecutableDirPath() + "\\res\\textures\\skybox3\\left.png",
        fileSys.GetExecutableDirPath() + "\\res\\textures\\skybox3\\top.png",
        fileSys.GetExecutableDirPath() + "\\res\\textures\\skybox3\\bottom.png",
        fileSys.GetExecutableDirPath() + "\\res\\textures\\skybox3\\front.png",
        fileSys.GetExecutableDirPath() + "\\res\\textures\\skybox3\\back.png"
    };

    CubeMap skybox(skyboxTextures);

    uint32_t amount = 20000;

    std::vector<float> modelMatrices(amount * 16);
    srand(static_cast<uint32_t>(glfwGetTime()));

    float radius = 150.0f;
    float offset = 25.0f;
    uint32_t iterator = 0;
    glm::mat4 movement = glm::mat4(1);
    glm::mat4 rotation = glm::mat4(1);

    for (uint32_t i = 0; i < amount; i++)
    {
        glm::mat4 model = glm::mat4(1);

        float angle = static_cast<float>(i) / static_cast<float>(amount) * 360.0f;
        float displacement = (rand() % static_cast<int32_t>(200.0f * offset)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % static_cast<int32_t>(200.0f * offset)) / 100.0f - offset;
        float y = displacement * 0.1f;
        displacement = (rand() % static_cast<int32_t>(200.0f * offset)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;

        model = glm::translate(model, glm::vec3(x, y, z));

        float scale = static_cast<float>((rand() % 20) / 100.0 + 0.05);
        model = glm::scale(model, glm::vec3(scale));

        float rotAngle = static_cast<float>(rand() % 360);
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        modelMatrices[iterator++] = model[0][0];
        modelMatrices[iterator++] = model[0][1];
        modelMatrices[iterator++] = model[0][2];
        modelMatrices[iterator++] = model[0][3];

        modelMatrices[iterator++] = model[1][0];
        modelMatrices[iterator++] = model[1][1];
        modelMatrices[iterator++] = model[1][2];
        modelMatrices[iterator++] = model[1][3];

        modelMatrices[iterator++] = model[2][0];
        modelMatrices[iterator++] = model[2][1];
        modelMatrices[iterator++] = model[2][2];
        modelMatrices[iterator++] = model[2][3];

        modelMatrices[iterator++] = model[3][0];
        modelMatrices[iterator++] = model[3][1];
        modelMatrices[iterator++] = model[3][2];
        modelMatrices[iterator++] = model[3][3];
    }


    Model modelPlanet(fileSys.GetExecutableDirPath() + "\\res\\models\\planet\\planet.obj");
    Model modelAsteroid(fileSys.GetExecutableDirPath() + "\\res\\models\\rock\\rock.obj", true, modelMatrices);

    modelPlanet.Load();
    modelAsteroid.Load();

    while (!Window::WindowShouldClose() && !m_ForcedEnd)
    {
        Window::WindowProcessInput(camera);

        timer.UpdateDelta();
        timer.UpdateFPS();

        Window::SetDeltaTime(timer.GetDeltaTime());
        Window::SetWindowTitle("FlyEngine : -)    FPS: " + timer.GetStringFPS());

        Window::Clear(0.0f, 0.0f, 0.0f, 1.0f, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), static_cast<float>(Window::GetWidth()) / static_cast<float>(Window::GetHeight()), 0.1f, 10000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1);
        
        model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
        model = glm::rotate(model, static_cast<float>(glfwGetTime()) * 0.04f, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, -1.0f, 1.0f));
        model = glm::scale(model, glm::vec3(12.0f));

        shaderPlanet.Bind();
        shaderPlanet.SetUniformMat4f("uMVP", projection * view * model);

        modelPlanet.Draw(shaderPlanet);

        movement = glm::mat4(1);
        movement = glm::rotate(movement, static_cast<float>(glfwGetTime() * 0.1), glm::vec3(0.0f, 1.0f, 0.0f));

        rotation = glm::mat4(1);
        rotation = glm::rotate(rotation, static_cast<float>(glfwGetTime() * 10.), glm::vec3(0.4f, 0.6f, 0.8f));

        shaderAsteroid.Bind();
        shaderAsteroid.SetUniformMat4f("uVP", projection * view);
        shaderAsteroid.SetUniformMat4f("uMovement", movement);
        shaderAsteroid.SetUniformMat4f("uRotation", rotation);

        modelAsteroid.Draw(shaderAsteroid, amount);

        glm::mat4 skyboxView = glm::mat4(glm::mat3(camera.GetViewMatrix()));

        shaderSkybox.Bind();
        shaderSkybox.SetUniformMat4f("uVP", projection * skyboxView);
        shaderSkybox.SetUniform1i("uSkybox", 0);

        skybox.Draw(shaderSkybox);

        Window::SwapBuffers();
        Window::PollEvents();
    }

    Window::Destroy();
    Window::TerminateGLFW();

    return true;
}

bool Scene::RenderScene8(HWND hText)
{
    using namespace feng;

    m_ForcedEnd = false;
    Log::SetHandleTextBox(hText);

    Window::Initialize(1600, 1100, "FlyEngine :-)", false, 4, 3, GLFW_OPENGL_CORE_PROFILE, true, 0.03f);

    if (!Window::ValidateWindow())
        return false;

    FileSystem fileSys;

    Timer timer;

    Camera camera(-10.0f, 1.5f, -30.0f, glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 90.0f, 5.0f, 45.0f);

    camera.AddKeyBinding(GLFW_KEY_ESCAPE, KeyActions::EXIT);
    camera.AddKeyBinding(GLFW_KEY_W, KeyActions::FORWARD);
    camera.AddKeyBinding(GLFW_KEY_A, KeyActions::LEFT);
    camera.AddKeyBinding(GLFW_KEY_S, KeyActions::BACKWARD);
    camera.AddKeyBinding(GLFW_KEY_D, KeyActions::RIGHT);
    camera.AddKeyBinding(GLFW_KEY_F11, KeyActions::FULLSCREEN);
    camera.AddMouseBinding(0, MouseActions::LOOKAROUND);
    camera.AddScrollBinding(0, ScrollActions::ZOOM);

    Model modelTitan(fileSys.GetExecutableDirPath() + "\\res\\models\\titan\\untitled.obj", false);
    modelTitan.Load();

    Shader shaderTitan(fileSys.GetExecutableDirPath() + "\\res\\shaders\\titan.shader");
    shaderTitan.SetUniform1f("uMaterial.shininess", 32.0f);

    glm::vec3 ambientDirlightColor(0.01f, 0.01f, 0.4f);
    glm::vec3 diffuseDirlightColor(0.1f, 0.1f, 0.4f);
    glm::vec3 specularDirlightColor(0.3f, 0.3f, 0.3f);

    glm::vec3 ambientSpotlightColor(0.0f, 0.0f, 0.0f);
    glm::vec3 diffuseSpotlightColor(0.6f);
    glm::vec3 specularSpotlightColor(0.6f);

    Window::SetFullscreen(true);

    Window::EnableFaceCulling();
    Window::SetCCWFaceCulling();

    float posX = 0.0f;
    float it = 0.0f;
    float elapsedTime = 0.0f;

    while (!Window::WindowShouldClose() && !m_ForcedEnd)
    {
        Window::WindowProcessInput(camera);

        timer.UpdateDelta();
        timer.UpdateFPS();

        Window::SetDeltaTime(timer.GetDeltaTime());
        Window::SetWindowTitle("FlyEngine : -)    FPS: " + timer.GetStringFPS());

        Window::Clear(0.0f, 0.0f, std::powf(0.15f, 2.2f), 1.0f, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), static_cast<float>(Window::GetWidth()) / static_cast<float>(Window::GetHeight()), 0.1f, 10000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1);

        posX -= (timer.GetDeltaTime());

        model = glm::translate(model, glm::vec3(posX, 0.0f, 0.0f));

        shaderTitan.Bind();
        shaderTitan.SetUniformMat4f("uMVP", projection * view * model);
        shaderTitan.SetUniformMat4f("uModel", model);
        shaderTitan.SetUniformMat3f("uTranInvModel", glm::transpose(glm::inverse(glm::mat3(model))));
        shaderTitan.SetUniformVec3f("uViewPos", camera.GetCameraPosition());
        shaderTitan.SetUniform3f("uDirLight.direction", 0.2f, 1.0f, 0.1f);
        shaderTitan.SetUniformVec3f("uDirLight.ambient", ambientDirlightColor);
        shaderTitan.SetUniformVec3f("uDirLight.diffuse", diffuseDirlightColor);
        shaderTitan.SetUniformVec3f("uDirLight.specular", specularDirlightColor);

        shaderTitan.SetUniformVec3f("uSpotLight.position", camera.GetCameraPosition());
        shaderTitan.SetUniformVec3f("uSpotLight.direction", camera.GetCameraFront());
        shaderTitan.SetUniform1f("uSpotLight.cutOff", glm::cos(glm::radians(2.5f)));
        shaderTitan.SetUniform1f("uSpotLight.outerCutOff", glm::cos(glm::radians(5.0f)));
        shaderTitan.SetUniformVec3f("uSpotLight.ambient", ambientSpotlightColor);
        shaderTitan.SetUniformVec3f("uSpotLight.diffuse", diffuseSpotlightColor);
        shaderTitan.SetUniformVec3f("uSpotLight.specular", specularSpotlightColor);
        shaderTitan.SetUniform1f("uSpotLight.constant", 1.0f);
        shaderTitan.SetUniform1f("uSpotLight.linear", 0.09f);
        shaderTitan.SetUniform1f("uSpotLight.quadratic", 0.009f);

        elapsedTime += timer.GetDeltaTime();

        if (elapsedTime > 30.0f)
        {
            shaderTitan.SetUniform1f("uMagnitude", it);

            it += 0.01f;
        }
        else
        {
            shaderTitan.SetUniform1f("uMagnitude", 0.0f);
        }

        modelTitan.Draw(shaderTitan);

        Window::SwapBuffers();
        Window::PollEvents();
    }

    Window::Destroy();
    Window::TerminateGLFW();

    return true;
}

bool Scene::RenderScene9(HWND hText)
{
    using namespace feng;

    m_ForcedEnd = false;
    Log::SetHandleTextBox(hText);

    Window::Initialize(1600, 1100, "FlyEngine :-)", false, 4, 3, GLFW_OPENGL_CORE_PROFILE, true, 0.03f);

    if (!Window::ValidateWindow())
        return false;

    FileSystem fileSys;

    Timer timer;

    Camera camera(-1.5f, 2.5f, 6.0f, glm::vec3(0.0f, 1.0f, 0.0f), -15.0f, -80.0f, 3.0f, 45.0f);
     
    camera.AddKeyBinding(GLFW_KEY_ESCAPE, KeyActions::EXIT);
    camera.AddKeyBinding(GLFW_KEY_W, KeyActions::FORWARD);
    camera.AddKeyBinding(GLFW_KEY_A, KeyActions::LEFT);
    camera.AddKeyBinding(GLFW_KEY_S, KeyActions::BACKWARD);
    camera.AddKeyBinding(GLFW_KEY_D, KeyActions::RIGHT);
    camera.AddKeyBinding(GLFW_KEY_F11, KeyActions::FULLSCREEN);
    camera.AddMouseBinding(0, MouseActions::LOOKAROUND);
    camera.AddScrollBinding(0, ScrollActions::ZOOM);

    std::vector<float> cubeVertices = {
        -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f, 1.0f,
         1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f,1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,1.0f,  1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f, -1.0f,  1.0f, -1.0f, -1.0f, 1.0f,
        -1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, -1.0f,  1.0f
    };


    VertexLayout cubeVerticesLayout;
    cubeVerticesLayout.AddVec3(0);

    Mesh cube(cubeVertices, cubeVerticesLayout);

    std::vector<float> planeVertices = {
        -10.0f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 10.0f,
        -10.0f, 0.0f, 10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        10.0f, 0.0f, 10.0f, 0.0f, 1.0f, 0.0f, 10.0f, 0.0f,
        10.0f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 10.0f, 10.0f
    };

    VertexLayout planeVerticesLayout;
    planeVerticesLayout.AddVec3(0);
    planeVerticesLayout.AddVec3(1);
    planeVerticesLayout.AddVec2(2);

    std::vector<uint32_t> planeIndices = {
        2, 1, 0,
        3, 2, 0
    };

    Texture planeTexture(fileSys.GetExecutableDirPath() + "\\res\\textures\\wood.png", "texture_diffuse", GL_REPEAT);
    std::vector<Texture> planeVecTexture = { planeTexture };

    Mesh plane(planeVertices, planeIndices, planeVecTexture, planeVerticesLayout);

    Shader planeShader(fileSys.GetExecutableDirPath() + "\\res\\shaders\\floorBlin.shader");
    planeShader.SetUniform1f("uMaterial.shininess", 32.0f);
    planeShader.SetUniformVec3f("uLightPos", glm::vec3(0.0f, 0.5f, 0.0f));

    Shader lightSourceShader(fileSys.GetExecutableDirPath() + "\\res\\shaders\\blinSource.shader");

    Window::EnableFaceCulling();
    Window::SetCWFaceCulling();

    while (!Window::WindowShouldClose() && !m_ForcedEnd)
    {
        Window::WindowProcessInput(camera);

        timer.UpdateDelta();
        timer.UpdateFPS();

        Window::SetDeltaTime(timer.GetDeltaTime());
        Window::SetWindowTitle("FlyEngine : -)    FPS: " + timer.GetStringFPS());

        Window::Clear(0.0f, 0.0f, 0.0f, 1.0f, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), static_cast<float>(Window::GetWidth()) / static_cast<float>(Window::GetHeight()), 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        planeShader.Bind();
        planeShader.SetUniformMat4f("uMVP", projection * view);
        planeShader.SetUniformVec3f("uViewPos", camera.GetCameraPosition());

        plane.Draw(planeShader);

        glm::mat4 model(1);
        model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
        model = glm::scale(model, glm::vec3(0.05f));

        lightSourceShader.Bind();
        lightSourceShader.SetUniformMat4f("uMVP", projection * view * model);

        cube.Draw(lightSourceShader);

        Window::SwapBuffers();
        Window::PollEvents();
    }

    Window::Destroy();
    Window::TerminateGLFW();

    return true;
}

bool Scene::RenderScene10(HWND hText)
{
    using namespace feng;

    m_ForcedEnd = false;
    Log::SetHandleTextBox(hText);

    Window::Initialize(1600, 1100, "FlyEngine :-)", false, 4, 3, GLFW_OPENGL_CORE_PROFILE, true, 0.03f);

    if (!Window::ValidateWindow())
        return false;

    FileSystem fileSys;

    Timer timer;

    Camera camera(-1.5f, 2.5f, 6.0f, glm::vec3(0.0f, 1.0f, 0.0f), -15.0f, -80.0f, 3.0f, 45.0f);

    camera.AddKeyBinding(GLFW_KEY_ESCAPE, KeyActions::EXIT);
    camera.AddKeyBinding(GLFW_KEY_W, KeyActions::FORWARD);
    camera.AddKeyBinding(GLFW_KEY_A, KeyActions::LEFT);
    camera.AddKeyBinding(GLFW_KEY_S, KeyActions::BACKWARD);
    camera.AddKeyBinding(GLFW_KEY_D, KeyActions::RIGHT);
    camera.AddKeyBinding(GLFW_KEY_F11, KeyActions::FULLSCREEN);
    camera.AddMouseBinding(0, MouseActions::LOOKAROUND);
    camera.AddScrollBinding(0, ScrollActions::ZOOM);

    std::vector<float> cubeVertices = {
        // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
             // bottom face
             -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
              1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
              1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
              1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
             -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             // top face
             -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
              1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
              1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
              1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
    };

    VertexLayout cubeVerticesLayout;
    cubeVerticesLayout.AddVec3(0);
    cubeVerticesLayout.AddVec3(1);
    cubeVerticesLayout.AddVec2(2);

    Mesh cube(cubeVertices, cubeVerticesLayout);

    std::vector<float> planeVertices = {
        -25.0f, 0.0f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f,
        -25.0f, 0.0f, 25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        25.0f, 0.0f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
        25.0f, 0.0f, -25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 25.0f
    };

    VertexLayout planeVerticesLayout;
    planeVerticesLayout.AddVec3(0);
    planeVerticesLayout.AddVec3(1);
    planeVerticesLayout.AddVec2(2);

    std::vector<uint32_t> planeIndices = {
        0, 1, 2,
        0, 2, 3
    };

    Texture planeTexture(fileSys.GetExecutableDirPath() + "\\res\\textures\\wood.png", "texture_diffuse", GL_REPEAT);
    std::vector<Texture> planeVecTexture = { planeTexture };

    Mesh plane(planeVertices, planeIndices, planeVecTexture, planeVerticesLayout);

    DepthMap dMap(2048, 2048);

    float nearPlane = 1.0f;
    float farPlane = 6.0f;

    float beta = 10.0f;
    float phase = 0.0f;
    float direction = 1.0f;
    bool directionChange1 = true;
    bool directionChange2 = false;

    glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);

    Shader depthMap(fileSys.GetExecutableDirPath() + "\\res\\shaders\\depthmap.shader");
    Shader depthMapVisualDebug(fileSys.GetExecutableDirPath() + "\\res\\shaders\\depthmapdebug.shader");
    depthMapVisualDebug.SetUniform1i("uDepthMap", 1);
    Shader sceneWShadowMap(fileSys.GetExecutableDirPath() + "\\res\\shaders\\scenewshadows.shader");
    sceneWShadowMap.SetUniform1i("uDepthMap", 1);
    sceneWShadowMap.SetUniform1f("uMaterial.shininess", 64.0f);
    Shader sunCube(fileSys.GetExecutableDirPath() + "\\res\\shaders\\minsun.shader");

    Window::EnableFaceCulling();
    Window::SetCCWFaceCulling();

    while (!Window::WindowShouldClose() && !m_ForcedEnd)
    {
        Window::WindowProcessInput(camera);

        timer.UpdateDelta();
        timer.UpdateFPS();

        Window::SetDeltaTime(timer.GetDeltaTime());
        Window::SetWindowTitle("FlyEngine : -)    FPS: " + timer.GetStringFPS());

        Window::Clear(0.01f, 0.01f, 0.01f, 1.0f, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, (5.52f - lightPos.y) * farPlane);
        glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 lightSpaceMatrix = lightProjection * lightView;

        glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), static_cast<float>(Window::GetWidth()) / static_cast<float>(Window::GetHeight()), 0.1f, 200.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1);
        
        dMap.Bind();
        Window::SetViewport(0, 0, dMap.GetTexWidth(), dMap.GetTexHeight());
        Window::Clear(GL_DEPTH_BUFFER_BIT);

        /*render scene from light perspective for depth map*/

        model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));

        depthMap.Bind();
        depthMap.SetUniformMat4f("uMVP", lightSpaceMatrix * model);

        plane.Draw(depthMap);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f));

        depthMap.Bind();
        depthMap.SetUniformMat4f("uMVP", lightSpaceMatrix * model);

        cube.Draw(depthMap);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.5f));

        depthMap.Bind();
        depthMap.SetUniformMat4f("uMVP", lightSpaceMatrix * model);

        cube.Draw(depthMap);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0f));
        model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));
        model = glm::scale(model, glm::vec3(0.25f));

        depthMap.Bind();
        depthMap.SetUniformMat4f("uMVP", lightSpaceMatrix * model);

        cube.Draw(depthMap);
        
        dMap.Unbind();

        Window::SetViewport(0, 0, Window::GetWidth(), Window::GetHeight());
        Window::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /*render scene using depth map as a shadow map*/

        dMap.BindTex(1);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));

        sceneWShadowMap.Bind();
        sceneWShadowMap.SetUniformVec3f("uLightPos", lightPos);
        sceneWShadowMap.SetUniformVec3f("uViewPos", camera.GetCameraPosition());
        sceneWShadowMap.SetUniformMat4f("uMVP", projection * view * model);
        sceneWShadowMap.SetUniformMat4f("uModel", model);
        sceneWShadowMap.SetUniformMat3f("uTranInvModel", glm::transpose(glm::inverse(glm::mat3(model))));
        sceneWShadowMap.SetUniformMat4f("uLightSpaceMatrix", lightSpaceMatrix);

        plane.Draw(sceneWShadowMap);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f));

        sceneWShadowMap.Bind();
        sceneWShadowMap.SetUniformMat4f("uMVP", projection * view * model);
        sceneWShadowMap.SetUniformMat4f("uModel", model);
        sceneWShadowMap.SetUniformMat3f("uTranInvModel", glm::transpose(glm::inverse(glm::mat3(model))));
        sceneWShadowMap.SetUniformMat4f("uLightSpaceMatrix", lightSpaceMatrix);

        cube.Draw(sceneWShadowMap);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.5f));

        sceneWShadowMap.Bind();
        sceneWShadowMap.SetUniformMat4f("uMVP", projection * view * model);
        sceneWShadowMap.SetUniformMat4f("uModel", model);
        sceneWShadowMap.SetUniformMat3f("uTranInvModel", glm::transpose(glm::inverse(glm::mat3(model))));
        sceneWShadowMap.SetUniformMat4f("uLightSpaceMatrix", lightSpaceMatrix);

        cube.Draw(sceneWShadowMap);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0f));
        model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));
        model = glm::scale(model, glm::vec3(0.25f));

        sceneWShadowMap.Bind();
        sceneWShadowMap.SetUniformMat4f("uMVP", projection * view * model);
        sceneWShadowMap.SetUniformMat4f("uModel", model);
        sceneWShadowMap.SetUniformMat3f("uTranInvModel", glm::transpose(glm::inverse(glm::mat3(model))));
        sceneWShadowMap.SetUniformMat4f("uLightSpaceMatrix", lightSpaceMatrix);

        cube.Draw(sceneWShadowMap);

        /*render minecraft like sun xd*/

        beta += timer.GetDeltaTime() * 0.1;

        if (beta > glm::radians(0.0f) && beta < glm::radians(180.0f))
        {
            phase = 0.0f;
        }
        else
        {
            phase = glm::radians(180.0f);
        }

        if (lightPos.y <= -5.0f && directionChange1)
        {
            direction *= -1.0f;
            directionChange1 = false;
            directionChange2 = true;
        }
        if (lightPos.y >= 4.525f && directionChange2)
        {
            direction *= -1.0f;
            directionChange1 = true;
            directionChange2 = false;
        }

        lightPos.x = 5.025f * direction * static_cast<float>(sin(beta + phase)) * static_cast<float>(cos(beta));
        lightPos.z = 5.025f * direction * static_cast<float>(sin(beta + phase)) * static_cast<float>(sin(beta));
        lightPos.y = 5.025f * static_cast<float>(cos(beta + phase)) - 0.5f;

        model = glm::mat4(1);
        model = glm::translate(model, glm::normalize(lightPos) * 100.0f);
        model = glm::scale(model, glm::vec3(10.0f));

        sunCube.Bind();
        sunCube.SetUniformMat4f("uMVP", projection * view * model);
        cube.Draw(sunCube);

        /*optionally render depth map to the screen for visual debug*/

        //dMap.Draw(depthMapVisualDebug, 1);
        
        Window::SwapBuffers();
        Window::PollEvents();
    }

    Window::Destroy();
    Window::TerminateGLFW();

    return true;
}

bool Scene::RenderScene11(HWND hText)
{
    using namespace feng;

    return true;
}

bool Scene::RenderScene12(HWND hText)
{
    using namespace feng;

    return true;
}

bool Scene::RenderScene13(HWND hText)
{
    using namespace feng;

    return true;
}

bool Scene::RenderScene14(HWND hText)
{
    using namespace feng;

    return true;
}

bool Scene::RenderScene15(HWND hText)
{
    using namespace feng;

    return true;
}

bool Scene::RenderScene16(HWND hText)
{
    using namespace feng;

    return true;
}

void Scene::ForceEnd()
{
    m_ForcedEnd = true;
}



