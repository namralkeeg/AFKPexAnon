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
#ifndef PEXBASE_HPP
#define PEXBASE_HPP

#include <string>
#include <vector>
#include <afk/fileformats/pex/pexheader.hpp>
#include <keeg/endian/conversion.hpp>

namespace afk { namespace fileformats { namespace pex {

class PexBase
{
public:
    /// Getters
    inline keeg::endian::Order getEndianOrder() const { return m_endianOrder; }
    const PexHeader& getPexHeader() const { return m_header; }
    inline std::string getSourceFileName() const { return m_sourceFileName; }
    inline std::string getUserName() const { return m_userName; }
    inline std::string getMachineName() const { return m_machineName; }
    const std::vector<uint8_t>& getData() const { return m_data; }

    /// Setters
    void setPexHeader(const PexHeader &pexHeader);
    void setSourceFileName(const std::string &sourceFileName);
    void setUserName(const std::string &userName);
    void setMachineName(const std::string &machineName);
    void setData(const std::vector<uint8_t> &data);

    /// Header version numbers identify the version of the pex file.
    virtual bool isPex(const PexHeader &pexHeader) = 0;
    virtual bool isPex(std::istream &instream);

    virtual std::size_t read(std::istream &instream);
    virtual std::size_t write(std::ostream &outstream);

    inline virtual ~PexBase() { }

protected:
    keeg::endian::Order m_endianOrder;
    PexHeader m_header;
    std::string m_sourceFileName;
    std::string m_userName;
    std::string m_machineName;
    std::vector<uint8_t> m_data;

    /// Protected constructor for abstract virtual base class.
    PexBase(const keeg::endian::Order &endianOrder);

    virtual std::size_t readHeader(std::istream &instream);
    virtual std::size_t writeHeader(std::ostream &outstream);
};

std::ostream & operator <<(std::ostream &o, const PexBase &pexBase);

} // pex namespace
} // fileformats namespace
} // afk namespace

#endif // PEXBASE_HPP
