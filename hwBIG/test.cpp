#ifndef __PROGTEST__
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <climits>
#include <cfloat>
#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <functional>
#include <iterator>
#include <stdexcept>
#include <variant>
#include <optional>
#include <compare>
#include <charconv>
#include <span>
#include <utility>

#include "include/CellValue.hpp"

// my headers
#include "include/CellPosition.hpp"
#include "include/Spreadsheet.hpp"

using namespace std::literals;

// progtest testing
bool valueMatch(const CValue& r, const CValue& s)
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

//------------------------------------------------------------------------------

int main ()
{
  #ifndef PT

  //* my tests

  // CellPosition x0("A7");

  // std::cout << x0 << std::endl;

  ExpressionBuilder cell;
  parseExpression("=3 + 4 * 7 ^ 5 - 2 - -8", cell);
  assert(valueMatch(cell.waitingList.top()->evaluate(), CellValue(67237.0)));

  Spreadsheet excel;

  assert(!excel.setCell(CellPosition("A3"), "==="));

  excel.setCell(CellPosition("A1"), "10"); 
  excel.setCell(CellPosition("A2"), "20.5");
  excel.setCell(CellPosition("A3"), "3e1");
  assert(valueMatch(excel.getValue(CellPosition("a1")), CellValue(10.)));
  assert(valueMatch(excel.getValue(CellPosition("a2")), CellValue(20.5)));
  assert(valueMatch(excel.getValue(CellPosition("a3")), CellValue(3e1)));

  excel.setCell(CellPosition("A4"), "=1 + 2*3 - -2 ^ 3");
  assert(valueMatch(excel.getValue(CellPosition("A4")), CellValue(15.)));

  excel.setCell(CellPosition("A5"), "=A1 + A2");
  assert(valueMatch(excel.getValue(CellPosition("A5")), CellValue(30.5)));
  excel.setCell(CellPosition("A6"), "=(A2 - 0.5) * a5");
  assert(valueMatch(excel.getValue(CellPosition("A6")), CellValue(610.)));

  excel.setCell(CellPosition("A8"), "= 1 + 2");
  assert(valueMatch(excel.getValue(CellPosition("A8")), CellValue(3.)));

  excel.setCell(CellPosition("A9"), "=1 + 2*3 + -2 ^ 4");
  assert(valueMatch(excel.getValue(CellPosition("A9")), CellValue(-9.)));

  excel.setCell(CellPosition("A10"), "=\"hello \"\" there\"");
  assert(valueMatch(excel.getValue(CellPosition("A10")), CellValue("hello \" there")));

  // testing conversion base26 <-> dec

  CellPosition a("AAA1");
  CellPosition b(703, 1);

  assert(a.string() == b.string());

  CellPosition a1("CBZ1");
  CellPosition b1(2106, 1);

  assert(a1.column() == b1.column());
  assert(a1.string() == b1.string());

  #define PRINT_TEST 0
  #if PRINT_TEST == 1
  for(size_t i = 0; i < 2500; ++i)
  {
      CellPosition tmp{i,1};
      std::cout << tmp.string() << '\n';
  }
  std::cout << std::endl;
  #endif

  // copyRect
  
  excel.copyRect(CellPosition("B1"), CellPosition("A1"), 1, 10);

  assert(valueMatch(excel.getValue(CellPosition("a1")), excel.getValue(CellPosition("B1"))));
  assert(valueMatch(excel.getValue(CellPosition("a2")), excel.getValue(CellPosition("B2"))));
  assert(valueMatch(excel.getValue(CellPosition("a3")), excel.getValue(CellPosition("B3"))));
  assert(valueMatch(excel.getValue(CellPosition("A4")), excel.getValue(CellPosition("B4"))));
  assert(valueMatch(excel.getValue(CellPosition("A5")), excel.getValue(CellPosition("B5"))));
  assert(valueMatch(excel.getValue(CellPosition("A6")), excel.getValue(CellPosition("B6"))));
  assert(valueMatch(excel.getValue(CellPosition("A8")), excel.getValue(CellPosition("B8"))));
  assert(valueMatch(excel.getValue(CellPosition("A9")), excel.getValue(CellPosition("B9"))));
  assert(valueMatch(excel.getValue(CellPosition("A10")), excel.getValue(CellPosition("b10"))));

  excel.setCell(CellPosition("A5"), "=A1 + B5 + B2 + A3");
  assert(valueMatch(excel.getValue(CellPosition("A5")), CellValue(10 + 10 + 20.5 + 20.5 + 3e1)));
  excel.setCell(CellPosition("A1"), "20");
  assert(valueMatch(excel.getValue(CellPosition("A5")), CellValue(20 + 10 + 20.5 + 20.5 + 3e1)));
  assert(valueMatch(excel.getValue(CellPosition("B5")), CellValue(30.5)));

  excel.setCell(CellPosition("B11"), "=$A$10");
  assert(valueMatch(excel.getValue(CellPosition("B11")), CellValue(excel.getValue(CellPosition("A10")))));

  excel.copyRect(CellPosition("C1"), CellPosition("B11"));
  assert(valueMatch(excel.getValue(CellPosition("C1")), CellValue(excel.getValue(CellPosition("B11")))));

  excel.setCell(CellPosition("C1"), "=B$2");

  excel.copyRect(CellPosition("B3"), CellPosition("C1"));
  assert(valueMatch(excel.getValue(CellPosition("B3")), CellValue(excel.getValue(CellPosition("A2")))));

  // save / load

  std::ostringstream save_excel;
  std::istringstream load_excel;
  assert(excel.save(save_excel));
  #define PRINT_SAVE_EXCEL 0
  #if PRINT_SAVE_EXCEL == 1
  std::cout << save_excel.str() << std::endl;
  #endif
  load_excel.str(save_excel.str());
  assert(excel.load(load_excel));
  load_excel.clear();
  save_excel.str("");
  save_excel.clear();
  assert(excel.save(save_excel));
  #if PRINT_SAVE_EXCEL == 1
  std::cout << save_excel.str() << std::endl;
  #endif
  Spreadsheet newExcel;
  load_excel.str(save_excel.str());
  assert(newExcel.load(load_excel));

  assert(valueMatch(excel.getValue(CellPosition("A1")), newExcel.getValue(CellPosition("A1"))));
  assert(valueMatch(excel.getValue(CellPosition("A2")), newExcel.getValue(CellPosition("A2"))));
  assert(valueMatch(excel.getValue(CellPosition("A3")), newExcel.getValue(CellPosition("A3"))));
  assert(valueMatch(excel.getValue(CellPosition("C1")), newExcel.getValue(CellPosition("C1"))));
  assert(valueMatch(excel.getValue(CellPosition("B5")), newExcel.getValue(CellPosition("B5"))));

  //

  assert(!excel.setCell(CellPosition("C10"), "=$$$$A1"));
  assert(!excel.setCell(CellPosition("C10"), "=$A$$2"));

  assert(newExcel.setCell(CellPosition("B3"), "=$B4"));
  assert(newExcel.setCell(CellPosition("B4"), "=1 + 2"));
  assert(newExcel.setCell(CellPosition("B2"), "=3 + 6"));
  
  assert(valueMatch(newExcel.getValue(CellPosition("B3")), CellValue(3.)));
  assert(valueMatch(newExcel.getValue(CellPosition("B4")), CellValue(3.)));
  assert(valueMatch(newExcel.getValue(CellPosition("B2")), CellValue(9.)));

  newExcel.copyRect(CellPosition("A1"), CellPosition("B3"));

  assert(valueMatch(newExcel.getValue(CellPosition("A1")), newExcel.getValue(CellPosition("B2"))));
  
  assert(newExcel.setCell(CellPosition("Z3"), "=$Z$1 + $Z$2"));
  assert(newExcel.setCell(CellPosition("Z2"), ", world!"));
  assert(newExcel.setCell(CellPosition("Z1"), "Hello"));

  assert(valueMatch(newExcel.getValue(CellPosition("Z3")), CellValue("Hello, world!")));

  std::ostringstream newExcel_save_before_load;
  std::ostringstream newExcel_save_after_load;
  assert(newExcel.save(newExcel_save_before_load));
  std::string data = newExcel_save_before_load.str();
  data[23] = '%';
  std::istringstream newExcel_load(data);
  assert(!newExcel.load(newExcel_load));
  assert(newExcel.save(newExcel_save_after_load));
  assert(newExcel_save_after_load.str() == newExcel_save_before_load.str());

  CellPosition abcde(1, 0);
  assert(abcde.string() == "A0");

  #else
  CSpreadsheet x0, x1;
  std::ostringstream oss;
  std::istringstream iss;
  std::string data;
  assert ( x0 . setCell ( CPos ( "A1" ), "10" ) );
  assert ( x0 . setCell ( CPos ( "A2" ), "20.5" ) );
  assert ( x0 . setCell ( CPos ( "A3" ), "3e1" ) );
  assert ( x0 . setCell ( CPos ( "A4" ), "=40" ) );
  assert ( x0 . setCell ( CPos ( "A5" ), "=5e+1" ) );
  assert ( x0 . setCell ( CPos ( "A6" ), "raw text with any characters, including a quote \" or a newline\n" ) );
  assert ( x0 . setCell ( CPos ( "A7" ), "=\"quoted string, quotes must be doubled: \"\". Moreover, backslashes are needed for C++.\"" ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A1" ) ), CValue ( 10.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A2" ) ), CValue ( 20.5 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A3" ) ), CValue ( 30.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A4" ) ), CValue ( 40.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A5" ) ), CValue ( 50.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A6" ) ), CValue ( "raw text with any characters, including a quote \" or a newline\n" ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A7" ) ), CValue ( "quoted string, quotes must be doubled: \". Moreover, backslashes are needed for C++." ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A8" ) ), CValue() ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "AAAA9999" ) ), CValue() ) );
  assert ( x0 . setCell ( CPos ( "B1" ), "=A1+A2*A3" ) );
  assert ( x0 . setCell ( CPos ( "B2" ), "= -A1 ^ 2 - A2 / 2   " ) );
  assert ( x0 . setCell ( CPos ( "B3" ), "= 2 ^ $A$1" ) );
  assert ( x0 . setCell ( CPos ( "B4" ), "=($A1+A$2)^2" ) );
  assert ( x0 . setCell ( CPos ( "B5" ), "=B1+B2+B3+B4" ) );
  assert ( x0 . setCell ( CPos ( "B6" ), "=B1+B2+B3+B4+B5" ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B1" ) ), CValue ( 625.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B2" ) ), CValue ( -110.25 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B3" ) ), CValue ( 1024.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B4" ) ), CValue ( 930.25 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B5" ) ), CValue ( 2469.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B6" ) ), CValue ( 4938.0 ) ) );
  assert ( x0 . setCell ( CPos ( "A1" ), "12" ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B1" ) ), CValue ( 627.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B2" ) ), CValue ( -154.25 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B4" ) ), CValue ( 1056.25 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B5" ) ), CValue ( 5625.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B6" ) ), CValue ( 11250.0 ) ) );
  x1 = x0;
  assert ( x0 . setCell ( CPos ( "A2" ), "100" ) );
  assert ( x1 . setCell ( CPos ( "A2" ), "=A3+A5+A4" ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B1" ) ), CValue ( 3012.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B2" ) ), CValue ( -194.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B4" ) ), CValue ( 12544.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B5" ) ), CValue ( 19458.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B6" ) ), CValue ( 38916.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B1" ) ), CValue ( 3612.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B2" ) ), CValue ( -204.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B4" ) ), CValue ( 17424.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B5" ) ), CValue ( 24928.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B6" ) ), CValue ( 49856.0 ) ) );
  oss . clear ();
  oss . str ( "" );
  assert ( x0 . save ( oss ) );
  data = oss . str ();
  iss . clear ();
  iss . str ( data );
  assert ( x1 . load ( iss ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B1" ) ), CValue ( 3012.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B2" ) ), CValue ( -194.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B4" ) ), CValue ( 12544.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B5" ) ), CValue ( 19458.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B6" ) ), CValue ( 38916.0 ) ) );
  assert ( x0 . setCell ( CPos ( "A3" ), "4e1" ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B1" ) ), CValue ( 3012.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B2" ) ), CValue ( -194.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B4" ) ), CValue ( 12544.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B5" ) ), CValue ( 19458.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B6" ) ), CValue ( 38916.0 ) ) );
  oss . clear ();
  oss . str ( "" );
  assert ( x0 . save ( oss ) );
  data = oss . str ();
  for ( size_t i = 0; i < std::min<size_t> ( data . length (), 10 ); i ++ )
    data[i] ^=0x5a;
  iss . clear ();
  iss . str ( data );
  assert ( ! x1 . load ( iss ) );
  assert ( x0 . setCell ( CPos ( "D0" ), "10" ) );
  assert ( x0 . setCell ( CPos ( "D1" ), "20" ) );
  assert ( x0 . setCell ( CPos ( "D2" ), "30" ) );
  assert ( x0 . setCell ( CPos ( "D3" ), "40" ) );
  assert ( x0 . setCell ( CPos ( "D4" ), "50" ) );
  assert ( x0 . setCell ( CPos ( "E0" ), "60" ) );
  assert ( x0 . setCell ( CPos ( "E1" ), "70" ) );
  assert ( x0 . setCell ( CPos ( "E2" ), "80" ) );
  assert ( x0 . setCell ( CPos ( "E3" ), "90" ) );
  assert ( x0 . setCell ( CPos ( "E4" ), "100" ) );
  assert ( x0 . setCell ( CPos ( "F10" ), "=D0+5" ) );
  assert ( x0 . setCell ( CPos ( "F11" ), "=$D0+5" ) );
  assert ( x0 . setCell ( CPos ( "F12" ), "=D$0+5" ) );
  assert ( x0 . setCell ( CPos ( "F13" ), "=$D$0+5" ) );
  x0 . copyRect ( CPos ( "G11" ), CPos ( "F10" ), 1, 4 );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F10" ) ), CValue ( 15.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F11" ) ), CValue ( 15.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F12" ) ), CValue ( 15.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F13" ) ), CValue ( 15.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F14" ) ), CValue() ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "G10" ) ), CValue() ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "G11" ) ), CValue ( 75.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "G12" ) ), CValue ( 25.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "G13" ) ), CValue ( 65.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "G14" ) ), CValue ( 15.0 ) ) );
  x0 . copyRect ( CPos ( "G11" ), CPos ( "F10" ), 2, 4 );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F10" ) ), CValue ( 15.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F11" ) ), CValue ( 15.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F12" ) ), CValue ( 15.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F13" ) ), CValue ( 15.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F14" ) ), CValue() ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "G10" ) ), CValue() ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "G11" ) ), CValue ( 75.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "G12" ) ), CValue ( 25.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "G13" ) ), CValue ( 65.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "G14" ) ), CValue ( 15.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "H10" ) ), CValue() ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "H11" ) ), CValue() ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "H12" ) ), CValue() ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "H13" ) ), CValue ( 35.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "H14" ) ), CValue() ) );
  assert ( x0 . setCell ( CPos ( "F0" ), "-27" ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "H14" ) ), CValue ( -22.0 ) ) );
  x0 . copyRect ( CPos ( "H12" ), CPos ( "H13" ), 1, 2 );
  assert ( valueMatch ( x0 . getValue ( CPos ( "H12" ) ), CValue ( 25.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "H13" ) ), CValue ( -22.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "H14" ) ), CValue ( -22.0 ) ) );
  #endif
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
