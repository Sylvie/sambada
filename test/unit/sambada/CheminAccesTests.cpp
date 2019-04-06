#include "catch.hpp"
#include "LecteurCheminAcces.hpp"

#include <iostream>

SCENARIO("Test that CheminAcces are compared correctly")
{
    GIVEN("An empty CheminAccess")
    {
        CheminAcces empty;
        empty.chemin="";
        empty.radical="";
        empty.extension="";

        WHEN("The empty CheminAccess is compared to another empty CheminAccess")
        {
            CheminAcces autre;
            THEN("They should be equal")
            {
                CHECK(empty == autre);
            }
        }

        WHEN("The empty CheminAccess is compared a CheminAccess with a non-empty path")
        {
            CheminAcces autre;
            autre.chemin = " ";
            THEN("They should not be equal")
            {
                CHECK_FALSE(empty == autre);
            }
        }

        WHEN("The empty CheminAccess is compared a CheminAccess with a non-empty radical")
        {
            CheminAcces autre;
            autre.radical = " ";
            THEN("They should not be equal")
            {
                CHECK_FALSE(empty == autre);
            }
        }

        WHEN("The empty CheminAccess is compared a CheminAccess with a non-empty extension")
        {
            CheminAcces autre;
            autre.extension = " ";
            THEN("They should not be equal")
            {
                CHECK_FALSE(empty == autre);
            }
        }
    }

    GIVEN("A CheminAccess")
    {
        CheminAcces chemin;
        chemin.chemin="some/path/";
        chemin.radical="myfile";
        chemin.extension=".myext";

        WHEN("The CheminAccess is compared to another CheminAccess")
        {
            CheminAcces autre;
            chemin.chemin="some/other/path/";
            chemin.radical="myfile";
            chemin.extension=".myext";
            THEN("They should not be equal")
            {
                CHECK_FALSE(chemin == autre);
            }
        }

        WHEN("The empty CheminAccess is compared to itself")
        {
            THEN("It should be equal to itself")
            {
                CHECK(chemin == chemin);
            }
        }

        WHEN("The empty CheminAccess is compared a CheminAccess with the same definition")
        {
            CheminAcces autre;
            autre.chemin="some/path/";
            autre.radical="myfile";
            autre.extension=".myext";
            THEN("They should be equal")
            {
                CHECK(chemin == autre);
            }
        }

        WHEN("The empty CheminAccess is compared to its copy")
        {
            CheminAcces autre(chemin);
            THEN("They should be equal")
            {
                CHECK(chemin == autre);
            }
        }
    }
}