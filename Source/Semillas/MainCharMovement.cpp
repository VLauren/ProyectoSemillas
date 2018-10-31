// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCharMovement.h"
#include "MainChar.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, text)

void UMainCharMovement::BeginPlay()
{
	Super::BeginPlay();

	Move = FVector::ZeroVector;
	YVel = 0;
}

void UMainCharMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	// SafeMoveUpdatedComponent

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
		return;

	// Calculo el vector de movimiento
	FVector movimientoDeseado = ConsumeInputVector().GetClampedToMaxSize(1.0f) * DeltaTime * AMainChar::MOVEMENT_SPEED;
	// Move = movimientoDeseado;
	Move = FMath::Lerp(Move, movimientoDeseado, 0.07f);
	Move.Z = YVel;

	FVector movimientoEsteFrame = Move;
	// FVector movimientoEsteFrame = movimientoDeseado;

	// Movimiento
	if (!movimientoEsteFrame.IsNearlyZero())
	{
		FHitResult Hit;

		// FRotator forward = ((ASwimer*)GetOwner())->CameraArm->RelativeRotation;
		// movimientoEsteFrame = forward.RotateVector(movimientoEsteFrame);

		// Movimiento
		SafeMoveUpdatedComponent(movimientoEsteFrame, UpdatedComponent->GetComponentRotation(), true, Hit);
		// Velocity += movimientoEsteFrame;

		// Si chocamos con algo, me deslizo sobre el
		if (Hit.IsValidBlockingHit())
			SlideAlongSurface(movimientoEsteFrame, 1.f - Hit.Time, Hit.Normal, Hit);

		
		// FString grounded = IsMovingOnGround() ? "true" : "false";
		FString grounded = IsFlying() ? "true" : "false";
		// UE_LOG(LogTemp, Warning, TEXT("velocity: %s, ground: %s"), *Velocity.ToString(), *grounded);
	}
	else
	{

	}

	if (!movimientoDeseado.IsNearlyZero())
	{
		// Rotacion de la malla
		FRotator ctrlRot = movimientoDeseado.Rotation();

		CurrentRotation = FMath::Lerp(CurrentRotation, ctrlRot, 0.1f);
		UpdatedComponent->GetOwner()->SetActorRotation(CurrentRotation);
	}

	if (IsGrounded())
	{
		// La velocidad Z es cero
		YVel = 0.0f;
	}
	else
	{
		// Aplico la gravedad
		YVel -= 0.35f;
	}

	if (justJumped > 0)
		justJumped--;
}

void UMainCharMovement::Jump()
{
	if (IsGrounded())
	{
		YVel = 12;
		justJumped = 4;
	}
}

bool UMainCharMovement::IsGrounded()
{
	if (justJumped > 0)
		return false;

	FHitResult OutHit;
	
	FVector Start = UpdatedComponent->GetOwner()->GetActorLocation();

	float CapsuleHalfHeight = Cast<UCapsuleComponent>(UpdatedComponent)->GetUnscaledCapsuleHalfHeight();
	FVector End = Start + FVector(0, 0, -CapsuleHalfHeight - 12); // Capsule Half Height = 88

	FCollisionQueryParams ColParams;

	// DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 1, 0, 1);

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, ColParams))
	{
		if (OutHit.bBlockingHit)
		{
			return true;

			print(FString::Printf(TEXT("You are hitting: %s"), *OutHit.GetActor()->GetName()));
			print(FString::Printf(TEXT("Impact Point: %s"), *OutHit.ImpactPoint.ToString()));
			print(FString::Printf(TEXT("Normal Point: %s"), *OutHit.ImpactNormal.ToString()));
		}
		return false;
	}

	// UWorld::LineTraceSingleByChannel()

	return false;
}



