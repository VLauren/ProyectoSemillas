
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MainCharMovement.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "MainChar.generated.h"

enum class EMainCharState : uint8
{
	STAND,
	RUN,
	AIR,
	ATTACK,
	HIT
};

UCLASS()
class SEMILLAS_API AMainChar : public APawn
{
	GENERATED_BODY()

	// Capsula
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* CapsuleComponent;

	// hitbox: caja
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* hitBox = nullptr;

	// camera boom
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	// follow camera
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	// movimiento
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UMainCharMovement* Movement;

public:
	AMainChar();

protected:
	virtual void BeginPlay() override;

public:

	static const float MOVEMENT_SPEED;

	static FVector GetPlayerLocation();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Mesh;

	// Animaciones
	// TODO

	// Estado
	EMainCharState CharState;

private:

	// referencia estatica al personaje
	static AMainChar* Instance;

	// Metodos de input
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Jump();
	void Attack();
	void Ability();

	// HACK
	bool Underground;

};
