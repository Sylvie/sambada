#ifndef SAMBADA_SAMBADAINPUTDATA_H
#define SAMBADA_SAMBADAINPUTDATA_H

#include <vector>
#include <string>

class SambadaInputData
{
public:

    std::vector <std::vector<std::string>> data;

    void verifieTailles(int nombreLignes, int nombreColonnes) const;
    void compare(const SambadaInputData &autre) const;
};

#endif // SAMBADA_SAMBADAINPUTDATA_H
