// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MDGA_MouseMove.generated.h"

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UMDGA_MouseMove : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UMDGA_MouseMove();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	bool GetHitResultUnderCursor(FHitResult& HitResult);
};
