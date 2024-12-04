#pragma once

#include "Window.h"

#include "Model.h"
#include "Camera.h"
#include "FrameBuffer.h"
#include "UniformBuffer.h"
#include "DepthMap.h"
#include "DepthMapEx.h"
#include "Skybox.h"
#include "BloomEffect.h"

#include "Log.h"

class Scene
{
private:
	static bool m_ForcedEnd;
public:
	static bool RenderScene1(HWND hText);

	static bool RenderScene2(HWND hText);

	static bool RenderScene3(HWND hText);

	static bool RenderScene4(HWND hText);

	static bool RenderScene5(HWND hText);

	static bool RenderScene6(HWND hText);

	static bool RenderScene7(HWND hText);

	static bool RenderScene8(HWND hText);

	static bool RenderScene9(HWND hText);

	static bool RenderScene10(HWND hText);

	static bool RenderScene11(HWND hText);

	static bool RenderScene12(HWND hText);

	static bool RenderScene13(HWND hText);

	static bool RenderScene14(HWND hText);

	static bool RenderScene15(HWND hText);

	static bool RenderScene16(HWND hText);

	static bool RenderScene17(HWND hText);

	static bool RenderScene18(HWND hText);

	static bool RenderScene19(HWND hText);

	static void ForceEnd();
};