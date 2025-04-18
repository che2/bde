// ball_attribute.h                                                   -*-C++-*-
#ifndef INCLUDED_BALL_ATTRIBUTE
#define INCLUDED_BALL_ATTRIBUTE

#include <bsls_ident.h>
BSLS_IDENT("$Id: $")

//@PURPOSE: Provide a representation of (literal) name/value pairs.
//
//@CLASSES:
//  ball::Attribute: (literal) name/value pair
//
//@SEE_ALSO: ball_managedattribute
//
//@DESCRIPTION: This component implements an unusual in-core value-semantic
// class, `ball::Attribute`.  Each instance of this type represents an
// attribute that consists of a (literal) name (held but not owned), and an
// associated value (owned) that can be an `int`, `long`, `long long`,
// `unsigned int`, unsigned long', `unsigned long long`, `bdlb::Guid`, or a
// `bsl::string`.
//
// This component participates in the implementation of "Rule-Based Logging".
// For more information on how to use that feature, please see the package
// level documentation and usage examples for "Rule-Based Logging".
//
// IMPORTANT: The attribute name, whose type is `const char *`, must therefore
// remain valid throughout the life time of the `ball::Attribute` object and
// that of any other `ball::Attribute` objects that are copied or assigned from
// the original object.  It is recommended that only null-terminated C-string
// literals be used for names.
//
///Attribute Naming Recommendations
///--------------------------------
// Attributes can be rendered as part of a log message and used for log
// post-processing and analysis.  It is recommended to use the following naming
// conventions for attribute names:
//
// * An attribute name should start with an alphabetic character, no other
//   special characters, digits should be allowed as the first character of
//   the attribute name.
// * An attribute name should not contain whitespaces.
// * An attribute name should contain only alphanumeric characters,
//   underscores(`_`), and dots(`.`).  Do not use any other special
//   characters.
//
// Disregarding these conventions may prevent the log output from being
// correctly parsed by commonly used log processing software.
//
///Usage
///-----
// This section illustrates intended use of this component.
//
///Example 1: Basic `Attribute` usage
/// - - - - - - - - - - - - - - - - -
// The following code creates four attributes having the same name, but
// different attribute value types.
// ```
//   ball::Attribute a1("day", "Sunday");
//   ball::Attribute a2("day", 7);
//   ball::Attribute a3("day", 7LL);
//   ball::Attribute a4("day", 7ULL);
// ```
// The names of the attributes can be found by calling the `name` method:
// ```
//   assert(0 == bsl::strcmp("day", a1.name()));
//   assert(0 == bsl::strcmp("day", a2.name()));
//   assert(0 == bsl::strcmp("day", a3.name()));
//   assert(0 == bsl::strcmp("day", a4.name()));
// ```
// The `value` method returns a non-modifiable reference to the
// `bdlb::Variant` object that manages the value of the attribute:
// ```
//   assert(true     == a1.value().is<bsl::string>());
//   assert("Sunday" == a1.value().the<bsl::string>());
//
//   assert(true     == a2.value().is<int>());
//   assert(7        == a2.value().the<int>());
//
//   assert(true     == a3.value().is<long long>());
//   assert(7        == a3.value().the<long long>());
//
//   assert(true     == a4.value().is<unsigned long long>());
//   assert(7        == a4.value().the<unsigned long long>());
// ```
// Note that the name string that is passed to the constructor of
// `ball::Attribute` *must* remain valid and unchanged after the
// `ball::Attribute` object is created.  In the next example, we create a
// temporary buffer to store the name string, and then use the buffer to
// create an attribute.  Note that any subsequent changes to this temporary
// buffer will also modify the name of the attribute:
// ```
//   char buffer[] = "Hello";
//   ball::Attribute a4(buffer, 1);                   // BAD IDEA!!!
//   bsl::strcpy(buffer, "World");
//   assert(0 == bsl::strcmp("World", a4.name()));
// ```
// The `ball::Attribute` class also provides a constructor that takes a value
// of type `ball::Attribute::Value`:
// ```
//   ball::Attribute::Value value;
//   value.assign<bsl::string>("Sunday");
//   ball::Attribute a5("day", value);
//   assert(a5 == a1);
// ```
//
///Example 2: Using `Attribute` to log pointers to opaque structure
/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Consider we have an event scheduler that operates on events referred to by
// event handle:
// ```
//   struct Event {
//       d_int d_id;
//   };
//
//   typedef Event * EventHandle;
// ```
// The event handler value can be logged using `ball::Attribute` as follows:
// ```
//   Event           event;
//   EventHandle     handle = &event;
//   ball::Attribute a7("event", handle);
//
//   assert(true   == a7.value().is<const void *>());
//   assert(handle == a7.value().the<const void *>());
// ```

#include <balscm_version.h>

#include <bdlb_guid.h>
#include <bdlb_variant.h>

#include <bslma_allocator.h>
#include <bslma_bslallocator.h>
#include <bslma_usesbslmaallocator.h>

#include <bslmf_nestedtraitdeclaration.h>

#include <bsls_assert.h>

#include <bsl_cstring.h>
#include <bsl_string.h>
#include <bsl_string_view.h>

namespace BloombergLP {
namespace ball {

                        // ===============
                        // class Attribute
                        // ===============

/// An `Attribute` object contains an attribute name which is not managed
/// and an attribute value which is managed.
class Attribute {

  public:
    // TYPES

    /// `Value` is an alias for the attribute type variant.
    typedef bdlb::Variant<int,
                          long,
                          long long,
                          unsigned int,
                          unsigned long,
                          unsigned long long,
                          bsl::string,
                          const void *,
                          bdlb::Guid> Value;

  private:
    // DATA
    bsl::string_view d_name;       // attribute name

    Value            d_value;      // attribute value

    mutable int      d_hashValue;  // hash value (-1 indicates it is unset)

    mutable int      d_hashSize;   // hash size from which the hash value was
                                   // calculated (0 indicates hash value is
                                   // unset)

    // FRIENDS
    friend bool operator==(const Attribute&, const Attribute&);
    friend bool operator!=(const Attribute&, const Attribute&);
    friend bsl::ostream& operator<<(bsl::ostream&, const Attribute&);

  public:
    // TYPES
    typedef bsl::allocator<char> allocator_type;

    // TRAITS
    BSLMF_NESTED_TRAIT_DECLARATION(Attribute, bslma::UsesBslmaAllocator);

    // CLASS METHODS

    /// Return a hash value calculated from the specified `attribute` using
    /// the specified `size` as the number of slots.  The hash value is
    /// guaranteed to be in the range `[0 .. size - 1]`.  The behavior is
    /// undefined unless `0 < size`.
    static int hash(const Attribute& attribute, int size);

    // CREATORS

    /// Create an `Attribute` object having the specified (literal) `name`
    /// and (character string) `value`.  Optionally specify an `allocator`
    /// (e.g., the address of a `bslma::Allocator` object) to supply memory;
    /// otherwise, the default allocator is used.  Note that `name` is not
    /// managed by this object and therefore must remain valid while in use
    /// by any `Attribute` object.
    Attribute(const char              *name,
              const bsl::string_view&  value,
              const allocator_type&    allocator = allocator_type());

    /// Create an `Attribute` object having the specified (literal) `name`
    /// and (character string) `value`.  Optionally specify an `allocator`
    /// (e.g., the address of a `bslma::Allocator` object) to supply memory;
    /// otherwise, the default allocator is used.  Note that `name` is not
    /// managed by this object and therefore must remain valid while in use
    /// by any `Attribute` object.
    Attribute(const char            *name,
              const char            *value,
              const allocator_type&  allocator = allocator_type());

    /// Create an `Attribute` object having the specified (literal) `name`
    /// and `value`.  Optionally specify an `allocator` (e.g., the address
    /// of a `bslma::Allocator` object) to supply memory; otherwise, the
    /// default allocator is used.  Note that `name` is not managed by this
    /// object and therefore must remain valid while in use by any
    /// `Attribute` object.
    Attribute(const char            *name,
              int                    value,
              const allocator_type&  allocator = allocator_type());
    Attribute(const char            *name,
              long                   value,
              const allocator_type&  allocator = allocator_type());
    Attribute(const char            *name,
              long long              value,
              const allocator_type&  allocator = allocator_type());
    Attribute(const char            *name,
              unsigned int           value,
              const allocator_type&  allocator = allocator_type());
    Attribute(const char            *name,
              unsigned long          value,
              const allocator_type&  allocator = allocator_type());
    Attribute(const char            *name,
              unsigned long long     value,
              const allocator_type&  allocator = allocator_type());
    Attribute(const char            *name,
              bdlb::Guid             value,
              const allocator_type&  allocator = allocator_type());

    /// Create an `Attribute` object having the specified (literal) `name`
    /// and (const-qualified void pointer) `value`.  Optionally specify an
    /// `allocator` (e.g., the address of a `bslma::Allocator` object) to
    /// supply memory; otherwise, the default allocator is used.
    Attribute(const char            *name,
              const void            *value,
              const allocator_type&  allocator = allocator_type());

    /// Create an `Attribute` object having the specified (literal) `name`
    /// and `value`.  Optionally specify an `allocator` (e.g., the address
    /// of a `bslma::Allocator` object) to supply memory; otherwise, the
    /// default allocator is used.  Note that `name` is not managed by this
    /// object and therefore must remain valid while in use by any
    /// `Attribute` object.
    Attribute(const char            *name,
              const Value&           value,
              const allocator_type&  allocator = allocator_type());

    /// Create an `Attribute` object having the same (literal) name and
    /// attribute value as the specified `original` object.  Optionally
    /// specify an `allocator` (e.g., the address of a `bslma::Allocator`
    /// object) to supply memory; otherwise, the default allocator is used.
    Attribute(const Attribute&      original,
              const allocator_type& allocator = allocator_type());

    /// Destroy this object.
    //! ~Attribute() = default;

    // MANIPULATORS

    /// Assign the value of the specified `rhs` object to this object.
    Attribute& operator=(const Attribute& rhs);

    /// Set the attribute name of this object to the specified (literal)
    /// `name`.  Note that `name` is not managed by this object and
    /// therefore must remain valid while in use by any `Attribute` object.
    void setName(const char *name);

    /// Set the attribute value of this object to the specified `value`.
    void setValue(const Value&             value);
    void setValue(int                      value);
    void setValue(long                     value);
    void setValue(long long                value);
    void setValue(unsigned int             value);
    void setValue(unsigned long            value);
    void setValue(unsigned long long       value);
    void setValue(bdlb::Guid               value);
    void setValue(const bsl::string_view&  value);
    void setValue(const char              *value);
    void setValue(const void              *value);

    // ACCESSORS

    /// Return the name of this object.
    const char *name() const;

    /// Return a reference to the non-modifiable attribute value of this
    /// object.
    const Value& value() const;

    /// Format this object to the specified output `stream` at the (absolute
    /// value of) the optionally specified indentation `level` and return a
    /// reference to `stream`.  If `level` is specified, optionally specify
    /// `spacesPerLevel`, the number of spaces per indentation level for
    /// this and all of its nested objects.  If `level` is negative,
    /// suppress indentation of the first line.  If `spacesPerLevel` is
    /// negative, format the entire output on one line, suppressing all but
    /// the initial indentation (as governed by `level`).  If `stream` is
    /// not valid on entry, this operation has no effect.
    bsl::ostream& print(bsl::ostream& stream,
                        int           level = 0,
                        int           spacesPerLevel = 4) const;

                                  // Aspects

    /// Return the allocator used by this object to supply memory.  Note
    /// that if no allocator was supplied at construction the default
    /// allocator in effect at construction is used.
    allocator_type get_allocator() const;
};

// FREE OPERATORS

/// Return `true` if the specified `lhs` and `rhs` objects have the same
/// value, and `false` otherwise.  Two `Attribute` objects have the same
/// value if they have the same name (but not necessarily the identical
/// representation in memory), the same attribute value type, and the same
/// attribute value.
bool operator==(const Attribute& lhs, const Attribute& rhs);

/// Return `true` if the specified `lhs` and `rhs` objects do not have the
/// same value, and `false` otherwise.  Two `Attribute` objects do not have
/// the same value if any of their respective names (value, not address),
/// attribute value types, or attribute values differ.
bool operator!=(const Attribute& lhs, const Attribute& rhs);

/// Write the value of the specified `attribute` to the specified `output`
/// stream.  Return the specified `output` stream.
bsl::ostream& operator<<(bsl::ostream& output, const Attribute& attribute);

// ============================================================================
//                              INLINE DEFINITIONS
// ============================================================================

                        // ---------------
                        // class Attribute
                        // ---------------

// CREATORS
inline
Attribute::Attribute(const char              *name,
                     const bsl::string_view&  value,
                     const allocator_type&    allocator)
: d_name(name)
, d_value(allocator.mechanism())
, d_hashValue(-1)
, d_hashSize(0)
{
    d_value.createInPlace<bsl::string>(value);
}

inline
Attribute::Attribute(const char            *name,
                     const char            *value,
                     const allocator_type&  allocator)
: d_name(name)
, d_value(allocator.mechanism())
, d_hashValue(-1)
, d_hashSize(0)
{
    d_value.createInPlace<bsl::string>(value);
}

inline
Attribute::Attribute(const char            *name,
                     int                    value,
                     const allocator_type&  allocator)
: d_name(name)
, d_value(value, allocator.mechanism())
, d_hashValue(-1)
, d_hashSize(0)
{
}

inline
Attribute::Attribute(const char            *name,
                     long                   value,
                     const allocator_type&  allocator)
: d_name(name)
, d_value(value, allocator.mechanism())
, d_hashValue(-1)
, d_hashSize(0)
{
}

inline
Attribute::Attribute(const char            *name,
                     long long              value,
                     const allocator_type&  allocator)
: d_name(name)
, d_value(value, allocator.mechanism())
, d_hashValue(-1)
, d_hashSize(0)
{
}

inline
Attribute::Attribute(const char            *name,
                     unsigned int           value,
                     const allocator_type&  allocator)
: d_name(name)
, d_value(value, allocator.mechanism())
, d_hashValue(-1)
, d_hashSize(0)
{
}

inline
Attribute::Attribute(const char            *name,
                     unsigned long          value,
                     const allocator_type&  allocator)
: d_name(name)
, d_value(value, allocator.mechanism())
, d_hashValue(-1)
, d_hashSize(0)
{
}

inline
Attribute::Attribute(const char            *name,
                     unsigned long long     value,
                     const allocator_type&  allocator)
: d_name(name)
, d_value(value, allocator.mechanism())
, d_hashValue(-1)
, d_hashSize(0)
{
}

inline
Attribute::Attribute(const char            *name,
                     bdlb::Guid             value,
                     const allocator_type&  allocator)
: d_name(name)
, d_value(value, allocator.mechanism())
, d_hashValue(-1)
, d_hashSize(0)
{
}

inline
Attribute::Attribute(const char            *name,
                     const void            *value,
                     const allocator_type&  allocator)
: d_name(name)
, d_value(value, allocator.mechanism())
, d_hashValue(-1)
, d_hashSize(0)
{
}

inline
Attribute::Attribute(const char            *name,
                     const Value&           value,
                     const allocator_type&  allocator)
: d_name(name)
, d_value(value, allocator.mechanism())
, d_hashValue(-1)
, d_hashSize(0)
{
}

inline
Attribute::Attribute(const Attribute&      original,
                     const allocator_type& allocator)
: d_name(original.d_name)
, d_value(original.d_value, allocator.mechanism())
, d_hashValue(original.d_hashValue)
, d_hashSize(original.d_hashSize)
{
}

// MANIPULATORS
inline
Attribute& Attribute::operator=(const Attribute& rhs)
{
    d_name      = rhs.d_name;
    d_value     = rhs.d_value;
    d_hashValue = rhs.d_hashValue;
    d_hashSize  = rhs.d_hashSize;
    return *this;
}

inline
void Attribute::setName(const char *name)
{
    d_name      = name;
    d_hashValue = -1;
}

inline
void Attribute::setValue(const Value& value)
{
    d_value     = value;
    d_hashValue = -1;
}

inline
void Attribute::setValue(int value)
{
    d_value     = value;
    d_hashValue = -1;
}

inline
void Attribute::setValue(long value)
{
    d_value     = value;
    d_hashValue = -1;
}

inline
void Attribute::setValue(long long value)
{
    d_value     = value;
    d_hashValue = -1;
}

inline
void Attribute::setValue(unsigned int value)
{
    d_value     = value;
    d_hashValue = -1;
}

inline
void Attribute::setValue(unsigned long value)
{
    d_value     = value;
    d_hashValue = -1;
}

inline
void Attribute::setValue(unsigned long long value)
{
    d_value     = value;
    d_hashValue = -1;
}

inline
void Attribute::setValue(bdlb::Guid value)
{
    d_value     = value;
    d_hashValue = -1;
}

inline
void Attribute::setValue(const bsl::string_view& value)
{
    d_value.createInPlace<bsl::string>(value);
    d_hashValue = -1;
}

inline
void Attribute::setValue(const char *value)
{
    d_value.createInPlace<bsl::string>(value);
    d_hashValue = -1;
}

inline
void Attribute::setValue(const void *value)
{
    d_value     = value;
    d_hashValue = -1;
}

// ACCESSORS
inline
const char *Attribute::name() const
{
    // guaranteed to be zero-terminated due to the constructors and `setValue`
    // taking name by `const char*`
    return d_name.data();
}

inline
const Attribute::Value& Attribute::value() const
{
    return d_value;
}

                                  // Aspects

inline
Attribute::allocator_type Attribute::get_allocator() const
{
    // Until bdlb::variant is converted to new allocator model.
    return d_value.getAllocator();
}

}  // close package namespace

// FREE OPERATORS
inline
bool ball::operator==(const Attribute& lhs, const Attribute& rhs)
{
    return lhs.d_name == rhs.d_name && lhs.d_value == rhs.d_value;
}

inline
bool ball::operator!=(const Attribute& lhs, const Attribute& rhs)
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
