#ifndef SAMBADA_SAMBADASPATIALAUTOCORRELATIONRESULTS_H
#define SAMBADA_SAMBADASPATIALAUTOCORRELATIONRESULTS_H

#include <vector>
#include <string>

class SambadaSpatialAutocorrelationResults
{
public:

	std::vector<std::string> header;
	std::vector<std::string> etiquettes;
	std::vector<std::vector<long double>> valeurs;

	void verifieTailles(int nombrePoints, int nombreVariables) const;

	void compare(const SambadaSpatialAutocorrelationResults &autre) const;

	void comparePValeurs(const SambadaSpatialAutocorrelationResults &autre) const;

private:
	void verifieTailleHeader(int nombreVariables) const;

	std::string colleHeaders() const;

	std::string colleChaines(const std::vector<std::string> &morceaux) const;

	void compare(const SambadaSpatialAutocorrelationResults &autre, double marge) const;
};


#endif // SAMBADA_SAMBADASPATIALAUTOCORRELATIONRESULTS_H
