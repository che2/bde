// bdlat_attributeinfo.h                                              -*-C++-*-
#ifndef INCLUDED_BDLAT_ATTRIBUTEINFO
#define INCLUDED_BDLAT_ATTRIBUTEINFO

#include <bsls_ident.h>
BSLS_IDENT("$Id: $")

//@PURPOSE: Provide a container for attribute information.
//
//@CLASSES:
//  bdlat_AttributeInfo: container for attribute information
//
//@SEE_ALSO: bdlat_sequencefunctions
//
//@DESCRIPTION: This component provides the `bdlat_AttributeInfo` `struct`,
// which is a container for holding information (properties) about a sequence
// attribute.  The properties of an attribute include its name and the length
// of its name, its distinct id within its containing type, its formatting
// mode, and a brief annotation.  Although each attribute property is publicly
// accessible, a manipulator and accessor is also supplied for each.
//
// When accessing or manipulating an attribute of a "sequence" type (using one
// of the functions from the `bdlat_SequenceFunctions` namespace), an instance
// of this `struct` will be passed as the second argument to the accessor or
// manipulator.
//
// Note that this `struct` needs to be a POD type.
//
///Usage
///-----
// This section illustrates intended use of this component.
//
///Example 1: Basic Usage
/// - - - - - - - - - - -
// Suppose you create an accessor for sequence attributes that prints each
// attribute to an output stream:
// ```
// #include <bdlat_attributeinfo.h>
// #include <ostream>
//
// using namespace BloombergLP;
//
// class PrintAttributeWithInfo {
//     // Print the attribute along with its name and annotation.
//
//     // PRIVATE DATA MEMBERS
//     bsl::ostream *d_stream_p;
//
//   public:
//     // CREATORS
//     PrintAttributeWithInfo(bsl::ostream *stream)
//     : d_stream_p(stream)
//     {
//     }
//
//     // OPERATIONS
//     template <typename TYPE>
//     int operator()(const TYPE&                attribute,
//                    const bdlat_AttributeInfo& info)
//     {
//         (*d_stream_p) << attribute << " ("
//                       << bsl::string(info.name(),
//                                      info.nameLength())
//                       << ", "
//                       << info.annotation() << ")\n";
//     }
// };
// ```

#include <bdlscm_version.h>

#include <bdlat_bdeatoverrides.h>

#include <bslmf_istriviallycopyable.h>
#include <bslmf_istriviallydefaultconstructible.h>
#include <bslmf_nestedtraitdeclaration.h>

#include <bsls_keyword.h>

#include <bsl_climits.h>
#include <bsl_cstring.h>
#include <bsl_iosfwd.h>

#ifndef BDE_DONT_ALLOW_TRANSITIVE_INCLUDES
#include <bslalg_typetraits.h>
#endif // BDE_DONT_ALLOW_TRANSITIVE_INCLUDES


namespace BloombergLP {

                         // ==========================
                         // struct bdlat_AttributeInfo
                         // ==========================

/// This `struct` holds information about an attribute.  Its data members
/// are `public` by design so that instances may be statically initialized.
struct bdlat_AttributeInfo {

    // PUBLIC DATA -- DO *NOT* REORDER
    int         d_id;              // distinct id of attribute
    const char *d_name_p;          // name of attribute
    int         d_nameLength;      // length of attribute name (0-terminator
                                   // not included)
    const char *d_annotation_p;    // attribute annotation
    int         d_formattingMode;  // formatting mode

    // TRAITS
    BSLMF_NESTED_TRAIT_DECLARATION(bdlat_AttributeInfo,
                                   bsl::is_trivially_copyable);
    BSLMF_NESTED_TRAIT_DECLARATION(bdlat_AttributeInfo,
                                   bsl::is_trivially_default_constructible);

    // CREATORS

    // The following methods are not defined by design:
    //..
    //   bdlat_AttributeInfo();
    //   bdlat_AttributeInfo(const bdlat_AttributeInfo& original);
    //   ~bdlat_AttributeInfo();
    //..
    // The corresponding methods supplied by the compiler are sufficient.

    // MANIPULATORS

    // The following method is not defined by design:
    //..
    //   bdlat_AttributeInfo& operator=(const bdlat_AttributeInfo& rhs);
    //..
    // The assignment operator supplied by the compiler is sufficient.

    /// Return a reference to the modifiable annotation of this attribute
    /// info object.
    const char *& annotation();

    /// Return a reference to the modifiable formatting mode of this
    /// attribute info object.
    int& formattingMode();

    /// Return a reference to the modifiable id of this attribute info
    /// object.
    int& id();

    /// Return a reference to the modifiable name of this attribute info
    /// object.
    const char *& name();

    /// Return a reference to the modifiable length of the name of this
    /// attribute info object.  Note that the 0-terminator is not included
    /// in the length.
    int& nameLength();

    // ACCESSORS

    /// Return the non-modifiable annotation of this attribute info object.
    BSLS_KEYWORD_CONSTEXPR
    const char *annotation() const;

    /// Return the formatting mode of this attribute info object.
    BSLS_KEYWORD_CONSTEXPR
    int formattingMode() const;

    /// Return the id of this attribute info object.
    BSLS_KEYWORD_CONSTEXPR
    int id() const;

    /// Return the non-modifiable name of this attribute info object.
    BSLS_KEYWORD_CONSTEXPR
    const char *name() const;

    /// Return the length of the name of this attribute info object.  Note
    /// that the 0-terminator is not included in the length.
    BSLS_KEYWORD_CONSTEXPR
    int nameLength() const;
};

// ============================================================================
//                            INLINE DEFINITIONS
// ============================================================================

// FREE OPERATORS

/// Return `true` if the specified `lhs` and `rhs` attribute info objects
/// have the same value, and `false` otherwise.  Two attribute info objects
/// have the same value if each of their respective properties are
/// identical.
inline
bool operator==(const bdlat_AttributeInfo& lhs,
                const bdlat_AttributeInfo& rhs);

/// Return `true` if the specified `lhs` and `rhs` attribute info objects do
/// not have the same value, and `false` otherwise.  Two attribute info
/// objects do not have the same value if at least one of their respective
/// properties is not identical.
inline
bool operator!=(const bdlat_AttributeInfo& lhs,
                const bdlat_AttributeInfo& rhs);

/// Write the value of the specified `attributeInfo` to the specified
/// `stream`.
bsl::ostream& operator<<(bsl::ostream&              stream,
                         const bdlat_AttributeInfo& attributeInfo);

// MANIPULATORS
inline
const char *& bdlat_AttributeInfo::annotation()
{
    return d_annotation_p;
}

inline
int& bdlat_AttributeInfo::formattingMode()
{
    return d_formattingMode;
}

inline
int& bdlat_AttributeInfo::id()
{
    return d_id;
}

inline
const char *& bdlat_AttributeInfo::name()
{
    return d_name_p;
}

inline
int& bdlat_AttributeInfo::nameLength()
{
    return d_nameLength;
}

// ACCESSORS
inline
BSLS_KEYWORD_CONSTEXPR
const char *bdlat_AttributeInfo::annotation() const
{
    return d_annotation_p;
}

inline
BSLS_KEYWORD_CONSTEXPR
int bdlat_AttributeInfo::formattingMode() const
{
    return d_formattingMode;
}

inline
BSLS_KEYWORD_CONSTEXPR
int bdlat_AttributeInfo::id() const
{
    return d_id;
}

inline
BSLS_KEYWORD_CONSTEXPR
const char *bdlat_AttributeInfo::name() const
{
    return d_name_p;
}

inline
BSLS_KEYWORD_CONSTEXPR
int bdlat_AttributeInfo::nameLength() const
{
    return d_nameLength;
}

// FREE OPERATORS

inline
bool operator==(const bdlat_AttributeInfo& lhs, const bdlat_AttributeInfo& rhs)
{
    return lhs.formattingMode() == rhs.formattingMode()
        && lhs.id()             == rhs.id()
        && lhs.nameLength()     == rhs.nameLength()
        && 0 == bsl::memcmp(lhs.name(), rhs.name(), lhs.nameLength())
        && 0 == bsl::strcmp(lhs.annotation(), rhs.annotation());
}

inline
bool operator!=(const bdlat_AttributeInfo& lhs, const bdlat_AttributeInfo& rhs)
{
    return !(lhs == rhs);
}

}  // close enterprise namespace

#endif

// ----------------------------------------------------------------------------
// Copyright 2015 Bloomberg Finance L.P.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ----------------------------- END-OF-FILE ----------------------------------
