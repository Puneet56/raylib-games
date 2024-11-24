#include <stdbool.h>
#include <stdio.h>

#include "raylib.h"

typedef struct Ball {
  Vector2 pos;
  int r;
  Vector2 speed;
} Ball;

typedef struct Paddle {
  Vector2 size;
  Vector2 pos;
  Vector2 v;
} Paddle;

typedef struct Game {
  bool game_over;
} Game;

int height = 600;
int width = 800;
int main() {
  InitWindow(width, height, "Hello raylib");
  SetTargetFPS(60);

  Game game = {false};
  Ball ball = {
      .pos = (Vector2){100, 100},
      .r = 20,
      .speed = (Vector2){4, 4},
  };
  Paddle paddle = {
      .pos = (Vector2){width / 2, height - 10},
      .size = {150, 10},
      .v = 10,
  };

  Color background = {80, 80, 80, 255};

  while (!WindowShouldClose()) {
    BeginDrawing();
    if (game.game_over) {
      DrawText("Game over", 100, 100, 40, WHITE);
      if (IsKeyDown(KEY_R)) {
        game.game_over = false;
        ball.pos.x = 100;
        ball.pos.y = 100;
      }
    } else {
      ClearBackground(background);
      if (ball.pos.x > width - ball.r || ball.pos.x < ball.r) {
        ball.speed.x *= -1;
      }

      if (ball.pos.y < ball.r || ball.pos.y > height - ball.r) {
        ball.speed.y *= -1;
      }

      if (ball.pos.y > height - ball.r) {
        game.game_over = true;
      }

      if (CheckCollisionCircleRec(ball.pos, ball.r,
                                  (Rectangle){paddle.pos.x, paddle.pos.y,
                                              paddle.size.x, paddle.size.y})) {
        ball.speed.y *= -1;
        ball.speed.x = (ball.pos.x - paddle.pos.x - paddle.size.x / 2) /
                       (paddle.size.x / 2) * 10;
      }

      ball.pos.x += ball.speed.x;
      ball.pos.y += ball.speed.y;
      DrawCircle(ball.pos.x, ball.pos.y, ball.r, YELLOW);

      DrawRectangle(paddle.pos.x, paddle.pos.y, paddle.size.x, paddle.size.y,
                    WHITE);

      if (IsKeyDown(KEY_L) && paddle.pos.x < width - paddle.size.x) {
        paddle.pos.x += paddle.v.x;
      }

      if (IsKeyDown(KEY_H) && paddle.pos.x > 0) {
        paddle.pos.x -= paddle.v.x;
      }
    }
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
