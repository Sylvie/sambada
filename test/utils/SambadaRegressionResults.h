#ifndef SAMBADA_SAMBADAREGRESSIONRESULTS_H
#define SAMBADA_SAMBADAREGRESSIONRESULTS_H

#include <vector>
#include <string>

class SambadaRegressionResults
{
public:

    std::vector <std::string> header;
    std::vector <std::vector<std::string>> etiquettes;
    std::vector <std::vector<long double>> valeurs;

    void verifieTailles(bool hasHeader, int dimension, int nombreModeles) const;
    void compare(const SambadaRegressionResults &autre) const;

private:
    void verifieTailleHeader(int dimension) const;
    std::string colleHeaders() const;
    std::string colleChaines(const std::vector< std::string >& morceaux) const;

    const static size_t indiceEfron = 4;
    const static size_t indiceAIC = 9;
};


#endif // SAMBADA_SAMBADAREGRESSIONRESULTS_H
