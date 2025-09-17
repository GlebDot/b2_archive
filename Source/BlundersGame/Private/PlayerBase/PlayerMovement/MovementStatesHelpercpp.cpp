#include "MovementStatesHelper.h"

#include "PlayerMovementStates/PlayerDashState.h"
#include "PlayerMovementStates/PlayerFallDownStates.h"
#include "PlayerMovementStates/PlayerJumpStates.h"
#include "PlayerMovementStates/PlayerMovementState.h"
#include "PlayerMovementStates/PlayerSlideState.h"
#include "PlayerMovementStates/SpecialAttacksStates.h"

namespace FMovementStatesInitializerLocal
{
	constexpr float JumpMultiplierHigh = 2.0f;
	constexpr float DashMultiplierLong = 1.25f;
}

void FMovementStatesInitializer::InitMovementStatesMap(
    TMap<EPlayerMovementStates, UPlayerMovementState*>& OutMap, AActor* Owner,
    const FCharacterMovementSetParams& MovementSetParams)
{
    static const FName DefaultName = FName(TEXT("MS_Default")); 
    UPlayerMovementState* Default = NewObject<UPlayerMovementState>(Owner, UPlayerMovementState::StaticClass(), DefaultName);
	Default->SetParameters(FMovementStateData{});
	OutMap.Add(EPlayerMovementStates::Default, Default);

	static const FName JumpGroundName = FName(TEXT("MS_GroundJump"));
	UPlayerJumpStateGround* JumpGround = NewObject<UPlayerJumpStateGround>(Owner, UPlayerJumpStateGround::StaticClass(), JumpGroundName);
	JumpGround->SetParameters(MovementSetParams.JumpParamsBase);
	OutMap.Add(EPlayerMovementStates::JumpGround, JumpGround);

	static const FName JumpAirName = FName(TEXT("MS_AirJump"));
	UPlayerJumpStateMidAir* JumpAir = NewObject<UPlayerJumpStateMidAir>(Owner, UPlayerJumpStateMidAir::StaticClass(), JumpAirName);
	JumpAir->SetParameters(MovementSetParams.JumpParamsBase);
	OutMap.Add(EPlayerMovementStates::JumpAir, JumpAir);

	static const FName JumpWallName = FName(TEXT("MS_WallJump"));
	UPlayerJumpStateWall* JumpWall = NewObject<UPlayerJumpStateWall>(Owner, UPlayerJumpStateWall::StaticClass(), JumpWallName);
	JumpWall->SetParameters(MovementSetParams.JumpParamsWall);
	OutMap.Add(EPlayerMovementStates::JumpWall, JumpWall);

	static const FName JumpLongName = FName(TEXT("MS_LongJump"));
	UPlayerJumpStateLong* JumpLong = NewObject<UPlayerJumpStateLong>(Owner, UPlayerJumpStateLong::StaticClass(), JumpLongName);
	JumpLong->SetParameters(MovementSetParams.JumpParamsLong);
	OutMap.Add(EPlayerMovementStates::JumpLong, JumpLong);

	static const FName JumpHighName = FName(TEXT("MS_HighJump"));
	UPlayerJumpStateHigh* JumpHigh = NewObject<UPlayerJumpStateHigh>(Owner, UPlayerJumpStateHigh::StaticClass(), JumpHighName);
	JumpHigh->SetParameters(MovementSetParams.JumpParamsBase, FMovementStatesInitializerLocal::JumpMultiplierHigh);
	OutMap.Add(EPlayerMovementStates::JumpHigh, JumpHigh);

	static const FName JumpJumperName = FName(TEXT("MS_JumperJump"));
	UPlayerJumpStateJumper* JumpJumper = NewObject<UPlayerJumpStateJumper>(Owner, UPlayerJumpStateJumper::StaticClass(), JumpJumperName);
	JumpJumper->SetParameters(MovementSetParams.JumpParamsBase);
	OutMap.Add(EPlayerMovementStates::JumpJumper, JumpJumper);

	static const FName JumpBackName = FName(TEXT("MS_BackJump"));
	UPlayerJumpStateBackward* JumpBack = NewObject<UPlayerJumpStateBackward>(Owner, UPlayerJumpStateBackward::StaticClass(), JumpBackName);
	JumpBack->SetParameters(MovementSetParams.JumpParamsBack);
	OutMap.Add(EPlayerMovementStates::JumpBack, JumpBack);

	static const FName JumpSliderName = FName(TEXT("MS_SliderJump"));
	UPlayerJumpStateSlider* JumpSlider = NewObject<UPlayerJumpStateSlider>(Owner, UPlayerJumpStateSlider::StaticClass(), JumpSliderName);
	JumpSlider->SetParameters(MovementSetParams.JumpParamsBase);
	OutMap.Add(EPlayerMovementStates::JumpSlider, JumpSlider);
	
	static const FName JumpEnemyName = FName(TEXT("MS_EnemyJump"));
	UPlayerJumpStateEnemy* JumpEnemy = NewObject<UPlayerJumpStateEnemy>(Owner, UPlayerJumpStateEnemy::StaticClass(), JumpEnemyName);
	JumpEnemy->SetParameters(MovementSetParams.JumpParamsBase);
	OutMap.Add(EPlayerMovementStates::JumpEnemy, JumpEnemy);


	static const FName DashBaseName = FName(TEXT("MS_DashBase"));
	UPlayerDashState* DashBase = NewObject<UPlayerDashState>(Owner, UPlayerDashState::StaticClass(), DashBaseName);
	DashBase->SetParameters(MovementSetParams.DashParamsBase);
	OutMap.Add(EPlayerMovementStates::DashBase, DashBase);

	static const FName DashVerticalName = FName(TEXT("MS_DashVertical"));
	UPlayerDashStateVertical* DashVertical = NewObject<UPlayerDashStateVertical>(Owner, UPlayerDashStateVertical::StaticClass(), DashVerticalName);
	DashVertical->SetParameters(MovementSetParams.DashParamsVertical);
	OutMap.Add(EPlayerMovementStates::DashVertical, DashVertical);

	static const FName DashLongName = FName(TEXT("MS_DashLong"));
	UPlayerDashStateLong* DashLong = NewObject<UPlayerDashStateLong>(Owner, UPlayerDashStateLong::StaticClass(), DashLongName);
	DashLong->SetParameters(MovementSetParams.DashParamsBase, FMovementStatesInitializerLocal::JumpMultiplierHigh);
	OutMap.Add(EPlayerMovementStates::DashLong, DashLong);

	static const FName DropDownBaseName = FName(TEXT("MS_DropDown"));
	UPlayerFallDownState* DropDownBase = NewObject<UPlayerFallDownState>(Owner, UPlayerFallDownState::StaticClass(), DropDownBaseName);
	DropDownBase->SetParameters(MovementSetParams.DropDownParamsBase);
	OutMap.Add(EPlayerMovementStates::DropDown, DropDownBase);

	// Do not setup Parameters since they do not need them
	static const FName AttackAirName = FName(TEXT("MS_AttackAir"));
	UPlayerAttackStateAir* AttackAir = NewObject<UPlayerAttackStateAir>(Owner, UPlayerAttackStateAir::StaticClass(), AttackAirName);
	OutMap.Add(EPlayerMovementStates::AirAttack, AttackAir);

	// Do not setup Parameters since they do not need them
	static const FName AttackGroundName = FName(TEXT("MS_AttackGround"));
	UPlayerAttackStateGround* AttackGround = NewObject<UPlayerAttackStateGround>(Owner, UPlayerAttackStateGround::StaticClass(), AttackGroundName);
	OutMap.Add(EPlayerMovementStates::GroundAttack, AttackGround);


	// I really do not know what to do with this shiet
	static const FName SlideBaseName = FName(TEXT("MS_SlideBase"));
	UPlayerSlideState* SlideBase = NewObject<UPlayerSlideState>(Owner, UPlayerSlideState::StaticClass(), SlideBaseName);
	OutMap.Add(EPlayerMovementStates::Slide, SlideBase);
}
