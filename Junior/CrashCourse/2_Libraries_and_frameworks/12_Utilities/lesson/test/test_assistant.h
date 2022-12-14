#pragma once

#include <optional>
#include <utility>
#include <any>
#include <variant>

// use in test for "optional"
struct JMatrix
{
	JMatrix(int iter) : iteration{ iter } {}
	const int iteration;
};

enum JPill { Red, Blue };

std::optional< JMatrix > take(JPill pill)
{
	if (pill == JPill::Blue) { return JMatrix{ 6 }; }
	return std::nullopt;
}

// use in test for "pair, tuple"
struct Socialite { const char* birthname; };
struct Valet { const char* surname; };
struct Acquaintance { const char* nickname; };

Socialite bertie{ "Wilberforce" };
Valet reginald{ "Jeeves" };
Acquaintance hildebrand{ "Typpy" };

// use in test for "any, variant"
struct EsacapeCapsule
{
	EsacapeCapsule(int kg) : weightKg{ kg } {}
	int weightKg;
};

struct BugblatterBeast
{
	BugblatterBeast() : isRavenous{ true }, weightKg{ 20000 }{}
	bool isRavenous;
	int weightKg;
};