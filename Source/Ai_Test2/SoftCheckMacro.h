#pragma once
#define SOFT_CHECK(expression, text) if (!(expression)) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT(text));