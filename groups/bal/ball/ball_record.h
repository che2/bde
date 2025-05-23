// ball_record.h                                                      -*-C++-*-
#ifndef INCLUDED_BALL_RECORD
#define INCLUDED_BALL_RECORD

#include <bsls_ident.h>
BSLS_IDENT("$Id: $")

//@PURPOSE: Provide a container for the fields and attributes of a log record.
//
//@CLASSES:
//  ball::Record: container for a log record's fields and attributes
//
//@SEE_ALSO: ball_recordattributes, ball_logger
//
//@DESCRIPTION: This component provides a single, unconstrained
// (value-semantic) attribute class, `ball::Record`, that is used to describe
// the properties of a logged message.
//
///Attributes
///----------
// ```
// Name                Type
// ------------------  -----------------------------------
// fixedFields         ball::RecordAttributes
// userFields          ball::UserFields
// attributes          bsl::vector<ball::ManagedAttribute>
// ```
// * `fixedFields`: mandatory log fields including timestamp, location,
//   severity, process id, and the log message.
// * `userFields`: user-managed fields associated with a log record.  Note
//    that use of these fields is deprecated and superseded by `attributes`.
// * `attributes`: user-managed name/value pairs associated with a log record.
//
// `ball::Record` aggregates a set of fixed fields and various user-defined
// fields and attributes into one record type, useful for transmitting a
// customized log record as a single instance rather than passing around
// individual attributes separately.  Note that this class is a pure attribute
// class with no constraints, other than the total memory required for the
// class.  Also note that this class is not thread-safe.
//
///Usage
///-----
// This section illustrates intended use of this component.
//
///Example 1: Basic Use of `ball::Record`
/// - - - - - - - - - - - - - - - - - - -
// The following example demonstrates how to create and set the properties of
// a `ball::Record`.  Note that users of the `ball` logging subsystem are not
// expected to create records directly.
//
// First we default create a `ball::Record`, `record`, and verify it has a
// default set of attributes:
// ```
// ball::Record record;
//
// assert(ball::RecordAttributes() == record.fixedFields());
// assert(0                        == record.customFields().length());
// ```
// Then, we set the fixed fields of the record to contain a simple message:
// ```
// int                 processId = bdls::ProcessUtil::getProcessId();
// bsls::Types::Uint64 threadId  = bslmt::ThreadUtil::selfIdAsUint64();
//
// ball::RecordAttributes attributes(bdlt::CurrentTime::utc(), // time stamp
//                                   processId,                // process id
//                                   threadId,                 // thread id
//                                   __FILE__,                 // filename
//                                   __LINE__,                 // line number
//                                   "ExampleCategory",        // category
//                                   ball::Severity::e_WARN,   // severity
//                                   "Simple Test Message");   // message
// record.setFixedFields(attributes);
//
// assert(attributes == record.fixedFields());
// ```
// Next, we add an additional attribute to the log record:
// ```
// record.addAttribute(ball::Attribute("myLib.name", "John Smith"));
// ```
// Finally, we write the record to a stream:
// ```
// bsl::ostringstream output;
// output << record << bsl::endl;
// ```

#include <balscm_version.h>

#include <ball_countingallocator.h>
#include <ball_managedattribute.h>
#include <ball_recordattributes.h>
#include <ball_userfields.h>

#include <bslma_allocator.h>
#include <bslma_default.h>
#include <bslma_usesbslmaallocator.h>

#include <bslmf_nestedtraitdeclaration.h>

#include <bsls_alignment.h>
#include <bsls_assert.h>

#include <bsl_iosfwd.h>
#include <bsl_vector.h>

#ifndef BDE_DONT_ALLOW_TRANSITIVE_INCLUDES
#include <bslalg_typetraits.h>
#endif // BDE_DONT_ALLOW_TRANSITIVE_INCLUDES

namespace BloombergLP {
namespace ball {

                           // ============
                           // class Record
                           // ============

/// This class provides a container for a set of fields that are appropriate
/// for a user-configurable log record.  The class contains a
/// `RecordAttributes` object that in turn holds a fixed set of fields, a
/// `ball::UserFields` object that holds a set of optional, user-defined
/// fields, and a set of attributes associated with this log record.  For
/// each of these three sub-containers there is an accessor for obtaining
/// the container value and a manipulator for changing that value.
///
/// Additionally, this class supports a complete set of *value* *semantic*
/// operations, including copy construction, assignment and equality
/// comparison, and `ostream` printing.  A precise operational definition of
/// when two instances have the same value can be found in the description
/// of `operator==` for the class.  This class is *exception* *neutral* with
/// no guarantee of rollback: If an exception is thrown during the
/// invocation of a method on a pre-existing instance, the object is left in
/// a valid state, but its value is undefined.  In no event is memory
/// leaked.  Finally, **aliasing** (e.g., using all or part of an object as
/// both source and destination) is supported in all cases.
class Record {

  private:
    // DATA
    CountingAllocator  d_allocator;     // memory allocator

    RecordAttributes   d_fixedFields;   // bytes used by fixed fields

    UserFields         d_userFields;    // bytes used by user fields

    bsl::vector<ManagedAttribute>
                       d_attributes;    // managed attributes

    bslma::Allocator  *d_allocator_p;   // allocator used to supply memory;
                                        // held but not own

    // FRIENDS
    friend bool operator==(const Record&, const Record&);

  public:
    // CLASS METHODS

    /// Destroy the specified `*object` and use the allocator held by `*object`
    /// to deallocate its memory footprint.  The behavior is undefined unless
    /// `object` is the address of a valid log record.
    static void deleteObject(const Record *object);

    // TRAITS
    BSLMF_NESTED_TRAIT_DECLARATION(Record, bslma::UsesBslmaAllocator);

    // CREATORS

    /// Create a log record having default values for its fixed fields and its
    /// user-defined fields.  Optionally specify a `basicAllocator` used to
    /// supply memory.  If `basicAllocator` is 0, the currently installed
    /// default allocator is used.
    explicit Record(bslma::Allocator *basicAllocator = 0);

    /// Create a log record with fixed fields having the value of the specified
    /// `fixedFields` and user-defined fields having the value of the specified
    /// `userFields`.  Optionally specify a `basicAllocator` used to supply
    /// memory.  If `basicAllocator` is 0, the currently installed default
    /// allocator is used.
    Record(const RecordAttributes&  fixedFields,
           const UserFields&        userFields,
           bslma::Allocator        *basicAllocator = 0);

    /// Create a log record having the value of the specified `original` log
    /// record.  Optionally specify a `basicAllocator` used to supply memory.
    /// If `basicAllocator` is 0, the currently installed default allocator is
    /// used.
    Record(const Record& original, bslma::Allocator *basicAllocator = 0);

    /// Destroy this object.
    //! ~Record() = default;

    // MANIPULATORS

    /// Assign to this log record the value of the specified `rhs` log record
    /// and return the reference to this modifiable record.
    Record& operator=(const Record& rhs);

    /// Clear this log record by removing the user fields, attributes, and
    /// clearing the fixed field's message buffer.  Note that this method is
    /// tailored for efficient memory use within the `ball` logging system.
    void clear();

    /// Add a managed copy of the specified `attribute` to the container of
    /// attributes maintained by this log record.
    void addAttribute(const ball::Attribute& attribute);

    /// Return the modifiable fixed fields of this log record.
    RecordAttributes& fixedFields();

    /// Set the fixed fields of this log record to the value of the
    /// specified `fixedFields`.
    void setFixedFields(const RecordAttributes& fixedFields);

    /// Set the custom user-defined fields of this log record to the value
    /// of the specified `userFields`.
    ///
    /// **DEPRECATED**: Use log record attributes.
    void setCustomFields(const ball::UserFields& userFields);

    /// Return a reference providing modifiable access to the custom
    /// user-defined fields of this log record.
    ///
    /// **DEPRECATED**: Use log record attributes.
    ball::UserFields& customFields();

    // ACCESSORS

    /// Return the non-modifiable fixed fields of this log record.
    const RecordAttributes& fixedFields() const;

    /// Return a reference providing non-modifiable access to the custom
    /// user-defined fields of this log record.
    ///
    /// **DEPRECATED**: Use log record attributes.
    const ball::UserFields& customFields() const;

    /// Return a reference providing non-modifiable access to the attributes
    /// of this log record.
    const bsl::vector<ball::ManagedAttribute>& attributes() const;

    /// Return the total number of bytes of dynamic memory allocated by
    /// this log record object.  Note that this value does not include
    /// `sizeof *this`.
    int numAllocatedBytes() const;

    /// Format this object to the specified output `stream` at the
    /// optionally specified indentation `level` and return a reference to
    /// the modifiable `stream`.  If `level` is specified, optionally
    /// specify `spacesPerLevel`, the number of spaces per indentation
    /// level for this and all of its nested objects.  Each line is
    /// indented by the absolute value of `level * spacesPerLevel`.  If
    /// `level` is negative, suppress indentation of the first line.  If
    /// `spacesPerLevel` is negative, suppress line breaks and format the
    /// entire output on one line.  If `stream` is initially invalid, this
    /// operation has no effect.
    bsl::ostream& print(bsl::ostream& stream,
                        int           level = 0,
                        int           spacesPerLevel = 4) const;
};

// FREE OPERATORS

/// Return `true` if the specified `lhs` and `rhs` log records have the same
/// value, and `false` otherwise.  Two log records have the same value if the
/// respective fixed fields have the same value and the respective user-defined
/// fields have the same value.
bool operator==(const Record& lhs, const Record& rhs);

/// Return `true` if the specified `lhs` and `rhs` log records do not have the
/// same value, and `false` otherwise.  Two log records do not have the same
/// value if either the respective fixed fields or user-defined fields do not
/// have the same value.
bool operator!=(const Record& lhs, const Record& rhs);

/// Format the members of the specified `record` to the specified output
/// `stream` and return a reference to the modifiable `stream`.
bsl::ostream& operator<<(bsl::ostream& stream, const Record& record);

// ============================================================================
//                              INLINE DEFINITIONS
// ============================================================================

                           // ------------
                           // class Record
                           // ------------

// CLASS METHODS
inline
void Record::deleteObject(const Record *object)
{
    object->d_allocator_p->deleteObjectRaw(object);
}

// CREATORS
inline
Record::Record(bslma::Allocator *basicAllocator)
: d_allocator(basicAllocator)
, d_fixedFields(&d_allocator)
, d_userFields(&d_allocator)
, d_attributes(&d_allocator)
, d_allocator_p(bslma::Default::allocator(basicAllocator))
{
}

inline
Record::Record(const RecordAttributes&  fixedFields,
               const ball::UserFields&  userFields,
               bslma::Allocator        *basicAllocator)
: d_allocator(basicAllocator)
, d_fixedFields(fixedFields, &d_allocator)
, d_userFields(userFields, &d_allocator)
, d_attributes(&d_allocator)
, d_allocator_p(bslma::Default::allocator(basicAllocator))
{
}

inline
Record::Record(const Record& original, bslma::Allocator *basicAllocator)
: d_allocator(basicAllocator)
, d_fixedFields(original.d_fixedFields, &d_allocator)
, d_userFields(original.d_userFields, &d_allocator)
, d_attributes(original.d_attributes, &d_allocator)
, d_allocator_p(bslma::Default::allocator(basicAllocator))
{
}

// MANIPULATORS
inline
Record& Record::operator=(const Record& rhs)
{
    if (this != &rhs) {
        d_fixedFields  = rhs.d_fixedFields;
        d_userFields   = rhs.d_userFields;
        d_attributes   = rhs.d_attributes;
    }
    return *this;
}

inline
void Record::addAttribute(const Attribute& attribute)
{
    d_attributes.push_back(ManagedAttribute(attribute));
}

inline
void Record::clear()
{
    fixedFields().clearMessage();
    customFields().removeAll();
    d_attributes.clear();
}

inline
RecordAttributes& Record::fixedFields()
{
    return d_fixedFields;
}

inline
void Record::setFixedFields(const RecordAttributes& fixedFields)
{
    d_fixedFields = fixedFields;
}

inline
void Record::setCustomFields(const ball::UserFields& userFields)
{
    d_userFields = userFields;
}

inline
ball::UserFields& Record::customFields()
{
    return d_userFields;
}

// ACCESSORS
inline
const RecordAttributes& Record::fixedFields() const
{
    return d_fixedFields;
}

inline
const ball::UserFields& Record::customFields() const
{
    return d_userFields;
}

inline
const bsl::vector<ball::ManagedAttribute>& Record::attributes() const
{
    return d_attributes;
}

inline
int Record::numAllocatedBytes() const
{
    return static_cast<int>(d_allocator.numBytesTotal());
}

}  // close package namespace

// FREE OPERATORS
inline
bool ball::operator==(const Record& lhs, const Record& rhs)
{
    return lhs.d_fixedFields  == rhs.d_fixedFields
        && lhs.d_userFields   == rhs.d_userFields
        && lhs.d_attributes   == rhs.d_attributes;
}

inline
bool ball::operator!=(const Record& lhs, const Record& rhs)
{
    return !(lhs == rhs);
}

inline
bsl::ostream& ball::operator<<(bsl::ostream& stream, const Record& record)
{
    return record.print(stream, 0, -1);
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
