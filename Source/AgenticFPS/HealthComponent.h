// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

/** Delegate broadcast whenever Health changes, whether from damage or healing */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedDelegate, float, CurrentHealth, float, MaxHealth);

/** Delegate broadcast once when Health reaches zero */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathDelegate);

/**
 *  Tracks Health and MaxHealth for the owning Actor.
 *  Handles incoming damage through TakeDamage and broadcasts delegates when health changes or reaches zero.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AGENTICFPS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	/** Constructor */
	UHealthComponent();

protected:

	/** Maximum health this component can have */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (ClampMin = 0, ClampMax = 10000))
	float MaxHealth = 100.0f;

	/** Current health remaining. Initialized to MaxHealth on BeginPlay */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float Health = 0.0f;

	/** If true, Health has reached zero and OnDeath has already been broadcast */
	bool bIsDead = false;

protected:

	/** Gameplay initialization */
	virtual void BeginPlay() override;

public:

	/** Broadcast whenever Health changes, whether from damage or healing */
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnHealthChangedDelegate OnHealthChanged;

	/** Broadcast once when Health reaches zero */
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnDeathDelegate OnDeath;

public:

	/** Applies incoming damage to Health. Returns the actual amount of damage applied */
	UFUNCTION(BlueprintCallable, Category = "Health")
	float TakeDamage(float DamageAmount);

	/** Heals the owner by the given amount, up to MaxHealth. Returns the actual amount healed */
	UFUNCTION(BlueprintCallable, Category = "Health")
	float Heal(float HealAmount);

	/** Sets MaxHealth. If bResetHealth is true, Health is reset to the new max */
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetMaxHealth(float NewMaxHealth, bool bResetHealth = true);

	/** Returns the current health */
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealth() const { return Health; }

	/** Returns the max health */
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetMaxHealth() const { return MaxHealth; }

	/** Returns the current health as a percentage [0-1] of max health */
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealthPercent() const;

	/** Returns true if Health has reached zero */
	UFUNCTION(BlueprintPure, Category = "Health")
	bool IsDead() const { return bIsDead; }

protected:

	/** Clamps NewHealth between 0 and MaxHealth, updates Health, and broadcasts OnHealthChanged / OnDeath as needed */
	void UpdateHealth(float NewHealth);
};
