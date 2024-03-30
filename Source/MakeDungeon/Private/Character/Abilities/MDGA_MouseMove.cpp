// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/MDGA_MouseMove.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "../MakeDungeon.h"
//#include "NiagaraSystem.h"
//#include "NiagaraFunctionLibrary.h"

UMDGA_MouseMove::UMDGA_MouseMove()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UMDGA_MouseMove::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	MD_LOG(LogMD, Log, TEXT("ActivateAbility"));

	/*FollowTime += GetWorld()->GetDeltaSeconds();

	FHitResult HitResult;
	if (ActorInfo->PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, HitResult))
	{
		CachedDestination = HitResult.Location;
	}

	APawn* Pawn = Cast<APawn>(ActorInfo->AvatarActor);
	FVector WorldDirection = (CachedDestination - Pawn->GetActorLocation()).GetSafeNormal();

	Pawn->AddMovementInput(WorldDirection);*/

}

void UMDGA_MouseMove::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (FollowTime <= ShortPressThreshold)
	{
		APawn* Pawn = Cast<APawn>(ActorInfo->AvatarActor);

		UAIBlueprintHelperLibrary::SimpleMoveToLocation(Pawn->GetController(), CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(Pawn->GetController(), FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;

	MD_LOG(LogMD, Log, TEXT("InputReleased"));

	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UMDGA_MouseMove::OnReachedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
