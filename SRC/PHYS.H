#ifndef PHYS_H
#define PHYS_H

float gravityVelocity = -9.81f;
float terminalVelocity = -67.056f;
float playerJumpVelocity = 4.0f;

float acceleration, maxSpeed;
float rotationSpeed = 0.25f, keyRotationSpeed = 2.25f;
float axis_speed[3], axis_keyDown[3];

bool playerMovement = true;	// If playerMovement is true, then the map will regenerate on screen.
bool playerIsTouchingGround;

vert playerCollideIntersect, playerVelocity;

#endif
