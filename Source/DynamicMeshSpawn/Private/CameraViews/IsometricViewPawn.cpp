// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraViews/IsometricViewPawn.h"

// Sets default values
AIsometricViewPawn::AIsometricViewPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(SceneRoot);
	SpringArmComponent->SetRelativeRotation(FRotator{ -45.0, 45.0, 0.0 });
	SpringArmComponent->TargetArmLength = 2500;
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->bEnableCameraRotationLag = true;
	SpringArmComponent->CameraLagSpeed = 5.0f;
	SpringArmComponent->CameraRotationLagSpeed = 30.0f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("IsometricCamera"));
	CameraComponent->SetupAttachment(SpringArmComponent, SpringArmComponent->SocketName);
	CameraComponent->AddRelativeRotation(FRotator{ 0, 0, 0 });

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));

	RotationSpeed = 2.0f;
	TargetSpringArmYaw = 45.0f;
}

// Called when the game starts or when spawned
void AIsometricViewPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AIsometricViewPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator CurrentRotation = SpringArmComponent->GetRelativeRotation();
	FRotator TargetRotation = FRotator(CurrentRotation.Pitch, TargetSpringArmYaw, CurrentRotation.Roll);
	SpringArmComponent->SetRelativeRotation(FMath::Lerp(CurrentRotation, TargetRotation, DeltaTime));
}

// Called to bind functionality to input
void AIsometricViewPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		UInputMappingContext* InputMappingContext = NewObject<UInputMappingContext>();
		
		UInputAction* CameraLeftRotationAction = NewObject<UInputAction>();
		CameraLeftRotationAction->ValueType = EInputActionValueType::Boolean;
		InputMappingContext->MapKey(CameraLeftRotationAction, EKeys::Q);

		UInputAction* CameraRightRotationAction = NewObject<UInputAction>();
		CameraRightRotationAction->ValueType = EInputActionValueType::Boolean;
		InputMappingContext->MapKey(CameraRightRotationAction, EKeys::E);

		UInputAction* CameraZoomInOut = NewObject<UInputAction>();
		CameraZoomInOut->ValueType = EInputActionValueType::Axis1D;
		InputMappingContext->MapKey(CameraZoomInOut, EKeys::MouseWheelAxis);

		EnhancedInputComponent->BindAction(CameraLeftRotationAction, ETriggerEvent::Completed, this, &AIsometricViewPawn::CameraLeftRotation);
		EnhancedInputComponent->BindAction(CameraRightRotationAction, ETriggerEvent::Completed, this, &AIsometricViewPawn::CameraRightRotation);
		EnhancedInputComponent->BindAction(CameraZoomInOut, ETriggerEvent::Triggered, this, &AIsometricViewPawn::CameraZoom);

		if (const APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}


void AIsometricViewPawn::CameraLeftRotation()
{
	TargetSpringArmYaw += 45;
	TargetSpringArmYaw = FMath::Wrap(TargetSpringArmYaw, -180.0f, 180.0f);
}

void AIsometricViewPawn::CameraRightRotation()
{
	TargetSpringArmYaw -= 45;
	TargetSpringArmYaw = FMath::Wrap(TargetSpringArmYaw, -180.0f, 180.0f);
}

void AIsometricViewPawn::CameraZoom(const FInputActionValue& ActionValue)
{
	float value = ActionValue.Get<float>();

	SpringArmComponent->TargetArmLength -= value * 50;
	SpringArmComponent->TargetArmLength = FMath::Clamp(SpringArmComponent->TargetArmLength, 1500, 3000);
}