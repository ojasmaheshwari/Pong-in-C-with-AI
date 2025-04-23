#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <SFML/Graphics.hpp>
#include <iostream>

void Imgui_init_once()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplOpenGL3_Init("#version 130");
}

void pong_engine(float &playerHeight, float &ballSpeed, float &movementSpeed, int &MAX_FPS, bool &competitive, bool &gamePaused, float ball_x, float ball_y, float count, float predicted_y, bool &bot)
{
    // std::cout << "x: " << ball_x << '\n';
    if (gamePaused)
    {
        ImGui::Begin("Pong");
        if (ImGui::Button("PLAY", {100, 50}))
            gamePaused = false;
        ImGui::End();

        ImGui::Begin("Inbuilt Pong Engine");
        ImGui::Text("Adjust ingame values (creative mode)");
        ImGui::Value("ball x: ", ball_x);
        ImGui::Value("ball y: ", ball_y);
        if (bot)
            ImGui::Value("Bot predicted y: ", predicted_y);
        ImGui::SliderFloat("Player Height", &(playerHeight), 0, 300);
        ImGui::SliderFloat("Ball speed", &(ballSpeed), 0, 20);
        ImGui::SliderFloat("Player speed", &(movementSpeed), 0, 200);
        ImGui::SliderInt("Max FPS", &(MAX_FPS), 0, 200);
        ImGui::Checkbox("Competitve", &competitive);
        ImGui::Checkbox("Lynx bot", &bot);
        ImGui::End();
        
    }
}