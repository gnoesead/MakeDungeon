// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/MDGA_MouseMove.h"

UMDGA_MouseMove::UMDGA_MouseMove()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UMDGA_MouseMove::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	FHitResult HitResult;
	if (GetHitResultUnderCursor(HitResult))
	{

	}
}

bool UMDGA_MouseMove::GetHitResultUnderCursor(FHitResult& HitResult)
{
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(GetActorInfo().AvatarActor);
	bool bHit = false;
	if (LocalPlayer && LocalPlayer->ViewportClient)
	{
		FVector2D MousePostition;
		if (LocalPlayer->ViewportClient->GetMousePosition(MousePostition))
		{
			bHit = GetActorInfo().PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, HitResult);
		}
	}

	return bHit;
}
