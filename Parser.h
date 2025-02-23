#pragma once
#include "Napis.h"

enum nonterminal { S, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, T, U, V, W, X, Y };

const nonterminal nonterminalProductionRules[36][3] = {
	{ S, I, S },
	{ S, Y, O },
	{ S, S, H },
	{ S, H, S },
	{ S, P, S },
	{ S, Q, S },
	{ S, C, B },
	{ S, R, D },
	{ S, T, L },
	{ B, C, B },
	{ B, R, D },
	{ B, T, L },
	{ C, R, D },
	{ C, T, L },
	{ F, W, F },
	{ F, U, L },
	{ G, X, G },
	{ G, I, S },
	{ G, Y, O },
	{ G, S, H },
	{ G, H, S },
	{ G, P, S },
	{ G, Q, S },
	{ G, C, B },
	{ G, R, D },
	{ G, T, L },
	{ H, Y, O },
	{ P, S, H },
	{ Q, S, A },
	{ R, D, J },
	{ T, K, F },
	{ U, K, G },
	{ V, U, L },
	{ W, V, M },
	{ X, S, M },
	{ Y, N, S }
};

bool CYK(const Napis& expression);

int NWD(int a, int b);

Napis floatToFraction(const Napis& Float); // not used
Napis floatToFraction(int digits, int offset, int periodOffset);

void onlyIntegers(Napis& expression);

int precedence(char opr);

Napis infixToPrefix(Napis infix);

// Regu³y produkcji wyra¿enia algebraicznego
// S -> -S | H | SH | HS | SHS | SAS | B
// A -> + | - | * | / | ^
// B -> CB | C
// C -> D | D.D | E | {F}
// D -> 0-9
// E -> a-zA-Z
// F -> {G},F | {G}
// G -> S,G | S
// H -> (S)
// 
// W postaci Chomsky'ego
// S -> IS | YO | SH | HS | PS | QS | CB | 0-9 | RD | a-zA-Z | TL
// A -> + | - | * | / | ^
// B -> CB | 0-9 | RD | a-zA-Z | TL
// C -> 0-9 | RD | a-zA-Z | TL
// D -> 0-9
// E -> a-zA-Z
// F -> WF | UL
// G -> XG | IS | YO | SH | HS | PS | QS | CB | 0-9 | RD | a-zA-Z | TL
// H -> YO
// I -> -
// J -> .
// K -> {
// L -> }
// M -> ,
// N -> (
// O -> )
// P -> SH
// Q -> SA
// R -> DJ
// T -> KF
// U -> KG
// V -> UL
// W -> VM
// X -> SM
// Y -> NS