#include "BlundersGame/Public/PlayerBase/PlayerCharacter.h"
#include "BlundersGame/Public/DamageComponents/ArmourDamageHandlerComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "BlundersGame/Public/PlayerBase/Components/KeyHolderComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/EngineTypes.h"
#include "Components/CapsuleComponent.h"
#include "BlundersGame/Public/PlayerBase/Components/PlayerAmmoComponent.h"
#include "BlundersGame/Public/PlayerBase/Components/PlayerSpeedEnergyComponent.h"
#include "BlundersGame/Public/PlayerBase/Components/AdvancedActionsComponent.h"
#include "BlundersGame/Public/PlayerBase/UI/PlayerHUD.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "GameManagment/BlundersGameModeBase.h"
#include "Interactables/Base/Interactable.h"
#include "PlayerBase/BlundersGameInstance.h"
#include "PlayerMovementStates/SpecialAttacksStates.h"
#include "PlayerBase/PlayerMovement/MomentumAccumulatorComponent.h"
#include "PlayerBase/PlayerMovement/PlayerMovementComponent.h"
#include "PlayerWeapons/Base/WeaponryComponent.h"

APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass(ACharacter::CharacterMovementComponentName, UPlayerMovementComponent::StaticClass()))
{
	PrimaryActorTick.bCanEverTick = true;

	//Spawn camera and SpingArm
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));

	//Attach camera and SpringArm
	SpringArm->SetupAttachment((USceneComponent*)GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	SpringArm->bInheritPitch = true;

	DamageHandlerComponent = CreateDefaultSubobject<UArmourDamageHandlerComponent>(TEXT("ArmourDamageHandler"));
	AmmoComponent = CreateDefaultSubobject<UPlayerAmmoComponent>(TEXT("AmmoComponent"));
	SpeedEnergyComponent = CreateDefaultSubobject<UPlayerSpeedEnergyComponent>(TEXT("SpeedEnergyComponent"));
	AdvancedActionsComponent = CreateDefaultSubobject<UAdvancedActionsComponent>(TEXT("AdvancedActionsComponent"));
	KeyHolderComponent = CreateDefaultSubobject<UKeyHolderComponent>(TEXT("KeyHolderComponent"));

	//Spawn Movement Manager and Bag Mesh
	MovementManager = CreateDefaultSubobject<UMovementManager>(TEXT("Movement Manager"));

	static FName const MomentumAccumulatorName = FName(TEXT("MomentumAccumulator"));
	MomentumAccumulator = CreateDefaultSubobject<UMomentumAccumulatorComponent>(MomentumAccumulatorName);

	static FName const WeaponryComponentName = FName(TEXT("WeaponryComponent"));
	Weaponry = CreateDefaultSubobject<UWeaponryComponent>(WeaponryComponentName);

	//Spawn wall jump trigger
	WallJumpCheckBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Wall Jump Check Box"));
	WallJumpCheckBox->SetupAttachment((USceneComponent*)GetCapsuleComponent());

	//Spawn player's bottom trigger
	MovementTriggerBottom = CreateDefaultSubobject<UBoxComponent>(TEXT("Movement Trigger Bottom"));
	MovementTriggerBottom->SetupAttachment((USceneComponent*)GetCapsuleComponent());
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	PlayerMovementState = PlayerMovementStates::Run;

	GetCapsuleComponent()->OnComponentHit.AddUniqueDynamic(this, &APlayerCharacter::OnCapsuleComponentHit);
	
	PlayerHUD = Cast<APlayerHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	Weaponry->Init(this);
	
	if (PlayerHUD)
	{
		PlayerHUD->Init(this, Weaponry);
		if (bInCombatState)
		{
			PlayerHUD->SetSightVisibility(ESlateVisibility::Visible);
		} else
		{
			PlayerHUD->SetSightVisibility(ESlateVisibility::Hidden);
		}
	}
}

void APlayerCharacter::Falling()
{
	Super::Falling();
	if (PlayerMovementState == PlayerMovementStates::Run)
	{
		PlayerMovementState = PlayerMovementStates::Jump;
	}
}

void APlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	if (PlayerMovementState == PlayerMovementStates::Jump)
	{
		ResetPlayerJumps();
		PlayerMovementState = PlayerMovementStates::Run;
	}

	if (PlayerMovementComponent->GravityScale > 1.0f)
	{
		PlayerMovementComponent->GravityScale = 1.0f;
	}

	if (CurrentPlayerMovementState)
	{
		CurrentPlayerMovementState->OnLanding();
	}
}


// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	const float CameraPitchAlpha = Camera->GetComponentRotation().Pitch / 90.0;

	//Location interpolation
	FVector SpringArmLocation = SpringArm->GetRelativeLocation();
	SpringArmLocation.X = -CameraPitchAlpha * SpringArmPositionOffset;
	SpringArmLocation = FMath::VInterpTo(SpringArm->GetRelativeLocation(), SpringArmLocation, DeltaTime, CameraInterpSpeed);

	//ArmLength Interpolation
	const float TargetLength = SpringArmDefaultLength - CameraPitchAlpha * SpringArmLengthOffset;
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, TargetLength, DeltaTime, CameraInterpSpeed);
	SpringArm->SetRelativeLocation(SpringArmLocation);
}

void APlayerCharacter::FellOutOfWorld(const UDamageType& dmgType)
{
	ABlundersGameModeBase *GameMode = Cast<ABlundersGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		GameMode->RestartPlayerAfterFalloff(this);
	}
}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PlayerMovementComponent = Cast<UPlayerMovementComponent>(GetMovementComponent());
	if (IsValid(PlayerMovementComponent))
	{
		PlayerMovementComponent->Init(MomentumAccumulator, MovementManager);
	}

	MovementManager->Init(MomentumAccumulator, PlayerMovementComponent, this);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::PlayerJumpStart);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::PlayerJumpEnd);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &APlayerCharacter::PlayerDash);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::PlayerCrouchPressed);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &APlayerCharacter::PlayerCrouchReleased);
	
	PlayerInputComponent->BindAction("SpecialAction", IE_Pressed, this, &APlayerCharacter::PlayerSpecialAction);
	PlayerInputComponent->BindAction("CombatStateSwitch", IE_Pressed, this, &APlayerCharacter::PlayerSwitchCombatState);
	PlayerInputComponent->BindAction("WeaponSelector", IE_Pressed, this, &APlayerCharacter::ActivateWeaponSelector);
	PlayerInputComponent->BindAction("WeaponSelector", IE_Released, this, &APlayerCharacter::DeactivateWeaponSelector);
	PlayerInputComponent->BindAction("Cigarette", IE_Pressed, this, &APlayerCharacter::PlayerCigaretteSmoke);

	PlayerInputComponent->BindAction("SpecialAbility", IE_Pressed, this, &APlayerCharacter::UseSpecialAbility);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::PlayerInteract);

	PlayerInputComponent->BindAction("ShowPlayerKeys", IE_Pressed, KeyHolderComponent, &UKeyHolderComponent::ToggleKeysScreenVisibility);

	Weaponry->SetupPlayerInputComponent(InputComponent);

	//Placeholder
	// PlayerInputComponent->BindAction("Heal", IE_Pressed, this, &APlayerCharacter::Heal);
	// PlayerInputComponent->BindAction("Resupply", IE_Pressed, this, &APlayerCharacter::Resupply);
}

void APlayerCharacter::SetCurrentState(UPlayerMovementState* NewState, const float& FloatValue, const FVector& VectorValue)
{
	//Execute function on current movement state
	//Some Movement states have function, which should be executed after it's gone
	if (CurrentPlayerMovementState)
	{
		CurrentPlayerMovementState->OnChange();
	}
	
	//Set new movement state
	CurrentPlayerMovementState = NewState;

	CurrentPlayerMovementState->SetVectorParameter(VectorValue);
	CurrentPlayerMovementState->SetScalarParameter(FloatValue);
	
	CurrentPlayerMovementState->OnExecute();	
}

void APlayerCharacter::SetCurrentState(EPlayerMovementStates MovementState, const float& FloatValue, const FVector& VectorValue)
{
	SetCurrentState(MovementManager->GetMovementState(MovementState), FloatValue, VectorValue);
}

void APlayerCharacter::ResetCurrentState()
{
	SetCurrentState(MovementManager->GetMovementState(EPlayerMovementStates::Default));
}

void APlayerCharacter::MoveForward(float value)
{
	MovementInputForward = value;
	AddMovementInput(GetActorForwardVector(), value);
}

void APlayerCharacter::MoveRight(float value)
{
	MovementInputRight = value;
	AddMovementInput(GetActorRightVector(), value);
}

void APlayerCharacter::PlayerJumpEnd()
{
	PlayerMovementComponent->GravityScale = 1.0;
}

void APlayerCharacter::PlayerJumpStart()
{
	const bool IsFalling = GetMovementComponent()->IsFalling();
	
	if (IsFalling)
	{
		bool IsWallJump = false;
		if (MovementManager->GetCanWallJump())
		{
			//If Player in the air it's a wall jump or a mid air jump
			TArray<FOverlapResult> OverlapResults;

			//See is WallJumpCheckBox Overlaps any WorldStatic Actors
			const FCollisionObjectQueryParams CollisionParams = FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic);
		
			IsWallJump = WallJumpCheckBox->ComponentOverlapMulti(OverlapResults, GetWorld(), WallJumpCheckBox->GetComponentLocation(),
                WallJumpCheckBox->GetComponentRotation(), ECollisionChannel::ECC_WorldStatic,
                FComponentQueryParams::DefaultComponentQueryParams, CollisionParams);
		}
		if (IsWallJump)
		{
			//It's wall jump
			SetCurrentState(MovementManager->GetMovementState(EPlayerMovementStates::JumpWall));
		} else
		{
			//It's mid air jump
			SetCurrentState(MovementManager->GetMovementState(EPlayerMovementStates::JumpAir));
		}
	} else
	{
		//Trick for game feel when you jump higher if you hold the button
		PlayerMovementComponent->GravityScale = 1.0;
		//It's ground jump
		SetCurrentState(MovementManager->GetMovementState(EPlayerMovementStates::JumpGround));
	}
}

void APlayerCharacter::PlayerDash()
{
	SetCurrentState(MovementManager->GetMovementState(EPlayerMovementStates::DashBase));
}

void APlayerCharacter::PlayerSpecialAction()
{
	if (MovementManager->SpecialMoveCheck())
	{
		// UPlayerAttackStateGround *GroundAttack = NewObject<UPlayerAttackStateGround>();
		SetCurrentState(MovementManager->GetMovementState(EPlayerMovementStates::GroundAttack));
		// FHitResult OutHit = FHitResult();
		// const FVector StartTrace = GetActorLocation();
		// const FVector EndTrace = StartTrace + GetActorUpVector() * -10000.0f;
		// FCollisionObjectQueryParams CollisionObjectQueryParams = FCollisionObjectQueryParams();
		// CollisionObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
		// GetWorld()->LineTraceSingleByObjectType(OutHit, StartTrace, EndTrace, CollisionObjectQueryParams);
		// PlayerFireReleased();
		
		// if (PlayerMovementState == PlayerMovementStates::Jump && PlayerMovementComponent->GravityScale > 1.0f)
		// {
		// 	//GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Green, "Special Attack Air");
		// 	UAirAttackState *AirAttack = NewObject<UAirAttackState>();
		// 	SetCurrentState(AirAttack);
		// } else
		// {
		// 	//GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Green, "Special Attack Ground");
		// 	UGroundAttackState *GroundAttack = NewObject<UGroundAttackState>();
		// 	SetCurrentState(GroundAttack);
		// }
	}
}

void APlayerCharacter::PlayerSlide()
{
	SetCurrentState(MovementManager->GetMovementState(EPlayerMovementStates::Slide));
}

void APlayerCharacter::PlayerSwitchCombatState()
{
	//TODO: HUD should use delegate
	if (bInCombatState)
	{
		if (PlayerHUD)
		{
			PlayerHUD->SetSightVisibility(ESlateVisibility::Hidden);
		}
	} else
	{
		if (PlayerHUD)
		{
			PlayerHUD->SetSightVisibility(ESlateVisibility::Visible);
		}
	}

	bInCombatState = !bInCombatState;
	OnPlayerSwitchedCombatState.Broadcast(bInCombatState);
}

void APlayerCharacter::ActivateWeaponSelector()
{
	if (PlayerHUD)
	{
		PlayerHUD->ShowWeaponSelector();
	}
}

void APlayerCharacter::DeactivateWeaponSelector()
{
	if (PlayerHUD)
	{
		PlayerHUD->HideWeaponSelector();
	}
}

void APlayerCharacter::UseSpecialAbility()
{
	if (OnSpecialAbilityUsed.IsBound())
	{
		OnSpecialAbilityUsed.Execute();
	}
}

void APlayerCharacter::PlayerInteract()
{
	FHitResult HitResult;
	const float TraceDistance = SpringArm->TargetArmLength + 250.0f;
	const FVector StartTrace = Camera->GetComponentLocation();
	const FVector EndTrace = Camera->GetForwardVector() * TraceDistance + StartTrace;
	const bool IsHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECollisionChannel::ECC_WorldStatic);

	DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Orange, false, 2.0f, 0, 2.0f);

	if (IsHit)
	{
		if (HitResult.GetActor()->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
		{
			IInteractable::Execute_Interact(HitResult.GetActor(), this);
		}
	}
}

void APlayerCharacter::PlayerCrouchPressed()
{
	ShouldCrouchAttack = false;
	GetWorld()->GetTimerManager().SetTimer(TH_CrouchAttack, this, &APlayerCharacter::PlayerCrouchAirAttack, 0.5f, false);
}

void APlayerCharacter::PlayerCrouchReleased()
{
	if (PlayerMovementState == PlayerMovementStates::Jump && !ShouldCrouchAttack)
	{
		SetCurrentState(MovementManager->GetMovementState(EPlayerMovementStates::DropDown));
		GetWorld()->GetTimerManager().ClearTimer(TH_CrouchAttack);
	}

	ShouldCrouchAttack = false;
}

void APlayerCharacter::PlayerCrouchAirAttack()
{
	ShouldCrouchAttack = true;
	GetWorld()->GetTimerManager().ClearTimer(TH_CrouchAttack);
	if (MovementManager->SpecialMoveCheck())
	{
		// UPlayerAttackStateAir *AirAttack = NewObject<UPlayerAttackStateAir>();
		SetCurrentState(MovementManager->GetMovementState(EPlayerMovementStates::AirAttack));
	} else
	{
		SetCurrentState(MovementManager->GetMovementState(EPlayerMovementStates::DropDown));
	}
}


void APlayerCharacter::PlayerCigaretteSmoke()
{
	if (!bInCombatState && CigaretteSmokeMontage && AmmoComponent->AmmoRequestCheck(EAmmoRequestType::Cigarette, 1))
	{
		AmmoComponent->ReduceAmmo(EAmmoRequestType::Cigarette, 1);
		UAnimInstance *AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(CigaretteSmokeMontage);
	}
}

void APlayerCharacter::Heal()
{
	SpeedEnergyComponent->UseEnergy(ESpeedEnergyUsageType::Health);
}

void APlayerCharacter::Resupply()
{
	SpeedEnergyComponent->UseEnergy(ESpeedEnergyUsageType::Ammo);
}

//Stop player movement and restore his initial friction
void APlayerCharacter::PlayerStop(float BrakingFrictionFactorSave,float BrakingFrictionSave)
{
	PlayerStop();
	PlayerMovementComponent->BrakingFriction = BrakingFrictionSave;
	PlayerMovementComponent->BrakingFrictionFactor = BrakingFrictionFactorSave;
}

//Complitely stop player and launch him a little bit
void APlayerCharacter::PlayerStop()
{
	//Saving velocity
	FVector VelocitySave = GetVelocity();
	GetMovementComponent()->StopMovementImmediately();
	VelocitySave.Normalize();
	LaunchCharacter(VelocitySave * 1000.0f, true, true);

	//Setting Movement state
	if (PlayerMovementState != PlayerMovementStates::Slide)
	{
		if (GetMovementComponent()->IsFalling())
		{
			PlayerMovementState = PlayerMovementStates::Jump;
		} else
		{
			PlayerMovementState = PlayerMovementStates::Run;
		}
	}
}

void APlayerCharacter::ResetPlayerJumps()
{
	MovementManager->ResetJumps();
}


void APlayerCharacter::OnConstruction(const FTransform &transform)
{
	Super::OnConstruction(transform);
	
	MovementObject = nullptr;

	SpeedEnergyComponent->SetPlayerRef(this);
	AdvancedActionsComponent->SetPlayerRef(this);

	
	CurrentPlayerMovementState = nullptr;
}

void APlayerCharacter::Destroyed()
{
	Super::Destroyed();

	ABlundersGameModeBase *GameMode = Cast<ABlundersGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		GameMode->RestartPlayerAfterDeath();
	}
}

void APlayerCharacter::OnCapsuleComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);

	//TraceStart
	FVector TraceStart = GetActorLocation();
	TraceStart.Z -= GetCapsuleComponent()->GetScaledCapsuleHalfHeight() / 2.0;

	//TraceEnd
	FVector TraceEnd = TraceStart + (GetActorForwardVector() * GetMovementInputForward() + GetActorRightVector() * GetMovementInputRight()) * 200.0;

	FHitResult HitInfo;
	//Lower Trace
	bool IsHit = GetWorld()->LineTraceSingleByObjectType(HitInfo,
													TraceStart,
													TraceEnd,
													ObjectQueryParams,
													FCollisionQueryParams::DefaultQueryParam);

	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 0.5);

	if (IsHit)
	{
		TraceStart.Z += GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		TraceEnd.Z += GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		//Upper Trace
		IsHit = GetWorld()->LineTraceSingleByObjectType(HitInfo,
														TraceStart,
														TraceEnd,
														ObjectQueryParams,
														FCollisionQueryParams::DefaultQueryParam);

		//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 0.5);
		//Only If we hit lower and not upper 
		if (!IsHit)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Yellow, "TeleportTime");
		}
	}
}
