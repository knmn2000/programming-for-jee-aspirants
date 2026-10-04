#include "raylib.h"
#include <cmath>
#include <vector>

using namespace std;

// structs.

const int screenWidth = 600;
const int screenHeight = 400;
struct Body {
  Vector2 position;
  Color color;
  int radius;
  int mass;
  float vx = 0;
  float vy = 0;
  float ax = 0;
  float ay = 0;

  void draw() const { DrawCircleV(position, radius, color); }
};

void drawBodies(vector<Body> &bodies) {
  for (const Body &b : bodies) {
    b.draw();
  }
}

void handleEdgeCondition(vector<Body> &bodies) {
  for (int i = 0; i < bodies.size(); i++) {
    // velocity
    bodies[i].vx += bodies[i].ax * GetFrameTime();
    bodies[i].vy += bodies[i].ay * GetFrameTime();
    // position
    bodies[i].position.x += bodies[i].vx * GetFrameTime();
    bodies[i].position.y += bodies[i].vy * GetFrameTime();

    // rightedge x = screenwidth
    if (bodies[i].position.x >= screenWidth - bodies[i].radius) {
      bodies[i].position.x = screenWidth - bodies[i].radius;
      bodies[i].vx = -bodies[i].vx;
    }
    // leftedge
    if (bodies[i].position.x - bodies[i].radius <= 0) {
      bodies[i].position.x = bodies[i].radius;
      bodies[i].vx = -bodies[i].vx;
    }
    // top
    if (bodies[i].position.y > screenHeight - bodies[i].radius) {
      bodies[i].position.y = screenHeight - bodies[i].radius;
      bodies[i].vy = -bodies[i].vy;
    }
    // bottom
    if (bodies[i].position.y + bodies[i].radius <= 0) {
      bodies[i].position.y = bodies[i].radius;
      bodies[i].vy = -bodies[i].vy;
    }
  }
}

void computePhysics(vector<Body> &bodies) {
  for (int i = 0; i < bodies.size(); i++) {
    // reset acc
    bodies[i].ax = 0;
    bodies[i].ay = 0;
  }

  for (int i = 0; i < bodies.size(); i++) {
    for (int j = i + 1; j < bodies.size(); j++) {
      float dx = bodies[j].position.x - bodies[i].position.x;
      float dy = bodies[j].position.y - bodies[i].position.y;

      float distance_sq = dx * dx + dy * dy + 150;
      float distance = sqrt(distance_sq);
      // f = gM1M2/r^2
      float force = 300 * (bodies[i].mass * bodies[j].mass) / distance_sq;

      // const char* text = TextFormat("force: %f ", force);
      // DrawText(text, 10, 10, 20, WHITE);
      float Fx = force * dx / distance;
      float Fy = force * dy / distance;

      // acc
      bodies[i].ax += Fx / bodies[i].mass;
      bodies[j].ax -= Fx / bodies[j].mass;
      //
      bodies[i].ay += Fy / bodies[i].mass;
      bodies[j].ay -= Fy / bodies[j].mass;
    }
  }

  handleEdgeCondition(bodies);
}

int main(void) {
  InitWindow(screenWidth, screenHeight, "raylib [core] example - delta time");
  int currentFps = 60;
  SetTargetFPS(currentFps);

  Body sun = {
      .position = {(float)screenWidth / 2.0f, (float)screenHeight / 2.0f},
      .color = YELLOW,
      .radius = 40,
      .mass = 5000,
      .vx = 0,
      .vy = 0};

  Body earth = {
      .position = {(float)screenWidth / 2.0f, (float)screenHeight / 2.0f - 400},
      .color = GREEN,
      .radius = 15,
      .mass = 170,
      .vx = 45.35f,
      .vy = 0};

  Body venus = {
      .position = {(float)screenWidth / 2.0f, (float)screenHeight / 2.0f - 450},
      .color = BLUE,
      .radius = 5,
      .mass = 8,
      .vx = 14.47f,
      .vy = 0};

  vector<Body> bodies = {earth, venus, sun};
  //--------------------------------------------------------------------------------------
  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    BeginDrawing();
    ClearBackground(BLACK);
    computePhysics(bodies);
    drawBodies(bodies);
    EndDrawing();
  }

  CloseWindow(); // Close window and OpenGL context

  return 0;
}
