// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageComponents/DamageTypes.h"

void FDamageEffects::operator+=(const FDamageEffects& Other)
{
	Stop += Other.Stop;
	BaseDamage += Other.BaseDamage;
}

void FDamageEffects::operator=(const FDamageEffects& Other)
{
	DamageType = Other.DamageType;
	DamageTypeStrength = Other.DamageTypeStrength;
	Stop = Other.Stop;
	BaseDamage = Other.BaseDamage;
	StatusEffectType = Other.StatusEffectType;
	StatusEffectStrength = Other.StatusEffectStrength;
}

FDamageEffects::FDamageEffects(
	float Damage,
	float Stop,
	EDamageType DmgType,
	float DmgTypeStrength,
	EStatusEffectType StatusEffect,
	float StatusStrength)
	: BaseDamage(Damage),
	  Stop(Stop),
	  DamageType(DmgType),
	  DamageTypeStrength(DmgTypeStrength),
	  StatusEffectType(StatusEffect),
	  StatusEffectStrength(StatusStrength)
{
}
