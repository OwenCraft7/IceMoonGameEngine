#ifndef PHYS_H
#define PHYS_H

float g_constant = -9.81f;
float terminal_velocity = -67.056f;
float playerJumpVelocity = 4.0f;
bool playerIsTouchingGround = true;

float acceleration, maxSpeed;
float rotationSpeed = 0.25f, keyRotationSpeed = 2.25f;
float axis_speed[3], axis_keyDown[3], prev_speed;
bool playerMovement = true; //  If playerMovement is true, then the map will regenerate on screen.

vec3 playerCollideIntersect, velocity;

#endif
