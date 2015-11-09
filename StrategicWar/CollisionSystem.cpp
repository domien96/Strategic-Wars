#include "CollisionSystem.h"
#include "EntityStream.h"
#include "PositionComponent.h"
#include "PlayerComponent.h"
#include "CaveSegmentComponent.h"
#include "TextureComponent.h"
#include "BonusComponent.h"
#include "Engine.h"
#include <ctime>
#include <cstdio>

using namespace std;

// static fields

void GetMinMaxVector2(Entity *e, Vector2 &min, Vector2 &max)
{
	PositionComponent* e_pos = dynamic_cast<PositionComponent*>(e->GetComponent(Component::POSITION));
	CaveSegmentComponent* e_cs = dynamic_cast<CaveSegmentComponent*>(e->GetComponent(Component::CAVE_SEGMENT));
	BonusComponent* e_bc = dynamic_cast<BonusComponent*>(e->GetComponent(Component::BONUS));

	if (e_cs) {
		float offsetx = e_pos->pos.x;
		float offsety = e_pos->pos.y;

		float minx = e_cs->body[0].x + offsetx;
		float miny = e_cs->body[0].y + offsety;
		float maxx = e_cs->body[0].x + offsetx;
		float maxy = e_cs->body[0].y + offsety;
		for (unsigned int i = 0; i < e_cs->body.size(); i++)
		{
			float x = e_cs->body[i].x + offsetx;
			float y = e_cs->body[i].y + offsety;
			if (x < minx)
				minx = x;
			if (y < miny)
				miny = y;
			if (x > maxx)
				maxx = x;
			if (y > maxy)
				maxy = y;
		}
		min = Vector2(minx, miny);
		max = Vector2(maxx, maxy);
	}
	else if (e_bc) {
		float offsetx = e_pos->pos.x;
		float offsety = e_pos->pos.y;

		float minx = e_bc->body[0].x + offsetx;
		float miny = e_bc->body[0].y + offsety;
		float maxx = e_bc->body[0].x + offsetx;
		float maxy = e_bc->body[0].y + offsety;
		for (unsigned int i = 0; i < e_bc->body.size(); i++)
		{
			float x = e_bc->body[i].x + offsetx;
			float y = e_bc->body[i].y + offsety;
			if (x < minx)
				minx = x;
			if (y < miny)
				miny = y;
			if (x > maxx)
				maxx = x;
			if (y > maxy)
				maxy = y;
		}
		min = Vector2(minx, miny);
		max = Vector2(maxx, maxy);
	}
}

bool isInPlayerBounds(Entity* player, Entity * e)
{
	float front_buff = 10;
	float back_buff = 10;
	PositionComponent* player_pos = dynamic_cast<PositionComponent*>(player->GetComponent(Component::POSITION));
	PlayerComponent* player_pc = dynamic_cast<PlayerComponent*>(player->GetComponent(Component::PLAYER));
	float minX, maxX, minY, maxY;
	minX = player_pos->pos.x - back_buff;
	minY = player_pos->pos.y - back_buff;
	maxX = player_pos->pos.x + player_pc->size.x + front_buff;
	maxY = player_pos->pos.y + player_pc->size.y + back_buff;

	Vector2 e_min, e_max;
	GetMinMaxVector2(e, e_min, e_max);

	return (e_min.x <= maxX && e_max.x >= minX) && (e_min.y <= maxY && e_max.y >= minY);
}

vector<Vector2> GetBody(Entity* entity)
{
	PlayerComponent*pc = dynamic_cast<PlayerComponent*>(entity->GetComponent(Component::PLAYER));
	CaveSegmentComponent *csc = dynamic_cast<CaveSegmentComponent*>(entity->GetComponent(Component::CAVE_SEGMENT));
	BonusComponent *bc = dynamic_cast<BonusComponent*>(entity->GetComponent(Component::BONUS));
	if (!pc && !csc && !bc)
		return vector<Vector2>();
	else if (pc)
		return pc->body;
	else if (csc)
		return csc->body;
	else if (bc)
		return bc->body;
}

void CollisionSystem::Update()
{
	Entity *player = *(engine->GetEntityStream()->WithTag(Component::PLAYER).begin());
	if (player == NULL)
		return;
	PlayerComponent *pc = dynamic_cast<PlayerComponent*>(player->GetComponent(Component::PLAYER));
	if (pc->state == PlayerComponent::STATE_FROZEN && !engine->GetContext()->IsDebugMode())
		return;

	// Load collidable entities
	set<Entity*> cave_segs = engine->GetEntityStream()->WithTag(Component::CAVE_SEGMENT);
	set<Entity*> bonuses = engine->GetEntityStream()->WithTag(Component::BONUS);

	vector<Entity*> v;
	v.insert(v.begin(), cave_segs.begin(), cave_segs.end());
	v.insert(v.begin(), bonuses.begin(), bonuses.end());
	bool collides = false;
	bool collisionTrigger = false;
	bool bonusTrigger = false;
	for (vector<Entity*>::iterator iter = v.begin(); iter != v.end(); iter++)
	{
		// Load needed components
		PositionComponent *p_poc = dynamic_cast<PositionComponent*>(player->GetComponent(Component::POSITION));
		PositionComponent *e_poc = dynamic_cast<PositionComponent*>((*iter)->GetComponent(Component::POSITION));

		// Player offset and body
		Vector2 player_offset = p_poc->pos;
		vector<Vector2> player_vertices = pc->body;

		// Entity offset and body
		Vector2 entity_offset = e_poc->pos;
		vector<Vector2> entity_vertices = GetBody(*iter);

		if (!isInPlayerBounds(player, *iter))
			continue;
		collides = IsColliding(player_offset, player_vertices, entity_offset, entity_vertices);

		if (collides)
		{
			CaveSegmentComponent *csc = dynamic_cast<CaveSegmentComponent*>((*iter)->GetComponent(Component::CAVE_SEGMENT));
			BonusComponent *bc = dynamic_cast<BonusComponent*>((*iter)->GetComponent(Component::BONUS));
			if (csc) {
				TextureComponent *tc = dynamic_cast<TextureComponent*>((*iter)->GetComponent(Component::TEXTURE));
				tc->color = Color(255, 0, 0);
			}
			else if (bc) {
				bonusTrigger |= true;
				bc->hit = true;
			}
		}
		collisionTrigger |= collides;
	}
	pc->bonus = bonusTrigger;
	if (collisionTrigger) pc->state = PlayerComponent::STATE_COLLIDING;
	else pc->state = PlayerComponent::STATE_NORMAL;
}

bool CollisionSystem::IsColliding(Vector2 pos1, vector<Vector2> vertices1, Vector2 pos2, vector<Vector2> vertices2)
{
	bool collides = true;
	vector<Vector2> edgesA = GetOffsetEdges(pos1, vertices1);
	vector<Vector2> edgesB = GetOffsetEdges(pos2, vertices2);
	Vector2 translationAxis;
	Vector2 edge;

	// Loop through all the edges of both polygons
	for (unsigned int edgeIdx = 0; edgeIdx < edgesA.size() + edgesB.size(); edgeIdx++)
	{
		if (edgeIdx < edgesA.size())
			edge = edgesA[edgeIdx];
		else
			edge = edgesB[edgeIdx - edgesA.size()];

		// 1. Are polygons currently intersecting

		// Find perpendicular axis to current edge
		Vector2 axis(-edge.y, edge.x);
		axis = axis.Normalize();

		// Find projection of polygon on current axis
		float minA = 0;
		float minB = 0;
		float maxA = 0;
		float maxB = 0;
		ProjectPolygon(GetOffsetVertices(pos1, vertices1), axis, minA, maxA);
		ProjectPolygon(GetOffsetVertices(pos2, vertices2), axis, minB, maxB);



		// Check if polygon projections overlap
		if (IntervalDistance(minA, maxA, minB, maxB) > 0)
			collides = false;

		if (!collides)
			break;
	}

	return collides;
}

vector<Vector2> CollisionSystem::GetOffsetEdges(Vector2 offset, vector<Vector2> vertices)
{
	vector<Vector2> edges;
	Vector2 prevPoint = vertices[0] + offset;
	for (unsigned int i = 1; i < vertices.size(); i++) {
		edges.push_back((vertices[i] + offset) - prevPoint);
		prevPoint = vertices[i] + offset;
	}
	edges.push_back((vertices[0] + offset) - (prevPoint));
	return edges;
}

vector<Vector2> CollisionSystem::GetOffsetVertices(Vector2 offset, vector<Vector2> vertices)
{
	vector<Vector2> offset_vertices;

	for (unsigned int i = 0; i < vertices.size(); i++) {
		offset_vertices.push_back(vertices[i] + offset);
	}
	return offset_vertices;
}

/* Calculate the projection of a polygon on an axis and returns it as a [min, max] interval */
void CollisionSystem::ProjectPolygon(vector<Vector2> offset_vertices_poly, Vector2 axis, float &min, float &max)
{
	float dotp = axis >> offset_vertices_poly[0];
	min = dotp;
	max = dotp;
	for (unsigned int i = 0; i < offset_vertices_poly.size(); i++)
	{
		dotp = offset_vertices_poly[i] >> axis;
		if (dotp < min)
			min = dotp;
		else if (dotp > max)
			max = dotp;
	}
}

/*
Calculate the distance between [minA, maxA] and [minB, maxB].
The distance will be negative if the intervals overlap.
*/
float CollisionSystem::IntervalDistance(float minA, float maxA, float minB, float maxB)
{
	if (minA < minB)
		return minB - maxA;
	else
		return minA - maxB;
}



