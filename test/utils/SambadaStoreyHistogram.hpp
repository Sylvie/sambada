#ifndef SAMBADA_SAMBADASTOREYHISTOGRAM_H
#define SAMBADA_SAMBADASTOREYHISTOGRAM_H

#include <vector>
#include <string>

class SambadaStoreyHistogram
{
public:

	std::vector<std::vector<long double>> header;
	std::vector<std::string> etiquettes;
	std::vector<std::vector<int>> valeurs;

	void verifieTailles(int nombreLignesValeurs) const;

	void compare(const SambadaStoreyHistogram& autre) const;

	bool infWasFound = false;

	const static size_t nombreLignesHeader = 2;
	const static size_t nombreIntervalles = 96;
	constexpr static size_t nombreIntervallesHeaders[nombreLignesHeader] = {96, 95};
};


#endif // SAMBADA_SAMBADASTOREYHISTOGRAM_H
