/*-----------------*/
/* Ice Moon phys.c */
/*                 */
/*     Physics     */
/*-----------------*/

#include "PHYS.H"

bool lineTriangleIntersection(vert TriA, vert TriB, vert TriC, vert LinePointA, vert* LinePointB)	// This function has wacky physics and needs fixing.
{
	vert edge1, edge2, n, ao, dao, linePosDifference;
	float det, inverseDet, u, v, t;

	edge1.x = TriB.x - TriA.x;
	edge1.y = TriB.y - TriA.y;
	edge1.z = TriB.z - TriA.z;

	edge2.x = TriC.x - TriA.x;
	edge2.y = TriC.y - TriA.y;
	edge2.z = TriC.z - TriA.z;

	linePosDifference.x = LinePointB->x - LinePointA.x;
	linePosDifference.y = LinePointB->y - LinePointA.y;
	linePosDifference.z = LinePointB->z - LinePointA.z;
	normalize(&linePosDifference);

	ao.x = LinePointA.x - TriA.x;
	ao.y = LinePointA.y - TriA.y;
	ao.z = LinePointA.z - TriA.z;

	cross(ao, linePosDifference, &dao);
	cross(edge1, edge2, &n);
	det = -dot(linePosDifference, n);
	inverseDet = 1.0f / det;

	u = dot(edge2, dao) * inverseDet;
	v = -dot(edge1, dao) * inverseDet;
	t = dot(ao, n) * inverseDet;

	if (fabs(det) >= 1e-6 && t < distance(LinePointA, *LinePointB) && u >= 0.0f && v >= 0.0f && (u + v) <= 1.0f)
	{
		LinePointB->x = LinePointA.x + t * linePosDifference.x;
		LinePointB->y = LinePointA.y + t * linePosDifference.y;
		LinePointB->z = LinePointA.z + t * linePosDifference.z;
		return true;
	}
	else return false;
}

bool sphereTriangleCollision(vert TriA, vert TriB, vert TriC, vert* SphereCenter, float SphereRadius)	// This function too has inaccurate physics.
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
	else return false;
}

void playerPhysicsUpdate()
{
	int i, lastTri;
	// Initialize previous player position
	prevPlayerPos.x = playerPos.x; prevPlayerPos.y = playerPos.y; prevPlayerPos.z = playerPos.z;
	
	if (!noclip)	// If player is not in noclip mode,
	{
		// Calculate velocity for the X and Z axes,
		playerVelocity.x = deltaTime * ((sin(camRotX) * axis_speed[0]) + (cos(camRotX) * axis_speed[1]));
		playerVelocity.z = deltaTime * ((cos(camRotX) * axis_speed[0]) - (sin(camRotX) * axis_speed[1]));
		if (axis_keyDown[2] > 0.0f && playerIsTouchingGround)  // If space bar is held down and player is on the ground
		{
			playerVelocity.y = playerJumpVelocity;	// Update player's Y velocity to jump
		}
		else if (!playerIsTouchingGround)			// Else, if player is in the air
		{
			playerVelocity.y += deltaTime * g_constant;	// Increase or decrease Y velocity depending on the gravity
			if (playerVelocity.y < terminal_velocity)	// Also check for terminal velocity
				playerVelocity.y = terminal_velocity;
		}
		else
			playerVelocity.y = 0.0f;	// If player is touching the ground and there's no space bar pressed, set Y velocity to 0.

		playerPos.y += deltaTime * playerVelocity.y;	// Update player's Y position

		playerPos.y += 0.25f;

		playerIsTouchingGround = false;
		checkRootNode(prevPlayerPos.x, prevPlayerPos.y, prevPlayerPos.z, false, &prevPlayerLeaf);
		lastTri = (prevPlayerLeaf + 1 != leaf_count) ? map_leaf[prevPlayerLeaf + 1] : tri_count;
		for (i = map_leaf[prevPlayerLeaf]; i < lastTri; i++)
		{
			//if (lineTriangleIntersection(map_vert[map_tri[i].v[0]], map_vert[map_tri[i].v[1]], map_vert[map_tri[i].v[2]], prevPlayerPos, &playerPos))
			//	playerIsTouchingGround = true;
			if (sphereTriangleCollision(map_vert[map_tri[i].v[0]], map_vert[map_tri[i].v[1]], map_vert[map_tri[i].v[2]], &playerPos, 0.25f))
				playerIsTouchingGround = true;
		}
		checkRootNode(playerPos.x, playerPos.y, playerPos.z, false, &playerLeaf);
		if (playerLeaf != prevPlayerLeaf)
		{
			lastTri = (playerLeaf + 1 != leaf_count) ? map_leaf[playerLeaf + 1] : tri_count;
			for (i = map_leaf[playerLeaf]; i < lastTri; i++)
			{
				//if (lineTriangleIntersection(map_vert[map_tri[i].v[0]], map_vert[map_tri[i].v[1]], map_vert[map_tri[i].v[2]], prevPlayerPos, &playerPos))
				//	playerIsTouchingGround = true;
				if (sphereTriangleCollision(map_vert[map_tri[i].v[0]], map_vert[map_tri[i].v[1]], map_vert[map_tri[i].v[2]], &playerPos, 0.25f))
					playerIsTouchingGround = true;
			}
		}

		playerPos.y -= 0.25f;
	}
	else	// If player is in noclip mode,
	{
		// Calculate player velocity for all three axes,
		playerVelocity.x = deltaTime * ((sin(camRotX) * axis_keyDown[0]) + (cos(camRotX) * axis_keyDown[1]));
		playerVelocity.y = deltaTime * axis_keyDown[2];
		playerVelocity.z = deltaTime * ((cos(camRotX) * axis_keyDown[0]) - (sin(camRotX) * axis_keyDown[1]));
		playerPos.y += playerVelocity.y;
	}
	playerPos.x += playerVelocity.x; playerPos.z += playerVelocity.z;

	if (prevPlayerPos.x != playerPos.x || prevPlayerPos.y != playerPos.y || prevPlayerPos.z != playerPos.z)
		playerMovement = true;	// If true, the level screen buffer will refresh.

	// The player's head is 1.7 meters high, and the eye height is always 0.2 meters below.
	headPosY = 1.7f;
	camPosY = headPosY - 0.2f + playerPos.y;
}
