#include "cmath"
#include "raylib.h"
#include <iostream>

const int screenWidth = 800;
const int screenHeight = 640;

const float stringLength = 275;
const float gravity = 9810;

using std::cout;
using std::endl;

void drawPendulum(float theta) {
  //          x            y
  DrawLine(screenWidth / 2, 0,
           stringLength * sinf(theta) + (float)screenWidth / 2,
           stringLength * cosf(theta), WHITE);

  DrawCircle(stringLength * sinf(theta) + (float)screenWidth / 2,
             stringLength * cosf(theta), 20.0, RED);
}

void calculateNewTheta(float &theta, float &d_theta, float &dd_theta) {
  dd_theta = -1 * (gravity / stringLength) * sinf(theta);
  d_theta = d_theta + dd_theta * GetFrameTime();
  theta = theta + d_theta * GetFrameTime();
  cout << theta << endl;
}

void writeOscillations(float theta, float &previous_theta, int &oscillations) {
  if (theta > 0 && previous_theta < 0) {
    oscillations++;
  }
  DrawText(TextFormat("OSCILLATIONS: %d", oscillations), 100, 100, 50, WHITE);
  previous_theta = theta;
}

int main() {
  InitWindow(screenWidth, screenHeight, "pendulum");
  SetTargetFPS(60);
  float theta = 45 * DEG2RAD;
  float d_theta = 0;
  float dd_theta = 0;
  float previous_theta = theta;
  int oscillations = 0;

  while (!WindowShouldClose() && GetTime() < 10) {
    BeginDrawing();
    // pendulum draw karna hai
    drawPendulum(theta);
    calculateNewTheta(theta, d_theta, dd_theta);
    writeOscillations(theta, previous_theta, oscillations);
    // physics calculate karna hai

    ClearBackground(BLACK);
    EndDrawing();
  }
  cout << "OSCILLATIONS: " << oscillations << endl;
  cout << "time: " << GetTime() << endl;
  CloseWindow();

  return 0;
}
