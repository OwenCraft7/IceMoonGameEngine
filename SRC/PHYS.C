/*-----------------*/
/* Ice Moon phys.c */
/*                 */
/*     Physics     */
/*-----------------*/

#include "PHYS.H"

bool rayTriangle_intersection(vec3 Origin, vec3 Destination, vec3 TriA, vec3 TriB, vec3 TriC, vec3* Intersect)
{
	vec3 e1, e2, n, ao, dao, dir;
	float det, invdet, t, u, v;

	dir.x = Destination.x - Origin.x;
	e1.x = TriB.x - TriA.x;
	e2.x = TriC.x - TriA.x;
	dir.y = Destination.y - Origin.y;
	e1.y = TriB.y - TriA.y;
	e2.y = TriC.y - TriA.y;
	dir.z = Destination.z - Origin.z;
	e1.z = TriB.z - TriA.z;
	e2.z = TriC.z - TriA.z;

	cross(e1.x, e1.y, e1.z, e2.x, e2.y, e2.z, &n.x, &n.y, &n.z);
	det = -dot(dir, n);
	if (det >= 0.000001)
	{
		invdet = 1 / det;
		ao.x = Origin.x - TriA.x;
		ao.y = Origin.y - TriA.y;
		ao.z = Origin.z - TriA.z;
		cross(ao.x, ao.y, ao.z, dir.x, dir.y, dir.z, &dao.x, &dao.y, &dao.z);
		u = dot(e2, dao) * invdet;
		v = -dot(e1, dao) * invdet;
		t = dot(ao, n) * invdet;
		if (t >= 0 && u >= 0 && v >= 0 && (u + v) <= 1)
		{
			Intersect->x = Origin.x + t * dir.x;
			Intersect->y = Origin.y + t * dir.y;
			Intersect->z = Origin.z + t * dir.z;
			return true;
		}
	}
	return false;
}

void playerPhysicsUpdate()
{
	//  Calculate velocity for the X and Z axes,
	velocity.x = deltaTime * ((sin(camRotX) * axis_speed[0]) + (cos(camRotX) * axis_speed[1]));
	velocity.z = deltaTime * ((cos(camRotX) * axis_speed[0]) - (sin(camRotX) * axis_speed[1]));
	//  Then add the player's position with the calculated velocity.
	prevPlayerPos.x = playerPos.x; prevPlayerPos.y = playerPos.y; prevPlayerPos.z = playerPos.z;
	playerPos.x += velocity.x; playerPos.z += velocity.z;

	if (!noclip)
	{
		if (axis_keyDown[2] == 1.0f && playerIsTouchingGround)  // If space bar is held down
		{
			velocity.y = playerJumpVelocity; // Update player's Y velocity to jump
			playerIsTouchingGround = false; // Player is in the air
		}
		else if (!playerIsTouchingGround)                       // If player is in the air
		{
			velocity.y += deltaTime * g_constant;    // Increase or decrease Y velocity depending on the gravity
			if (velocity.y < terminal_velocity)      // Also check for terminal velocity
				velocity.y = terminal_velocity;
		}
		else
			velocity.y = 0.0f;	// If player is touching the ground, set Y velocity to 0.

		playerPos.y += deltaTime * velocity.y;
		playerIsTouchingGround = (playerPos.y < 0.0f);
		if (playerIsTouchingGround)
			playerPos.y = 0.0f;
	}
	else
	{
		velocity.y = deltaTime * axis_speed[2];
		playerPos.y += velocity.y;
	}

	if (prevPlayerPos.x != playerPos.x || prevPlayerPos.y != playerPos.y || prevPlayerPos.z != playerPos.z)
		playerMovement = true;

	// The player's head is 1.7 meters high, and the eye height is always 0.2 meters below.
	headPosY = 1.7f;
	camPosY = headPosY - 0.2f + playerPos.y;
}
