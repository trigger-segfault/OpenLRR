// Random.cpp : C++ classes for the random number generator used by LRR's C runtime library.
//

#include "Random.hpp"


#pragma region LCG number generation

// Plain view of number generation:
//state = (state * 214013) + 2531011;
//return ((sint32)state >> 16) & 0x7fff;

// Step 1: Modifies and returns the next LCG state. Which can then be used with
// `uint32 lcgRandFromState(uint32)` to generate the next random number.
static constexpr uint32 lcgNextState(uint32 state)
{
	return (state * Random::LCGEngine::multiplier) + Random::LCGEngine::increment;
	//return (state * 214013) + 2531011;
}
// Step 2: Generates a random number from the modified LCG state returned by
// `uint32 lcgNextState(uint32)`.
static constexpr sint32 lcgRandFromState(uint32 state)
{
	// Using & here, because its cheaper for our modulus term of `0x7fff`.
	return ((sint32)state >> Random::LCGEngine::shift) & Random::LCGEngine::modulus;
	//return ((sint32)state >> 16) & 0x7fff;
}

#pragma endregion



#pragma region LCG engine classes

Random::LCGEngine::LCGEngine(uint32 initialSeed)
	: m_state(initialSeed)
{
}

sint32 Random::LCGEngine::Next()
{
	this->m_state = lcgNextState(this->m_state);
	return lcgRandFromState(this->m_state);
}



Random::WrapperLCGEngine::WrapperLCGEngine(uint32& stateInMemory)
	: m_stateInMemory(stateInMemory)
{
	this->m_state = this->m_stateInMemory;
}

sint32 Random::WrapperLCGEngine::Next()
{
	this->m_stateInMemory = this->m_state = lcgNextState(this->m_stateInMemory);
	return lcgRandFromState(this->m_stateInMemory);
}

#pragma endregion
