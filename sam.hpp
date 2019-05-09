#pragma once

#include <string>
#include "cigar.hpp"
#include <cstdint>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <ostream>
#include <istream>
#include "interval.hpp"

namespace hc
{

struct SAMRecord
{
    std::string   QNAME;
    std::uint16_t FLAG;
    std::string   RNAME;
    std::uint32_t POS;
    std::uint16_t MAPQ;
    Cigar         CIGAR;
    std::string   RNEXT;
    std::uint32_t PNEXT;
    std::int32_t  TLEN;
    std::string   SEQ;
    std::string   QUAL;

    bool READ_PAIRED()                     const {return (FLAG & 0x1  ) != 0;}
    bool PROPER_PAIR()                     const {return (FLAG & 0x2  ) != 0;}
    bool READ_UNMAPPED()                   const {return (FLAG & 0x4  ) != 0;}
    bool MATE_UNMAPPED()                   const {return (FLAG & 0x8  ) != 0;}
    bool READ_REVERSE_STRAND()             const {return (FLAG & 0x10 ) != 0;}
    bool MATE_REVERSE_STRAND()             const {return (FLAG & 0x20 ) != 0;}
    bool FIRST_OF_PAIR()                   const {return (FLAG & 0x40 ) != 0;}
    bool SECOND_OF_PAIR()                  const {return (FLAG & 0x80 ) != 0;}
    bool SECONDARY_ALIGNMENT()             const {return (FLAG & 0x100) != 0;}
    bool READ_FAILS_VENDOR_QUALITY_CHECK() const {return (FLAG & 0x200) != 0;}
    bool DUPLICATE_READ()                  const {return (FLAG & 0x400) != 0;}
    bool SUPPLEMENTARY_ALIGNMENT()         const {return (FLAG & 0x800) != 0;}

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & QNAME;
        ar & FLAG;
        ar & RNAME;
        ar & POS;
        ar & MAPQ;
        ar & CIGAR;
        ar & RNEXT;
        ar & PNEXT;
        ar & TLEN;
        ar & SEQ;
        ar & QUAL;
    }

    bool empty() const { return SEQ.empty(); }
    auto size()  const { return SEQ.size(); }
    auto get_alignment_begin() const { return POS - 1; }
    auto get_alignment_end()   const { return get_alignment_begin() + CIGAR.get_reference_length(); }
    auto get_interval() const { return Interval(RNAME, get_alignment_begin(), get_alignment_end()); }
};

auto& operator<<(std::ostream& os, const SAMRecord& record)
{
    os << record.QNAME << '\t'
       << record.FLAG  << '\t'
       << record.RNAME << '\t'
       << record.POS   << '\t'
       << record.MAPQ  << '\t'
       << record.CIGAR << '\t'
       << record.RNEXT << '\t'
       << record.PNEXT << '\t'
       << record.TLEN  << '\t'
       << record.SEQ   << '\t'
       << record.QUAL;
    return os;
}

auto& operator>>(std::istream& is, SAMRecord& record)
{
    is >> record.QNAME
       >> record.FLAG
       >> record.RNAME
       >> record.POS
       >> record.MAPQ
       >> record.CIGAR
       >> record.RNEXT
       >> record.PNEXT
       >> record.TLEN
       >> record.SEQ
       >> record.QUAL;
    return is;
}

} // hc
