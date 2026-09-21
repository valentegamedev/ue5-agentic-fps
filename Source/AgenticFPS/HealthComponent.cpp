// Copyright Epic Games, Inc. All Rights Reserved.

#include "HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// start at full health
	Health = MaxHealth;
}

float UHealthComponent::TakeDamage(float DamageAmount)
{
	if (bIsDead || DamageAmount <= 0.0f)
	{
		return 0.0f;
	}

	const float ActualDamage = FMath::Min(DamageAmount, Health);

	UpdateHealth(Health - DamageAmount);

	return ActualDamage;
}

float UHealthComponent::Heal(float HealAmount)
{
	if (bIsDead || HealAmount <= 0.0f)
	{
		return 0.0f;
	}

	const float ActualHeal = FMath::Min(HealAmount, MaxHealth - Health);

	UpdateHealth(Health + HealAmount);

	return ActualHeal;
}

void UHealthComponent::SetMaxHealth(float NewMaxHealth, bool bResetHealth)
{
	MaxHealth = FMath::Max(NewMaxHealth, 0.0f);

	UpdateHealth(bResetHealth ? MaxHealth : Health);
}

float UHealthComponent::GetHealthPercent() const
{
	return (MaxHealth > 0.0f) ? (Health / MaxHealth) : 0.0f;
}

void UHealthComponent::UpdateHealth(float NewHealth)
{
	const float ClampedHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);

	if (FMath::IsNearlyEqual(ClampedHealth, Health))
	{
		return;
	}

	Health = ClampedHealth;

	OnHealthChanged.Broadcast(Health, MaxHealth);

	if (Health <= 0.0f && !bIsDead)
	{
		bIsDead = true;
		OnDeath.Broadcast();
	}
}
