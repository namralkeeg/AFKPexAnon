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
#include <afk/fileformats/pex/pexbase.hpp>
#include <keeg/io/binaryreaders.hpp>
#include <keeg/io/binarywriters.hpp>
#include <algorithm>
#include <exception>
#include <iostream>
#include <iterator>

namespace afk { namespace fileformats { namespace pex {

namespace ki = keeg::io;
namespace ke = keeg::endian;

void PexBase::setPexHeader(const PexHeader &pexHeader)
{
    m_header = pexHeader;
}

void PexBase::setSourceFileName(const std::string &sourceFileName)
{
    m_sourceFileName = sourceFileName;
}

void PexBase::setUserName(const std::string &userName)
{
    m_userName = userName;
}

void PexBase::setMachineName(const std::string &machineName)
{
    m_machineName = machineName;
}

void PexBase::setData(const std::vector<uint8_t> &data)
{
    m_data.resize(data.size());
    std::copy(std::begin(data), std::end(data), std::begin(m_data));
}

bool PexBase::isPex(std::istream &instream)
{
    try
    {
        if (instream)
        {
            instream.seekg(0, std::ios::beg);
            if (readHeader(instream))
            {
                if (isPex(m_header))
                    return true;
            }
        }
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
        return false;
    }

    return false;
}

std::size_t PexBase::read(std::istream &instream)
{
    std::size_t status = 0;
    try
    {
        if (instream)
        {
            if (isPex(instream))
            {
                status = ki::readWString(instream, m_sourceFileName, m_endianOrder);
                status = ki::readWString(instream, m_userName, m_endianOrder);
                status = ki::readWString(instream, m_machineName, m_endianOrder);

                auto headerEndPosition = instream.tellg();
                instream.seekg(0, std::ios::end);
                auto fileSize = instream.tellg();
                auto dataSize = fileSize - headerEndPosition;

                instream.seekg(headerEndPosition, std::ios::beg);
                if (ki::readBytes(instream, m_data, dataSize))
                    status = static_cast<std::size_t>(fileSize);
            }
        }
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
        return status;
    }

    return status;
}

std::size_t PexBase::write(std::ostream &outstream)
{
    std::size_t status = 0;
    try
    {
        if (outstream)
        {
            outstream.seekp(0, std::ios::beg);
            status = writeHeader(outstream);
            if (status)
            {
                status += ki::writeWString(outstream, m_sourceFileName, m_endianOrder);
                status += ki::writeWString(outstream, m_userName, m_endianOrder);
                status += ki::writeWString(outstream, m_machineName, m_endianOrder);

                status += ki::writeBytes(outstream, m_data, m_data.size());
            }
        }
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
        return 0;
    }

    return status;
}

PexBase::PexBase(const keeg::endian::Order &endianOrder) : m_endianOrder(endianOrder)
{ }

std::size_t PexBase::readHeader(std::istream &instream)
{
    std::size_t status = 0;
    try
    {
        if (instream)
        {
            instream.seekg(0, std::ios::beg);
            status = ki::readPODType<PexHeader>(instream, m_header);
            if (status)
            {
                switch (m_header.magic) {
                case UINT32_C(0xFA57C0DE):
                    break;
                /// File data is in reverse endian of the host machine.
                case UINT32_C(0xDEC057FA):
                    m_header.magic = ke::swap(m_header.magic);
                    m_header.gameId = ke::swap(m_header.gameId);
                    m_header.compilationTime = ke::swap(m_header.compilationTime);
                    break;
                default:
                    /// File probably isn't a pex file or an unknown version.
                    status = 0;
                    break;
                }
            }
        }
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
        return 0;
    }

    return status;
}

std::size_t PexBase::writeHeader(std::ostream &outstream)
{
    std::size_t status = 0;
    try
    {
        if (outstream)
        {
            PexHeader header;

            switch (m_endianOrder) {
            case ke::Order::big:
                header.magic = ke::native_to_big(m_header.magic);
                header.majorVersion = ke::native_to_big(m_header.majorVersion);
                header.minorVersion = ke::native_to_big(m_header.minorVersion);
                header.gameId = ke::native_to_big(m_header.gameId);
                header.compilationTime = ke::native_to_big(m_header.compilationTime);
                break;
            case ke::Order::little:
                header.magic = ke::native_to_little(m_header.magic);
                header.majorVersion = ke::native_to_little(m_header.majorVersion);
                header.minorVersion = ke::native_to_little(m_header.minorVersion);
                header.gameId = ke::native_to_little(m_header.gameId);
                header.compilationTime = ke::native_to_little(m_header.compilationTime);
            default:
                header = m_header;
                break;
            }

            outstream.seekp(0, std::ios::beg);
            status = ki::writePODType<PexHeader>(outstream, header);
        }
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
        return 0;
    }

    return status;
}

std::ostream & operator <<(std::ostream &o, const PexBase &pexBase)
{
    o << pexBase.getPexHeader();
    o << "Script name:\t" << pexBase.getSourceFileName() << std::endl;
    o << "User Name:\t" << pexBase.getUserName() << std::endl;
    o << "Machine Name:\t" << pexBase.getMachineName() << std::endl;

    return o;
}

} // pex namespace
} // fileformats namespace
} // afk namespace
