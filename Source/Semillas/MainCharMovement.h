// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "MainCharMovement.generated.h"

/**
 * 
 */
UCLASS()
class SEMILLAS_API UMainCharMovement : public UPawnMovementComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	void Jump();

	bool IsGrounded();

private:
	class USkeletalMeshComponent* Mesh = nullptr;

	FVector Move;
	float YVel;
	int32 justJumped;
	FRotator CurrentRotation;
};
