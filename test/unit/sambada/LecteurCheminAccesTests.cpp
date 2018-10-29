#include "catch.hpp"
#include "LecteurCheminAcces.h"

#include <string>

SCENARIO("Test that complete paths are parsed correctly")
{
    LecteurCheminAcces lecteur;

    GIVEN("An empty path")
    {
        std::string path("");
        WHEN("The path is read")
        {
            CheminAcces cheminAcces = lecteur.decompose(path);
            THEN("The CheminAccess should be empty")
            {
                CHECK(cheminAcces == CheminAcces());
            }
        }
    }



}