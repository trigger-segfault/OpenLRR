// BezierCurve.cpp :
//

#include "../../engine/Main.h"
#include "../../engine/core/Maths.h"

#include "BezierCurve.h"


/// DEBUG:
//#define BEZIERCURVE_LOGGING


#ifdef BEZIERCURVE_LOGGING

static struct RoutingLog
{
	const LegoRR::BezierCurve* key; // used as key
	FILE* file; // output log file
	uint32 lastTime;
};
static struct RoutingDistanceArgs
{
	const char* name;
	uint32 time;
	Point2F r;
	real32 newLength;
};

static uint32 routeLogDistanceQueue = 0; // BezierCurve_Vector2DDistance is called without passing BezierCurve*
static RoutingDistanceArgs routeLogDistanceArgs[10] = { 0 }; // always called in pairs before BuildPoints
static std::vector<RoutingLog> routeLogs{};

static RoutingLog* Routing_FindLog(const LegoRR::BezierCurve* route)
{
	if (routeLogs.empty()) ::_mkdir("logs");


	for (size_t i = 0; i < routeLogs.size(); i++) {
		if (routeLogs[i].key == route)
			return &routeLogs[i];
	}
	char buff[1024];
	std::sprintf(buff, "logs/routing_%p.log", route);
	FILE* file = std::fopen(buff, "at");
	routeLogs.push_back(RoutingLog{ route, file, 0U });
	return &routeLogs[routeLogs.size() - 1];
}

static void Routing_QueueDistance(const char* name, const Point2F* r, real32 newLength)
{
	uint32 time = Gods98::Main_GetTime();
	if (routeLogDistanceQueue >= 2) {
		std::printf("ERROR: Unexpected routeLogDistanceQueue count %i during Routing_QueueDistance!", (sint32)routeLogDistanceQueue);

		if (routeLogDistanceQueue >= _countof(routeLogDistanceArgs))
			return;
	}
	routeLogDistanceArgs[routeLogDistanceQueue].name = name;
	routeLogDistanceArgs[routeLogDistanceQueue].time = time;
	routeLogDistanceArgs[routeLogDistanceQueue].r = *r;
	routeLogDistanceArgs[routeLogDistanceQueue].newLength = newLength;
	routeLogDistanceQueue++;
}

#define Routing_LogBegin(log, curve, time, name) std::fprintf((log)->file, "%p [%010u]  %s( ", (void*)(curve), (uint32)(time), (const char*)(name))
#define Routing_LogEnd(log) std::fprintf((log)->file, " )\n")
	//; std::fflush(log->file)

static void Routing_LogCall(const LegoRR::BezierCurve* route, const char* name, const char* msg, ...)
{
	uint32 time = Gods98::Main_GetTime();
	RoutingLog* log = Routing_FindLog(route);
	if (routeLogDistanceQueue) {
		if (routeLogDistanceQueue != 2)
			std::printf("ERROR: Unexpected routeLogDistanceQueue count %i during Routing_LogCall!", (sint32)routeLogDistanceQueue);

		for (uint32 i = 0; i < routeLogDistanceQueue; i++) {
			RoutingDistanceArgs* args = &routeLogDistanceArgs[i];
			Routing_LogBegin(log, route, args->time, args->name);
			//std::fprintf(log->file, "[%i]  a={%f, %f} , b={%f, %f}", (sint32)i, args->a.x, args->a.y, args->b.x, args->b.y);
			std::fprintf(log->file, "r={%f, %f} , newLength=%f", args->r.x, args->r.y, args->newLength);
			//std::fprintf(log->file, "a={%f, %f} , b={%f, %f}", args->a.x, args->a.y, args->b.x, args->b.y);
			Routing_LogEnd(log);
		}
		routeLogDistanceQueue = 0;
	}
	std::va_list argptr;
	va_start(argptr, msg);
	Routing_LogBegin(log, route, time, name);
	std::vfprintf(log->file, msg, argptr);
	Routing_LogEnd(log);
	va_end(argptr);
	
	std::fflush(log->file);
	log->lastTime = time;
}

#endif /* BEZIERCURVE_LOGGING */

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00406520>
void __cdecl LegoRR::BezierCurve_Curve(OUT Point2F* r, const Point2F* p0, const Point2F* p1, const Point2F* p2, const Point2F*  p3, real32 t)
{
	log_firstcall();

	real32 fVar1 = p1->x * 3.0f + p0->x;
	real32 t2 = t*t;
	real32 t3 = t*t*t;
	real32 fVar4 = (fVar1 - p0->x) * 3.0f;
	real32 fVar3 = ((p2->x - p3->x * 3.0f) - fVar1) * 3.0f - fVar4;
	real32 fVar6 = p1->y * 3.0f + p0->y;
	r->x = fVar4 * t + fVar3 * t2 + (((p2->x - p0->x) - fVar4) - fVar3) * t3 + p0->x;
	fVar3 = (fVar6 - p0->y) * 3.0f;
	fVar1 = ((p2->y - p3->y * 3.0f) - fVar6) * 3.0f - fVar3;
	r->y = fVar3 * t + fVar1 * t2 + (((p2->y - p0->y) - fVar3) - fVar1) * t3 + p0->y;
}

// sqrt(((a.x-b.x)*(a.x-b.x)) + ((a.y-b.y)*(a.y-b.y)))
// <LegoRR.exe @00406660>
real32 __cdecl LegoRR::BezierCurve_Vector2DDistance(const Point2F* a, const Point2F* b)
{
	log_firstcall();

	return std::sqrt(((a->x-b->x)*(a->x-b->x)) + ((a->y-b->y)*(a->y-b->y)));
}

// r = norm(r) * newLength
// NOTE: Unlike `Maths_Vector2DSetLength`, this function modifies the input point.
// <LegoRR.exe @00406690>
Point2F* __cdecl LegoRR::BezierCurve_Vector2DChangeLength(IN OUT Point2F* r, real32 newLength)
{
	log_firstcall();
#ifdef BEZIERCURVE_LOGGING
	Routing_QueueDistance("BezierCurve_Vector2DChangeLength", r, newLength);
#endif

	/// FIXME: Two points of failure, zero length, or zero newLength
	real32 m = std::sqrt((r->x*r->x) + (r->y*r->y)) / newLength;
	r->x /= m;
	r->y /= m;
	return r;
}

// <LegoRR.exe @004066e0>
real32 __cdecl LegoRR::BezierCurve_UpdateDistances(BezierCurve* curve)
{
	log_firstcall();
#ifdef BEZIERCURVE_LOGGING
	Routing_LogCall(curve, "BezierCurve_UpdateDistances", "");
#endif

	uint32 loop = 1;
	real32 l = 0.0f;
	curve->distances[0] = 0.0f;
	for (uint32 loop = 1; loop < curve->count; loop++) {
#ifdef BEZIERCURVE_LOGGING
		Routing_LogCall(curve, "Routing_Vector2DDistance", "curve->points[%2u-1]={%f, %f} , curve->points[%2u]={%f, %f}", loop, curve->points[loop-1].x, curve->points[loop-1].y, loop, curve->points[loop].x, curve->points[loop].y);
#endif
		real32 dist = BezierCurve_Vector2DDistance(&curve->points[loop-1], &curve->points[loop]);
		curve->distances[loop] = dist;
		l += dist;
	}
	return l;
}

// <LegoRR.exe @00406750>
void __cdecl LegoRR::BezierCurve_BuildPoints(BezierCurve* curve, const Point2F* p0, const Point2F* p1, const Point2F* p2, const Point2F* p3, uint32 count)
{
	log_firstcall();
#ifdef BEZIERCURVE_LOGGING
	Routing_LogCall(curve, "BezierCurve_BuildPoints", "p0={%f, %f} , p1={%f, %f} , p2={%f, %f} , p3={%f, %f}", p0->x, p0->y, p1->x, p1->y, p2->x, p2->y, p3->x, p3->y);
#endif

	count = std::min(count, (uint32)BEZIERCURVE_MAXPOINTS);

	curve->count = count;
	curve->points[0] = *p0;

	for (uint32 loop = 1; loop < count; loop++) {
		real32 t = (real32)loop * (1.0f / (real32)(count - 1));
#ifdef BEZIERCURVE_LOGGING
		Routing_LogCall(curve, "Routing_Curve", "curve->points[%2u]={%f, %f} , t=%f", loop, curve->points[loop].x, curve->points[loop].y, t);
#endif
		BezierCurve_Curve(&curve->points[loop], p0, p1, p2, p3, t);
	}
}

// <LegoRR.exe @004067f0>
uint32 __cdecl LegoRR::BezierCurve_Interpolate(const BezierCurve* curve, real32 currentDist, OUT Point2F* r)
{
	log_firstcall();
#ifdef BEZIERCURVE_LOGGING
	Routing_LogCall(curve, "BezierCurve_Interpolate", "currentDist=%f , r={%f, %f}", currentDist, r->x, r->y);
#endif

	real32 endDist = 0.0f;
	uint32 loop;
	for (loop = 1; loop < curve->count; loop++) {
		endDist += curve->distances[loop];
		if (endDist > currentDist) break;
	}

	if (loop < curve->count) {
		// assignment has no effect
		*r = curve->points[loop - 1];

		real32 delta = (currentDist - (endDist - curve->distances[loop])) / curve->distances[loop];
		r->x = (curve->points[loop].x - curve->points[loop-1].x) * delta + curve->points[loop-1].x;
		r->y = (curve->points[loop].y - curve->points[loop-1].y) * delta + curve->points[loop-1].y;
		return loop;
	}
	else {
		*r = curve->points[loop - 1];
		return loop;
	}
}

#pragma endregion
