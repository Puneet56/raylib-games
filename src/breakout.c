#include <stdbool.h>
#include <stdio.h>

#include "raylib.h"

// Bricks can change color as they're hit, e.g., Bright Red → Bright Orange →
// Lemon Yellow → Destroyed.
// Glow or neon effects can add depth;
// simulate this by applying a halo effect around elements.

Color BACKGROUND = {0x0d, 0x1b, 0x2a, 255};
Color PADDLE = {0x39, 0xff, 0x14, 255};
Color BALL = {0xff, 0xdd, 0x00, 255};
Color BRICKS_1 = {0xff, 0x33, 0x33, 255}; // Bright Red
Color BRICKS_2 = {0xff, 0x99, 0x33, 255}; // Vivid Orange
Color BRICKS_3 = {0xff, 0xff, 0x33, 255}; // Bright Yellow
Color BRICKS_4 = {0x33, 0xcc, 0xff, 255}; // Electric Blue
Color BORDER = {0x8c, 0x8c, 0x8c, 255};   // Metallic Silver
Color SPECIAL = {0xff, 0x00, 0x7f, 255};
Color BONUS = {0xff, 0xd7, 0x00, 255};

typedef enum GameState { NOT_RUNNING, RUNNING, OVER } GameState;

typedef struct Game {
  GameState state;
} Game;

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

typedef struct Brick {
  bool isBroken;
  Vector2 pos;
} Brick;

/********************************************************
 * Most important. This loop will be used everwhere
 * whenever we need to iterate 1D array as 2D array
 *
 *       col 0, col 1, col 2, col 3, col 4
 * row 0
 * row 1
 * row 2
 *
 * rows = 3
 * cols = 5
 *
 * for i of rows
 *    for j of cols
 *      center = i * cols + j     (current element)
 *      top = (i-1) * cols + j    (bounds i > 0)
 *      right = i * cols + (j+1)  (bounds j < cols - 1)
 *      bottom = (i+1) * cols + j (bounds i < rows - 1)
 *      left = i * cols + (j-1)   (bounds j > 0)
 *
 ********************************************************/

int main() {
  //--------------------------------------------------------------------------------------
  // Initialization
  int height = 600;
  int width = 800;
  Game game = {.state = NOT_RUNNING};

  Vector2 initialBallPos = {(float)width / 2, height - 150};
  Vector2 initialBallSpeed = {4, 4};
  Vector2 initialPaddlePos = {(float)width / 2, height - 10};

  Ball ball = {
      .pos = initialBallPos,
      .r = 20,
      .speed = initialBallSpeed,
  };
  Paddle paddle = {
      .pos = initialPaddlePos,
      .size = {150, 10},
      .vel = 10,
  };

  int brickCols = 10;
  int brickRows = 3;
  int bricksCount = brickCols * brickRows;

  Vector2 brickSize = {(float)width / brickCols, 20};
  int topOffset = 40;
  Brick bricks[bricksCount];
  for (int i = 0; i < brickRows; i++) {
    for (int j = 0; j < brickCols; j++) {
      Vector2 pos = {j * brickSize.x, i * brickSize.y + topOffset};
      Brick b = {false, pos};
      bricks[i * brickCols + j] = b;
    }
  }

  int brokenBricksCount = 0;
  InitWindow(width, height, "Breakout");
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    BeginDrawing();
    DrawFPS(10, 10);

    if (game.state == NOT_RUNNING) {
      DrawText("< H        L >", width / 2 - 150, height / 2 - 50, 40, WHITE);
      DrawText("Press space!", width / 2 - 150, height / 2, 40, WHITE);

      if (IsKeyDown(KEY_SPACE)) {
        game.state = RUNNING;
      }
    }

    if (game.state == OVER) {
      char *t = "Game over!";
      if (brokenBricksCount == bricksCount) {
        t = "You won!";
      }
      DrawText(t, width / 2 - 150, height / 2 - 50, 40, WHITE);
      DrawText("Press space!", width / 2 - 150, height / 2, 40, WHITE);

      if (IsKeyDown(KEY_SPACE)) {
        game.state = RUNNING;
        ball.pos = initialBallPos;
        ball.speed = initialBallSpeed;
        paddle.pos = initialPaddlePos;
        brokenBricksCount = 0;
        for (int i = 0; i < brickRows; i++) {
          for (int j = 0; j < brickCols; j++) {
            Brick *b = &bricks[i * brickCols + j];
            b->isBroken = false;
          }
        }
      }
    }
    if (game.state == RUNNING) {
      //--------------------------------------------------------------------------------------
      // Movement
      ball.pos.x += ball.speed.x;
      ball.pos.y += ball.speed.y;
      if (IsKeyDown(KEY_L) && paddle.pos.x < width - paddle.size.x) {
        paddle.pos.x += paddle.vel.x;
      }

      if (IsKeyDown(KEY_H) && paddle.pos.x > 0) {
        paddle.pos.x -= paddle.vel.x;
      }
    }

    //--------------------------------------------------------------------------------------
    // Draw
    ClearBackground(BACKGROUND);
    DrawCircleV(ball.pos, ball.r, BALL);
    DrawRectangleV(paddle.pos, paddle.size, PADDLE);

    for (int i = 0; i < brickRows; i++) {
      for (int j = 0; j < brickCols; j++) {
        Brick b = bricks[i * brickCols + j];
        if (b.isBroken) {
          continue;
        }

        Color c = BRICKS_1;
        if (i == 1) {
          c = BRICKS_2;
        } else if (i == 2) {
          c = BRICKS_3;
        } else if (i == 3) {
          c = BRICKS_4;
        }
        DrawRectangleV(b.pos, brickSize, c);
        DrawRectangleLinesEx(
            (Rectangle){b.pos.x, b.pos.y, brickSize.x, brickSize.y}, 1, BORDER);
      }
    }

    //--------------------------------------------------------------------------------------
    // coliision walls and paddle
    if (ball.pos.x > width - ball.r || ball.pos.x < ball.r) {
      ball.speed.x *= -1;
    }

    if (ball.pos.y < ball.r || ball.pos.y > height - ball.r) {
      ball.speed.y *= -1;
    }

    Rectangle paddleBoundryRectangle = {
        .x = paddle.pos.x,
        .y = paddle.pos.y,
        .width = paddle.size.x,
        .height = paddle.size.y,
    };

    bool isBallAndPaddleCollided =
        CheckCollisionCircleRec(ball.pos, ball.r, paddleBoundryRectangle);

    if (isBallAndPaddleCollided) {
      ball.speed.y *= -1;
      // reference =
      // https://github.com/raysan5/raylib-games/blob/master/classics/src/arkanoid.c
      ball.speed.x = (ball.pos.x - paddle.pos.x - paddle.size.x / 2) /
                     (paddle.size.x / 2) * 15;
    }

    //--------------------------------------------------------------------------------------
    // coliision bricks
    for (int i = 0; i < brickRows; i++) {
      for (int j = 0; j < brickCols; j++) {
        Brick *b = &bricks[i * brickCols + j];
        if (b->isBroken) {
          continue;
        }
        Rectangle brickBoundryRectangle = {
            .x = b->pos.x,
            .y = b->pos.y,
            .width = brickSize.x,
            .height = brickSize.y,
        };

        // This implementation has bugs, need to check collision on sides
        if (CheckCollisionCircleRec(ball.pos, ball.r, brickBoundryRectangle)) {
          b->isBroken = true;
          ball.speed.y *= -1;
          brokenBricksCount++;
        }
      }
    }

    //--------------------------------------------------------------------------------------
    // State change
    if (ball.pos.y > height - ball.r || brokenBricksCount == bricksCount) {
      game.state = OVER;
    }

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
