#include "catch.hpp"
#include "LecteurCheminAcces.hpp"

#include <string>

SCENARIO("Test that LecteurCheminAcces parses complete paths correctly") {
    LecteurCheminAcces lecteur;

    GIVEN("An empty path")
    {
        std::string path("");
        WHEN("The path is read")
        {
            CheminAcces cheminAcces = lecteur.decompose(path);
            THEN("The CheminAccess should be empty")
            {
                CHECK(cheminAcces.chemin == "");
                CHECK(cheminAcces.radical == "");
                CHECK(cheminAcces.extension == "");
            }
        }
    }

    GIVEN("A filename without extension")
    {
        std::string path("myfile");
        WHEN("The path is read")
        {
            CheminAcces cheminAcces = lecteur.decompose(path);
            THEN("The CheminAccess should be correct")
            {
                CHECK(cheminAcces.chemin == "");
                CHECK(cheminAcces.radical == "myfile");
                CHECK(cheminAcces.extension == "");
            }
        }
    }

    GIVEN("A filename with extension")
    {
        std::string path("myfile.xyz");
        WHEN("The path is read")
        {
            CheminAcces cheminAcces = lecteur.decompose(path);
            THEN("The CheminAccess should be correct")
            {
                CHECK(cheminAcces.chemin == "");
                CHECK(cheminAcces.radical == "myfile");
                CHECK(cheminAcces.extension == ".xyz");
            }
        }
    }

    GIVEN("A filename with prefix, but no extension")
    {
        std::string path("../mypath/myfile");
        WHEN("The path is read")
        {
            CheminAcces cheminAcces = lecteur.decompose(path);
            THEN("The CheminAccess should be correct")
            {
                CHECK(cheminAcces.chemin == "../mypath/");
                CHECK(cheminAcces.radical == "myfile");
                CHECK(cheminAcces.extension == "");
            }
        }
    }

    GIVEN("A filename with prefix and extension")
    {
        std::string path("../mypath/myfile.xyz");
        WHEN("The path is read")
        {
            CheminAcces cheminAcces = lecteur.decompose(path);
            THEN("The CheminAccess should be correct")
            {
                CHECK(cheminAcces.chemin == "../mypath/");
                CHECK(cheminAcces.radical == "myfile");
                CHECK(cheminAcces.extension == ".xyz");
            }
        }
    }

    GIVEN("A filename with simple prefix, but no extension")
    {
        std::string path("mypath/myfolder/myfile");
        WHEN("The path is read")
        {
            CheminAcces cheminAcces = lecteur.decompose(path);
            THEN("The CheminAccess should be correct")
            {
                CHECK(cheminAcces.chemin == "mypath/myfolder/");
                CHECK(cheminAcces.radical == "myfile");
                CHECK(cheminAcces.extension == "");
            }
        }
    }

    GIVEN("A filename with simple prefix and extension")
    {
        std::string path("mypath/myfolder/myfile.xyz");
        WHEN("The path is read")
        {
            CheminAcces cheminAcces = lecteur.decompose(path);
            THEN("The CheminAccess should be correct")
            {
                CHECK(cheminAcces.chemin == "mypath/myfolder/");
                CHECK(cheminAcces.radical == "myfile");
                CHECK(cheminAcces.extension == ".xyz");
            }
        }
    }

    GIVEN("A filename with prefix, a dot in the name and an extension")
    {
        std::string path("../mypath/myfile.thisway.xyz");
        WHEN("The path is read")
        {
            CheminAcces cheminAcces = lecteur.decompose(path);
            THEN("The CheminAccess should be correct")
            {
                CHECK(cheminAcces.chemin == "../mypath/");
                CHECK(cheminAcces.radical == "myfile.thisway");
                CHECK(cheminAcces.extension == ".xyz");
            }
        }
    }

    GIVEN("A filename with a inside dot and an extension")
    {
        std::string path("myfile.thisway.xyz");
        WHEN("The path is read")
        {
            CheminAcces cheminAcces = lecteur.decompose(path);
            THEN("The CheminAccess should be correct")
            {
                CHECK(cheminAcces.chemin == "");
                CHECK(cheminAcces.radical == "myfile.thisway");
                CHECK(cheminAcces.extension == ".xyz");
            }
        }
    }

    GIVEN("A filename with very long prefix and extension")
    {
        std::string path("./mypath/myfolder/./here/../up/some/other/folder/myfile.xyz");
        WHEN("The path is read")
        {
            CheminAcces cheminAcces = lecteur.decompose(path);
            THEN("The CheminAccess should be correct")
            {
                CHECK(cheminAcces.chemin == "./mypath/myfolder/./here/../up/some/other/folder/");
                CHECK(cheminAcces.radical == "myfile");
                CHECK(cheminAcces.extension == ".xyz");
            }
        }
    }

    GIVEN("A filename with very long prefix but no extension")
    {
        std::string path("./mypath/myfolder/./here/../up/some/other/folder/myfile");
        WHEN("The path is read")
        {
            CheminAcces cheminAcces = lecteur.decompose(path);
            THEN("The CheminAccess should be correct")
            {
                CHECK(cheminAcces.chemin == "./mypath/myfolder/./here/../up/some/other/folder/");
                CHECK(cheminAcces.radical == "myfile");
                CHECK(cheminAcces.extension == "");
            }
        }
    }
}