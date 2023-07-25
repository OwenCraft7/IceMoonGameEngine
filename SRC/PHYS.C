/*-----------------*/
/* Ice Moon phys.c */
/*                 */
/*     Physics     */
/*-----------------*/

#include "PHYS.H"

bool sphereTriangleCollision(vert TriA, vert TriB, vert TriC, vert* SphereCenter, float SphereRadius)
{
	vert edge1, edge2, v0, closest, collisionNormal;
	float a, b, c, d, e, s, t, dist, det, inverseLength, numer, denom, tmp1, tmp2;

	edge1.x = TriB.x - TriA.x;
	edge1.y = TriB.y - TriA.y;
	edge1.z = TriB.z - TriA.z;

	edge2.x = TriC.x - TriA.x;
	edge2.y = TriC.y - TriA.y;
	edge2.z = TriC.z - TriA.z;

	v0.x = TriA.x - SphereCenter->x;
	v0.y = TriA.y - SphereCenter->y;
	v0.z = TriA.z - SphereCenter->z;

	a = dot(edge1, edge1);
	b = dot(edge2, edge1);
	c = dot(edge2, edge2);
	d = dot(edge1, v0);
	e = dot(edge2, v0);

	det = a * c - b * b;
	s = b * e - c * d;
	t = b * d - a * e;

	if (s + t <= det)
	{
		if (s < 0.0f)
		{
			if (t < 0.0f)
			{
				if (d < 0.0f)
				{
					t = 0.0f;
					if (-d >= a)
						s = 1.0f;
					else
						s = -d / a;
				}
				else
				{
					s = 0.0f;
					if (e >= 0.0f)
						t = 0.0f;
					else if (-e >= c)
						t = 1.0f;
					else
						t = -e / c;
				}
			}
			else
			{
				s = 0.0f;
				if (e >= 0.0f)
					t = 0.0f;
				else if (-e >= c)
					t = 1.0f;
				else
					t = -e / c;
			}
		}
		else if (t < 0.0f)
		{
			t = 0.0f;
			if (d >= 0.0f)
				s = 0.0f;
			else if (-d >= a)
				s = 1.0f;
			else
				s = -d / a;
		}
		else { det = 1.0f / det;  s *= det; t *= det; }
	}
	else
	{
		if (s < 0.0f)
		{
			tmp1 = b + d; tmp2 = c + e;
			if (tmp2 > tmp1)
			{
				numer = tmp2 - tmp1;
				denom = a - 2.0f * b + c;
				if (numer >= denom)
					s = 1.0f;
				else
					s = numer / denom;
				t = 1.0f - s;
			}
			else
			{
				s = 0.0f;
				if (tmp2 <= 0.0f)
					t = 1.0f;
				else if (e >= 0.0f)
					t = 0.0f;
				else
					t = -e / c;
			}
		}
		else if (t < 0.0f)
		{
			tmp1 = b + e;
			tmp2 = a + d;
			if (tmp2 >= tmp1)
			{
				numer = (c + e) - (b - d);
				denom = a - 2.0f * b + c;
				if (numer >= denom)
					s = 1.0f;
				else
					s = numer / denom;
				t = 1.0f - s;
			}
			else
			{
				t = 0.0f;
				if (c + e <= 0.0f)
					s = 1.0f;
				else if (e >= 0.0f)
					s = 0.0f;
				else
					s = -e / c;
			}
		}
		else
		{
			numer = (c + e) - (b - d);
			if (numer <= 0.0f)
				s = 0.0f;
			else
			{
				denom = a - 2.0f * b + c;
				if (numer >= denom)
					s = 1.0f;
				else
					s = numer / denom;
			}
			t = 1.0f - s;
		}
	}

	closest.x = TriA.x + s * edge1.x + t * edge2.x;
	closest.y = TriA.y + s * edge1.y + t * edge2.y;
	closest.z = TriA.z + s * edge1.z + t * edge2.z;

	if (distance(closest, *SphereCenter) <= SphereRadius)
	{
		collisionNormal.x = SphereCenter->x - closest.x;
		collisionNormal.y = SphereCenter->y - closest.y;
		collisionNormal.z = SphereCenter->z - closest.z;
		normalize(&collisionNormal);
		SphereCenter->x = closest.x + collisionNormal.x * (SphereRadius + 0.01f);
		SphereCenter->y = closest.y + collisionNormal.y * (SphereRadius + 0.01f);
		SphereCenter->z = closest.z + collisionNormal.z * (SphereRadius + 0.01f);

		return true;
	}
	else
		return false;
}

void playerPhysicsUpdate()
{
	vert currentTriA, currentTriB, currentTriC;
	int i;
	int lastTri = (playerLeaf + 1 != leaf_count) ? map_leaf[playerLeaf + 1] : tri_count;
	//	Initialize previous player position
	prevPlayerPos.x = playerPos.x; prevPlayerPos.y = playerPos.y; prevPlayerPos.z = playerPos.z;
	
	if (!noclip)
	{
		//  Calculate velocity for the X and Z axes,
		velocity.x = deltaTime * ((sin(camRotX) * axis_speed[0]) + (cos(camRotX) * axis_speed[1]));
		velocity.z = deltaTime * ((cos(camRotX) * axis_speed[0]) - (sin(camRotX) * axis_speed[1]));

		if (axis_keyDown[2] > 0.0f && playerIsTouchingGround)  // If space bar is held down and player is on the ground
		{
			velocity.y = playerJumpVelocity;	// Update player's Y velocity to jump
			playerIsTouchingGround = false;		// Player is in the air
		}
		else if (!playerIsTouchingGround)		// Else, if player is in the air
		{
			velocity.y += deltaTime * g_constant;	// Increase or decrease Y velocity depending on the gravity
			if (velocity.y < terminal_velocity)		// Also check for terminal velocity
				velocity.y = terminal_velocity;
		}
		else
			velocity.y = 0.0f;	// If player is touching the ground and there's no space bar pressed, set Y velocity to 0.

		playerPos.y += deltaTime * velocity.y;	// Update player's Y position

		playerPos.y += 0.25f;
		
		currentTriA.x = 1.0f; currentTriA.y = 0.0f; currentTriA.z = 2.0f;
		currentTriB.x = -1.0f; currentTriB.y = 0.0f; currentTriB.z = -2.0f;
		currentTriC.x = 3.0f; currentTriC.y = 0.0f; currentTriC.z = -2.0f;
		/*
		playerIsTouchingGround = sphereTriangleCollision(currentTriA, currentTriB, currentTriC, &playerPos, 0.25f);
		for (i = map_leaf[playerLeaf]; i < lastTri; i++)
		{
			currentTriA = map_vert[map_tri->v[0]];
			currentTriB = map_vert[map_tri->v[1]];
			currentTriC = map_vert[map_tri->v[2]];
		*/
			playerIsTouchingGround = sphereTriangleCollision(currentTriA, currentTriB, currentTriC, &playerPos, 0.25f);
		//}
		playerPos.y -= 0.25f;
		/*
		playerIsTouchingGround = (playerPos.y < 0.0f);
		if (playerIsTouchingGround)						// The player is touching the ground if it goes below Y=0.
			playerPos.y = 0.0f;	// Send the player back up.
		*/
	}
	else
	{
		//  Calculate velocity for all three axes,
		velocity.x = deltaTime * ((sin(camRotX) * axis_keyDown[0]) + (cos(camRotX) * axis_keyDown[1]));
		velocity.y = deltaTime * axis_keyDown[2];
		velocity.z = deltaTime * ((cos(camRotX) * axis_keyDown[0]) - (sin(camRotX) * axis_keyDown[1]));
		playerPos.y += velocity.y;
	}
	playerPos.x += velocity.x; playerPos.z += velocity.z;

	if (prevPlayerPos.x != playerPos.x || prevPlayerPos.y != playerPos.y || prevPlayerPos.z != playerPos.z)
		playerMovement = true;	// When true, the level screen buffer will refresh.

	// The player's head is 1.7 meters high, and the eye height is always 0.2 meters below.
	headPosY = 1.7f;
	camPosY = headPosY - 0.2f + playerPos.y;
}
