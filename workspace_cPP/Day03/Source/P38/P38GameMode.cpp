// Fill out your copyright notice in the Description page of Project Settings.

#include "P38GameMode.h"
#include "P38Pawn.h"

AP38GameMode::AP38GameMode()
{
	DefaultPawnClass = AP38Pawn::StaticClass();
}