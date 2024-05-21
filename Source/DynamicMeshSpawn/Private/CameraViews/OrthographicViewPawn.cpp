// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraViews/OrthographicViewPawn.h"
#include "Math/MathFwd.h"

// Sets default values
AOrthographicViewPawn::AOrthographicViewPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetCapsuleHalfHeight(50);
	CapsuleComponent->SetCapsuleRadius(20);
	CapsuleComponent->SetEnableGravity(false);
	RootComponent = CapsuleComponent;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 0.0f;
	SpringArmComponent->TargetOffset = FVector(0, 0, 600);
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->bEnableCameraRotationLag = true;
	SpringArmComponent->CameraLagSpeed = 5.0f;
	SpringArmComponent->CameraRotationLagSpeed = 30.0f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetRelativeRotation(FRotator(-90.f, 0, 0));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

	CameraCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CameraCollisionComponent"));
	CameraCollisionComponent->InitBoxExtent(FVector(50.0f, 23.5f, 20.f));
	CameraCollisionComponent->SetRelativeLocation(FVector(-45.f, 0.f, 0.f));
	CameraCollisionComponent->SetupAttachment(CameraComponent);
	CameraCollisionComponent->SetCollisionProfileName(TEXT("CameraCollision Component"));

	CameraCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CameraCollisionComponent->SetCollisionObjectType(ECC_Pawn);
	CameraCollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
}

// Called when the game starts or when spawned
void AOrthographicViewPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOrthographicViewPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AOrthographicViewPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		UInputMappingContext* InputMappingContext = NewObject<UInputMappingContext>();

		UInputAction* MoveAction = NewObject<UInputAction>();
		MoveAction->ValueType = EInputActionValueType::Axis2D;

		UInputModifierSwizzleAxis* YXZSwizzleAxisModifier = NewObject<UInputModifierSwizzleAxis>();
		YXZSwizzleAxisModifier->Order = EInputAxisSwizzle::YXZ;

		UInputModifierNegate* MoveActionNegateModifier = NewObject<UInputModifierNegate>();
		MoveActionNegateModifier->bX = true;
		MoveActionNegateModifier->bY = true;
		MoveActionNegateModifier->bZ = true;

		InputMappingContext->MapKey(MoveAction, EKeys::W);

		FEnhancedActionKeyMapping& MoveActionBackwardKeyMapping = InputMappingContext->MapKey(MoveAction, EKeys::S);
		MoveActionBackwardKeyMapping.Modifiers.Add(MoveActionNegateModifier);

		FEnhancedActionKeyMapping& MoveActionRightKeyMapping = InputMappingContext->MapKey(MoveAction, EKeys::D);
		MoveActionRightKeyMapping.Modifiers.Add(YXZSwizzleAxisModifier);

		FEnhancedActionKeyMapping& MoveActionLeftKeyMapping = InputMappingContext->MapKey(MoveAction, EKeys::A);
		MoveActionLeftKeyMapping.Modifiers.Add(YXZSwizzleAxisModifier);
		MoveActionLeftKeyMapping.Modifiers.Add(MoveActionNegateModifier);

		//--------------- Zoom Action ---------------------//
		UInputAction* ZoomAction = NewObject<UInputAction>();
		ZoomAction->ValueType = EInputActionValueType::Axis1D;

		FEnhancedActionKeyMapping& ZoomMapping = InputMappingContext->MapKey(ZoomAction, EKeys::MouseWheelAxis);
		
		UInputAction* CameraRotation = NewObject<UInputAction>();
		CameraRotation->ValueType = EInputActionValueType::Axis1D;
		FEnhancedActionKeyMapping& MouseXRotation = InputMappingContext->MapKey(CameraRotation, EKeys::MouseX);
		//MouseXRotation.Modifiers.Add(YXZSwizzleAxisModifier);


		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AOrthographicViewPawn::Zoom);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AOrthographicViewPawn::Move);
		EnhancedInputComponent->BindAction(CameraRotation, ETriggerEvent::Triggered, this, &AOrthographicViewPawn::Rotate);

		if (const APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}

	}
}

void AOrthographicViewPawn::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// add movement 
	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AOrthographicViewPawn::Zoom(const FInputActionValue& Value) {

	if (CameraComponent) {
		float ZoomDelta = Value.Get<float>(); // 1 / -1

		//CameraComponent->SetRelativeRotation(FRotator(-90, 0, 0));
		SpringArmComponent->TargetOffset.Z -= ZoomDelta * 10;
		SpringArmComponent->TargetOffset.Z = FMath::Clamp(SpringArmComponent->TargetOffset.Z, 0, 1500);
	}
}

void AOrthographicViewPawn::Rotate(const FInputActionValue& ActionValue)
{
	float MouseX = ActionValue.Get<float>();
	if (CameraComponent)
	{
		FRotator CameraRotation = CameraComponent->GetRelativeRotation();

		CameraRotation.Roll += MouseX;
		CameraComponent->SetRelativeRotation(CameraRotation);
	}
}