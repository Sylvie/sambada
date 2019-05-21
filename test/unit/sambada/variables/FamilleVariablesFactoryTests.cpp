#include "catch.hpp"
#include "variables/FamilleVariablesFactory.hpp"

TEST_CASE("Test that FamilleVariableFactory can create FamilleVariables", "[famille-variables-factory-unit]")
{
	sambada::FamilleVariablesFactory factory;
	const int nombrePoints(4);
	const int nombreVariables(5);
	std::vector<bool> valeurs = {1, 1, 1, 1,
	                             0, 1, 0, 1,
	                             1, 0, 1, 1,
	                             1, 1, 0, 0,
	                             1, 0, 0, 1};
	sambada::MatriceBools masque(nombrePoints, nombreVariables, valeurs.cbegin());

	/*
	for (int i(0); i < nombrePoints; ++i)
	{
		std::cout << masque(i, 0) << " " << masque(i, 1) << " " << masque(i, 2) << " " << masque(i, 3) << " " << masque(i, 4) << " " << std::endl;
	}
	std::cout << std::endl;
	*/
	/* Masque
	 1 0 1 1 1
	 1 1 0 1 0
	 1 0 1 0 0
	 1 1 1 0 1
	*/

	sambada::FamilleVariables famille;

	SECTION("No variables nor generations leads to empty family")
	{
		famille = factory.genereFamille(0, 0, masque);

		CHECK(famille.size() == 0);
	}

	SECTION("No variables leads to empty family")
	{
		famille = factory.genereFamille(0, 3, sambada::MatriceBools(nombrePoints, 0));

		CHECK(famille.size() == 0);
	}

	SECTION("No generations leads to empty family")
	{
		famille = factory.genereFamille(nombreVariables, 0, masque);

		CHECK(famille.size() == 0);
	}

	SECTION("Negative number of variables leads to empty family")
	{
		famille = factory.genereFamille(-4, 3, sambada::MatriceBools(nombrePoints, 0));

		CHECK(famille.size() == 0);
	}

	SECTION("Negative number of generations leads to empty family")
	{
		famille = factory.genereFamille(nombreVariables, -2, masque);

		CHECK(famille.size() == 0);
	}

	SECTION("Too large number of generations is corrected")
	{
		famille = factory.genereFamille(nombreVariables, 6, masque);

		CHECK(famille.size() == 6);
	}

	SECTION("Too large number of generations is corrected")
	{
		famille = factory.genereFamille(nombreVariables, 7, masque);

		CHECK(famille.size() == 6);
	}

	SECTION("Creating one-generation family gives expected results")
	{
		famille = factory.genereFamille(nombreVariables, 1, masque);

		CHECKED_IF(famille.size() == 2)
		{
			CHECKED_IF(famille[0].size() == 1)
			{
				const sambada::EtiquetteCombinaisonVariables& etiquetteNulle = famille[0].cbegin()->first;
				const sambada::CombinaisonVariables& combinaisonNulle = famille[0].cbegin()->second;
				CHECK(etiquetteNulle.size() == 0);
				CHECK(combinaisonNulle.variables.size() == 0);
				CHECK(combinaisonNulle.parents.size() == 0);
				CHECK(combinaisonNulle.masque.size() == 1);
			}

			CHECKED_IF(famille[1].size() == nombreVariables)
			{
				sambada::GenerationVariables::const_iterator combinaisonCourante(famille[1].cbegin());
				for (int i(0); i < nombreVariables; ++i)
				{
					const sambada::EtiquetteCombinaisonVariables& etiquette = combinaisonCourante->first;
					const sambada::CombinaisonVariables& combinaison = combinaisonCourante->second;
					CHECKED_IF(etiquette.size() == 1)
					{
						CHECK(*etiquette.cbegin() == i);
					}
					CHECKED_IF(combinaison.variables.size() == 1)
					{
						CHECK(*combinaison.variables.cbegin() == i);
					}
					CHECKED_IF(combinaison.parents.size() == 1)
					{
						CHECK((*combinaison.parents.cbegin()) == famille[0].cbegin()->first);
					}
					CHECKED_IF(combinaison.masque.size() == nombrePoints)
					{
						CHECK(combinaison.masque.rows() == nombrePoints);
						CHECK(combinaison.masque.cols() == 1);
						for (int j(0); j<nombrePoints; ++j)
						{
							CHECK(combinaison.masque(j, 0) == masque(j, i));
						}
					}

					++combinaisonCourante;
				}
			}

		}

	}

	SECTION("Creating two-generation family gives expected results")
	{
		famille = factory.genereFamille(nombreVariables, 2, masque);

		CHECKED_IF(famille.size() == 3)
		{
			CHECKED_IF(famille[0].size() == 1)
			{
				const sambada::EtiquetteCombinaisonVariables& etiquetteNulle = famille[0].cbegin()->first;
				const sambada::CombinaisonVariables& combinaisonNulle = famille[0].cbegin()->second;
				CHECK(etiquetteNulle.size() == 0);
				CHECK(combinaisonNulle.variables.size() == 0);
				CHECK(combinaisonNulle.parents.size() == 0);
				CHECK(combinaisonNulle.masque.size() == 1);
			}

			CHECKED_IF(famille[1].size() == nombreVariables)
			{
				sambada::GenerationVariables::const_iterator combinaisonCourante(famille[1].cbegin());
				for (int i(0); i < nombreVariables; ++i)
				{
					const sambada::EtiquetteCombinaisonVariables& etiquette = combinaisonCourante->first;
					const sambada::CombinaisonVariables& combinaison = combinaisonCourante->second;
					CHECKED_IF(etiquette.size() == 1)
					{
						CHECK(*etiquette.cbegin() == i);
					}
					CHECKED_IF(combinaison.variables.size() == 1)
					{
						CHECK(*combinaison.variables.cbegin() == i);
					}
					CHECKED_IF(combinaison.parents.size() == 1)
					{
						CHECK((*combinaison.parents.cbegin()) == famille[0].cbegin()->first);
					}
					CHECKED_IF(combinaison.masque.size() == nombrePoints)
					{
						CHECK(combinaison.masque.rows() == nombrePoints);
						CHECK(combinaison.masque.cols() == 1);
						for (int j(0); j<nombrePoints; ++j)
						{
							CHECK(combinaison.masque(j, 0) == masque(j, i));
						}
					}

					++combinaisonCourante;
				}
			}
		}

		int nombreCombinaisonADeuxVariables(10);
		std::vector<std::vector<int>> pairesVariables = {{0, 1}, {0, 2}, {0, 3}, {0, 4},
		                                       {1, 2}, {1, 3}, {1, 4},
		                                       {2, 3}, {2, 4},
		                                       {3, 4}};
		std::vector<std::vector<bool>> masques =
				{
					{false, true, false, true},
					{true, false, true, true},
					{true, true, false, false},
					{true, false, false, true},

					{false, false, false, true},
					{false, true, false, false},
					{false, false, false, true},

					{true, false, false, false},
					{true, false, false, true},

					{true, false, false, false},
				};

		CHECKED_IF(famille[2].size() == nombreCombinaisonADeuxVariables)
		{
			sambada::GenerationVariables::const_iterator combinaisonCourante(famille[2].cbegin());
			for (int i(0); i < nombreCombinaisonADeuxVariables; ++i)
			{
				const sambada::EtiquetteCombinaisonVariables& etiquette = combinaisonCourante->first;
				const sambada::CombinaisonVariables& combinaison = combinaisonCourante->second;
				CHECKED_IF(etiquette.size() == 2)
				{
					CHECK(*etiquette.cbegin() == pairesVariables[i][0]);
					CHECK(*etiquette.crbegin() == pairesVariables[i][1]);
				}
				CHECKED_IF(combinaison.variables.size() == 2)
				{
					CHECK(*combinaison.variables.cbegin() == pairesVariables[i][0]);
					CHECK(*combinaison.variables.crbegin() == pairesVariables[i][1]);
				}
				CHECKED_IF(combinaison.parents.size() == 2)
				{
					CHECK(*(*combinaison.parents.cbegin()).cbegin() == pairesVariables[i][0]);
					CHECK(*(*combinaison.parents.crbegin()).cbegin() == pairesVariables[i][1]);
				}
				CHECKED_IF(combinaison.masque.size() == nombrePoints)
				{
					CHECK(combinaison.masque.rows() == nombrePoints);
					CHECK(combinaison.masque.cols() == 1);
					for (int j(0); j<nombrePoints; ++j)
					{
						CHECK(combinaison.masque(j, 0) == masques[i][j]);
					}
				}

				++combinaisonCourante;
			}
		}
	}

	SECTION("Creating three-generation family gives expected results")
	{
		famille = factory.genereFamille(nombreVariables, 3, masque);

		CHECKED_IF(famille.size() == 4)
		{
			CHECKED_IF(famille[0].size() == 1)
			{
				const sambada::EtiquetteCombinaisonVariables& etiquetteNulle = famille[0].cbegin()->first;
				const sambada::CombinaisonVariables& combinaisonNulle = famille[0].cbegin()->second;
				CHECK(etiquetteNulle.size() == 0);
				CHECK(combinaisonNulle.variables.size() == 0);
				CHECK(combinaisonNulle.parents.size() == 0);
				CHECK(combinaisonNulle.masque.size() == 1);
			}

			CHECKED_IF(famille[1].size() == nombreVariables)
			{
				sambada::GenerationVariables::const_iterator combinaisonCourante(famille[1].cbegin());
				for (int i(0); i < nombreVariables; ++i)
				{
					const sambada::EtiquetteCombinaisonVariables& etiquette = combinaisonCourante->first;
					const sambada::CombinaisonVariables& combinaison = combinaisonCourante->second;
					CHECKED_IF(etiquette.size() == 1)
					{
						CHECK(*etiquette.cbegin() == i);
					}
					CHECKED_IF(combinaison.variables.size() == 1)
					{
						CHECK(*combinaison.variables.cbegin() == i);
					}
					CHECKED_IF(combinaison.parents.size() == 1)
					{
						CHECK((*combinaison.parents.cbegin()) == famille[0].cbegin()->first);
					}
					CHECKED_IF(combinaison.masque.size() == nombrePoints)
					{
						CHECK(combinaison.masque.rows() == nombrePoints);
						CHECK(combinaison.masque.cols() == 1);
						for (int j(0); j<nombrePoints; ++j)
						{
							CHECK(combinaison.masque(j, 0) == masque(j, i));
						}
					}

					++combinaisonCourante;
				}
			}
		}

		int nombreCombinaisonADeuxVariables(10);
		std::vector<std::vector<int>> pairesVariables = {{0, 1}, {0, 2}, {0, 3}, {0, 4},
		                                                 {1, 2}, {1, 3}, {1, 4},
		                                                 {2, 3}, {2, 4},
		                                                 {3, 4}};
		std::vector<std::vector<bool>> masquesPaires =
				{
						{false, true, false, true},
						{true, false, true, true},
						{true, true, false, false},
						{true, false, false, true},

						{false, false, false, true},
						{false, true, false, false},
						{false, false, false, true},

						{true, false, false, false},
						{true, false, false, true},

						{true, false, false, false},
				};

		CHECKED_IF(famille[2].size() == nombreCombinaisonADeuxVariables)
		{
			sambada::GenerationVariables::const_iterator combinaisonCourante(famille[2].cbegin());
			for (int i(0); i < nombreCombinaisonADeuxVariables; ++i)
			{
				const sambada::EtiquetteCombinaisonVariables& etiquette = combinaisonCourante->first;
				const sambada::CombinaisonVariables& combinaison = combinaisonCourante->second;
				CHECKED_IF(etiquette.size() == 2)
				{
					CHECK(*etiquette.cbegin() == pairesVariables[i][0]);
					CHECK(*etiquette.crbegin() == pairesVariables[i][1]);
				}
				CHECKED_IF(combinaison.variables.size() == 2)
				{
					CHECK(*combinaison.variables.cbegin() == pairesVariables[i][0]);
					CHECK(*combinaison.variables.crbegin() == pairesVariables[i][1]);
				}
				CHECKED_IF(combinaison.parents.size() == 2)
				{
					CHECK(*(*combinaison.parents.cbegin()).cbegin() == pairesVariables[i][0]);
					CHECK(*(*combinaison.parents.crbegin()).cbegin() == pairesVariables[i][1]);
				}
				CHECKED_IF(combinaison.masque.size() == nombrePoints)
				{
					CHECK(combinaison.masque.rows() == nombrePoints);
					CHECK(combinaison.masque.cols() == 1);
					for (int j(0); j<nombrePoints; ++j)
					{
						CHECK(combinaison.masque(j, 0) == masquesPaires[i][j]);
					}
				}

				++combinaisonCourante;
			}
		}

		int nombreCombinaisonATroisVariables(10);
		std::vector<std::vector<int>> tripletsVariables = {
				{0, 1, 2}, {0, 1, 3}, {0, 1, 4},
		        {0, 2, 3}, {0, 2, 4},
		        {0, 3, 4},
		        {1, 2, 3}, {1, 2, 4},
		        {1, 3, 4}, {2, 3, 4}
		};

		std::vector<std::vector<bool>> masquesTriplets =
				{
						{false, false, false, true},
						{false, true, false, false},
						{false, false, false, true},

						{true, false, false, false},
						{true, false, false, true},

						{true, false, false, false},

						{false, false, false, false},
						{false, false, false, true},

						{false, false, false, false},
						{true, false, false, false},
				};


		CHECKED_IF(famille[3].size() == nombreCombinaisonATroisVariables)
		{
			sambada::GenerationVariables::const_iterator combinaisonCourante(famille[3].cbegin());
			for (int i(0); i < nombreCombinaisonATroisVariables; ++i)
			{
				const sambada::EtiquetteCombinaisonVariables& etiquette = combinaisonCourante->first;
				const sambada::CombinaisonVariables& combinaison = combinaisonCourante->second;
				CHECKED_IF(etiquette.size() == 3)
				{
					CHECK(*etiquette.cbegin() == tripletsVariables[i][0]);
					CHECK(*(++etiquette.cbegin()) == tripletsVariables[i][1]);
					CHECK(*etiquette.crbegin() == tripletsVariables[i][2]);
				}
				CHECKED_IF(combinaison.variables.size() == 3)
				{
					CHECK(*combinaison.variables.cbegin() == tripletsVariables[i][0]);
					CHECK(*(++combinaison.variables.cbegin()) == tripletsVariables[i][1]);
					CHECK(*combinaison.variables.crbegin() == tripletsVariables[i][2]);
				}
				CHECKED_IF(combinaison.parents.size() == 3)
				{
					CHECK(*(*combinaison.parents.cbegin()).cbegin() == tripletsVariables[i][0]);
					CHECK(*(*combinaison.parents.cbegin()).crbegin() == tripletsVariables[i][1]);

					CHECK(*(*(++combinaison.parents.cbegin())).cbegin() == tripletsVariables[i][0]);
					CHECK(*(*(++combinaison.parents.cbegin())).crbegin() == tripletsVariables[i][2]);

					CHECK(*(*combinaison.parents.crbegin()).cbegin() == tripletsVariables[i][1]);
					CHECK(*(*combinaison.parents.crbegin()).crbegin() == tripletsVariables[i][2]);
				}
				CHECKED_IF(combinaison.masque.size() == nombrePoints)
				{
					CHECK(combinaison.masque.rows() == nombrePoints);
					CHECK(combinaison.masque.cols() == 1);
					for (int j(0); j<nombrePoints; ++j)
					{
						CHECK(combinaison.masque(j, 0) == masquesTriplets[i][j]);
					}
				}

				++combinaisonCourante;
			}
		}

	}
}