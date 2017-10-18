/*
 * Copyright (C) 2017 Larry Lopez
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
#include "afkpexanon.hpp"
#include <afk/fileformats/pex/pexfactory.hpp>
#include <keeg/common/enums.hpp>
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iterator>

namespace afk {

using namespace std;
using namespace afk::fileformats::pex;
namespace bf = boost::filesystem;
namespace bpo = boost::program_options;

AFKPexAnon::AFKPexAnon(const int &argc, char *argv[]) : m_argc(argc), m_argv(argv)
{ }

int AFKPexAnon::run()
{
    if (!processProgramOptions())
        return EXIT_FAILURE;

    if (m_showVersion)
    {
        showVersion();
        if (!m_showHelp)
            return EXIT_SUCCESS;
    }

    if (m_showHelp)
    {
        showHelp(m_visibleOptions);
        return EXIT_SUCCESS;
    }

    try
    {
        std::vector<bf::path> entries;
        for (const auto &dir: m_sourceFolders)
        {
            if (m_verboseMode)
                std::cout << "Searching: " << dir << std::endl;

            /// Recursively add files from subfolders.
            if (m_recursiveFolders)
            {
                for (auto &entry: traverseDirectoryRecursive(dir))
                    if (isValidFile(entry))
                        entries.push_back(entry.path());
            }
            /// Only add files from the root of each specified folder.
            else
            {
                for (auto &entry: traverseDirectory(dir))
                    if (isValidFile(entry))
                        entries.push_back(entry.path());
            }
        }

        /// Sort the files in alphabetical order.
        std::sort(std::begin(entries), std::end(entries));

        if (entries.size() > 0)
            std::cout << "Anonymizing " << entries.size() << " File(s):" << std::endl;

        for (bf::path &entry: entries)
        {
            /// Check if the file is a recognized type.
            std::unique_ptr<PexBase> pexOrig;
            {
                ifstream entryFile(entry.string(), std::ios::binary);
                pexOrig = fileformats::pex::PexFactory::createUniquePex(entryFile);
                if (pexOrig)
                    pexOrig->read(entryFile);
            }

            if (pexOrig)
            {
                if (m_backupFiles)
                {
                    bf::path backupPath = entry;
                    backupPath.replace_extension(m_backupExtension);

                    if (m_verboseMode)
                        std::cout << "Creating backup file: " << backupPath.string() << std::endl;

                    if (!createBackupFile(entry))
                        throw std::runtime_error("Unable to create backup file: " + backupPath.string());
                }

                std::cout << entry << std::endl;
                if (m_verboseMode)
                    std::cout << *pexOrig << std::endl;

                /// Create a temporary working file in case there's an error.
                if (backupAndChangeExt(entry, defaultTempExtension))
                {
                    bf::path tempPath = entry;
                    tempPath.replace_extension(defaultTempExtension);
                    std::unique_ptr<PexBase> pexDest;
                    {
                        ifstream destFile(tempPath.string(), std::ios::binary);
                        pexDest = PexFactory::createUniquePex(destFile);
                        if (pexDest)
                            pexDest->read(destFile);
                    }

                    if (pexDest)
                    {
                        /// Fill the machine name with mask characters.
                        pexDest->setMachineName(std::string(pexDest->getMachineName().size(), m_mask));
                        /// Fill the user name with mask characters.
                        pexDest->setUserName(std::string(pexDest->getUserName().size(), m_mask));

                        /// Strip the path from script names in fallout 4 pex's.
                        /// No idea why Bethesda in their infinte wisdom decided to add the path from the
                        /// temporary folder to the source file?
                        if (pexDest->getPexHeader().gameId == keeg::common::enumToIntegral(GameID::fallout4))
                        {
                            pexDest->setSourceFileName(bf::path(pexDest->getSourceFileName()).filename().string());
                        }

                        /// Write out the changes to the temp file.
                        {
                            ofstream destFile(tempPath.string(), std::ios::binary | std::ios::trunc);
                            if (!pexDest->write(destFile))
                            {
                                /// Failed to write out to the temp file properly. Attemp to clean up.
                                destFile.close();
                                bf::remove(tempPath);
                                throw std::runtime_error("Unable to write to temporary file: " + tempPath.string());
                            }
                        }

                        /// Read the temp file back in and compare the data to the original.
                        {
                            ifstream destFile(tempPath.string(), std::ios::binary);
                            if (!pexDest->read(destFile))
                            {
                                /// something bad happened.
                                /// Failed to read the temp file properly. Attemp to clean up.
                                destFile.close();
                                bf::remove(tempPath);
                                throw std::runtime_error("Unable to read temporary file: " + tempPath.string());
                            }
                        }

                        /// Validate the data after the header and swap files if it's valid.
                        if ((pexOrig->getPexHeader() == pexDest->getPexHeader())
                                && (pexOrig->getData() == pexDest->getData()))
                        {
                            bf::remove(entry);
                            bf::rename(tempPath, entry);
                        }
                        else
                        {
                            bf::remove(tempPath);
                            std::cout << "Unable to validate data skipping: " + entry.string() << std::endl;
                        }
                    }
                    /// Reading in the temp file failed.
                    else
                    {
                        throw std::runtime_error("Reading in the temp file: " + tempPath.string() + " failed.");
                    }
                }
                else
                {
                    throw std::runtime_error("Unable to create temporary files");
                }
            }
            else
            {
                std::cout << "Unrecognized file type: " << entry << std::endl;
            }
        }
    }
    catch (std::exception const &ex)
    {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

bool AFKPexAnon::isValidFile(bf::directory_entry const &entry)
{
    try
    {
        auto it = std::find(std::begin(m_validExtensions),
                            std::end(m_validExtensions),
                            entry.path().extension().string());

        return bf::is_regular_file(entry)
                && !bf::is_other(entry)
                && (it != std::end(m_validExtensions));
    }
    catch (std::exception const &ex)
    {
        std::cerr << ex.what() << std::endl;
        return false;
    }
}

bool AFKPexAnon::backupAndChangeExt(const boost::filesystem::path &filePath, const string &ext)
{
    try
    {
        bf::path backupFile = filePath;
        backupFile.replace_extension(ext);
        bf::copy_file(filePath, backupFile, bf::copy_option::fail_if_exists);
        return true;
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
        return false;
    }
}

bool AFKPexAnon::createBackupFile(const bf::path &filePath)
{
    return backupAndChangeExt(filePath, m_backupExtension);
}

bool AFKPexAnon::removeBackupFile(const bf::path &filePath)
{
    try
    {
        bf::path backupFile = filePath;
        backupFile.replace_extension(m_backupExtension);
        return bf::remove(backupFile);
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
        return false;
    }
}

void AFKPexAnon::showHelp(const bpo::options_description &desc)
{
    std::cout << desc << std::endl;
}

void AFKPexAnon::showVersion()
{
    std::cout << afkPexAnonDesString << std::endl;
}

bool AFKPexAnon::processProgramOptions()
{
    bpo::options_description genericOptions{"Generic Options"};
    bpo::options_description configOptions{"Config Options"};
    bpo::options_description hiddenOptions{"Hidden Options"};

    genericOptions.add_options()
        (
            "help,h",
            bpo::value<bool>(&m_showHelp)
                ->default_value(false)
                ->implicit_value(true)
                ->zero_tokens(),
            "Help Screen"
        )
        (
            "config,c",
            bpo::value<string>(&m_configFileName)
                ->default_value(defaultConfigFileName),
            "Use configuration file."
        )
        (
            "version",
            bpo::value<bool>(&m_showVersion)
                ->default_value(false)
                ->implicit_value(true)
                ->zero_tokens(),
            "Show application version information."
        );

    configOptions.add_options()
        (
            "source,s",
            bpo::value<std::vector<std::string> >(&m_sourceFolders)
                ->default_value(std::vector<std::string>(1, defaultCurrentFolder), defaultCurrentFolder)
                ->multitoken()
                ->composing(),
            "Source Folder(s), defaults to current folder."
        )
        (
            "backup,b",
            bpo::value<bool>(&m_backupFiles)
                ->default_value(false)
                ->implicit_value(true)
                ->zero_tokens(),
            "Enables the creation of backup Files."
        )
        (
            "mask,m",
            bpo::value<char>(&m_mask)
                ->default_value('*'),
            "Character to mask computer and user name. Defaults to *"
        )
        (
            "recursive,r",
            bpo::value<bool>(&m_recursiveFolders)
                ->default_value(false)
                ->implicit_value(true)
                ->zero_tokens(),
            "Recursively process all subfolders."
        )
        (
            "verbose",
            bpo::value<bool>(&m_verboseMode)
                ->default_value(false)
                ->implicit_value(true)
                ->zero_tokens(),
            "Enables verbose output mode."
        );

    m_posOptions.add("source", -1);

    hiddenOptions.add_options()
        (
            "backup-extension",
            bpo::value<std::string>(&m_backupExtension)
                ->default_value(defaultBackupExtension),
            "Backup files extension."
        )
        (
            "valid-extension",
            bpo::value<std::vector<std::string>>(&m_validExtensions)
                ->default_value(std::vector<std::string>(1, defaultPexExtension), defaultPexExtension)
                ->multitoken()
                ->composing(),
            "File Extensions to process."
        );

    m_cmdlineOptions.add(genericOptions).add(configOptions).add(hiddenOptions);
    m_configFileOptions.add(configOptions).add(hiddenOptions);
    m_visibleOptions.add(genericOptions).add(configOptions);

    bpo::command_line_parser parser{m_argc, m_argv};
    parser.options(m_cmdlineOptions).positional(m_posOptions).allow_unregistered().style(
                bpo::command_line_style::default_style |
                bpo::command_line_style::allow_slash_for_short
    );

    try
    {
        bpo::parsed_options parsedOptions = parser.run();
        bpo::store(parsedOptions, m_vm);
        bpo::notify(m_vm);

        {
            std::ifstream configFile{m_configFileName};
            if (configFile)
            {
                bpo::store(bpo::parse_config_file(configFile, m_configFileOptions), m_vm);
                bpo::notify(m_vm);
            }
        }

    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
        return false;
    }

    return true;
}

} // afk namespace
