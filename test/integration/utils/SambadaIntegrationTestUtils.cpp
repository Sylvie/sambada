#include "SambadaIntegrationTestUtils.hpp"
#include "SambadaStoreyHistogramMapper.hpp"

#include <fstream>
#include <sstream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

std::string SambadaIntegrationTestUtils::computePlatformSpecificProgramName(const std::string& baseProgramName)
{
	std::string programName(baseProgramName);
	std::string programExtension("");
#ifdef sambada_executable_extension
	programExtension = sambada_executable_extension;
#endif
	if (programExtension.size() > 0)
	{
		programName += programExtension;
	}
	return programName;
}

std::string SambadaIntegrationTestUtils::getTopSourceDirectory()
{
	std::string topSourceDirectory = "..";
#ifdef sambada_top_source_dir
	topSourceDirectory = sambada_top_source_dir;
#endif
	topSourceDirectory += "/";
	return topSourceDirectory;
}

SambadaRegressionResults SambadaIntegrationTestUtils::readRegressionResults(std::ifstream& lecteur, bool hasHeader, int dimension)
{
	SambadaRegressionResults results;

	if (hasHeader)
	{
		std::string header("");
		getline(lecteur, header);

		std::istringstream iss(header);
		std::string lu;
		while (iss >> lu >> std::ws)
		{
			results.header.push_back(lu);
		}

		lecteur >> std::ws;
	}
	lecteur >> std::ws;

	while (!lecteur.eof())
	{
		std::vector<std::string> ligne(0);
		for (int i(0); i <= dimension; ++i)
		{
			std::string lu("");
			lecteur >> lu >> std::ws;
			ligne.push_back(lu);
			if (lecteur.eof())
			{
				break;
			}
		}
		results.etiquettes.push_back(ligne);

		if (lecteur.eof())
		{
			break;
		}

		std::string concatenatedValues("");
		getline(lecteur, concatenatedValues);

		std::vector<long double> values(0);
		std::istringstream iss(concatenatedValues);
		long double value;
		while (iss >> value >> std::ws)
		{
			values.push_back(value);
		}
		results.valeurs.push_back(values);

		lecteur >> std::ws;
	}

	return results;
}

SambadaSpatialAutocorrelationResults SambadaIntegrationTestUtils::readSpatialAutocorrelationResults(std::ifstream& lecteur, int nombreVariables)
{
	SambadaSpatialAutocorrelationResults results;

	std::string header("");
	getline(lecteur, header);

	std::istringstream iss(header);
	std::string lu;
	while (iss >> lu >> std::ws)
	{
		results.header.push_back(lu);
	}

	lecteur >> std::ws;
	lecteur >> std::ws;

	while (!lecteur.eof())
	{
		// etiquette
		std::string lu("");
		lecteur >> lu >> std::ws;
		results.etiquettes.push_back(lu);

		if (lecteur.eof())
		{
			break;
		}

		std::vector<std::string> ligne(0);
		std::string concatenatedValues("");
		getline(lecteur, concatenatedValues);

		std::vector<long double> values(0);
		std::istringstream iss(concatenatedValues);
		long double value;
		while (iss >> value >> std::ws)
		{
			values.push_back(value);
		}
		results.valeurs.push_back(values);

		lecteur >> std::ws;
	}

	return results;
}

SambadaStoreyHistogram SambadaIntegrationTestUtils::readStoreyHistogram(std::ifstream& lecteur)
{
	SambadaStoreyHistogramMapper mapper;

	SambadaStoreyHistogram histogram(mapper.readStoreyHistogram(lecteur));

	return histogram;
}


SambadaInputData SambadaIntegrationTestUtils::readInputData(std::ifstream& lecteur, int nombreColonnes)
{
	SambadaInputData donnees;

	while (!lecteur.eof())
	{
		std::vector<std::string> ligne(0);
		for (int i(0); i < nombreColonnes; ++i)
		{
			std::string lu("");
			lecteur >> lu >> std::ws;
			ligne.push_back(lu);
			if (lecteur.eof())
			{
				break;
			}
		}
		donnees.data.push_back(ligne);

		if (lecteur.eof())
		{
			break;
		}

		lecteur >> std::ws;
	}

	return donnees;
}

void SambadaIntegrationTestUtils::copyFileAndUpdatePermissions(const std::string& inputFile, const std::string& outputFile)
{
	fs::path pathInputFile(fs::path(inputFile.c_str()));
	fs::path pathOutputFile(fs::path(outputFile.c_str()));

	fs::copy(pathInputFile, pathOutputFile, fs::copy_options::overwrite_existing);
	fs::permissions(pathOutputFile, fs::perms::owner_all | fs::perms::group_all | fs::perms::others_read);
}

bool SambadaIntegrationTestUtils::doesFileExist(const std::string& filename)
{
	return fs::exists(fs::path(filename.c_str()));
}

bool SambadaIntegrationTestUtils::doesAnyFileExist(const std::vector<std::string>& filenames)
{
	bool result(false);
	for (size_t i(0); i < filenames.size() && !result; ++i)
	{
		if (doesFileExist(filenames[i]))
		{
			result = true;
		}
	}
	return result;
}

void SambadaIntegrationTestUtils::removeFiles(const std::vector<std::string>& filenames)
{
	for (size_t i(0); i < filenames.size(); ++i)
	{
		remove(filenames[i].c_str());
	}
}