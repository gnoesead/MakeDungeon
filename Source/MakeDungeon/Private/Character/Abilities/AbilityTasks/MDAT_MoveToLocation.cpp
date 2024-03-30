// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/AbilityTasks/MDAT_MoveToLocation.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

UMDAT_MoveToLocation::UMDAT_MoveToLocation()
{
	bTickingTask = true;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

UMDAT_MoveToLocation* UMDAT_MoveToLocation::CreateTask(UGameplayAbility* OwningAbility)
{
	UMDAT_MoveToLocation* NewTask = NewAbilityTask<UMDAT_MoveToLocation>(OwningAbility);

	return NewTask;
}

void UMDAT_MoveToLocation::Activate()
{
	FollowTime += GetWorld()->GetDeltaSeconds();

	
}

void UMDAT_MoveToLocation::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	FGameplayAbilityActorInfo ActorInfo = Ability->GetActorInfo();

	FHitResult HitResult;
	if (ActorInfo.PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, HitResult))
	{
		CachedDestination = HitResult.Location;
	}

	APawn* Pawn = Cast<APawn>(GetAvatarActor());
	FVector WorldDirection = (CachedDestination - Pawn->GetActorLocation()).GetSafeNormal();

	Pawn->AddMovementInput(WorldDirection);

	
}

void UMDAT_MoveToLocation::OnDestroy(bool AbilityEnded)
{


	Super::OnDestroy(AbilityEnded);
}

void UMDAT_MoveToLocation::Finish()
{
	if (FollowTime <= ShortPressThreshold)
	{
		APawn* Pawn = Cast<APawn>(GetAvatarActor());

		UAIBlueprintHelperLibrary::SimpleMoveToLocation(Pawn->GetController(), CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(Pawn->GetController(), FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}
}

void UMDAT_MoveToLocation::OnCanceledCallback()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnCanceled.Broadcast();
	}
}
