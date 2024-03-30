// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "MDAT_MoveToLocation.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMoveToLocationDelegate);

/**
 * 
 */
UCLASS()
class MAKEDUNGEON_API UMDAT_MoveToLocation : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UMDAT_MoveToLocation();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UNiagaraSystem* FXCursor;

	UPROPERTY(BlueprintAssignable)
	FMoveToLocationDelegate OnFinish;

	UPROPERTY(BlueprintAssignable)
	FMoveToLocationDelegate OnCanceled;


	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "JumpAndWaitForLanding", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UMDAT_MoveToLocation* CreateTask(UGameplayAbility* OwningAbility);

	virtual void Activate() override;

	virtual void TickTask(float DeltaTime) override;

	virtual void OnDestroy(bool AbilityEnded) override;

protected:
	UFUNCTION()
	void Finish();

	UFUNCTION()
	void OnCanceledCallback();

private:
	float FollowTime;

	FVector CachedDestination;
};
