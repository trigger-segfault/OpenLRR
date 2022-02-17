// Random.hpp : C++ classes for the random number generator used by LRR's C runtime library.
//

#pragma once

#include "../../common.h"


namespace Random
{; // !<---

#pragma region LCG Engine

/**
 * @brief Random number generator engine: MS Quick C - Linear Congruential Generator (LCG).
 *        Standalone version that uses its own state field.
 */
class LCGEngine
{
	// <https://www.csee.umbc.edu/courses/undergraduate/426/fall14/lectures/l19/l19slides.pdf>
	// <https://web.archive.org/web/20210913181809/https://www.csee.umbc.edu/courses/undergraduate/426/fall14/lectures/l19/l19slides.pdf>

	// Linear Congruential Generator (LCG) Parameters
	// 
	// |    impl    |  n   |   a    |    c    |
	// |:----------:|:----:|:------:|:-------:|
	// | MS Quick C | 2^32 | 214013 | 2531011 |

public:
	static constexpr const sint32 multiplier = 214013; /** @brief The multiplier term (a). */
	static constexpr const sint32 increment = 2531011; /** @brief The increment term (c). */
	static constexpr const sint32 shift          = 16; /** @brief The shift right term before applying the modulus. */
	static constexpr const sint32 modulus    = 0x7fff; /** @brief The modulus term (m). */
	static constexpr const uint32 default_seed    = 1; /** @brief The default seed (1). */

	static constexpr const sint32 rand_max  = modulus; /** @brief The maximum value producible by the engine. */

	/**
	 * @brief Constructs a standalone LCG engine.
	 * @param initialSeed Optional seeded state to start with.
	 */
	LCGEngine(uint32 initialSeed = default_seed);

	/**
	 * @brief Gets the current value for the engine's state. Can be used in combination
	 *        with `void Seed(uint32)` to backup and restore the state of the PRNG.
	 */
	inline uint32 State() const { return this->m_state; }

	/**
	 * @brief Seeds the engine's state. Can be used in combination with
	 *        `uint32 State()` to backup and restore the state of the PRNG.
	 * @param seed The new seed.
	 */
	inline void Seed(uint32 seed) { this->m_state = seed; }

	/**
	 * @brief Generates the next random number in the engine's state.
	 */
	sint32 Next();

protected:
	uint32 m_state;
};


/**
 * @brief Random number generator engine: MS Quick C - Linear Congruential Generator (LCG).
 *        Reference version that manipulates existing memory.
 */
class WrapperLCGEngine : public LCGEngine // Inherit from `LCGEngine` just for the constants.
{
public:
	/**
	 * @brief Constructs an LCG engine wrapped around an existing state field.
	 * @param stateInMemory The reference to memory holding the engine state.
	 */
	WrapperLCGEngine(uint32& stateInMemory);

	/**
	 * @brief Gets the current value for the engine's state. Can be used in combination
	 *        with `void Seed(uint32)` to backup and restore the state of the PRNG.
	 */
	inline uint32 State() const { return this->m_stateInMemory; }
	/**
	 * @brief Seeds the engine's state. Can be used in combination with
	 *        `uint32 State()` to backup and restore the state of the PRNG.
	 * @param seed The new seed.
	 */
	inline void Seed(uint32 seed) { this->m_stateInMemory = this->m_state = seed; }

	/**
	 * @brief Generates the next random number in the engine's state.
	 */
	sint32 Next();

private:
	uint32& m_stateInMemory;
};

#pragma endregion

}
