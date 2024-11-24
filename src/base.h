#ifndef BASE_H
#define BASE_H

#include <stdint.h>
#include <stddef.h>

//sdl
#include <SDL3/SDL_timer.h>

// base types
typedef int8_t i8;
typedef uint8_t u8;
typedef int16_t i16;
typedef uint16_t u16;
typedef int32_t i32;
typedef uint32_t u32;
typedef int64_t i64;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

constexpr f32 MILLISECONDS_TO_SECONDS = 0.001f;
constexpr f32 MAX_DELTA_TIME = 0.1f;

///////////////////////////////////
/////~ get delta from SDL
struct TimeState
{
	u64 lastTicks = 0;
	f32 deltaTime = 0.0f;
};

inline f32 GetDeltaTime(TimeState& time)
{
	u64 currentTicks = SDL_GetTicks();
	u64 deltaTicks = currentTicks - time.lastTicks;
	time.lastTicks = currentTicks;

	time.deltaTime = deltaTicks * MILLISECONDS_TO_SECONDS;
	return (time.deltaTime > MAX_DELTA_TIME) ? MAX_DELTA_TIME : time.deltaTime;
}

//direction
enum Direction
{
	Up,
	Down,
	Left,
	Right
};

//math

#endif
