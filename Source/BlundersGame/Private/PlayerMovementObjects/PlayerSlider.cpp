// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/PlayerMovementObjects/PlayerSlider.h"
#include "BlundersGame/Public/PlayerBase/PlayerCharacter.h"

//Function for Player Reached Slider 
void APlayerSlider::OnActorOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
    //Checking if overlapped actor is actually a player
    Player = Cast<APlayerCharacter>(OtherActor);
    if (Player && !IsUsed)
    {        
        OnActorBeginOverlap.Clear();
        //Player can overlap spline between it's two points (Ceil and Floor)
        //So in order to calculate starting TrackAlpha we should get
        //Length to Ceil, Length to Floor and interpolate between then
        //Here all required parameters are set
        const float CurrentSplineKey = SplineComponent->FindInputKeyClosestToWorldLocation(Player->GetActorLocation());
        const int CeilKey = FMath::CeilToInt(CurrentSplineKey);
        const int FloorKey = FMath::FloorToInt(CurrentSplineKey);
        const float FracKey = FMath::Frac(CurrentSplineKey);

        //Calculating current distance, where overlapping have occured
        //By Lerping between Ceil distance and Floor distance
        const float CurrentDistance = FMath::Lerp(SplineComponent->GetDistanceAlongSplineAtSplinePoint(CeilKey),
                                                  SplineComponent->GetDistanceAlongSplineAtSplinePoint(FloorKey),
                                                  FracKey);

        //TrackAlpha
        TrackAlpha = CurrentDistance / TrackLength;
        //Setting Track direction
        const FVector SplineDirection = SplineComponent->FindDirectionClosestToWorldLocation(Player->GetActorLocation(), ESplineCoordinateSpace::World);
        //Using dot product to find an angle cos between Player orientation and Spline direction
        if ((SplineDirection | Player->GetActorForwardVector()) < 0.0)
        {
            //Moving Backwards along spline
            TrackAlphaSumator = -CharacterSpeed / TrackLength * GetWorld()->GetDeltaSeconds();
            YawOffset = 180.0;
            PitchRollMultiplier = -1.0;

        } else
        {
            //Moving Forwards along spline
            TrackAlphaSumator = CharacterSpeed / TrackLength * GetWorld()->GetDeltaSeconds();
            YawOffset = 0.0;
            PitchRollMultiplier = 1.0;
        }

        SetIsUsed(true);
        Player->MovementObject = this;
        Player->PlayerSlide();
    }
}

void APlayerSlider::BeginPlay()
{
    Super::BeginPlay();

    TrackLength = GetSegmentsCount() * SegmentLength;
    OnActorBeginOverlap.AddDynamic(this, &APlayerSlider::OnActorOverlapBegin);
}

void APlayerSlider::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (Player)
    {
        //Calculating current track distance
        //Maybe not the best solution, but basically
        //Interpolating between 0.0(start of track)
        //And trackLength(end of the track) by parameter
        //TrackAlpha - it's changing each tick
        const float CurrentTrackDistance = FMath::Lerp(0.0f, TrackLength, TrackAlpha);                

        //Player Rotation calculation
        FRotator PlayerRotation = SplineComponent->GetRotationAtDistanceAlongSpline(CurrentTrackDistance, ESplineCoordinateSpace::World);
        PlayerRotation.Roll *= PitchRollMultiplier;
        PlayerRotation.Pitch *= PitchRollMultiplier;
        PlayerRotation.Yaw += YawOffset;

        //Player Location calculation
        FVector PlayerLocation = SplineComponent->GetLocationAtDistanceAlongSpline(CurrentTrackDistance, ESplineCoordinateSpace::World);
        const float PlayerHalfHeight = Player->GetSimpleCollisionHalfHeight();
        const FVector TrackNormal = SplineComponent->GetUpVectorAtDistanceAlongSpline(CurrentTrackDistance, ESplineCoordinateSpace::World);
        PlayerLocation += (ZOffset + PlayerHalfHeight) * TrackNormal;

        //Setting Player's location and rotation
        Player->SetActorLocation(PlayerLocation);
        Player->SetActorRotation(PlayerRotation);

        TrackAlpha += TrackAlphaSumator;

        //Should be Implementation of player leaving slider
        if (TrackAlpha > 1.0 || TrackAlpha < 0.0)
        {            
            Player->SetCurrentState(EPlayerMovementStates::Slide);
        }
    }
}

APlayerSlider::APlayerSlider()
{
    PrimaryActorTick.bCanEverTick = true;
    SetActorTickEnabled(false);

    Player = nullptr;
    IsUsed = false;
}
