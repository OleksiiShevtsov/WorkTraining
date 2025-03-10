#pragma once

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_RUNNER

#include <catch2/catch_all.hpp>
#include <string>

TEST_CASE( "std::string supports constructing" )
{
    SECTION( "empty string" )
    {
        std::string cheese;
        REQUIRE( cheese.empty() );
    }

    SECTION( "repeated characters" )
    {
        std::string roadsideAssistance( 3, 'A' );
        REQUIRE( roadsideAssistance == "AAA" );
    }
}

TEST_CASE( "std::string supports constructing substrings" )
{
    auto word = "gobbledygook"; // const char*
    REQUIRE( std::string( word ) == "gobbledygook" );
    REQUIRE( std::string( word, 6 ) == "gobble" );
}

TEST_CASE( "std::string supports" )
{
    std::string word( "catawampus" );
    SECTION( "copy construction" )
    {
        REQUIRE( std::string( word ) == "catawampus" );
    }

    SECTION( "move construction" )
    {
        REQUIRE( std::string( move( word ) ) == "catawampus" );
    }

    SECTION( "constructing from substrings" )
    {
        REQUIRE( std::string( word, 0, 3 ) == "cat" );
        REQUIRE( std::string( word, 4 ) == "wampus" );
    }
}

TEST_CASE( "constructing a string with" )
{
    SECTION( "std::string(char*) stops at embedded nulls" )
    {
        std::string str( "idioglossia\0ellohay!" );
        REQUIRE( str.length() == 11 );
    }

    SECTION( "operator\"\"s incorporates embedded nulls" )
    {
         using namespace std::literals::string_literals;
        auto strLit = "idioglossia\0ellohay!"s;
        REQUIRE( strLit.length() == 20 );
    }
}

TEST_CASE( "string's c_str method makes null-terminated strings" )
{
    std::string word( "horripilation" );
    auto asCstr = word.c_str();
    REQUIRE( asCstr[ 0 ] == 'h' );
    REQUIRE( asCstr[ 1 ] == 'o' );
    REQUIRE( asCstr[ 11 ] == 'o' );
    REQUIRE( asCstr[ 12 ] == 'n' );
    REQUIRE( asCstr[ 13 ] == '\0' );
}

TEST_CASE( "std::string supports comparison with" )
{
     using namespace std::literals::string_literals;
    std::string word( "allusion" );
    SECTION( "operator== and !=" )
    {
        REQUIRE( word == "allusion" );
        REQUIRE( word == "allusion"s );
        REQUIRE( word != "Allusion"s );
        REQUIRE( word != "illusion"s );
        REQUIRE_FALSE( word == "illusion"s );
    }
    SECTION( "operator<" )
    {
        REQUIRE( word < "illusion" );
        REQUIRE( word < "illusion"s );
        REQUIRE( word > "Illusion"s );
    }
}

TEST_CASE( "std::string supports appending with" )
{
    std::string word( "butt" );
    SECTION( "push_back" )
    {
        word.push_back( 'e' );
        REQUIRE( word == "butte" );
    }
    SECTION( "operator+=" )
    {
        word += "erfinger";
        REQUIRE( word == "butterfinger" );
    }
    SECTION( "append char" )
    {
        word.append( 1, 's' );
        REQUIRE( word == "butts" );
    }
    SECTION( "append char*" )
    {
        word.append( "stockings", 5 );
        REQUIRE( word == "buttstock" );
    }
    SECTION( "append (half-open range)" )
    {
        std::string other( "onomatopoeia" );
        word.append( other.begin(), other.begin() + 2 );
        REQUIRE( word == "button" );
    }
}

TEST_CASE( "std::string supports removal with" )
{
    std::string word( "therein" );
    SECTION( "pop_back" )
    {
        word.pop_back();
        word.pop_back();
        REQUIRE( word == "there" );
    }
    SECTION( "clear" )
    {
        word.clear();
        REQUIRE( word.empty() );
    }
    SECTION( "erese using half-open rang" )
    {
        word.erase( word.begin(), word.begin() + 3 );
        REQUIRE( word == "rein" );
    }
    SECTION( "erese using an index and length" )
    {
        word.erase( 5, 2 );
        REQUIRE( word == "there" );
    }
}

TEST_CASE( "std::string replace works with" )
{
    std::string word( "substitution" );
    SECTION( "a range and a char*" )
    {
        word.replace( word.begin() + 9, word.end(), "e" );
        REQUIRE( word == "substitute" );
    }
    SECTION( "two ranges" )
    {
        std::string other( "innuendo" );
        word.replace( word.begin(), word.begin() + 3,
                      other.begin(), other.begin() + 2 );
        REQUIRE( word == "institution" );
    }
    SECTION( "an index/length and a string" )
    {
        std::string other( "vers" );
        word.replace( 3, 6, other );
        REQUIRE( word == "subversion" );
    }
}

TEST_CASE( "std::string resize" )
{
    std::string word( "shamp" );
    SECTION( "can remove elements" )
    {
        word.resize( 4 );
        REQUIRE( word == "sham" );
    }
    SECTION( "can add elements" )
    {
        word.resize( 7, 'o' );
        REQUIRE( word == "shampoo" );
    }
}

TEST_CASE( "std::string substr with" )
{
    std::string word( "hobbits" );
    SECTION( "no arguments copies the string" )
    {
        REQUIRE( word.substr() == "hobbits" );
    }
    SECTION( "position takes the remainder" )
    {
        REQUIRE( word.substr( 3 ) == "bits" );
    }
    SECTION( "position/index takes a substring" )
    {
        REQUIRE( word.substr( 3, 3 ) == "bit" );
    }
}

TEST_CASE( "std::string find" )
{
    using namespace std::literals::string_literals;
    std::string word( "pizzazz" );
    SECTION( "locates substrings from strings" )
    {
        REQUIRE( word.find( "zz"s ) == 2 ); // pi(z)zazz
    }
    SECTION( "accepts a position argument" )
    {
        REQUIRE( word.find( "zz"s, 3 ) == 5 ); // pizza(z)z
    }
    SECTION( "locates substrings from char*" )
    {
        REQUIRE( word.find( "zaz" ) == 3 ); // piz(z)azz
    }
    SECTION( "returns npos when not found" )
    {
        REQUIRE( word.find( 'x' ) == std::string::npos );
    }
}

TEST_CASE( "std::string rfind" )
{
    using namespace std::literals::string_literals;
    std::string word( "pizzazz" );
    SECTION( "locates substrings from strings" )
    {
        REQUIRE( word.rfind( "zz"s ) == 5 ); // pizza(z)z
    }
    SECTION( "accepts a position argument" )
    {
        REQUIRE( word.rfind( "zz"s, 3 ) == 2 ); // pi(z)zazz
    }
    SECTION( "locates substrings from char*" )
    {
        REQUIRE( word.rfind( "zaz" ) == 3 ); // piz(z)azz
    }
    SECTION( "returns npos when not found" )
    {
        REQUIRE( word.rfind( 'x' ) == std::string::npos );
    }
}

TEST_CASE( "std::string find_first_of" )
{
    using namespace std::literals::string_literals;
    std::string sentence( "I am a Zizzer-Zazzer-Zuzz as you can plainly see." );
    SECTION( "locates characters within another string" )
    {
        REQUIRE( sentence.find_first_of( "Zz"s ) == 7 ); // (Z)izzer
    }
    SECTION( "accepts a position argument" )
    {
        REQUIRE( sentence.find_first_of( "Zz"s, 11 ) == 14 ); // // (Z)azzer
    }
    SECTION( "returns npos when not found" )
    {
        REQUIRE( sentence.find_first_of( 'Xx' ) == std::string::npos );
    }
}
