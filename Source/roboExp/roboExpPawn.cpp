// Copyright Epic Games, Inc. All Rights Reserved.

#include "roboExpPawn.h"
#include "roboExpWheelFront.h"
#include "roboExpWheelRear.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "roboExp.h"

#define LOCTEXT_NAMESPACE "VehiclePawn"

AroboExpPawn::AroboExpPawn()
{
	// construct the front camera boom
	FrontSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Front Spring Arm"));
	FrontSpringArm->SetupAttachment(GetMesh());
	FrontSpringArm->TargetArmLength = 0.0f;
	FrontSpringArm->bDoCollisionTest = false;
	FrontSpringArm->bEnableCameraRotationLag = true;
	FrontSpringArm->CameraRotationLagSpeed = 15.0f;
	FrontSpringArm->SetRelativeLocation(FVector(30.0f, 0.0f, 120.0f));

	FrontCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Front Camera"));
	FrontCamera->SetupAttachment(FrontSpringArm);
	FrontCamera->bAutoActivate = false;

	// construct the back camera boom
	BackSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Back Spring Arm"));
	BackSpringArm->SetupAttachment(GetMesh());
	BackSpringArm->TargetArmLength = 650.0f;
	BackSpringArm->SocketOffset.Z = 150.0f;
	BackSpringArm->bDoCollisionTest = false;
	BackSpringArm->bInheritPitch = false;
	BackSpringArm->bInheritRoll = false;
	BackSpringArm->bEnableCameraRotationLag = true;
	BackSpringArm->CameraRotationLagSpeed = 2.0f;
	BackSpringArm->CameraLagMaxDistance = 50.0f;

	BackCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Back Camera"));
	BackCamera->SetupAttachment(BackSpringArm);

	// Configure the car mesh
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(FName("Vehicle"));

	// get the Chaos Wheeled movement component
	ChaosVehicleMovement = CastChecked<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement());

}

void AroboExpPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// steering 
		EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Triggered, this, &AroboExpPawn::Steering);
		EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Completed, this, &AroboExpPawn::Steering);

		// throttle 
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Triggered, this, &AroboExpPawn::Throttle);
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Completed, this, &AroboExpPawn::Throttle);

		// break 
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Triggered, this, &AroboExpPawn::Brake);
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Started, this, &AroboExpPawn::StartBrake);
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Completed, this, &AroboExpPawn::StopBrake);

		// handbrake 
		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Started, this, &AroboExpPawn::StartHandbrake);
		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Completed, this, &AroboExpPawn::StopHandbrake);

		// look around 
		EnhancedInputComponent->BindAction(LookAroundAction, ETriggerEvent::Triggered, this, &AroboExpPawn::LookAround);

		// toggle camera 
		EnhancedInputComponent->BindAction(ToggleCameraAction, ETriggerEvent::Triggered, this, &AroboExpPawn::ToggleCamera);

		// reset the vehicle 
		EnhancedInputComponent->BindAction(ResetVehicleAction, ETriggerEvent::Triggered, this, &AroboExpPawn::ResetVehicle);
	}
	else
	{
		UE_LOG(LogroboExp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AroboExpPawn::Tick(float Delta)
{
	Super::Tick(Delta);

	// add some angular damping if the vehicle is in midair
	bool bMovingOnGround = ChaosVehicleMovement->IsMovingOnGround();
	GetMesh()->SetAngularDamping(bMovingOnGround ? 0.0f : 3.0f);

	// realign the camera yaw to face front
	float CameraYaw = BackSpringArm->GetRelativeRotation().Yaw;
	CameraYaw = FMath::FInterpTo(CameraYaw, 0.0f, Delta, 1.0f);

	BackSpringArm->SetRelativeRotation(FRotator(0.0f, CameraYaw, 0.0f));
}

void AroboExpPawn::Steering(const FInputActionValue& Value)
{
	// route the input
	DoSteering(Value.Get<float>());
}

void AroboExpPawn::Throttle(const FInputActionValue& Value)
{
	// route the input
	DoThrottle(Value.Get<float>());
}

void AroboExpPawn::Brake(const FInputActionValue& Value)
{
	// route the input
	DoBrake(Value.Get<float>());
}

void AroboExpPawn::StartBrake(const FInputActionValue& Value)
{
	// route the input
	DoBrakeStart();
}

void AroboExpPawn::StopBrake(const FInputActionValue& Value)
{
	// route the input
	DoBrakeStop();
}

void AroboExpPawn::StartHandbrake(const FInputActionValue& Value)
{
	// route the input
	DoHandbrakeStart();
}

void AroboExpPawn::StopHandbrake(const FInputActionValue& Value)
{
	// route the input
	DoHandbrakeStop();
}

void AroboExpPawn::LookAround(const FInputActionValue& Value)
{
	// route the input
	DoLookAround(Value.Get<float>());
}

void AroboExpPawn::ToggleCamera(const FInputActionValue& Value)
{
	// route the input
	DoToggleCamera();
}

void AroboExpPawn::ResetVehicle(const FInputActionValue& Value)
{
	// route the input
	DoResetVehicle();
}

void AroboExpPawn::DoSteering(float SteeringValue)
{
	// add the input
	ChaosVehicleMovement->SetSteeringInput(SteeringValue);
}

void AroboExpPawn::DoThrottle(float ThrottleValue)
{
	// add the input
	ChaosVehicleMovement->SetThrottleInput(ThrottleValue);

	// reset the brake input
	ChaosVehicleMovement->SetBrakeInput(0.0f);
}

void AroboExpPawn::DoBrake(float BrakeValue)
{
	// add the input
	ChaosVehicleMovement->SetBrakeInput(BrakeValue);

	// reset the throttle input
	ChaosVehicleMovement->SetThrottleInput(0.0f);
}

void AroboExpPawn::DoBrakeStart()
{
	// call the Blueprint hook for the brake lights
	BrakeLights(true);
}

void AroboExpPawn::DoBrakeStop()
{
	// call the Blueprint hook for the brake lights
	BrakeLights(false);

	// reset brake input to zero
	ChaosVehicleMovement->SetBrakeInput(0.0f);
}

void AroboExpPawn::DoHandbrakeStart()
{
	// add the input
	ChaosVehicleMovement->SetHandbrakeInput(true);

	// call the Blueprint hook for the break lights
	BrakeLights(true);
}

void AroboExpPawn::DoHandbrakeStop()
{
	// add the input
	ChaosVehicleMovement->SetHandbrakeInput(false);

	// call the Blueprint hook for the break lights
	BrakeLights(false);
}

void AroboExpPawn::DoLookAround(float YawDelta)
{
	// rotate the spring arm
	BackSpringArm->AddLocalRotation(FRotator(0.0f, YawDelta, 0.0f));
}

void AroboExpPawn::DoToggleCamera()
{
	// toggle the active camera flag
	bFrontCameraActive = !bFrontCameraActive;

	FrontCamera->SetActive(bFrontCameraActive);
	BackCamera->SetActive(!bFrontCameraActive);
}

void AroboExpPawn::DoResetVehicle()
{
	// reset to a location slightly above our current one
	FVector ResetLocation = GetActorLocation() + FVector(0.0f, 0.0f, 50.0f);

	// reset to our yaw. Ignore pitch and roll
	FRotator ResetRotation = GetActorRotation();
	ResetRotation.Pitch = 0.0f;
	ResetRotation.Roll = 0.0f;

	// teleport the actor to the reset spot and reset physics
	SetActorTransform(FTransform(ResetRotation, ResetLocation, FVector::OneVector), false, nullptr, ETeleportType::TeleportPhysics);

	GetMesh()->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	GetMesh()->SetPhysicsLinearVelocity(FVector::ZeroVector);
}

#undef LOCTEXT_NAMESPACE