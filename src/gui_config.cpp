// gui_config.cpp  ImGui config panel (Chinese UI)
#include "gui_config.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <cstdio>

bool GUI_ShowConfigPanel(BlackholeConfig& cfg) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DECORATED, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* win = glfwCreateWindow(520, 420, "Black Hole - Config", nullptr, nullptr);
    if (!win) return true;
    glfwMakeContextCurrent(win);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = nullptr;
    ImGui::StyleColorsDark();

    // Load Chinese font
    static const ImWchar ranges[] = {
        0x0020, 0x00FF,
        0x4E00, 0x9FFF,
        0,
    };
    io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/msyh.ttc", 18.0f, nullptr, ranges);
    io.Fonts->Build();

    ImGui_ImplGlfw_InitForOpenGL(win, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    const char* modes[] = { "始终显示", "空闲检测" };

    bool done = false;
    while (!glfwWindowShouldClose(win) && !done) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(520, 420));
        ImGui::Begin("黑洞设置", nullptr,
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

        ImGui::TextColored(ImVec4(0.4f, 0.7f, 1.0f, 1.0f), "黑洞桌面叠加效果");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("模式:");
        ImGui::SameLine();
        ImGui::Combo("##mode", &cfg.mode, modes, 2);

        if (cfg.mode == 1) {
            ImGui::Spacing();
            ImGui::Text("空闲超时: %d 秒 (%d 分钟)", cfg.idleSec, cfg.idleSec / 60);
            ImGui::SliderInt("##timeout", &cfg.idleSec, 10, 1800);
        }

        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::SetCursorPosX(200);
        if (ImGui::Button("启  动", ImVec2(120, 40))) {
            cfg.confirmed = true;
            done = true;
        }

        ImGui::Spacing();
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "ESC 退出 | 点击启动运行黑洞");

        ImGui::End();
        ImGui::Render();

        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(win);

        if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            break;
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(win);

    return cfg.confirmed;
}