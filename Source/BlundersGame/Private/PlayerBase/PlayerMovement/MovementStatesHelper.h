#pragma once

#include "CoreMinimal.h"
#include "PlayerBase/MovementManager.h"


class UPlayerMovementState;

namespace FMovementStatesInitializer
{
	void InitMovementStatesMap(
		TMap<EPlayerMovementStates, UPlayerMovementState*>& OutMap, AActor* Owner,
		const FCharacterMovementSetParams& MovementSetParams);
}
