#include <stdbool.h>
#include <stdio.h>

#include "raylib.h"

int height = 600;
int width = 800;
int main() {
  InitWindow(width, height, "Hello raylib");
  SetTargetFPS(60);

  bool game_over = false;

  Vector2 ballC = {100, 100};
  int ballR = 20;
  Vector2 ballS = {4, 4};

  int paddleV = 10;
  Rectangle paddle = {
      .x = width / 2,
      .height = 10,
      .y = height - 10,
      .width = 150,
  };

  Color background = {80, 80, 80, 255};

  while (!WindowShouldClose()) {
    BeginDrawing();
    if (game_over) {
      DrawText("Game over", 100, 100, 40, WHITE);
      if (IsKeyDown(KEY_R)) {
        game_over = false;
        ballC.x = 100;
        ballC.y = 100;
      }
    } else {
      ClearBackground(background);
      if (ballC.x > width - ballR || ballC.x < ballR) {
        ballS.x *= -1;
      }

      if (ballC.y < ballR || ballC.y > height - ballR) {
        ballS.y *= -1;
      }

      if (ballC.y > height - ballR) {
        game_over = true;
      }

      if (CheckCollisionCircleRec(ballC, ballR, paddle)) {
        ballS.y *= -1;
        ballS.x = (ballC.x - paddle.x) / (paddle.width / 2) * 5;
      }

      ballC.x += ballS.x;
      ballC.y += ballS.y;
      DrawCircle(ballC.x, ballC.y, ballR, YELLOW);

      DrawRectangle(paddle.x, paddle.y, paddle.width, paddle.height, WHITE);

      if (IsKeyDown(KEY_L) && paddle.x < width - paddle.width) {
        paddle.x += paddleV;
      }

      if (IsKeyDown(KEY_H) && paddle.x > 0) {
        paddle.x -= paddleV;
      }
    }
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
