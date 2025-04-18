// balxml_typesprintutil.h                                            -*-C++-*-
#ifndef INCLUDED_BALXML_TYPESPRINTUTIL
#define INCLUDED_BALXML_TYPESPRINTUTIL

#include <bsls_ident.h>
BSLS_IDENT("$Id: $")

//@PURPOSE: Provide a utility for printing types using XML formatting.
//
//@CLASSES:
//   balxml::TypesPrintUtil: utility for printing using XML formatting
//
//@SEE_ALSO: http://www.w3.org/TR/xmlschema-2/
//
//@DESCRIPTION: The `balxml::TypesPrintUtil` `struct` provided by this
// component contains the following functions:
//
//: `print`:
//:   Print an object using the supplied formatting mode.
//:
//: `printBase64`:
//:   Print an object using `bdlat_FormattingMode::e_BASE64`.
//:
//: `printDecimal`:
//:   Print an object using `bdlat_FormattingMode::e_DEC`.
//:
//: `printDefault`:
//:   Print an object using `bdlat_FormattingMode::e_DEFAULT`.
//:
//: `printHex`:
//:   Print an object using `bdlat_FormattingMode::e_HEX`.
//:
//: `printList`:
//:   Print an object using `bdlat_FormattingMode::e_IS_LIST`.
//:
//: `printText`:
//:   Print an object using `bdlat_FormattingMode::e_TEXT`.
//
// The output is generated according to each type's lexical representation as
// described in the XML Schema Specification, which is available at
// `http://www.w3.org/TR/xmlschema-2/`.  The text input is parsed and output
// according to the XML-1.0 with UTF-8 encoding, which prevents control
// characters (accepted by UTF-8) but otherwise accepts valid characters as
// described in the Unicode Standard 4.0, which is available at
// `http://www.unicode.org/versions/Unicode4.0.0/` (well-formed UTF-8 byte
// sequences are described in Chapter 3, Section 3.9 and Table 3.5).
//
// The following C++ Type / Formatting Mode combinations are supported by this
// component:
// ```
//     C++ Type                            Formatting Mode
//     --------                            ---------------
//     bool                                e_DEFAULT, e_DEC, e_TEXT
//     char                                e_DEFAULT, e_DEC, e_TEXT
//     unsigned char                       e_DEFAULT, e_DEC
//     [unsigned] short                    e_DEFAULT, e_DEC
//     [unsigned] int                      e_DEFAULT, e_DEC
//     [unsigned] long                     e_DEFAULT, e_DEC
//     bsls::Types::[Uint64|Int64]         e_DEFAULT, e_DEC
//     float                               e_DEFAULT, e_DEC
//     double                              e_DEFAULT, e_DEC
//     bdldfp::Decimal64                   e_DEFAULT, e_DEC
//     bsl::string                         e_DEFAULT, e_TEXT, e_BASE64, e_HEX
//     bdlt::Date                          e_DEFAULT
//     bdlt::DateTz                        e_DEFAULT
//     bdlt::Datetime                      e_DEFAULT
//     bdlt::DateTimeTz                    e_DEFAULT
//     bdlt::Time                          e_DEFAULT
//     bdlt::TimeTz                        e_DEFAULT
//     bdlb::Variant2<DateTz, Date>        e_DEFAULT
//     bdlb::Variant2<TimeTz, Time>        e_DEFAULT
//     Variant2<DatetimeTz, Datetime>      e_DEFAULT
//     bsl::vector<char>                   e_DEFAULT, e_BASE64, e_HEX, e_TEXT,
//                                         e_IS_LIST
// ```
// In addition to the types listed above, this component also recognizes the
// following `bdlat` type categories:
// ```
//     'bdlat' Type Category               Formatting Mode
//     ---------------------               ---------------
//     Enumeration                         e_DEFAULT, e_TEXT, e_DECIMAL
//     CustomizedType                      Base type's formatting modes
//     Array                               e_IS_LIST
// ```
// When `bdlat_FormattingMode::e_DEFAULT` is used, the actual formatting mode
// selected is based on the following mapping:
// ```
//     C++ Type                            Default Formatting Mode
//     --------                            -----------------------
//     bool                                e_DEC or e_TEXT
//     [unsigned] char                     e_DEC
//     [unsigned] short                    e_DEC
//     [unsigned] int                      e_DEC
//     [unsigned] long                     e_DEC
//     bsls::Types::[Uint64|Int64]         e_DEC
//     bsl::string                         e_TEXT
//     bsl::vector<char>                   e_BASE64
//
//     'bdlat' Type Category               Default Formatting Mode
//     ---------------------               -----------------------
//     Enumeration                         e_TEXT
// ```
//
///Behavior of `printText` on Non-Valid Strings
///--------------------------------------------
// The output of `printText` will always be valid XML 1.0 with UTF-8 encoding.
// When attempting to print text data that contains non-valid UTF-8 characters
// or non-printable control characters using `printText`, this component prints
// the valid characters up to and excluding the first invalid character.  See
// the second example in the `Usage` section for an illustration.
//
///Usage
///-----
// The following snippets of code illustrates how to print an
// `bsl::vector<char>` object in Base64 format:
// ```
// #include <balxml_typesprintutil.h>
//
// #include <cassert>
// #include <sstream>
// #include <vector>
//
// using namespace BloombergLP;
//
// void usageExample1()
// {
//     bsl::ostringstream ss;
//
//     bsl::vector<char> vec;
//     vec.push_back('a');
//     vec.push_back('b');
//     vec.push_back('c');
//     vec.push_back('d');
//
//     const char EXPECTED_RESULT[] = "YWJjZA==";
//
//     balxml::TypesPrintUtil::printBase64(ss, vec);
//     assert(EXPECTED_RESULT == ss.str());
// }
// ```
// The following snippet of shows what can be expected when printing valid or
// invalid data via `printText`:
// ```
// void usageExample2()
// {
//     bsl::ostringstream ss;
//
//     const char VALID_STR[] = "Hello \t 'World'";
// ```
// Note that all characters in the range `0x01` to `0x7F` are valid first bytes
// (including printable ASCII, TAB `0x09`, or LF `0x0a`, but excluding control
// characters other than TAB and LF) and that ampersand (`&` or `0x26`),
// less-than (`<` or `0x3c`), greater-than (`>` or `0x3e`), apostrophe
// (`0x27`), and quote (") will be printed as `&amp;`, `&lt;`, `&gt`,
// `&apos;` and `&quot;` respectively.  Hence the expected output for the above
// string `VALID_STR` is:
// ```
//     const char EXPECTED_RESULT[] = "Hello \t &apos;World&apos;";
// ```
// We can test that `printText` will successfully print the string:
// ```
//     balxml::TypesPrintUtil::printText(ss, VALID_STR);
//     assert(ss.good());
//     assert(EXPECTED_RESULT == ss.str());
// ```
// In addition, when invalid data is printed, the stream is set to a bad state
// which is the proper means for the user to detect an error, as shown in the
// following code snippet:
// ```
//     ss.str("");
//     const char INVALID_STR[]  = "Hello \300\t 'World'";
//     balxml::TypesPrintUtil::printText(ss, INVALID_STR);
//     assert(ss.fail());
//     assert("Hello " == ss.str());
// }
// ```

#include <balscm_version.h>

#include <balxml_encoderoptions.h>

#include <bdlat_arrayfunctions.h>
#include <bdlat_arrayutil.h>
#include <bdlat_customizedtypefunctions.h>
#include <bdlat_enumfunctions.h>
#include <bdlat_formattingmode.h>
#include <bdlat_nullablevalueutil.h>
#include <bdlat_typecategory.h>

#include <bdldfp_decimal.h>

#include <bdlt_date.h>
#include <bdlt_datetime.h>
#include <bdlt_datetimetz.h>
#include <bdlt_datetz.h>
#include <bdlt_iso8601util.h>
#include <bdlt_time.h>
#include <bdlt_timetz.h>

#include <bslmf_enableif.h>
#include <bslmf_isclass.h>
#include <bslmf_isconvertible.h>

#include <bsls_assert.h>
#include <bsls_types.h>

#include <bdlb_float.h>

#include <bsl_iomanip.h>
#include <bsl_istream.h>
#include <bsl_ios.h>
#include <bsl_ostream.h>
#include <bsl_string.h>
#include <bsl_vector.h>

namespace BloombergLP {
namespace balxml {

                           // =====================
                           // struct TypesPrintUtil
                           // =====================

/// This `struct` contains functions for printing objects to output streams
/// using various formatting modes.
struct TypesPrintUtil {

    // CLASS METHODS

    /// Format the specified `object` to the specified output `stream` using
    /// the specified `formattingMode` and the optionally specified
    /// `encoderOptions` and return a reference to `stream`.  The behavior
    /// is undefined unless the parameterized `TYPE` and the
    /// `formattingMode` combination is supported (supported combinations
    /// are listed in the component-level documentation).
    template <class TYPE>
    static bsl::ostream& print(bsl::ostream&         stream,
                               const TYPE&           object,
                               int                   formattingMode,
                               const EncoderOptions *encoderOptions = 0);

    /// Format the specified `object` to the specified output `stream` using
    /// the `bdlat_FormattingMode::e_BASE64` formatting mode and the
    /// optionally specified `encoderOptions`.  Return a reference to
    /// `stream`.
    template <class TYPE>
    static bsl::ostream& printBase64(bsl::ostream&         stream,
                                     const TYPE&           object,
                                     const EncoderOptions *encoderOptions = 0);

    /// Format the specified `object` to the specified output `stream` using
    /// the `bdlat_FormattingMode::e_DEC` formatting mode and the
    /// optionally specified `encoderOptions`.  Return a reference to
    /// `stream`.
    template <class TYPE>
    static bsl::ostream& printDecimal(
                                     bsl::ostream&         stream,
                                     const TYPE&           object,
                                     const EncoderOptions *encoderOptions = 0);

    /// Format the specified `object` to the specified output `stream` using
    /// the `bdlat_FormattingMode::e_DEFAULT` formatting mode and the
    /// optionally specified `encoderOptions`.  Return a reference to
    /// `stream`.
    template <class TYPE>
    static bsl::ostream& printDefault(
                                     bsl::ostream&         stream,
                                     const TYPE&           object,
                                     const EncoderOptions *encoderOptions = 0);

    /// Format the specified `object` to the specified output `stream` using
    /// the `bdlat_FormattingMode::e_HEX` formatting mode and the
    /// optionally specified `encoderOptions`.  Return a reference to
    /// `stream`.
    template <class TYPE>
    static bsl::ostream& printHex(bsl::ostream&         stream,
                                  const TYPE&           object,
                                  const EncoderOptions *encoderOptions = 0);

    /// Format the specified `object` to the specified output `stream` using
    /// the `bdlat_FormattingMode::e_LIST` formatting mode and the
    /// optionally specified `encoderOptions`.  Return a reference to
    /// `stream`.
    template <class TYPE>
    static bsl::ostream& printList(bsl::ostream&         stream,
                                   const TYPE&           object,
                                   const EncoderOptions *encoderOptions = 0);

    /// Format the specified `object` to the specified output `stream` using
    /// the `bdlat_FormattingMode::e_TEXT` formatting mode and the
    /// optionally specified `encoderOptions`.  Return a reference to
    /// `stream`.  The string representation of `object` must be a valid
    /// UTF-8 string and may not contain any control characters other than
    /// TAB, NL, and CR (i.e., no binary data) unless
    /// `encoderOptions->allowControlCharacters()` is `true`, in which
    /// control characters will be encoded as is.  Upon detecting an invalid
    /// byte, the output stops and the `failbit` is be set on the output
    /// `stream`.  In the case of an invalid byte in a multi-byte
    /// character, the output stops after the previous character and no
    /// byte in this character is output.
    template <class TYPE>
    static bsl::ostream& printText(bsl::ostream&         stream,
                                   const TYPE&           object,
                                   const EncoderOptions *encoderOptions = 0);
};

                         // =========================
                         // struct TypesPrintUtil_Imp
                         // =========================

/// This `struct` contains functions that are used in the implementation of
/// this component.
struct TypesPrintUtil_Imp {

    // TYPES

    /// `DateOrDateTz` is a convenient alias for
    /// `bdlb::Variant2<Date, DateTz>`.
    typedef bdlb::Variant2<bdlt::Date, bdlt::DateTz>      DateOrDateTz;

    /// `TimeOrTimeTz` is a convenient alias for
    /// `bdlb::Variant2<Time, TimeTz>`.
    typedef bdlb::Variant2<bdlt::Time, bdlt::TimeTz>      TimeOrTimeTz;

    /// `DatetimeOrDatetimeTz` is a convenient alias for
    /// `bdlb::Variant2<Datetime, DatetimeTz>`.
    typedef bdlb::Variant2<bdlt::Datetime, bdlt::DatetimeTz>
                                                          DatetimeOrDatetimeTz;

    // CLASS METHODS

    /// Encode the specified `value` into XML using ISO 8601 format and
    /// output the result to the specified `stream` using the specified
    /// `encoderOptions`.
    template <class TYPE>
    static bsl::ostream& printDateAndTime(
                                         bsl::ostream&         stream,
                                         const TYPE&           value,
                                         const EncoderOptions *encoderOptions);

                            // BASE64 FUNCTIONS

    template <class TYPE>
    static bsl::ostream& printBase64(
                            bsl::ostream&                       stream,
                            const TYPE&                         object,
                            const EncoderOptions               *encoderOptions,
                            bdlat_TypeCategory::CustomizedType);

    template <class TYPE>
    static bsl::ostream& printBase64(
                               bsl::ostream&                    stream,
                               const TYPE&                      object,
                               const EncoderOptions            *encoderOptions,
                               bdlat_TypeCategory::DynamicType);

    template <class TYPE, class ANY_CATEGORY>
    static bsl::ostream& printBase64(bsl::ostream&         stream,
                                     const TYPE&           object,
                                     const EncoderOptions *encoderOptions,
                                     ANY_CATEGORY);

    static bsl::ostream& printBase64(
                                    bsl::ostream&               stream,
                                    const bsl::string_view&     object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);

    template <class TYPE>
    static typename bsl::enable_if<
        bsl::is_class<TYPE>::value &&
            bsl::is_convertible<TYPE, bsl::string_view>::value,
        bsl::ostream&>::type
    printBase64(bsl::ostream&         stream,
                const TYPE&           object,
                const EncoderOptions *encoderOptions,
                bdlat_TypeCategory::Simple);

    static bsl::ostream& printBase64(bsl::ostream&              stream,
                                     const bsl::vector<char>&   object,
                                     const EncoderOptions      *encoderOptions,
                                     bdlat_TypeCategory::Array);

                            // DECIMAL FUNCTIONS

    template <class TYPE>
    static bsl::ostream& printDecimal(
                               bsl::ostream&                    stream,
                               const TYPE&                      object,
                               const EncoderOptions            *encoderOptions,
                               bdlat_TypeCategory::Enumeration);

    template <class TYPE>
    static bsl::ostream& printDecimal(
                            bsl::ostream&                       stream,
                            const TYPE&                         object,
                            const EncoderOptions               *encoderOptions,
                            bdlat_TypeCategory::CustomizedType);

        template <class TYPE>
    static bsl::ostream& printDecimal(
                               bsl::ostream&                    stream,
                               const TYPE&                      object,
                               const EncoderOptions            *encoderOptions,
                               bdlat_TypeCategory::DynamicType);

    template <class TYPE, class ANY_CATEGORY>
    static bsl::ostream& printDecimal(bsl::ostream&         stream,
                                      const TYPE&           object,
                                      const EncoderOptions *encoderOptions,
                                      ANY_CATEGORY);

    static bsl::ostream& printDecimal(
                                    bsl::ostream&               stream,
                                    const bool&                 object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);
    static bsl::ostream& printDecimal(
                                    bsl::ostream&               stream,
                                    const char&                 object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);
    static bsl::ostream& printDecimal(
                                    bsl::ostream&               stream,
                                    const short&                object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);
    static bsl::ostream& printDecimal(
                                    bsl::ostream&               stream,
                                    const int&                  object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);
    static bsl::ostream& printDecimal(
                                    bsl::ostream&               stream,
                                    const long&                 object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);
    static bsl::ostream& printDecimal(
                                    bsl::ostream&               stream,
                                    const bsls::Types::Int64&   object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);
    static bsl::ostream& printDecimal(
                                    bsl::ostream&               stream,
                                    const unsigned char&        object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);
    static bsl::ostream& printDecimal(
                                    bsl::ostream&               stream,
                                    const unsigned short&       object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);
    static bsl::ostream& printDecimal(
                                    bsl::ostream&               stream,
                                    const unsigned int&         object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);
    static bsl::ostream& printDecimal(
                                    bsl::ostream&               stream,
                                    const unsigned long&        object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);
    static bsl::ostream& printDecimal(
                                    bsl::ostream&               stream,
                                    const bsls::Types::Uint64&  object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);

    static bsl::ostream& printDecimal(
                                    bsl::ostream&               stream,
                                    const float&                object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);

    static bsl::ostream& printDecimal(
                                    bsl::ostream&               stream,
                                    const double&               object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);
    static bsl::ostream& printDecimal(
                                    bsl::ostream&               stream,
                                    const bdldfp::Decimal64&    object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);

                            // DEFAULT FUNCTIONS

    template <class TYPE>
    static bsl::ostream& printDefault(
                               bsl::ostream&                    stream,
                               const TYPE&                      object,
                               const EncoderOptions            *encoderOptions,
                               bdlat_TypeCategory::Enumeration);

    template <class TYPE>
    static bsl::ostream& printDefault(
                            bsl::ostream&                       stream,
                            const TYPE&                         object,
                            const EncoderOptions               *encoderOptions,
                            bdlat_TypeCategory::CustomizedType);

    template <class TYPE>
    static bsl::ostream& printDefault(
                             bsl::ostream&                      stream,
                             const TYPE&                        object,
                             const EncoderOptions              *encoderOptions,
                             bdlat_TypeCategory::DynamicType);

    template <class TYPE>
    static bsl::ostream& printDefault(
                             bsl::ostream&                      stream,
                             const TYPE&                        object,
                             const EncoderOptions              *encoderOptions,
                             bdlat_TypeCategory::NullableValue);

    template <class TYPE, class ANY_CATEGORY>
    static bsl::ostream& printDefault(bsl::ostream&         stream,
                                      const TYPE&           object,
                                      const EncoderOptions *encoderOptions,
                                      ANY_CATEGORY);

    static bsl::ostream& printDefault(
                                    bsl::ostream&               stream,
                                    const bool&                 object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);
    static bsl::ostream& printDefault(
                                    bsl::ostream&               stream,
                                    const char&                 object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);
    static bsl::ostream& printDefault(
                                    bsl::ostream&               stream,
                                    const short&                object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);
    static bsl::ostream& printDefault(
                                    bsl::ostream&               stream,
                                    const int&                  object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);
    static bsl::ostream& printDefault(
                                    bsl::ostream&               stream,
                                    const long&                 object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);
    static bsl::ostream& printDefault(
                                    bsl::ostream&               stream,
                                    const bsls::Types::Int64&   object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);
    static bsl::ostream& printDefault(
                                    bsl::ostream&               stream,
                                    const unsigned char&        object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);
    static bsl::ostream& printDefault(
                                    bsl::ostream&               stream,
                                    const unsigned short&       object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);
    static bsl::ostream& printDefault(
                                    bsl::ostream&               stream,
                                    const unsigned int&         object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);
    static bsl::ostream& printDefault(
                                    bsl::ostream&               stream,
                                    const unsigned long&        object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);
    static bsl::ostream& printDefault(
                                    bsl::ostream&               stream,
                                    const bsls::Types::Uint64&  object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);
    static bsl::ostream& printDefault(
                                    bsl::ostream&               stream,
                                    const float&                object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);
    static bsl::ostream& printDefault(
                                    bsl::ostream&               stream,
                                    const double&               object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);
    static bsl::ostream& printDefault(
                                    bsl::ostream&               stream,
                                    const bdldfp::Decimal64&    object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);
    static bsl::ostream& printDefault(
                                    bsl::ostream&               stream,
                                    const char                 *object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);
    static bsl::ostream& printDefault(
                                    bsl::ostream&               stream,
                                    const bsl::string_view&     object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);

    template <class TYPE>
    static typename bsl::enable_if<
        bsl::is_class<TYPE>::value &&
            bsl::is_convertible<TYPE, bsl::string_view>::value,
        bsl::ostream&>::type
    printDefault(bsl::ostream&         stream,
                const TYPE&           object,
                const EncoderOptions *encoderOptions,
                bdlat_TypeCategory::Simple);

    static bsl::ostream& printDefault(
                                    bsl::ostream&               stream,
                                    const bdlt::Date&           object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);
    static bsl::ostream& printDefault(
                                    bsl::ostream&               stream,
                                    const bdlt::DateTz&         object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);
    static bsl::ostream& printDefault(
                                    bsl::ostream&               stream,
                                    const bdlt::Datetime&       object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);
    static bsl::ostream& printDefault(
                                    bsl::ostream&               stream,
                                    const bdlt::DatetimeTz&     object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);
    static bsl::ostream& printDefault(
                                    bsl::ostream&               stream,
                                    const bdlt::Time&           object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);
    static bsl::ostream& printDefault(
                                    bsl::ostream&               stream,
                                    const bdlt::TimeTz&         object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);
    static bsl::ostream& printDefault(
                                    bsl::ostream&               stream,
                                    const DateOrDateTz&         object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);
    static bsl::ostream& printDefault(
                                    bsl::ostream&               stream,
                                    const TimeOrTimeTz&         object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple);
    static bsl::ostream& printDefault(
                                   bsl::ostream&                stream,
                                   const DatetimeOrDatetimeTz&  object,
                                   const EncoderOptions        *encoderOptions,
                                   bdlat_TypeCategory::Simple);
    static bsl::ostream& printDefault(
                                     bsl::ostream&              stream,
                                     const bsl::vector<char>&   object,
                                     const EncoderOptions      *encoderOptions,
                                     bdlat_TypeCategory::Array);

                            // HEX FUNCTIONS

    template <class TYPE>
    static bsl::ostream& printHex(
                            bsl::ostream&                       stream,
                            const TYPE&                         object,
                            const EncoderOptions               *encoderOptions,
                            bdlat_TypeCategory::CustomizedType);

    template <class TYPE>
    static bsl::ostream& printHex(
                               bsl::ostream&                    stream,
                               const TYPE&                      object,
                               const EncoderOptions            *encoderOptions,
                               bdlat_TypeCategory::DynamicType);

    template <class TYPE, class ANY_CATEGORY>
    static bsl::ostream& printHex(bsl::ostream&         stream,
                                  const TYPE&           object,
                                  const EncoderOptions *encoderOptions,
                                  ANY_CATEGORY);

    static bsl::ostream& printHex(bsl::ostream&               stream,
                                  const bsl::string_view&     object,
                                  const EncoderOptions       *encoderOptions,
                                  bdlat_TypeCategory::Simple);

    template <class TYPE>
    static typename bsl::enable_if<
        bsl::is_class<TYPE>::value &&
            bsl::is_convertible<TYPE, bsl::string_view>::value,
        bsl::ostream&>::type
    printHex(bsl::ostream&         stream,
             const TYPE&           object,
             const EncoderOptions *encoderOptions,
             bdlat_TypeCategory::Simple);

    static bsl::ostream& printHex(bsl::ostream&              stream,
                                  const bsl::vector<char>&   object,
                                  const EncoderOptions      *encoderOptions,
                                  bdlat_TypeCategory::Array);

                               // LIST FUNCTIONS

    template <class TYPE>
    static bsl::ostream& printList(bsl::ostream&              stream,
                                   const TYPE&                object,
                                   const EncoderOptions      *encoderOptions,
                                   bdlat_TypeCategory::Array);

    template <class TYPE>
    static bsl::ostream& printList(
                               bsl::ostream&                    stream,
                               const TYPE&                      object,
                               const EncoderOptions            *encoderOptions,
                               bdlat_TypeCategory::DynamicType);

    template <class TYPE, class ANY_CATEGORY>
    static bsl::ostream& printList(bsl::ostream&         stream,
                                   const TYPE&           object,
                                   const EncoderOptions *encoderOptions,
                                   ANY_CATEGORY);

                            // TEXT FUNCTIONS

    template <class TYPE>
    static bsl::ostream& printText(
                               bsl::ostream&                    stream,
                               const TYPE&                      object,
                               const EncoderOptions            *encoderOptions,
                               bdlat_TypeCategory::Enumeration);

    template <class TYPE>
    static bsl::ostream& printText(
                            bsl::ostream&                       stream,
                            const TYPE&                         object,
                            const EncoderOptions               *encoderOptions,
                            bdlat_TypeCategory::CustomizedType);

    template <class TYPE>
    static bsl::ostream& printText(
                               bsl::ostream&                    stream,
                               const TYPE&                      object,
                               const EncoderOptions            *encoderOptions,
                               bdlat_TypeCategory::DynamicType);

    template <class TYPE, class ANY_CATEGORY>
    static bsl::ostream& printText(bsl::ostream&         stream,
                                   const TYPE&           object,
                                   const EncoderOptions *encoderOptions,
                                   ANY_CATEGORY);

    static bsl::ostream& printText(bsl::ostream&               stream,
                                   const bool&                 object,
                                   const EncoderOptions       *encoderOptions,
                                   bdlat_TypeCategory::Simple);
    static bsl::ostream& printText(bsl::ostream&               stream,
                                   const char&                 object,
                                   const EncoderOptions       *encoderOptions,
                                   bdlat_TypeCategory::Simple);
    static bsl::ostream& printText(bsl::ostream&               stream,
                                   const char                 *object,
                                   const EncoderOptions       *encoderOptions,
                                   bdlat_TypeCategory::Simple);
    static bsl::ostream& printText(bsl::ostream&               stream,
                                   const bsl::string_view&     object,
                                   const EncoderOptions       *encoderOptions,
                                   bdlat_TypeCategory::Simple);

    template <class TYPE>
    static typename bsl::enable_if<
        bsl::is_class<TYPE>::value &&
            bsl::is_convertible<TYPE, bsl::string_view>::value,
        bsl::ostream&>::type
    printText(bsl::ostream&         stream,
              const TYPE&           object,
              const EncoderOptions *encoderOptions,
              bdlat_TypeCategory::Simple);

    static bsl::ostream& printText(bsl::ostream&              stream,
                                   const bsl::vector<char>&   object,
                                   const EncoderOptions      *encoderOptions,
                                   bdlat_TypeCategory::Array);
};

// ============================================================================
//                               PROXY CLASSES
// ============================================================================

                    // ===================================
                    // class TypesPrintUtilImp_PrintBase64
                    // ===================================

/// Component-private struct.  Do not use.
class TypesPrintUtilImp_PrintBase64 {

    // DATA
    bsl::ostream         *d_stream_p;
    const EncoderOptions *d_encoderOptions_p;

  public:
    // CREATORS
    TypesPrintUtilImp_PrintBase64(bsl::ostream         *stream,
                                  const EncoderOptions *encoderOptions)
    : d_stream_p(stream)
    , d_encoderOptions_p(encoderOptions)
    {
    }

    // ACCESSORS
    template <class TYPE, class ANY_CATEGORY>
    int operator()(const TYPE& object, ANY_CATEGORY category) const
    {
        TypesPrintUtil_Imp::printBase64(*d_stream_p,
                                        object,
                                        d_encoderOptions_p,
                                        category);
        return static_cast<bool>(*d_stream_p) ? 0 : -1;
    }

    template <class TYPE>
    int operator()(const TYPE&, bslmf::Nil) const
    {
        BSLS_ASSERT_SAFE(0);

        return -1;
    }
};

                    // ====================================
                    // class TypesPrintUtilImp_PrintDecimal
                    // ====================================

/// Component-private class.  Do not use.
class TypesPrintUtilImp_PrintDecimal {

    // DATA
    bsl::ostream         *d_stream_p;
    const EncoderOptions *d_encoderOptions_p;

  public:
    // CREATORS
    TypesPrintUtilImp_PrintDecimal(bsl::ostream         *stream,
                                   const EncoderOptions *encoderOptions)
    : d_stream_p(stream)
    , d_encoderOptions_p(encoderOptions)
    {
    }

    // ACCESSORS
    template <class TYPE, class ANY_CATEGORY>
    int operator()(const TYPE& object, ANY_CATEGORY category) const
    {
        TypesPrintUtil_Imp::printDecimal(*d_stream_p,
                                         object,
                                         d_encoderOptions_p,
                                         category);
        return static_cast<bool>(*d_stream_p) ? 0 : -1;
    }

    template <class TYPE>
    int operator()(const TYPE&, bslmf::Nil) const
    {
        BSLS_ASSERT_SAFE(0);

        return -1;
    }
};

                    // ====================================
                    // class TypesPrintUtilImp_PrintDefault
                    // ====================================

/// Component-private class.  Do not use.
class TypesPrintUtilImp_PrintDefault {

    // DATA
    bsl::ostream         *d_stream_p;
    const EncoderOptions *d_encoderOptions_p;

  public:
    // CREATORS
    TypesPrintUtilImp_PrintDefault(bsl::ostream         *stream,
                                   const EncoderOptions *encoderOptions)
    : d_stream_p(stream)
    , d_encoderOptions_p(encoderOptions)
    {
    }

    // ACCESSORS
    template <class TYPE, class ANY_CATEGORY>
    int operator()(const TYPE& object, ANY_CATEGORY category) const
    {
        TypesPrintUtil_Imp::printDefault(*d_stream_p,
                                         object,
                                         d_encoderOptions_p,
                                         category);
        return static_cast<bool>(*d_stream_p) ? 0 : -1;
    }

    template <class TYPE>
    int operator()(const TYPE&, bslmf::Nil) const
    {
        BSLS_ASSERT_SAFE(0);

        return -1;
    }
};

                      // ================================
                      // class TypesPrintUtilImp_PrintHex
                      // ================================

/// Component-private class.  Do not use.
class TypesPrintUtilImp_PrintHex {

    // DATA
    bsl::ostream         *d_stream_p;
    const EncoderOptions *d_encoderOptions_p;

  public:
    // CREATORS
    TypesPrintUtilImp_PrintHex(bsl::ostream         *stream,
                               const EncoderOptions *encoderOptions)
    : d_stream_p(stream)
    , d_encoderOptions_p(encoderOptions)
    {
    }

    // ACCESSORS
    template <class TYPE, class ANY_CATEGORY>
    int operator()(const TYPE& object, ANY_CATEGORY category) const
    {
        TypesPrintUtil_Imp::printHex(*d_stream_p,
                                     object,
                                     d_encoderOptions_p,
                                     category);
        return static_cast<bool>(*d_stream_p) ? 0 : -1;
    }

    template <class TYPE>
    int operator()(const TYPE&, bslmf::Nil) const
    {
        BSLS_ASSERT_SAFE(0);

        return -1;
    }
};

                     // =================================
                     // class TypesPrintUtilImp_PrintList
                     // =================================

/// Component-private class.  Do not use.
class TypesPrintUtilImp_PrintList {

    // DATA
    bsl::ostream         *d_stream_p;
    const EncoderOptions *d_encoderOptions_p;

  public:
    // CREATORS
    TypesPrintUtilImp_PrintList(bsl::ostream         *stream,
                                const EncoderOptions *encoderOptions)
    : d_stream_p(stream)
    , d_encoderOptions_p(encoderOptions)
    {
    }

    // ACCESSORS
    template <class TYPE, class ANY_CATEGORY>
    int operator()(const TYPE& object, ANY_CATEGORY category) const
    {
        TypesPrintUtil_Imp::printList(*d_stream_p,
                                      object,
                                      d_encoderOptions_p,
                                      category);
        return static_cast<bool>(*d_stream_p) ? 0 : -1;
    }

    template <class TYPE>
    int operator()(const TYPE&, bslmf::Nil) const
    {
        BSLS_ASSERT_SAFE(0);

        return -1;
    }
};

              // ===============================================
              // class TypesPrintUtilImp_PrintListElementDefault
              // ===============================================

/// Component-private class.  Do not use.
class TypesPrintUtilImp_PrintListElementDefault {

    // DATA
    bool                  d_doesNextElemNeedDelimiter;
    bsl::ostream         *d_stream_p;
    const EncoderOptions *d_encoderOptions_p;

  public:
    // CREATORS
    TypesPrintUtilImp_PrintListElementDefault(
                                          bsl::ostream         *stream,
                                          const EncoderOptions *encoderOptions)
    : d_doesNextElemNeedDelimiter(false)
    , d_stream_p(stream)
    , d_encoderOptions_p(encoderOptions)
    {
    }

    // MANIPULATORS
    template <class TYPE>
    int operator()(const TYPE& object, bdlat_TypeCategory::NullableValue)
    {
        if (bdlat_NullableValueFunctions::isNull(object)) {
            return static_cast<bool>(*d_stream_p) ? 0 : -1;           // RETURN
        }

        if (d_doesNextElemNeedDelimiter) {
            *d_stream_p << " ";
        }

        d_doesNextElemNeedDelimiter = true;
        TypesPrintUtilImp_PrintDefault printDefault(d_stream_p,
                                                    d_encoderOptions_p);
        return bdlat::NullableValueUtil::accessValueByCategory(object,
                                                               printDefault);
    }

    template <class TYPE, class OTHER_CATEGORY>
    int operator()(const TYPE& object, OTHER_CATEGORY category)
    {
        if (d_doesNextElemNeedDelimiter) {
            *d_stream_p << " ";
        }

        d_doesNextElemNeedDelimiter = true;
        TypesPrintUtil_Imp::printDefault(*d_stream_p,
                                         object,
                                         d_encoderOptions_p,
                                         category);
        return static_cast<bool>(*d_stream_p) ? 0 : -1;
    }

    template <class TYPE>
    int operator()(const TYPE&, bslmf::Nil)
    {
        BSLS_ASSERT_SAFE(0);

        return -1;
    }
};

                     // =================================
                     // class TypesPrintUtilImp_PrintText
                     // =================================

/// Component-private class.  Do not use.
class TypesPrintUtilImp_PrintText {

    // DATA
    bsl::ostream         *d_stream_p;
    const EncoderOptions *d_encoderOptions_p;

  public:
    // CREATORS
    TypesPrintUtilImp_PrintText(bsl::ostream         *stream,
                                const EncoderOptions *encoderOptions)
    : d_stream_p(stream)
    , d_encoderOptions_p(encoderOptions)
    {
    }

    // ACCESSORS
    template <class TYPE, class ANY_CATEGORY>
    int operator()(const TYPE& object, ANY_CATEGORY category) const
    {
        TypesPrintUtil_Imp::printText(*d_stream_p,
                                      object,
                                      d_encoderOptions_p,
                                      category);
        return static_cast<bool>(*d_stream_p) ? 0 : -1;
    }

    template <class TYPE>
    int operator()(const TYPE&, bslmf::Nil) const
    {
        BSLS_ASSERT_SAFE(0);

        return -1;
    }
};

// ============================================================================
//                            INLINE DEFINITIONS
// ============================================================================

                           // ---------------------
                           // struct TypesPrintUtil
                           // ---------------------

template <class TYPE>
bsl::ostream& TypesPrintUtil::print(bsl::ostream&         stream,
                                    const TYPE&           object,
                                    int                   formattingMode,
                                    const EncoderOptions *encoderOptions)
{
    typedef typename bdlat_TypeCategory::Select<TYPE>::Type Tag;

    if (formattingMode & bdlat_FormattingMode::e_LIST) {
        return TypesPrintUtil::printList(stream, object, encoderOptions);
    }

    switch (formattingMode & bdlat_FormattingMode::e_TYPE_MASK) {
      case bdlat_FormattingMode::e_BASE64: {
        return TypesPrintUtil_Imp::printBase64(
            stream, object, encoderOptions, Tag());
      } break;
      case bdlat_FormattingMode::e_DEC: {
        return TypesPrintUtil_Imp::printDecimal(
            stream, object, encoderOptions, Tag());
      } break;
      case bdlat_FormattingMode::e_DEFAULT: {
        return TypesPrintUtil_Imp::printDefault(
            stream, object, encoderOptions, Tag());
      } break;
      case bdlat_FormattingMode::e_HEX: {
        return TypesPrintUtil_Imp::printHex(
            stream, object, encoderOptions, Tag());
      } break;
      case bdlat_FormattingMode::e_TEXT: {
        return TypesPrintUtil_Imp::printText(
            stream, object, encoderOptions, Tag());
      } break;
      default: {
        BSLS_ASSERT_SAFE(0 == "Unsupported operation!");
        stream.setstate(bsl::ios_base::failbit);
        return stream;                                                // RETURN
      } break;
    }

    return stream;
}

template <class TYPE>
inline
bsl::ostream& TypesPrintUtil::printBase64(bsl::ostream&         stream,
                                          const TYPE&           object,
                                          const EncoderOptions *encoderOptions)
{
    typedef typename bdlat_TypeCategory::Select<TYPE>::Type Tag;
    return TypesPrintUtil_Imp::printBase64(
        stream, object, encoderOptions, Tag());
}

template <class TYPE>
inline
bsl::ostream& TypesPrintUtil::printDecimal(
                                          bsl::ostream&         stream,
                                          const TYPE&           object,
                                          const EncoderOptions *encoderOptions)
{

    typedef typename bdlat_TypeCategory::Select<TYPE>::Type Tag;
    return TypesPrintUtil_Imp::printDecimal(
        stream, object, encoderOptions, Tag());
}

template <class TYPE>
inline
bsl::ostream& TypesPrintUtil::printDefault(
                                          bsl::ostream&         stream,
                                          const TYPE&           object,
                                          const EncoderOptions *encoderOptions)
{
    typedef typename bdlat_TypeCategory::Select<TYPE>::Type Tag;
    return TypesPrintUtil_Imp::printDefault(
        stream, object, encoderOptions, Tag());
}

template <class TYPE>
inline
bsl::ostream& TypesPrintUtil::printHex(bsl::ostream&         stream,
                                       const TYPE&           object,
                                       const EncoderOptions *encoderOptions)
{

    typedef typename bdlat_TypeCategory::Select<TYPE>::Type Tag;
    return TypesPrintUtil_Imp::printHex(
        stream, object, encoderOptions, Tag());
}

template <class TYPE>
inline
bsl::ostream& TypesPrintUtil::printList(bsl::ostream&         stream,
                                        const TYPE&           object,
                                        const EncoderOptions *encoderOptions)
{
    typedef typename bdlat_TypeCategory::Select<TYPE>::Type Tag;
    return TypesPrintUtil_Imp::printList(
        stream, object, encoderOptions, Tag());
}

template <class TYPE>
inline
bsl::ostream& TypesPrintUtil::printText(bsl::ostream&         stream,
                                        const TYPE&           object,
                                        const EncoderOptions *encoderOptions)
{

    typedef typename bdlat_TypeCategory::Select<TYPE>::Type Tag;
    return TypesPrintUtil_Imp::printText(
        stream, object, encoderOptions, Tag());
}

                         // -------------------------
                         // struct TypesPrintUtil_Imp
                         // -------------------------

// BASE64 FUNCTIONS
template <typename TYPE>
inline
bsl::ostream& TypesPrintUtil_Imp::printBase64(
                               bsl::ostream&                    stream,
                               const TYPE&                      object,
                               const EncoderOptions            *encoderOptions,
                               bdlat_TypeCategory::CustomizedType)
{
    return TypesPrintUtil::printBase64(
                      stream,
                      bdlat_CustomizedTypeFunctions::convertToBaseType(object),
                      encoderOptions);
}

template <typename TYPE>
inline
bsl::ostream& TypesPrintUtil_Imp::printBase64(
                               bsl::ostream&                    stream,
                               const TYPE&                      object,
                               const EncoderOptions            *encoderOptions,
                               bdlat_TypeCategory::DynamicType)
{
    const TypesPrintUtilImp_PrintBase64 printBase64Cb(&stream, encoderOptions);
    bdlat_TypeCategoryUtil::accessByCategory(object, printBase64Cb);
    return stream;
}

template <typename TYPE>
inline
typename bsl::enable_if<bsl::is_class<TYPE>::value &&
                            bsl::is_convertible<TYPE, bsl::string_view>::value,
                        bsl::ostream&>::type
TypesPrintUtil_Imp::printBase64(bsl::ostream&         stream,
                                 const TYPE&           object,
                                 const EncoderOptions *encoderOptions,
                                 bdlat_TypeCategory::Simple)
{
    return printBase64(stream,
                       static_cast<bsl::string_view>(object),
                       encoderOptions,
                       bdlat_TypeCategory::Simple());
}

template <class TYPE, class ANY_CATEGORY>
inline
bsl::ostream& TypesPrintUtil_Imp::printBase64(bsl::ostream&         stream,
                                              const TYPE&,
                                              const EncoderOptions *,
                                              ANY_CATEGORY)
{
    BSLS_ASSERT_SAFE(0 == "Unsupported operation!");

    stream.setstate(bsl::ios_base::failbit);

    // Note: 'printBase64' for 'bsl::string' and 'bsl::vector<char>' is inside
    //       the CPP file.

    return stream;
}

// DECIMAL FUNCTIONS

template <typename TYPE>
inline
bsl::ostream& TypesPrintUtil_Imp::printDecimal(
                               bsl::ostream&                    stream,
                               const TYPE&                      object,
                               const EncoderOptions            *encoderOptions,
                               bdlat_TypeCategory::DynamicType)
{
    const TypesPrintUtilImp_PrintDecimal printDecimalCb(&stream,
                                                        encoderOptions);
    bdlat_TypeCategoryUtil::accessByCategory(object, printDecimalCb);
    return stream;
}

template <class TYPE>
inline
bsl::ostream& TypesPrintUtil_Imp::printDecimal(
                               bsl::ostream&                    stream,
                               const TYPE&                      object,
                               const EncoderOptions            *encoderOptions,
                               bdlat_TypeCategory::Enumeration)
{
    int intValue;

    bdlat_EnumFunctions::toInt(&intValue, object);

    return TypesPrintUtil::printDecimal(stream,
                                        intValue,
                                        encoderOptions);
}

template <class TYPE>
inline
bsl::ostream& TypesPrintUtil_Imp::printDecimal(
                            bsl::ostream&                       stream,
                            const TYPE&                         object,
                            const EncoderOptions               *encoderOptions,
                            bdlat_TypeCategory::CustomizedType)
{
    return TypesPrintUtil::printDecimal(
                      stream,
                      bdlat_CustomizedTypeFunctions::convertToBaseType(object),
                      encoderOptions);
}

template <class TYPE, class ANY_CATEGORY>
inline
bsl::ostream& TypesPrintUtil_Imp::printDecimal(bsl::ostream&         stream,
                                               const TYPE&,
                                               const EncoderOptions *,
                                               ANY_CATEGORY)
{
    BSLS_ASSERT_SAFE(0 == "Unsupported operation!");

    stream.setstate(bsl::ios_base::failbit);

    return stream;
}

inline
bsl::ostream& TypesPrintUtil_Imp::printDecimal(
                                            bsl::ostream&               stream,
                                            const bool&                 object,
                                            const EncoderOptions       *,
                                            bdlat_TypeCategory::Simple)
{
    return stream << (object ? 1 : 0);
}

inline
bsl::ostream& TypesPrintUtil_Imp::printDecimal(
                                            bsl::ostream&               stream,
                                            const char&                 object,
                                            const EncoderOptions       *,
                                            bdlat_TypeCategory::Simple)
{
    signed char temp(object);  // Note that 'char' is unsigned on IBM.

    return stream << int(temp);
}

inline
bsl::ostream& TypesPrintUtil_Imp::printDecimal(
                                            bsl::ostream&               stream,
                                            const short&                object,
                                            const EncoderOptions       *,
                                            bdlat_TypeCategory::Simple)
{
    return stream << object;
}

inline
bsl::ostream& TypesPrintUtil_Imp::printDecimal(
                                            bsl::ostream&               stream,
                                            const int&                  object,
                                            const EncoderOptions       *,
                                            bdlat_TypeCategory::Simple)
{
    return stream << object;
}

inline
bsl::ostream& TypesPrintUtil_Imp::printDecimal(
                                            bsl::ostream&               stream,
                                            const long&                 object,
                                            const EncoderOptions       *,
                                            bdlat_TypeCategory::Simple)
{
    return stream << object;
}

inline
bsl::ostream& TypesPrintUtil_Imp::printDecimal(
                                            bsl::ostream&               stream,
                                            const bsls::Types::Int64&   object,
                                            const EncoderOptions       *,
                                            bdlat_TypeCategory::Simple)
{
    return stream << object;
}

inline
bsl::ostream& TypesPrintUtil_Imp::printDecimal(
                                            bsl::ostream&               stream,
                                            const unsigned char&        object,
                                            const EncoderOptions       *,
                                            bdlat_TypeCategory::Simple)
{
    unsigned short us = object;
    return stream << us;
}

inline
bsl::ostream& TypesPrintUtil_Imp::printDecimal(
                                            bsl::ostream&               stream,
                                            const unsigned short&       object,
                                            const EncoderOptions       *,
                                            bdlat_TypeCategory::Simple)
{
    return stream << object;
}

inline
bsl::ostream& TypesPrintUtil_Imp::printDecimal(
                                            bsl::ostream&               stream,
                                            const unsigned int&         object,
                                            const EncoderOptions       *,
                                            bdlat_TypeCategory::Simple)
{
    return stream << object;
}

inline
bsl::ostream& TypesPrintUtil_Imp::printDecimal(
                                            bsl::ostream&               stream,
                                            const unsigned long&        object,
                                            const EncoderOptions       *,
                                            bdlat_TypeCategory::Simple)
{
    return stream << object;
}

inline
bsl::ostream& TypesPrintUtil_Imp::printDecimal(
                                            bsl::ostream&               stream,
                                            const bsls::Types::Uint64&  object,
                                            const EncoderOptions       *,
                                            bdlat_TypeCategory::Simple)
{
    return stream << object;
}

// DEFAULT FUNCTIONS

template <typename TYPE>
inline
bsl::ostream& TypesPrintUtil_Imp::printDefault(
                               bsl::ostream&                    stream,
                               const TYPE&                      object,
                               const EncoderOptions            *encoderOptions,
                               bdlat_TypeCategory::DynamicType)
{
    const TypesPrintUtilImp_PrintDefault printDefaultCb(&stream,
                                                        encoderOptions);
    bdlat_TypeCategoryUtil::accessByCategory(object, printDefaultCb);
    return stream;
}

template <class TYPE>
inline
bsl::ostream& TypesPrintUtil_Imp::printDefault(
                               bsl::ostream&                    stream,
                               const TYPE&                      object,
                               const EncoderOptions            *encoderOptions,
                               bdlat_TypeCategory::Enumeration)
{
    bsl::string stringVal;

    bdlat_EnumFunctions::toString(&stringVal, object);

    return TypesPrintUtil::printText(stream, stringVal, encoderOptions);
}

template <class TYPE>
inline
bsl::ostream& TypesPrintUtil_Imp::printDefault(
                            bsl::ostream&                       stream,
                            const TYPE&                         object,
                            const EncoderOptions               *encoderOptions,
                            bdlat_TypeCategory::CustomizedType)
{
    return TypesPrintUtil::printDefault(
                      stream,
                      bdlat_CustomizedTypeFunctions::convertToBaseType(object),
                      encoderOptions);
}

template <class TYPE>
inline
bsl::ostream& TypesPrintUtil_Imp::printDefault(
                             bsl::ostream&                      stream,
                             const TYPE&                        object,
                             const EncoderOptions              *encoderOptions,
                             bdlat_TypeCategory::NullableValue)
{
    if (bdlat_NullableValueFunctions::isNull(object)) {
        return stream;                                                // RETURN
    }

    TypesPrintUtilImp_PrintDefault printDefault(&stream, encoderOptions);
    bdlat::NullableValueUtil::accessValueByCategory(object, printDefault);
    return stream;
}

template <class TYPE, class ANY_CATEGORY>
inline
bsl::ostream& TypesPrintUtil_Imp::printDefault(bsl::ostream&         stream,
                                               const TYPE&,
                                               const EncoderOptions *,
                                               ANY_CATEGORY)
{
    BSLS_ASSERT_SAFE(0 == "Unsupported operation!");

    stream.setstate(bsl::ios_base::failbit);

    return stream;
}

inline
bsl::ostream& TypesPrintUtil_Imp::printDefault(
                                    bsl::ostream&               stream,
                                    const bool&                 object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple)
{
    return printText(stream,
                     object,
                     encoderOptions,
                     bdlat_TypeCategory::Simple());
}

inline
bsl::ostream& TypesPrintUtil_Imp::printDefault(
                                    bsl::ostream&               stream,
                                    const char&                 object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple)
{
    return printDecimal(stream,
                        object,
                        encoderOptions,
                        bdlat_TypeCategory::Simple());
}

inline
bsl::ostream& TypesPrintUtil_Imp::printDefault(
                                    bsl::ostream&               stream,
                                    const short&                object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple)
{
    return printDecimal(stream,
                        object,
                        encoderOptions,
                        bdlat_TypeCategory::Simple());
}

inline
bsl::ostream& TypesPrintUtil_Imp::printDefault(
                                    bsl::ostream&               stream,
                                    const int&                  object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple)
{
    return printDecimal(stream,
                        object,
                        encoderOptions,
                        bdlat_TypeCategory::Simple());
}

inline
bsl::ostream& TypesPrintUtil_Imp::printDefault(
                                    bsl::ostream&               stream,
                                    const long&                 object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple)
{
    return printDecimal(stream,
                        object,
                        encoderOptions,
                        bdlat_TypeCategory::Simple());
}

inline
bsl::ostream& TypesPrintUtil_Imp::printDefault(
                                    bsl::ostream&               stream,
                                    const bsls::Types::Int64&   object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple)
{
    return printDecimal(stream,
                        object,
                        encoderOptions,
                        bdlat_TypeCategory::Simple());
}

inline
bsl::ostream& TypesPrintUtil_Imp::printDefault(
                                    bsl::ostream&               stream,
                                    const unsigned char&        object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple)
{
    return printDecimal(stream,
                        object,
                        encoderOptions,
                        bdlat_TypeCategory::Simple());
}

inline
bsl::ostream& TypesPrintUtil_Imp::printDefault(
                                    bsl::ostream&               stream,
                                    const unsigned short&       object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple)
{
    return printDecimal(stream,
                        object,
                        encoderOptions,
                        bdlat_TypeCategory::Simple());
}

inline
bsl::ostream& TypesPrintUtil_Imp::printDefault(
                                    bsl::ostream&               stream,
                                    const unsigned int&         object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple)
{
    return printDecimal(stream,
                        object,
                        encoderOptions,
                        bdlat_TypeCategory::Simple());
}

inline
bsl::ostream& TypesPrintUtil_Imp::printDefault(
                                    bsl::ostream&               stream,
                                    const unsigned long&        object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple)
{
    return printDecimal(stream,
                        object,
                        encoderOptions,
                        bdlat_TypeCategory::Simple());
}

inline
bsl::ostream& TypesPrintUtil_Imp::printDefault(
                                    bsl::ostream&               stream,
                                    const bsls::Types::Uint64&  object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple)
{
    return printDecimal(stream,
                        object,
                        encoderOptions,
                        bdlat_TypeCategory::Simple());
}

inline
bsl::ostream& TypesPrintUtil_Imp::printDefault(
                                    bsl::ostream&               stream,
                                    const char                 *object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple)
{
    return printText(stream,
                     object,
                     encoderOptions,
                     bdlat_TypeCategory::Simple());
}

inline
bsl::ostream& TypesPrintUtil_Imp::printDefault(
                                    bsl::ostream&               stream,
                                    const bsl::string_view&     object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple)
{
    return printText(stream,
                     object,
                     encoderOptions,
                     bdlat_TypeCategory::Simple());
}

template <class TYPE>
typename bsl::enable_if<bsl::is_class<TYPE>::value &&
                            bsl::is_convertible<TYPE, bsl::string_view>::value,
                        bsl::ostream&>::type
TypesPrintUtil_Imp::printDefault(bsl::ostream&         stream,
                                  const TYPE&           object,
                                  const EncoderOptions *encoderOptions,
                                  bdlat_TypeCategory::Simple)
{
    return printDefault(stream,
                        static_cast<const bsl::string_view&>(object),
                        encoderOptions,
                        bdlat_TypeCategory::Simple());
}

template <class TYPE>
inline
bsl::ostream& TypesPrintUtil_Imp::printDateAndTime(
                                          bsl::ostream&         stream,
                                          const TYPE&           value,
                                          const EncoderOptions *encoderOptions)
{
    bdlt::Iso8601UtilConfiguration config;
    if (encoderOptions) {
        config.setFractionalSecondPrecision(
                          encoderOptions->datetimeFractionalSecondPrecision());
        config.setUseZAbbreviationForUtc(
                                     encoderOptions->useZAbbreviationForUtc());
    }
    else {
        config.setFractionalSecondPrecision(6);
        config.setUseZAbbreviationForUtc(false);
    }
    return bdlt::Iso8601Util::generate(stream, value, config);
}


inline
bsl::ostream& TypesPrintUtil_Imp::printDefault(
                                    bsl::ostream&               stream,
                                    const bdlt::Date&           object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple)
{
    return printDateAndTime(stream, object, encoderOptions);
}

inline
bsl::ostream& TypesPrintUtil_Imp::printDefault(
                                    bsl::ostream&               stream,
                                    const bdlt::DateTz&         object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple)
{
    return printDateAndTime(stream, object, encoderOptions);
}

inline
bsl::ostream& TypesPrintUtil_Imp::printDefault(
                                         bsl::ostream&          stream,
                                         const bdlt::Datetime&  object,
                                         const EncoderOptions  *encoderOptions,
                                         bdlat_TypeCategory::Simple)
{
    return printDateAndTime(stream, object, encoderOptions);
}

inline
bsl::ostream& TypesPrintUtil_Imp::printDefault(
                                       bsl::ostream&            stream,
                                       const bdlt::DatetimeTz&  object,
                                       const EncoderOptions    *encoderOptions,
                                       bdlat_TypeCategory::Simple)
{
    return printDateAndTime(stream, object, encoderOptions);
}

inline
bsl::ostream& TypesPrintUtil_Imp::printDefault(
                                          bsl::ostream&         stream,
                                          const bdlt::Time&     object,
                                          const EncoderOptions *encoderOptions,
                                          bdlat_TypeCategory::Simple)
{
    return printDateAndTime(stream, object, encoderOptions);
}

inline
bsl::ostream& TypesPrintUtil_Imp::printDefault(
                                    bsl::ostream&               stream,
                                    const bdlt::TimeTz&         object,
                                    const EncoderOptions       *encoderOptions,
                                    bdlat_TypeCategory::Simple)
{
    return printDateAndTime(stream, object, encoderOptions);
}

inline
bsl::ostream& TypesPrintUtil_Imp::printDefault(
                                       bsl::ostream&            stream,
                                       const DateOrDateTz&      object,
                                       const EncoderOptions    *encoderOptions,
                                       bdlat_TypeCategory::Simple)
{
    return printDateAndTime(stream, object, encoderOptions);
}

inline
bsl::ostream& TypesPrintUtil_Imp::printDefault(
                                          bsl::ostream&         stream,
                                          const TimeOrTimeTz&     object,
                                          const EncoderOptions *encoderOptions,
                                          bdlat_TypeCategory::Simple)
{
    return printDateAndTime(stream, object, encoderOptions);
}

inline
bsl::ostream& TypesPrintUtil_Imp::printDefault(
                                   bsl::ostream&                stream,
                                   const DatetimeOrDatetimeTz&  object,
                                   const EncoderOptions        *encoderOptions,
                                   bdlat_TypeCategory::Simple)
{
    return printDateAndTime(stream, object, encoderOptions);
}

inline
bsl::ostream& TypesPrintUtil_Imp::printDefault(
                                     bsl::ostream&              stream,
                                     const bsl::vector<char>&   object,
                                     const EncoderOptions      *encoderOptions,
                                     bdlat_TypeCategory::Array)
{
    return printBase64(stream,
                       object,
                       encoderOptions,
                       bdlat_TypeCategory::Array());
}

// HEX FUNCTIONS
template <typename TYPE>
inline
bsl::ostream& TypesPrintUtil_Imp::printHex(
                            bsl::ostream&                       stream,
                            const TYPE&                         object,
                            const EncoderOptions               *encoderOptions,
                            bdlat_TypeCategory::CustomizedType)
{
    return TypesPrintUtil::printHex(
                      stream,
                      bdlat_CustomizedTypeFunctions::convertToBaseType(object),
                      encoderOptions);
}

template <typename TYPE>
inline
bsl::ostream& TypesPrintUtil_Imp::printHex(
                               bsl::ostream&                    stream,
                               const TYPE&                      object,
                               const EncoderOptions            *encoderOptions,
                               bdlat_TypeCategory::DynamicType)
{
    const TypesPrintUtilImp_PrintHex printHexCb(&stream, encoderOptions);
    bdlat_TypeCategoryUtil::accessByCategory(object, printHexCb);
    return stream;
}

template <class TYPE, class ANY_CATEGORY>
inline
bsl::ostream& TypesPrintUtil_Imp::printHex(bsl::ostream&         stream,
                                           const TYPE&,
                                           const EncoderOptions *,
                                           ANY_CATEGORY)
{
    BSLS_ASSERT_SAFE(0 == "Unsupported operation!");

    stream.setstate(bsl::ios_base::failbit);

    // Note: 'printHex' for 'bsl::string' and 'bsl::vector<char>' is inside the
    //       CPP file.

    return stream;
}

template <class TYPE>
inline
typename bsl::enable_if<bsl::is_class<TYPE>::value &&
                            bsl::is_convertible<TYPE, bsl::string_view>::value,
                        bsl::ostream&>::type
TypesPrintUtil_Imp::printHex(bsl::ostream&         stream,
                             const TYPE&           object,
                             const EncoderOptions *encoderOptions,
                             bdlat_TypeCategory::Simple)
{
    return printHex(stream,
                    static_cast<const bsl::string_view&>(object),
                    encoderOptions,
                    bdlat_TypeCategory::Simple());
}

// LIST FUNCTIONS

template <typename TYPE>
inline
bsl::ostream& TypesPrintUtil_Imp::printList(
                               bsl::ostream&                    stream,
                               const TYPE&                      object,
                               const EncoderOptions            *encoderOptions,
                               bdlat_TypeCategory::DynamicType)
{
    const TypesPrintUtilImp_PrintList printListCb(&stream, encoderOptions);
    bdlat_TypeCategoryUtil::accessByCategory(object, printListCb);
    return stream;
}

template <class TYPE>
bsl::ostream& TypesPrintUtil_Imp::printList(
                                     bsl::ostream&              stream,
                                     const TYPE&                object,
                                     const EncoderOptions      *encoderOptions,
                                     bdlat_TypeCategory::Array)
{
    const bsl::size_t size = bdlat_ArrayFunctions::size(object);

    if (0 == size) {
        return stream;                                                // RETURN
    }

    TypesPrintUtilImp_PrintListElementDefault printElement(&stream,
                                                            encoderOptions);
    bdlat::ArrayUtil::accessElementByCategory(object, printElement, 0);

    for (bsl::size_t idx = 1; idx != size; ++idx) {
        bdlat::ArrayUtil::accessElementByCategory(
            object, printElement, static_cast<int>(idx));
    }

    return stream;
}

template <class TYPE, class ANY_CATEGORY>
inline
bsl::ostream& TypesPrintUtil_Imp::printList(bsl::ostream&         stream,
                                            const TYPE&,
                                            const EncoderOptions *,
                                            ANY_CATEGORY)
{
    BSLS_ASSERT_SAFE(0 == "Unsupported operation!");

    stream.setstate(bsl::ios_base::failbit);

    return stream;
}

// TEXT FUNCTIONS

template <typename TYPE>
inline
bsl::ostream& TypesPrintUtil_Imp::printText(
                               bsl::ostream&                    stream,
                               const TYPE&                      object,
                               const EncoderOptions            *encoderOptions,
                               bdlat_TypeCategory::DynamicType)
{
    const TypesPrintUtilImp_PrintText printTextCb(&stream, encoderOptions);
    bdlat_TypeCategoryUtil::accessByCategory(object, printTextCb);
    return stream;
}

template <class TYPE>
inline
bsl::ostream& TypesPrintUtil_Imp::printText(
                               bsl::ostream&                    stream,
                               const TYPE&                      object,
                               const EncoderOptions            *encoderOptions,
                               bdlat_TypeCategory::Enumeration)
{
    bsl::string stringVal;

    bdlat_EnumFunctions::toString(&stringVal, object);

    return TypesPrintUtil::printText(stream, stringVal, encoderOptions);
}

template <class TYPE>
inline
bsl::ostream& TypesPrintUtil_Imp::printText(
                            bsl::ostream&                       stream,
                            const TYPE&                         object,
                            const EncoderOptions               *encoderOptions,
                            bdlat_TypeCategory::CustomizedType)
{
    return TypesPrintUtil::printText(
                      stream,
                      bdlat_CustomizedTypeFunctions::convertToBaseType(object),
                      encoderOptions);
}

template <class TYPE, class ANY_CATEGORY>
inline
bsl::ostream& TypesPrintUtil_Imp::printText(bsl::ostream&         stream,
                                            const TYPE&,
                                            const EncoderOptions *,
                                            ANY_CATEGORY)
{
    BSLS_ASSERT_SAFE(0 == "Unsupported operation!");

    stream.setstate(bsl::ios_base::failbit);

    return stream;
}

inline
bsl::ostream& TypesPrintUtil_Imp::printText(bsl::ostream&               stream,
                                            const bool&                 object,
                                            const EncoderOptions       *,
                                            bdlat_TypeCategory::Simple)
{
    return stream << (object ? "true" : "false");
}

template <class TYPE>
typename bsl::enable_if<bsl::is_class<TYPE>::value &&
                            bsl::is_convertible<TYPE, bsl::string_view>::value,
                        bsl::ostream&>::type
TypesPrintUtil_Imp::printText(bsl::ostream&         stream,
                              const TYPE&           object,
                              const EncoderOptions *encoderOptions,
                              bdlat_TypeCategory::Simple)
{
    return printText(stream,
                     static_cast<const bsl::string_view&>(object),
                     encoderOptions,
                     bdlat_TypeCategory::Simple());
}

}  // close package namespace
}  // close enterprise namespace

#endif

// ----------------------------------------------------------------------------
// Copyright 2017 Bloomberg Finance L.P.
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
