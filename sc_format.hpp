#pragma once

// Formatters for:
// + sc_time       {:[t]}
// + sc_int<W>     {:[u|m][p][c|d|b|o|x]}
// + sc_uint<W>    {:[u|m][p][c|d|b|o|x]}
// + sc_bigint<W>  {:[u|m][p][c|d|b|o|x]}
// + sc_biguint<W> {:[u|m][p][c|d|b|o|x]}
// + sc_lv<W>      {:[u|m][p][l|d|b|o|x]}
// + sc_logic      {:[l]}
// + sc_fix        {:[u|m][p][d|b|o|x]}
// + sc_ufix       {:[u|m][p][d|b|o|x]}
// + sc_fixed<W>   {:[e|f|d|b|o|x]}
// + sc_ufixed<W>  {:[e|f|d|b|o|x]}
//
// where:
//   t -> time
//   u -> unsigned two's complement
//   m -> signed-magniture
//   p -> prefix
//   c -> canonical signed digit
//   b -> binary
//   d -> decimal
//   e -> exponent
//   f -> full
//   o -> octal
//   x -> hexadecimal

#include <systemc>
#include <string>
#include <fmt/format.h>

using namespace std::string_view_literals;

//------------------------------------------------------------------------------
template<> // Custom formatter for sc_core::sc_time
struct fmt::formatter<sc_core::sc_time> : fmt::formatter<std::string> {
  char presentation = 't'; // 't'=>normal
  constexpr auto parse( format_parse_context& ctx ) -> decltype( ctx.begin() )
  {
    auto it = ctx.begin(), end = ctx.end();
    if ( it != end && ( *it == 't' ) ) { presentation = *it++; }
    if ( it != end && *it != '}' ) { throw format_error( "invalid format" ); }
    return it;
  }

  auto format( const sc_core::sc_time& time, format_context& ctx ) const -> decltype( ctx.out() )
  {
    if ( presentation == 't' ) {
      return format_to( ctx.out(), "{}", time.to_string() );
    } else {
      return format_to( ctx.out(), "Formatting error to sc_time" );
    }
  }
};

//------------------------------------------------------------------------------
template< int W > // Custom formatter for sc_dt::sc_int<W>
struct fmt::formatter<sc_dt::sc_int<W>> : fmt::formatter<std::string> {
  char presentation = 'd';
  char sign = '-';
  bool prefix = false;

  constexpr auto parse( format_parse_context& ctx ) -> decltype( ctx.begin() )
  {
    auto it = ctx.begin(), end = ctx.end();
    if ( it != end && ( "mu"sv.find_first_of( *it ) != std::string::npos ) ) { sign = *it++; }
    if ( it != end && *it == 'p' ) {
      prefix = true;
      ++it;
    }
    if ( it != end && ( "cdbox"sv.find_first_of( *it ) != std::string::npos ) ) { presentation = *it++; }
    if ( it != end && *it != '}' ) { throw format_error( "invalid format" ); }
    return it;
  }

  auto format( const sc_dt::sc_int<W>& data, format_context& ctx ) const -> decltype( ctx.out() )
  {
    using namespace sc_dt;
    switch ( presentation ) {
      case 'd':
        return format_to( ctx.out(), "{}", data.to_string( SC_DEC, prefix ) );
      case 'b': {
        switch ( sign ) {
          case 'm':
            return format_to( ctx.out(), "{}", data.to_string( SC_BIN_SM, prefix ) );
          case 'u':
            return format_to( ctx.out(), "{}", data.to_string( SC_BIN_US, prefix ) );
          default:
            return format_to( ctx.out(), "{}", data.to_string( SC_BIN, prefix ) );
        }
      }
      case 'o': {
        switch ( sign ) {
          case 'm':
            return format_to( ctx.out(), "{}", data.to_string( SC_OCT_SM, prefix ) );
          case 'u':
            return format_to( ctx.out(), "{}", data.to_string( SC_OCT_US, prefix ) );
          default:
            return format_to( ctx.out(), "{}", data.to_string( SC_OCT, prefix ) );
        }
      }
      case 'x': {
        switch ( sign ) {
          case 'm':
            return format_to( ctx.out(), "{}", data.to_string( SC_HEX_SM, prefix ) );
          case 'u':
            return format_to( ctx.out(), "{}", data.to_string( SC_HEX_US, prefix ) );
          default:
            return format_to( ctx.out(), "{}", data.to_string( SC_HEX, prefix ) );
        }
      }
      case 'c':
        return format_to( ctx.out(), "{}", data.to_string( SC_CSD, prefix ) );
      default:
        return format_to( ctx.out(), "Formatting error to sc_int" );
    }
  }
};

//------------------------------------------------------------------------------
template< int W > // Custom formatter for sc_dt::sc_uint<W>
struct fmt::formatter<sc_dt::sc_uint<W>> : fmt::formatter<std::string> {
  char presentation = 'd';
  char sign = '-';
  bool prefix = false;

  constexpr auto parse( format_parse_context& ctx ) -> decltype( ctx.begin() )
  {
    auto it = ctx.begin(), end = ctx.end();
    if ( it != end && ( "mu"sv.find_first_of( *it ) != std::string::npos ) ) { sign = *it++; }
    if ( it != end && *it == 'p' ) {
      prefix = true;
      ++it;
    }
    if ( it != end && ( "dbox"sv.find_first_of( *it ) != std::string::npos ) ) { presentation = *it++; }
    if ( it != end && *it != '}' ) { throw format_error( "invalid format" ); }
    return it;
  }

  auto format( const sc_dt::sc_uint<W>& data, format_context& ctx ) const -> decltype( ctx.out() )
  {
    using namespace sc_dt;
    switch ( presentation ) {
      case 'd':
        return format_to( ctx.out(), "{}", data.to_string( SC_DEC, prefix ) );
      case 'b': {
        switch ( sign ) {
          case 'm':
            return format_to( ctx.out(), "{}", data.to_string( SC_BIN_SM, prefix ) );
          case 'u':
            return format_to( ctx.out(), "{}", data.to_string( SC_BIN_US, prefix ) );
          default:
            return format_to( ctx.out(), "{}", data.to_string( SC_BIN, prefix ) );
        }
      }
      case 'o': {
        switch ( sign ) {
          case 'm':
            return format_to( ctx.out(), "{}", data.to_string( SC_OCT_SM, prefix ) );
          case 'u':
            return format_to( ctx.out(), "{}", data.to_string( SC_OCT_US, prefix ) );
          default:
            return format_to( ctx.out(), "{}", data.to_string( SC_OCT, prefix ) );
        }
      }
      case 'x': {
        switch ( sign ) {
          case 'm':
            return format_to( ctx.out(), "{}", data.to_string( SC_HEX_SM, prefix ) );
          case 'u':
            return format_to( ctx.out(), "{}", data.to_string( SC_HEX_US, prefix ) );
          default:
            return format_to( ctx.out(), "{}", data.to_string( SC_HEX, prefix ) );
        }
      }
      default:
        return format_to( ctx.out(), "Formatting error to sc_uint" );
    }
  }
};

//------------------------------------------------------------------------------
template< int W > // Custom formatter for sc_dt::sc_bigint<W>
struct fmt::formatter<sc_dt::sc_bigint<W>> : fmt::formatter<std::string> {
  char presentation = 'd';
  char sign = '-';
  bool prefix = false;

  constexpr auto parse( format_parse_context& ctx ) -> decltype( ctx.begin() )
  {
    auto it = ctx.begin(), end = ctx.end();
    if ( it != end && ( "mu"sv.find_first_of( *it ) != std::string::npos ) ) { sign = *it++; }
    if ( it != end && *it == 'p' ) {
      prefix = true;
      ++it;
    }
    if ( it != end && ( "dbox"sv.find_first_of( *it ) != std::string::npos ) ) { presentation = *it++; }
    if ( it != end && *it != '}' ) { throw format_error( "invalid format" ); }
    return it;
  }

  auto format( const sc_dt::sc_bigint<W>& data, format_context& ctx ) const -> decltype( ctx.out() )
  {
    using namespace sc_dt;
    switch ( presentation ) {
      case 'd':
        return format_to( ctx.out(), "{}", data.to_string( SC_DEC, prefix ) );
      case 'b': {
        switch ( sign ) {
          case 'm':
            return format_to( ctx.out(), "{}", data.to_string( SC_BIN_SM, prefix ) );
          case 'u':
            return format_to( ctx.out(), "{}", data.to_string( SC_BIN_US, prefix ) );
          default:
            return format_to( ctx.out(), "{}", data.to_string( SC_BIN, prefix ) );
        }
      }
      case 'o': {
        switch ( sign ) {
          case 'm':
            return format_to( ctx.out(), "{}", data.to_string( SC_OCT_SM, prefix ) );
          case 'u':
            return format_to( ctx.out(), "{}", data.to_string( SC_OCT_US, prefix ) );
          default:
            return format_to( ctx.out(), "{}", data.to_string( SC_OCT, prefix ) );
        }
      }
      case 'x': {
        switch ( sign ) {
          case 'm':
            return format_to( ctx.out(), "{}", data.to_string( SC_HEX_SM, prefix ) );
          case 'u':
            return format_to( ctx.out(), "{}", data.to_string( SC_HEX_US, prefix ) );
          default:
            return format_to( ctx.out(), "{}", data.to_string( SC_HEX, prefix ) );
        }
      }
      default:
        return format_to( ctx.out(), "Formatting error to sc_bigint<W>" );
    }
  }
};

//------------------------------------------------------------------------------
template< int W > // Custom formatter for sc_dt::sc_biguint<W>
struct fmt::formatter<sc_dt::sc_biguint<W>> : fmt::formatter<std::string> {
  char presentation = 'd';
  bool prefix = false;
  char sign = '-';

  constexpr auto parse( format_parse_context& ctx ) -> decltype( ctx.begin() )
  {
    auto it = ctx.begin(), end = ctx.end();
    if ( it != end && ( "mu"sv.find_first_of( *it ) != std::string::npos ) ) { sign = *it++; }
    if ( it != end && *it == 'p' ) {
      prefix = true;
      ++it;
    }
    if ( it != end && ( "dbox"sv.find_first_of( *it ) != std::string::npos ) ) { presentation = *it++; }
    if ( it != end && *it != '}' ) { throw format_error( "invalid format" ); }
    return it;
  }

  auto format( const sc_dt::sc_biguint<W>& data, format_context& ctx ) const -> decltype( ctx.out() )
  {
    using namespace sc_dt;
    switch ( presentation ) {
      case 'd':
        return format_to( ctx.out(), "{}", data.to_string( SC_DEC, prefix ) );
      case 'b': {
        switch ( sign ) {
          case 'm':
            return format_to( ctx.out(), "{}", data.to_string( SC_BIN_SM, prefix ) );
          case 'u':
            return format_to( ctx.out(), "{}", data.to_string( SC_BIN_US, prefix ) );
          default:
            return format_to( ctx.out(), "{}", data.to_string( SC_BIN, prefix ) );
        }
      }
      case 'o': {
        switch ( sign ) {
          case 'm':
            return format_to( ctx.out(), "{}", data.to_string( SC_OCT_SM, prefix ) );
          case 'u':
            return format_to( ctx.out(), "{}", data.to_string( SC_OCT_US, prefix ) );
          default:
            return format_to( ctx.out(), "{}", data.to_string( SC_OCT, prefix ) );
        }
      }
      case 'x': {
        switch ( sign ) {
          case 'm':
            return format_to( ctx.out(), "{}", data.to_string( SC_HEX_SM, prefix ) );
          case 'u':
            return format_to( ctx.out(), "{}", data.to_string( SC_HEX_US, prefix ) );
          default:
            return format_to( ctx.out(), "{}", data.to_string( SC_HEX, prefix ) );
        }
      }
      default:
        return format_to( ctx.out(), "Formatting error to sc_biguint<W>" );
    }
  }
};

//------------------------------------------------------------------------------
template< int W > // Custom formatter for sc_dt::sc_lv<W>
struct fmt::formatter<sc_dt::sc_lv<W>> : fmt::formatter<std::string> {
  char presentation = 'l';
  bool prefix = false;
  char sign = '-';

  constexpr auto parse( format_parse_context& ctx ) -> decltype( ctx.begin() )
  {
    auto it = ctx.begin(), end = ctx.end();
    if ( it != end && ( "mu"sv.find_first_of( *it ) != std::string::npos ) ) { sign = *it++; }
    if ( it != end && *it == 'p' ) {
      prefix = true;
      ++it;
    }
    if ( it != end && ( "ldbox"sv.find_first_of( *it ) != std::string::npos ) ) { presentation = *it++; }
    if ( it != end && *it != '}' ) { throw format_error( "invalid format" ); }
    return it;
  }

  auto format( const sc_dt::sc_lv<W>& data, format_context& ctx ) const -> decltype( ctx.out() )
  {
    using namespace sc_dt;
    switch ( presentation ) {
      case 'l':
        return format_to( ctx.out(), "{}", data.to_string() );
      case 'd':
        return format_to( ctx.out(), "{}", data.to_string( SC_DEC, prefix ) );
      case 'b': {
        switch ( sign ) {
          case 'm':
            return format_to( ctx.out(), "{}", data.to_string( SC_BIN_SM, prefix ) );
          case 'u':
            return format_to( ctx.out(), "{}", data.to_string( SC_BIN_US, prefix ) );
          default:
            return format_to( ctx.out(), "{}", data.to_string( SC_BIN, prefix ) );
        }
      }
      case 'o': {
        switch ( sign ) {
          case 'm':
            return format_to( ctx.out(), "{}", data.to_string( SC_OCT_SM, prefix ) );
          case 'u':
            return format_to( ctx.out(), "{}", data.to_string( SC_OCT_US, prefix ) );
          default:
            return format_to( ctx.out(), "{}", data.to_string( SC_OCT, prefix ) );
        }
      }
      case 'x': {
        switch ( sign ) {
          case 'm':
            return format_to( ctx.out(), "{}", data.to_string( SC_HEX_SM, prefix ) );
          case 'u':
            return format_to( ctx.out(), "{}", data.to_string( SC_HEX_US, prefix ) );
          default:
            return format_to( ctx.out(), "{}", data.to_string( SC_HEX, prefix ) );
        }
      }
      default:
        return format_to( ctx.out(), "Formatting error to sc_lv<W>" );
    }
  }
};

//------------------------------------------------------------------------------
template<> // Custom formatter for sc_dt::sc_logic
struct fmt::formatter<sc_dt::sc_logic> : fmt::formatter<std::string> {
  char presentation = 'l';

  constexpr auto parse( format_parse_context& ctx ) -> decltype( ctx.begin() )
  {
    auto it = ctx.begin(), end = ctx.end();
    if ( it != end && ( *it == 'l' ) ) { presentation = *it++; }
    if ( it != end && *it != '}' ) { throw format_error( "invalid format" ); }
    return it;
  }

  auto format( const sc_dt::sc_logic& data, format_context& ctx ) const -> decltype( ctx.out() )
  {
    if ( presentation == 'l' ) {
      return format_to( ctx.out(), "{}", data.to_char() );
    } else {
      return format_to( ctx.out(), "Formatting error to sc_logic" );
    }
  }
};

//------------------------------------------------------------------------------
template<> // Custom formatter for sc_dt::sc_fix
struct fmt::formatter<sc_dt::sc_fix> : fmt::formatter<std::string> {
  char presentation = 'd';
  bool prefix = false;
  char sign = '-';

  constexpr auto parse( format_parse_context& ctx ) -> decltype( ctx.begin() )
  {
    auto it = ctx.begin(), end = ctx.end();
    if ( it != end && *it == 'p' ) {
      prefix = true;
      ++it;
    }
    if ( it != end && ( "efdbox"sv.find_first_of( *it ) != std::string::npos ) ) { presentation = *it++; }
    if ( it != end && *it != '}' ) { throw format_error( "invalid format" ); }
    return it;
  }

  auto format( const sc_dt::sc_fix& data, format_context& ctx ) const -> decltype( ctx.out() )
  {
    using namespace sc_dt;
    switch ( presentation ) {
      case 'd':
        return format_to( ctx.out(), "{}", data.to_string( SC_DEC, prefix ) );
      case 'e':
        return format_to( ctx.out(), "{}", data.to_string( SC_BIN, false, SC_E ) );
      case 'f':
        return format_to( ctx.out(), "{}", data.to_string( SC_BIN, false, SC_F ) );
      case 'b': {
        switch ( sign ) {
          case 'm':
            return format_to( ctx.out(), "{}", data.to_string( SC_BIN_SM, prefix ) );
          case 'u':
            return format_to( ctx.out(), "{}", data.to_string( SC_BIN_US, prefix ) );
          default:
            return format_to( ctx.out(), "{}", data.to_string( SC_BIN, prefix ) );
        }
      }
      case 'o': {
        switch ( sign ) {
          case 'm':
            return format_to( ctx.out(), "{}", data.to_string( SC_OCT_SM, prefix ) );
          case 'u':
            return format_to( ctx.out(), "{}", data.to_string( SC_OCT_US, prefix ) );
          default:
            return format_to( ctx.out(), "{}", data.to_string( SC_OCT, prefix ) );
        }
      }
      case 'x': {
        switch ( sign ) {
          case 'm':
            return format_to( ctx.out(), "{}", data.to_string( SC_HEX_SM, prefix ) );
          case 'u':
            return format_to( ctx.out(), "{}", data.to_string( SC_HEX_US, prefix ) );
          default:
            return format_to( ctx.out(), "{}", data.to_string( SC_HEX, prefix ) );
        }
      }
      default:
        return format_to( ctx.out(), "Formatting error to sc_fix" );
    }
  }
};

//------------------------------------------------------------------------------
template<> // Custom formatter for sc_dt::sc_ufix
struct fmt::formatter<sc_dt::sc_ufix> : fmt::formatter<std::string> {
  char presentation = 'd';
  bool prefix = false;
  char sign = '-';

  constexpr auto parse( format_parse_context& ctx ) -> decltype( ctx.begin() )
  {
    auto it = ctx.begin(), end = ctx.end();
    if ( it != end && ( "mu"sv.find_first_of( *it ) != std::string::npos ) ) { sign = *it++; }
    if ( it != end && *it == 'p' ) {
      prefix = true;
      ++it;
    }
    if ( it != end && ( "efdbox"sv.find_first_of( *it ) != std::string::npos ) ) { presentation = *it++; }
    if ( it != end && *it != '}' ) { throw format_error( "invalid format" ); }
    return it;
  }

  auto format( const sc_dt::sc_ufix& data, format_context& ctx ) const -> decltype( ctx.out() )
  {
    using namespace sc_dt;
    switch ( presentation ) {
      case 'e':
        return format_to( ctx.out(), "{}", data.to_string( SC_BIN, false, SC_E ) );
      case 'f':
        return format_to( ctx.out(), "{}", data.to_string( SC_BIN, false, SC_F ) );
      case 'd':
        return format_to( ctx.out(), "{}", data.to_string( SC_DEC, prefix ) );
      case 'b': {
        switch ( sign ) {
          case 'm':
            return format_to( ctx.out(), "{}", data.to_string( SC_BIN_SM, prefix ) );
          case 'u':
            return format_to( ctx.out(), "{}", data.to_string( SC_BIN_US, prefix ) );
          default:
            return format_to( ctx.out(), "{}", data.to_string( SC_BIN, prefix ) );
        }
      }
      case 'o': {
        switch ( sign ) {
          case 'm':
            return format_to( ctx.out(), "{}", data.to_string( SC_OCT_SM, prefix ) );
          case 'u':
            return format_to( ctx.out(), "{}", data.to_string( SC_OCT_US, prefix ) );
          default:
            return format_to( ctx.out(), "{}", data.to_string( SC_OCT, prefix ) );
        }
      }
      case 'x': {
        switch ( sign ) {
          case 'm':
            return format_to( ctx.out(), "{}", data.to_string( SC_HEX_SM, prefix ) );
          case 'u':
            return format_to( ctx.out(), "{}", data.to_string( SC_HEX_US, prefix ) );
          default:
            return format_to( ctx.out(), "{}", data.to_string( SC_HEX, prefix ) );
        }
      }
      default:
        return format_to( ctx.out(), "Formatting error to sc_ufix" );
    }
  }
};

//------------------------------------------------------------------------------
template< int WL, int IL, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N > // Custom formatter for sc_dt::sc_fix
struct fmt::formatter<sc_dt::sc_fixed<WL, IL, Q, O, N>> : fmt::formatter<std::string> {
  char presentation = 'd';
  bool prefix = false;
  char sign = '-';

  constexpr auto parse( format_parse_context& ctx ) -> decltype( ctx.begin() )
  {
    auto it = ctx.begin(), end = ctx.end();
    if ( it != end && ( "mu"sv.find_first_of( *it ) != std::string::npos ) ) { sign = *it++; }
    if ( it != end && *it == 'p' ) {
      prefix = true;
      ++it;
    }
    if ( it != end && ( "efdbox"sv.find_first_of( *it ) != std::string::npos ) ) { presentation = *it++; }
    if ( it != end && *it != '}' ) { throw format_error( "invalid format" ); }
    return it;
  }

  auto format( const sc_dt::sc_fixed<WL, IL, Q, O, N>& data, format_context& ctx ) const -> decltype( ctx.out() )
  {
    using namespace sc_dt;
    switch ( presentation ) {
      case 'd':
        return format_to( ctx.out(), "{}", data.to_string( SC_DEC, prefix ) );
      case 'e':
        return format_to( ctx.out(), "{}", data.to_string( SC_BIN, false, SC_E ) );
      case 'f':
        return format_to( ctx.out(), "{}", data.to_string( SC_BIN, false, SC_F ) );
      case 'b': {
        switch ( sign ) {
          case 'm':
            return format_to( ctx.out(), "{}", data.to_string( SC_BIN_SM, prefix ) );
          case 'u':
            return format_to( ctx.out(), "{}", data.to_string( SC_BIN_US, prefix ) );
          default:
            return format_to( ctx.out(), "{}", data.to_string( SC_BIN, prefix ) );
        }
      }
      case 'o': {
        switch ( sign ) {
          case 'm':
            return format_to( ctx.out(), "{}", data.to_string( SC_OCT_SM, prefix ) );
          case 'u':
            return format_to( ctx.out(), "{}", data.to_string( SC_OCT_US, prefix ) );
          default:
            return format_to( ctx.out(), "{}", data.to_string( SC_OCT, prefix ) );
        }
      }
      case 'x': {
        switch ( sign ) {
          case 'm':
            return format_to( ctx.out(), "{}", data.to_string( SC_HEX_SM, prefix ) );
          case 'u':
            return format_to( ctx.out(), "{}", data.to_string( SC_HEX_US, prefix ) );
          default:
            return format_to( ctx.out(), "{}", data.to_string( SC_HEX, prefix ) );
        }
      }
      default:
        return format_to( ctx.out(), "Formatting error to sc_fix" );
    }
  }
};

//------------------------------------------------------------------------------
template< int WL, int IL, sc_dt::sc_q_mode Q, sc_dt::sc_o_mode O, int N > // Custom formatter for sc_dt::sc_fix
struct fmt::formatter<sc_dt::sc_ufixed<WL, IL, Q, O, N>> : fmt::formatter<std::string> {
  char presentation = 'd';
  bool prefix = false;
  char sign = '-';

  constexpr auto parse( format_parse_context& ctx ) -> decltype( ctx.begin() )
  {
    auto it = ctx.begin(), end = ctx.end();
    if ( it != end && ( "mu"sv.find_first_of( *it ) != std::string::npos ) ) { sign = *it++; }
    if ( it != end && *it == 'p' ) {
      prefix = true;
      ++it;
    }
    if ( it != end && ( "efdbox"sv.find_first_of( *it ) != std::string::npos ) ) { presentation = *it++; }
    if ( it != end && *it != '}' ) { throw format_error( "invalid format" ); }
    return it;
  }

  auto format( const sc_dt::sc_ufixed<WL, IL, Q, O, N>& data, format_context& ctx ) const -> decltype( ctx.out() )
  {
    using namespace sc_dt;
    switch ( presentation ) {
      case 'e':
        return format_to( ctx.out(), "{}", data.to_string( SC_BIN, false, SC_E ) );
      case 'f':
        return format_to( ctx.out(), "{}", data.to_string( SC_BIN, false, SC_F ) );
      case 'd':
        return format_to( ctx.out(), "{}", data.to_string( SC_DEC, prefix ) );
      case 'b': {
        switch ( sign ) {
          case 'm':
            return format_to( ctx.out(), "{}", data.to_string( SC_BIN_SM, prefix ) );
          case 'u':
            return format_to( ctx.out(), "{}", data.to_string( SC_BIN_US, prefix ) );
          default:
            return format_to( ctx.out(), "{}", data.to_string( SC_BIN, prefix ) );
        }
      }
      case 'o': {
        switch ( sign ) {
          case 'm':
            return format_to( ctx.out(), "{}", data.to_string( SC_OCT_SM, prefix ) );
          case 'u':
            return format_to( ctx.out(), "{}", data.to_string( SC_OCT_US, prefix ) );
          default:
            return format_to( ctx.out(), "{}", data.to_string( SC_OCT, prefix ) );
        }
      }
      case 'x': {
        switch ( sign ) {
          case 'm':
            return format_to( ctx.out(), "{}", data.to_string( SC_HEX_SM, prefix ) );
          case 'u':
            return format_to( ctx.out(), "{}", data.to_string( SC_HEX_US, prefix ) );
          default:
            return format_to( ctx.out(), "{}", data.to_string( SC_HEX, prefix ) );
        }
      }
      default:
        return format_to( ctx.out(), "Formatting error to sc_ufix" );
    }
  }
};

// TAGS: Doulos, Systemc, format, SOURCE
// ----------------------------------------------------------------------------
//
// This file is licensed under Apache-2.0, and
// Copyright 2023 Doulos Inc. <mailto:info@doulos.com>
// See accompanying LICENSE or visit <https://www.apache.org/licenses/LICENSE-2.0.txt> for more details.
