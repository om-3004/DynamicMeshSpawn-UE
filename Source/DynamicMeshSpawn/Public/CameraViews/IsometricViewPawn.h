// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "IsometricViewPawn.generated.h"

UCLASS()
class DYNAMICMESHSPAWN_API AIsometricViewPawn : public APawn
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComponent;
 
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UFloatingPawnMovement* FloatingPawnMovement;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float TargetSpringArmYaw;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float RotationSpeed;

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void CameraLeftRotation();

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void CameraRightRotation();

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void CameraZoom(const FInputActionValue& ActionValue); 

public:
	// Sets default values for this pawn's properties
	AIsometricViewPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
