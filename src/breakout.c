#include <stdbool.h>
#include <stdio.h>

#include "raylib.h"

// Element	Color Name	Hex Code
// Background	Deep Space Blue	#0d1b2a
// Paddle	Neon Green	#39ff14
// Ball	Bright Yellow	#ffdd00
// Bricks (Row 1)	Bright Red	#ff4500
// Bricks (Row 2)	Bright Orange	#ffa500
// Bricks (Row 3)	Lemon Yellow	#ffff66
// Special Effects	Neon Pink	#ff007f
// Bonus Points	Gold	#ffd700

Color BACKGROUND = {0x0d, 0x1b, 0x2a, 255};
Color PADDLE = {0x39, 0xff, 0x14, 255};
Color BALL = {0xff, 0xdd, 0x00, 255};
Color BRICKS = {0xff, 0x45, 0x00, 255};
Color SPECIAL = {0xff, 0x00, 0x7f, 255};
Color BONUS = {0xff, 0xd7, 0x00, 255};

typedef struct Ball {
  Vector2 pos;
  int r;
  Vector2 speed;
} Ball;

typedef struct Paddle {
  Vector2 size;
  Vector2 pos;
  Vector2 vel;
} Paddle;

typedef struct Game {
  bool game_over;
} Game;

int height = 600;
int width = 800;
int main() {
  InitWindow(width, height, "Hello raylib");
  SetTargetFPS(60);

  Game game = {
      .game_over = false,
  };

  Ball ball = {
      .pos = (Vector2){100, 100},
      .r = 20,
      .speed = (Vector2){4, 4},
  };
  Paddle paddle = {
      .pos = (Vector2){width / 2, height - 10},
      .size = {150, 10},
      .vel = 10,
  };

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
      ClearBackground(BACKGROUND);
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
      DrawCircle(ball.pos.x, ball.pos.y, ball.r, BALL);

      DrawRectangle(paddle.pos.x, paddle.pos.y, paddle.size.x, paddle.size.y,
                    PADDLE);

      if (IsKeyDown(KEY_L) && paddle.pos.x < width - paddle.size.x) {
        paddle.pos.x += paddle.vel.x;
      }

      if (IsKeyDown(KEY_H) && paddle.pos.x > 0) {
        paddle.pos.x -= paddle.vel.x;
      }
    }
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
