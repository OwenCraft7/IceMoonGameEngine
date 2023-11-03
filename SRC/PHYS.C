/*-----------------*/
/* Ice Moon phys.c */
/*                 */
/*     Physics     */
/*-----------------*/

#include "PHYS.H"

bool lineTriangleIntersection(vert TriA, vert TriB, vert TriC, vert LinePointA, vert LinePointB, vert *Intersect)	// This function has wacky physics and needs fixing.
{
	vert e1, e2, n;		vert ao, dao, rayDir;
	float t, u, v;		float det, invdet;

	rayDir.x = LinePointB.x - LinePointA.x;		rayDir.y = LinePointB.y - LinePointA.y;		rayDir.z = LinePointB.z - LinePointA.z;
	normalize(&rayDir);

	e2.x = TriB.x - TriA.x;		e2.y = TriB.y - TriA.y;		e2.z = TriB.z - TriA.z;
	e1.x = TriC.x - TriA.x;		e1.y = TriC.y - TriA.y;		e1.z = TriC.z - TriA.z;
	cross(e1, e2, &n);

	det = -dot(rayDir, n);
	invdet = 1.0f / det;

	ao.x = LinePointA.x - TriA.x;	ao.y = LinePointA.y - TriA.y;	ao.z = LinePointA.z - TriA.z;
	cross(ao, rayDir, &dao);

	u = dot(e2, dao) * invdet;
	v = -dot(e1, dao) * invdet;
	t = dot(ao, n) * invdet;

	if (det >= 1e-6 && t >= 0.0 && u >= 0.0 && v >= 0.0 && (u + v) <= 1.0 && t < distance(LinePointA, LinePointB))
	{
		Intersect->x = LinePointA.x + rayDir.x * t;
		Intersect->y = LinePointA.y + rayDir.y * t;
		Intersect->z = LinePointA.z + rayDir.z * t;
		return true;
	}
	else return false;
}

bool sphereTriangleCollision(vert TriA, vert TriB, vert TriC, vert* SphereCenter, float SphereRadius)	// This function too has inaccurate physics.
{
	vert edge1, edge2, v0, closest, collisionNormal;
	float a, b, c, d, e, s, t, det, dist, numer, denom, tmp1, tmp2;

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
	dist = SphereRadius - distance(closest, *SphereCenter);

	if (dist >= 0.0f)
	{
		collisionNormal.x = SphereCenter->x - closest.x;
		collisionNormal.y = SphereCenter->y - closest.y;
		collisionNormal.z = SphereCenter->z - closest.z;
		normalize(&collisionNormal);
		SphereCenter->x += collisionNormal.x * dist;
		SphereCenter->y += collisionNormal.y * dist;
		SphereCenter->z += collisionNormal.z * dist;
		if (collisionNormal.y >= 0.7f)
			playerIsTouchingGround = true;
		return true;
	}
	else return false;
}

void playerPhysicsUpdate()
{
	int i, lastTri;
	prevPlayerPos.x = playerPos.x; prevPlayerPos.y = playerPos.y; prevPlayerPos.z = playerPos.z;	// Initialize previous player position
	
	if (!noclip)	// If player cannot clip through everything,
	{
		// Calculate velocity for the X and Z axes,
		playerVelocity.x = deltaTime * ((sin(camRotX) * axis_speed[0]) + (cos(camRotX) * axis_speed[1]));
		playerVelocity.z = deltaTime * ((cos(camRotX) * axis_speed[0]) - (sin(camRotX) * axis_speed[1]));
		playerPos.x += playerVelocity.x; playerPos.z += playerVelocity.z;

		if (playerIsTouchingGround)						// If player is on the ground...
		{
			if (axis_keyDown[2] > 0.0f)						// If spacebar is pressed...
				playerVelocity.y = playerJumpVelocity;			// Update player's Y velocity to jump.
			else playerVelocity.y = 0.0f;					// Else, set Y velocity to 0.
		}
		else											// Else, if player is in the air...
		{
			playerVelocity.y += deltaTime * gravityVelocity;	// Increase or decrease Y velocity depending on the gravity.
			if (playerVelocity.y < terminalVelocity)			// Also, check for terminal velocity.
				playerVelocity.y = terminalVelocity;
		}

		playerPos.y += deltaTime * playerVelocity.y;	// Update player's Y position
		playerPos.y += 0.25f;

		playerIsTouchingGround = false;

		// Check the node leaf the player was in the previous frame.
		checkRootNode(prevPlayerPos.x, prevPlayerPos.y, prevPlayerPos.z, false, &prevPlayerLeaf);

		// Check the last triangle in that node leaf.
		lastTri = (prevPlayerLeaf + 1 != leaf_count) ? map_leaf[prevPlayerLeaf + 1] : tri_count;

		// For all triangles in that node leaf, do a line-triangle intersection test, where the line goes from the player's previous location to its current one.
		for (i = map_leaf[prevPlayerLeaf]; i < lastTri; i++)
			lineTriangleIntersection(map_vert[map_tri[i].v[0]], map_vert[map_tri[i].v[1]], map_vert[map_tri[i].v[2]], prevPlayerPos, playerPos, &playerPos);

		// For all triangles in the map, test triangle collision on the player itself.
		for (i = 0; i < tri_count; i++)
			sphereTriangleCollision(map_vert[map_tri[i].v[0]], map_vert[map_tri[i].v[1]], map_vert[map_tri[i].v[2]], &playerPos, 0.25f);

		playerPos.y -= 0.25f;
	}
	else	// If player can clip through everything,
	{
		// Calculate player velocity for all three axes,
		playerVelocity.x = deltaTime * ((sin(camRotX) * axis_keyDown[0]) + (cos(camRotX) * axis_keyDown[1]));
		playerVelocity.y = deltaTime * axis_keyDown[2];
		playerVelocity.z = deltaTime * ((cos(camRotX) * axis_keyDown[0]) - (sin(camRotX) * axis_keyDown[1]));
		playerPos.x += playerVelocity.x; playerPos.y += playerVelocity.y; playerPos.z += playerVelocity.z;
	}

	if (prevPlayerPos.x != playerPos.x || prevPlayerPos.y != playerPos.y || prevPlayerPos.z != playerPos.z)
		playerMovement = true;	// If true, the level screen buffer will refresh.

	// The player's head is 1.7 meters high, and the eye height is always 0.2 meters below.
	headPosY = 1.7f;
	camPosY = headPosY - 0.2f + playerPos.y;
}
