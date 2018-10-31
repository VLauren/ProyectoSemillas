#include "MainChar.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/Engine.h"

const float AMainChar::MOVEMENT_SPEED = 500.0f;

AMainChar* AMainChar::Instance;

AMainChar::AMainChar()
{
	PrimaryActorTick.bCanEverTick = true;

	// Creo la capsula de colision del personaje
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	CapsuleComponent->InitCapsuleSize(34.0f, 88.0f);
	CapsuleComponent->SetCollisionProfileName("MainChar");
	CapsuleComponent->CanCharacterStepUpOn = ECB_No;
	CapsuleComponent->bCheckAsyncSceneOnMove = false;
	CapsuleComponent->SetCanEverAffectNavigation(false);
	CapsuleComponent->bDynamicObstacle = true;
	RootComponent = CapsuleComponent;

	CapsuleComponent->SetVisibility(true);
	CapsuleComponent->SetHiddenInGame(false);

	//Cel spring arm para la camara
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 700.0f;

	// Agrego la camara
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Con el input, la camara no rota pero el brazo si
	CameraBoom->bUsePawnControlRotation = true;
	FollowCamera->bUsePawnControlRotation = true;

	// Malla
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	if (Mesh != nullptr)
	{
		Mesh->AlwaysLoadOnClient = true;
		Mesh->AlwaysLoadOnServer = true;
		Mesh->bOwnerNoSee = false;
		Mesh->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPose;
		Mesh->bCastDynamicShadow = true;
		Mesh->bAffectDynamicIndirectLighting = true;
		Mesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		Mesh->SetupAttachment(CapsuleComponent);
		static FName MeshCollisionProfileName(TEXT("CharacterMesh"));
		Mesh->SetCollisionProfileName(MeshCollisionProfileName);
		Mesh->SetGenerateOverlapEvents(false);
		Mesh->SetCanEverAffectNavigation(false);
	}

	// Componente de movimiento
	Movement = CreateDefaultSubobject<UMainCharMovement>(TEXT("Movement"));
	Movement->UpdatedComponent = RootComponent;

	Instance = this;
}

// Called when the game starts or when spawned
void AMainChar::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMainChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// input de camara
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	// input de movimiento (controller)
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainChar::MoveRight);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainChar::MoveForward);

	// input de acciones
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMainChar::Attack);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainChar::Jump);
	PlayerInputComponent->BindAction("Ability", IE_Pressed, this, &AMainChar::Ability);
}

void AMainChar::MoveForward(float AxisValue)
{
	// UE_LOG(LogTemp, Warning, TEXT("MoveForward %f"), AxisValue);

	if (Movement && (Movement->UpdatedComponent == RootComponent))
		// Movement->AddInputVector(FVector(1, 0, 0) * AxisValue);
		// Movement->AddInputVector(FRotator(0, GetControlRotation().Yaw, 0).RotateVector(GetActorForwardVector()) * AxisValue);
		Movement->AddInputVector(FRotator(0, GetControlRotation().Yaw, 0).RotateVector(FVector(1, 0, 0)) * AxisValue);
}

void AMainChar::MoveRight(float AxisValue)
{
	// UE_LOG(LogTemp, Warning, TEXT("MoveRight %f"), AxisValue);

	if (Movement && (Movement->UpdatedComponent == RootComponent))
		// Movement->AddInputVector(FVector(0, 1, 0) * AxisValue);
		// Movement->AddInputVector(FRotator(0, GetControlRotation().Yaw, 0).RotateVector(GetActorRightVector()) * AxisValue);
		Movement->AddInputVector(FRotator(0, GetControlRotation().Yaw, 0).RotateVector(FVector(0, 1, 0)) * AxisValue);
}

void AMainChar::Jump()
{
	UE_LOG(LogTemp, Warning, TEXT("Jump"));

	//HACK
	if(!Underground)
		Movement->Jump();
}

void AMainChar::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack"));
}

void AMainChar::Ability()
{
	UE_LOG(LogTemp, Warning, TEXT("Ability"));


	// HACK por ahora hago la habilidad directamente aqui
	if (!Underground)
	{
		// Hago invisible los meshes

		// Capsula mas baja
		CapsuleComponent->SetCapsuleSize(34.0f, 34.0f);
		AddActorWorldOffset(FVector(0, 0, -54.0f));

		TArray<UStaticMeshComponent*> Meshes;
		GetComponents<UStaticMeshComponent>(Meshes);
		for (size_t i = 0; i < Meshes.Num(); i++)
		{
			Meshes[i]->SetVisibility(false);
		}

		Underground = true;
	}
	else
	{
		TArray<UStaticMeshComponent*> Meshes;
		GetComponents<UStaticMeshComponent>(Meshes);
		for (size_t i = 0; i < Meshes.Num(); i++)
			Meshes[i]->SetVisibility(true);

		CapsuleComponent->SetCapsuleSize(34.0f, 88.0f);
		Underground = false;
	}

}

FVector AMainChar::GetPlayerLocation()
{
	return FVector();
}

