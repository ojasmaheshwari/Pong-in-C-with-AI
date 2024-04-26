#pragma once

void Imgui_init_once();
void pong_engine(int &playerHeight, float &ballSpeed, float &movementSpeed, int &MAX_FPS, bool &competitive, bool &gamePaused, float ball_x, float ball_y, float count, float predicted_y, bool &bot);