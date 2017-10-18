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
#ifndef AFKPEXANON_HPP
#define AFKPEXANON_HPP

#include <string>
#include <iostream>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/range/iterator_range.hpp>
#include "version.hpp"

namespace afk {

class AFKPexAnon
{
public:
    AFKPexAnon(const int &argc, char *argv[]);
    inline virtual ~AFKPexAnon() { }
    virtual int run();

protected:
    template <typename T>
    boost::iterator_range<boost::filesystem::recursive_directory_iterator> traverseDirectoryRecursive(T const &dir)
    {
        boost::filesystem::recursive_directory_iterator l;
        try
        {
            boost::filesystem::recursive_directory_iterator f(dir);
            return boost::make_iterator_range(f, l);
        }
        catch(std::exception const &ex)
        {
            std::cerr << ex.what() << std::endl;
            return boost::make_iterator_range(l, l);
        }
    }

    template <typename T>
    boost::iterator_range<boost::filesystem::directory_iterator> traverseDirectory(T const &dir)
    {
        boost::filesystem::directory_iterator l;
        try
        {
            boost::filesystem::directory_iterator f(dir);
            return boost::make_iterator_range(f, l);
        }
        catch (std::exception const &ex)
        {
            std::cerr << ex.what() << std::endl;
            return boost::make_iterator_range(l, l);
        }
    }

    bool isValidFile(boost::filesystem::directory_entry const &entry);

    bool backupAndChangeExt(const boost::filesystem::path &filePath, const std::string &ext);
    bool createBackupFile(const boost::filesystem::path &filePath);
    bool removeBackupFile(const boost::filesystem::path &filePath);

    virtual bool processProgramOptions();
    virtual void showHelp(const boost::program_options::options_description &desc);
    virtual void showVersion();

protected:
    int    m_argc;
    char** m_argv;

    const std::string defaultBackupExtension{".bak"};
    const std::string defaultPexExtension{".pex"};
    const std::string defaultCurrentFolder{"."};
    const std::string defaultConfigFileName{"afkpexanon.cfg"};
    const std::string defaultTempExtension{".tmp"};
    const std::string afkPexAnonDesString{"AFKPexAnon PEX Anonymizer V"+version::VERSION_STRING};

    /// Boost Program Options
    boost::program_options::options_description m_cmdlineOptions{"Cmdline Options"};
    boost::program_options::options_description m_configFileOptions{"Config File Options"};
    boost::program_options::options_description m_visibleOptions{afkPexAnonDesString};
    boost::program_options::positional_options_description m_posOptions;
    boost::program_options::variables_map m_vm;


    /// Valid extensions of files to process.
    std::vector<std::string> m_validExtensions;
    /// List of source root folders to search.
    std::vector<std::string> m_sourceFolders;
    /// Extension to use for backup files.
    std::string m_backupExtension;
    /// Name of the config file.
    std::string m_configFileName;
    /// Backup files switch.
    bool m_backupFiles;
    /// Character to mask computer and user names.
    char m_mask;
    /// Recurse through sub-folders switch.
    bool m_recursiveFolders;
    /// Show help switch.
    bool m_showHelp;
    /// Show version switch.
    bool m_showVersion;
    /// Turn on verbose mode switch.
    bool m_verboseMode;

private:

};

} // afk namespace

#endif // AFKPEXANON_HPP
