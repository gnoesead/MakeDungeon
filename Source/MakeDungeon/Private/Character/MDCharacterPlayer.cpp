// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MDCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Data/MDCharacterControlData.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Player/MDPlayerState.h"
#include "AbilitySystemComponent.h"
#include "../MakeDungeon.h"

AMDCharacterPlayer::AMDCharacterPlayer()
{
	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMouseMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/MakeDungeon/Input/Actions/IA_SetDestination_Click.IA_SetDestination_Click'"));
	if (nullptr != InputActionMouseMoveRef.Object)
	{
		MouseMoveAction = InputActionMouseMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionKeyboardMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/MakeDungeon/Input/Actions/IA_KeyboardMove.IA_KeyboardMove'"));
	if (nullptr != InputActionKeyboardMoveRef.Object)
	{
		KeyboardMoveAction = InputActionKeyboardMoveRef.Object;
	}
}

void AMDCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AMDPlayerState* PS = GetPlayerState<AMDPlayerState>();
	if (PS)
	{
		AbilitySystemComponent = PS->GetAbilitySystemComponent();
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);

		for (const auto& StartInputAbility : InputAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartInputAbility.Value);
			StartSpec.InputID = StartInputAbility.Key;
			AbilitySystemComponent->GiveAbility(StartSpec);
		}
	}

	SetCharacterControl();

	/*APlayerController* PlayerController = CastChecked<APlayerController>(NewController);
	PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));*/
}

void AMDCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	//EnhancedInputComponent->BindAction(MouseMoveAction, ETriggerEvent::Triggered, this, &AMDCharacterPlayer::MouseMove);
	EnhancedInputComponent->BindAction(KeyboardMoveAction, ETriggerEvent::Triggered, this, &AMDCharacterPlayer::KeyboardMove);
}

void AMDCharacterPlayer::SetCharacterControl()
{
	SetCharacterControlData(CharacterControl);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		SubSystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = CharacterControl->InputMappingContext;
		if (NewMappingContext)
		{
			SubSystem->AddMappingContext(NewMappingContext, 0);
		}
	}
}

void AMDCharacterPlayer::SetCharacterControlData(const UMDCharacterControlData* CharacterControlData)
{
	// Pawn
	bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

	// CharacterMovement
	GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;

	// Camera
	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
}

void AMDCharacterPlayer::SetupGASInputComponent()
{
	if (IsValid(AbilitySystemComponent) && IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

		EnhancedInputComponent->BindAction(MouseMoveAction, ETriggerEvent::Triggered, this, &AMDCharacterPlayer::GASInputPressed, 0);
		EnhancedInputComponent->BindAction(KeyboardMoveAction, ETriggerEvent::Triggered, this, &AMDCharacterPlayer::KeyboardMove);
	}
}

void AMDCharacterPlayer::GASInputPressed(int32 InputId)
{
	FGameplayAbilitySpec* Spec = AbilitySystemComponent->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		Spec->InputPressed = true;
		if (Spec->IsActive())
		{
			AbilitySystemComponent->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			AbilitySystemComponent->TryActivateAbility(Spec->Handle);
		}
	}
}

void AMDCharacterPlayer::KeyboardMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	float InputSizeSquared = MovementVector.SquaredLength();
	float MovementVectorSize = 1.f;
	float MovementVectorSizeSquared = MovementVector.SquaredLength();
	if (MovementVectorSizeSquared > 1.f)
	{
		MovementVector.Normalize();
		MovementVectorSizeSquared = 1.f;
	}
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}

	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.f);
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	AddMovementInput(MoveDirection, MovementVectorSize);
}
