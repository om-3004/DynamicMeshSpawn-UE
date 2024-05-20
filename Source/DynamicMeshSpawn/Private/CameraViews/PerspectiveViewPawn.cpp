// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraViews/PerspectiveViewPawn.h"

// Sets default values
APerspectiveViewPawn::APerspectiveViewPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetCapsuleHalfHeight(50);
	CapsuleComponent->SetCapsuleRadius(20);
	CapsuleComponent->SetEnableGravity(false);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CapsuleComponent->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	CapsuleComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	RootComponent = CapsuleComponent;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComponent->SetupAttachment(CapsuleComponent);

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;
	bUseControllerRotationRoll = false;

}

// Called when the game starts or when spawned
void APerspectiveViewPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APerspectiveViewPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APerspectiveViewPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		UInputMappingContext* InputMappingContext = NewObject<UInputMappingContext>();

		UInputAction* MoveAction = NewObject<UInputAction>();
		MoveAction->ValueType = EInputActionValueType::Axis3D;
		InputMappingContext->MapKey(MoveAction, EKeys::W);

		UInputModifierSwizzleAxis* YXZSwizzle = NewObject<UInputModifierSwizzleAxis>();
		YXZSwizzle->Order = EInputAxisSwizzle::YXZ;
		UInputModifierSwizzleAxis* ZYXSwizzle = NewObject<UInputModifierSwizzleAxis>();
		ZYXSwizzle->Order = EInputAxisSwizzle::ZYX;

		UInputModifierNegate* NegateMoveModifier = NewObject<UInputModifierNegate>();
		NegateMoveModifier->bX = true;
		NegateMoveModifier->bY = true;
		NegateMoveModifier->bZ = true;

		FEnhancedActionKeyMapping& MoveBackwardKeyMapping = InputMappingContext->MapKey(MoveAction, EKeys::S);
		MoveBackwardKeyMapping.Modifiers.Add(NegateMoveModifier);

		FEnhancedActionKeyMapping& MoveRightKeyMapping = InputMappingContext->MapKey(MoveAction, EKeys::D);
		MoveRightKeyMapping.Modifiers.Add(YXZSwizzle);

		FEnhancedActionKeyMapping& MoveLeftKeyMapping = InputMappingContext->MapKey(MoveAction, EKeys::A);
		MoveLeftKeyMapping.Modifiers.Add(YXZSwizzle);
		MoveLeftKeyMapping.Modifiers.Add(NegateMoveModifier);

		FEnhancedActionKeyMapping& MoveUpKeyMapping = InputMappingContext->MapKey(MoveAction, EKeys::E);
		MoveUpKeyMapping.Modifiers.Add(ZYXSwizzle);

		FEnhancedActionKeyMapping& MoveDownKeyMapping = InputMappingContext->MapKey(MoveAction, EKeys::Q);
		MoveDownKeyMapping.Modifiers.Add(ZYXSwizzle);
		MoveDownKeyMapping.Modifiers.Add(NegateMoveModifier);

		UInputAction* LookAction = NewObject<UInputAction>();
		LookAction->ValueType = EInputActionValueType::Axis2D;

		UInputModifierNegate* NegateLookModifier = NewObject<UInputModifierNegate>();
		NegateLookModifier->bX = false;
		NegateLookModifier->bY = true;
		NegateLookModifier->bZ = false;

		FEnhancedActionKeyMapping& LookKeyMapping = InputMappingContext->MapKey(LookAction, EKeys::Mouse2D);
		LookKeyMapping.Modifiers.Add(NegateLookModifier);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APerspectiveViewPawn::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APerspectiveViewPawn::Look);

		if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}

void APerspectiveViewPawn::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void APerspectiveViewPawn::Move(const FInputActionValue& Value)
{
	FVector MovementInput = Value.Get<FVector>();
	FRotator Rotation = Controller->GetControlRotation();
	FRotator YawRotation(0, Rotation.Yaw, 0);

	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	FVector UpDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Z);

	AddMovementInput(ForwardDirection, MovementInput.X);
	AddMovementInput(RightDirection, MovementInput.Y);
	AddMovementInput(UpDirection, MovementInput.Z);
}
