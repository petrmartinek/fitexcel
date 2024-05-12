#ifndef CELL_VALUE_INCLUDED
#define CELL_VALUE_INCLUDED

#include <variant>
#include <cmath>
#include <cfloat>
#include <string>

using CellValue = std::variant<std::monostate, double, std::string>;

// borrowed from test.cpp just renamed cvalue to my naming convention
inline bool valueMatchTop(const CellValue& r, const CellValue& s)
{
  if ( r . index () != s . index () )
    return false;
  if ( r . index () == 0 )
    return true;
  if ( r . index () == 2 )
    return std::get<std::string> ( r ) == std::get<std::string> ( s );
  if ( std::isnan ( std::get<double> ( r ) ) && std::isnan ( std::get<double> ( s ) ) )
    return true;
  if ( std::isinf ( std::get<double> ( r ) ) && std::isinf ( std::get<double> ( s ) ) )
    return ( std::get<double> ( r ) < 0 && std::get<double> ( s ) < 0 )
           || ( std::get<double> ( r ) > 0 && std::get<double> ( s ) > 0 );
  return fabs ( std::get<double> ( r ) - std::get<double> ( s ) ) <= 1e8 * DBL_EPSILON * fabs ( std::get<double> ( r ) );
}

using CValue = CellValue; // progtest
#endif