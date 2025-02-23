#include "Parser.h"

#include "Stos.h"
#include <iostream>

bool CYK(const Napis& expression)
{
	int n = expression.getLen();
	if (n <= 0) return false;

	bool*** P = new bool** [n];
	for (int i = 0; i < n; i++) {
		P[i] = new bool* [n];
		for (int j = 0; j < n; j++) {
			P[i][j] = new bool[25];
			for (int k = 0; k < 25; k++)
				P[i][j][k] = false;
		}
	}

	for (int s = 0; s < n; s++) {
		char chr = expression[s];
		if (std::isdigit(chr)) {
			P[0][s][S] = true;
			P[0][s][B] = true;
			P[0][s][C] = true;
			P[0][s][D] = true;
			P[0][s][G] = true;
		}
		else if (std::isalpha(chr)) {
			P[0][s][S] = true;
			P[0][s][B] = true;
			P[0][s][C] = true;
			P[0][s][E] = true;
			P[0][s][G] = true;
		}
		else if (chr == '.') {
			P[0][s][J] = true;
		}
		else if (chr == '+' || chr == '*' || chr == '/' || chr == '^') {
			P[0][s][A] = true;
		}
		else if (chr == '-') {
			P[0][s][A] = true;
			P[0][s][I] = true;
		}
		else if (chr == '(') {
			P[0][s][N] = true;
		}
		else if (chr == ')') {
			P[0][s][O] = true;
		}
		else if (chr == '{') {
			P[0][s][K] = true;
		}
		else if (chr == '}') {
			P[0][s][L] = true;
		}
		else if (chr == ',') {
			P[0][s][M] = true;
		}
		else {
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < n; j++)
					delete P[i][j];
				delete[] P[i];
			}
			delete[] P;
			return false;
		}
	}

	for (int l = 1; l < n; l++) {
		for (int s = 0; s < n - l; s++) {
			for (int p = 0; p < l; p++) {
				for (int i = 0; i < 36; i++) {
					if (P[p][s][nonterminalProductionRules[i][1]] &&
						P[l - p - 1][s + p + 1][nonterminalProductionRules[i][2]])
						P[l][s][nonterminalProductionRules[i][0]] = true;
				}
			}
		}
	}

	// Wyœwietl tabelkê CYK: wynik wstaw do Excel->Dane->Tekst_jako_kolumny rozdzielaj¹c znakiem |

	//for (int i = n - 1; i >= 0; i--) {
	//	for (int j = 0; j < n; j++) {
	//		bool first = true;
	//		for (int k = 0; k < 25; k++) {
	//			if (P[i][j][k]) {
	//				if (!first) std::cout << ',';
	//				first = false;
	//				char c = k > 0 ? k + 64 : 'S';
	//				std::cout << c;
	//			}
	//		}
	//		if (j < n - 1) std::cout << " | ";
	//	}
	//	std::cout << std::endl;
	//}

	bool result = P[n - 1][0][0];

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			delete P[i][j];
		delete[] P[i];
	}
	delete[] P;

	return result;
}

int NWD(int a, int b)
{
	int c;
	while (a > 0) {
		c = a;
		a = b % a;
		b = c;
	}
	return b;
}

Napis floatToFraction(const Napis& Float)
{
	int licznik = 0, mianownik = 1, okres = 1;
	bool czyCzescUlamkowa = false, czyOkres = false;

	int i = 0, n = Float.getLen();

	while (i < n) {
		char chr = Float[i];
		if (chr == '.')
			czyCzescUlamkowa = true;
		else if (chr == '(')
			czyOkres = true;
		else if (chr == ')')
			czyOkres = false;
		else {
			licznik = 10 * licznik + chr - 48;
			if (czyCzescUlamkowa) mianownik *= 10;
			if (czyOkres) okres *= 10;
		}
		i++;
	}
	if (okres > 1) {
		licznik -= licznik / okres;
		mianownik -= mianownik / okres;
	}
	int nwd = NWD(licznik, mianownik);
	licznik /= nwd;
	mianownik /= nwd;
	return '(' + Napis(licznik) + '/' + Napis(mianownik) + ')';
}

Napis floatToFraction(int digits, int offset, int periodOffset)
{
	//           x = 1.2(34)
	//         10x = 12.(34)
	//       1000x = 1234.(34)
	// 
	// 1000x - 10x = 1234.(34) - 12.(34)
	//        990x = 1222
	//           x = 1222/990

	if (periodOffset > 1) {
		digits -= digits / periodOffset;
		offset -= offset / periodOffset;
	}
	int nwd = NWD(digits, offset);
	digits /= nwd;
	offset /= nwd;
	return '(' + Napis(digits) + '/' + Napis(offset) + ')';
}

void onlyIntegers(Napis& expression)
{
	int i = 0, startIndex = -1, cancelIndex = -1, digits = 0, offset = 0, periodOffset = 0;

	while (i < expression.getLen()) {
		char chr = expression[i];
		if (std::isdigit(chr)) {
			if (startIndex == -1) startIndex = i;
			digits = 10 * digits + chr - 48;
			offset *= 10;
			periodOffset *= 10;
		}
		else if (chr == '.' && offset == 0) offset = 1;
		else if (chr == '(' && offset > 0) {
			cancelIndex = i;
			periodOffset = 1;
		}
		else if (offset > 1) {
			if (periodOffset > 0) {
				if (chr == ')') i++;
				else {
					i = cancelIndex;
					digits /= periodOffset;
					offset /= periodOffset;
					periodOffset = 0;
				}
			}
			else if (chr == '.') {
				i--;
				digits /= 10;
				offset /= 10;
			}
			expression.remove(startIndex, i - startIndex);

			Napis ulamek = floatToFraction(digits, offset, periodOffset);
			expression.insert(ulamek, startIndex);

			i = startIndex + ulamek.getLen() - 1;
			startIndex = -1;
			digits = offset = periodOffset = 0;
		}
		else {
			startIndex = -1;
			digits = 0;
		}
		i++;
	}
	if (offset > 1) {
		expression.remove(startIndex, i - startIndex);
		expression.insert(floatToFraction(digits, offset, periodOffset), startIndex);
	}
}

int precedence(char opr)
{
	switch (opr) {
	case '+':
	case '-':
		return 1;
	case '*':
	case '/':
		return 2;
	case '^':
		return 3;
	default:
		return 4;
	}
}

Napis infixToPrefix(Napis infix)
{
	onlyIntegers(infix);
	
	int i = 0;
	if (infix[0] == '-') infix[0] = '~';
	
	while (i < infix.getLen() - 1) {
		char a = infix[i], b = infix[i + 1];
		if ((std::isalnum(a) || a == ')' || a == '}') &&
			(std::isalnum(b) || b == '(' || b == '{') &&
			!(std::isdigit(a) && std::isdigit(b))) {
			infix.insert('*', i + 1);
			i++;
		}
		if ((a == '+' || a == '-' || a == '*' || a == '/' || a == '^' ||
			a == '(' || a == '{' || a == '~') && b == '-') infix[i + 1] = '~';
		i++;
	}
	Stos stos;
	Napis prefix;
	bool lastChrIsSameOperand = true;
	int matrixParensBalance = 0;

	for (int i = infix.getLen() - 1; i >= 0; i--) {
		char chr = infix[i];
		if (chr == '{') {
			matrixParensBalance--;
			lastChrIsSameOperand = (matrixParensBalance != 0);
			prefix += chr;
		}
		else if (std::isalnum(chr) || chr == '}' || chr == ',' || matrixParensBalance != 0) {
			if (!lastChrIsSameOperand) prefix += ' ';
			lastChrIsSameOperand = true;
			prefix += chr;
			if (chr == '}')	matrixParensBalance++;
		}
		else if (chr == '~') {
			prefix += ' ';
			lastChrIsSameOperand = true;
			prefix += chr;
		}
		else if (chr == ')') {
			lastChrIsSameOperand = (prefix == "");
			stos.push(chr);
		}
		else if (chr == '(') {
			lastChrIsSameOperand = false;
			while (!stos.isEmpty() && stos.top() != ')') {
				prefix += ' ';
				prefix += stos.pop();
			}
			if (stos.isEmpty()) throw std::invalid_argument("Error: Parser: Unbalanced parentheses!");
			stos.pop();
		}
		else {
			lastChrIsSameOperand = false;
			while (!stos.isEmpty() && stos.top() != ')' && precedence(stos.top()) > precedence(chr)) {
				prefix += ' ';
				prefix += stos.pop();
			}
			stos.push(chr);
		}
	}

	while (!stos.isEmpty() && stos.top() != ')') {
		prefix += ' ';
		prefix += stos.pop();
	}

	if (!stos.isEmpty()) throw std::invalid_argument("Error: Parser: Unbalanced parentheses!");

	return prefix.reversed();
}