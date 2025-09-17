#include "PlayerBase/PlayerMovement/CharacterMovementParams.h"

FMovementStateData& FMovementStateData::operator*=(float Right)
{
    StateVelocity *= Right;
    StateCooldown *= Right;
    StateEnergyCost *= Right;
    StateWorkTime *= Right;
    MovementArcAngle *= Right;
    return *this;
}
