#define BOOST_TEST_MODULE Tidop flags test
#include <boost/test/unit_test.hpp>
#include <tidop/core/flags.h>

using namespace tl;

//enums para pruebas

// unscoped enumeration type
enum class ePrueba1 {

};

enum class ePrueba2 : int8_t {
  flag00 = (0 << 0),
  flag01 = (1 << 0),
  flag02 = (1 << 1),
  flag03 = (1 << 2),
  flag04 = (1 << 3),
  flag05 = (1 << 4),
  flag06 = (1 << 5),
  flag07 = (1 << 6)
};

ALLOW_BITWISE_FLAG_OPERATIONS(ePrueba2)

enum class ePrueba3 : int16_t {
  flag00 = (0 << 0),
  flag01 = (1 << 0),
  flag02 = (1 << 1),
  flag03 = (1 << 2),
  flag04 = (1 << 3),
  flag05 = (1 << 4),
  flag06 = (1 << 5),
  flag07 = (1 << 6),
  flag08 = (1 << 7),
  flag09 = (1 << 8),
  flag10 = (1 << 9),
  flag11 = (1 << 10),
  flag12 = (1 << 11),
  flag13 = (1 << 12),
  flag14 = (1 << 13),
  flag15 = (1 << 14)
};

ALLOW_BITWISE_FLAG_OPERATIONS(ePrueba3)


BOOST_AUTO_TEST_CASE(EnumFlags_constructors)
{
  // Constructor vacio
  EnumFlags<ePrueba2> flagEmpty;
  BOOST_CHECK_EQUAL(0, static_cast<EnumFlags<ePrueba2>::Type>(flagEmpty.flags()));

   // Constructor copia
  EnumFlags<ePrueba2> flag_copy(flagEmpty);
  BOOST_CHECK_EQUAL(0, static_cast<EnumFlags<ePrueba2>::Type>(flag_copy.flags()));

  // Constructor por enun
  EnumFlags<ePrueba2> flag(ePrueba2::flag01);

  BOOST_CHECK_EQUAL(false, flag.isActive(ePrueba2::flag00));
  BOOST_CHECK(flag.isActive(ePrueba2::flag01));
  BOOST_CHECK_EQUAL(false, flag.isActive(ePrueba2::flag02));
  BOOST_CHECK_EQUAL(false, flag.isActive(ePrueba2::flag03));
  BOOST_CHECK_EQUAL(false, flag.isActive(ePrueba2::flag04));
  BOOST_CHECK_EQUAL(false, flag.isActive(ePrueba2::flag05));
  BOOST_CHECK_EQUAL(false, flag.isActive(ePrueba2::flag06));
  BOOST_CHECK_EQUAL(false, flag.isActive(ePrueba2::flag07));
}

BOOST_AUTO_TEST_CASE(EnumFlags_assign) 
{
  EnumFlags<ePrueba2> flagEmpty;
  flagEmpty = ePrueba2::flag01;

  BOOST_CHECK(flagEmpty.isActive(ePrueba2::flag01));

  flagEmpty = ePrueba2::flag02 | ePrueba2::flag03;

  BOOST_CHECK_EQUAL(false, flagEmpty.isActive(ePrueba2::flag01));
  BOOST_CHECK(flagEmpty.isActive(ePrueba2::flag02));
  BOOST_CHECK(flagEmpty.isActive(ePrueba2::flag03));

  flagEmpty = ePrueba2::flag02 | ePrueba2::flag03 | ePrueba2::flag01;
  BOOST_CHECK_EQUAL(false, flagEmpty.isActive(ePrueba2::flag00));
  BOOST_CHECK(flagEmpty.isActive(ePrueba2::flag01));
  BOOST_CHECK(flagEmpty.isActive(ePrueba2::flag02));
  BOOST_CHECK(flagEmpty.isActive(ePrueba2::flag03));
  BOOST_CHECK_EQUAL(false, flagEmpty.isActive(ePrueba2::flag04));
  BOOST_CHECK_EQUAL(false, flagEmpty.isActive(ePrueba2::flag05));
  BOOST_CHECK_EQUAL(false, flagEmpty.isActive(ePrueba2::flag06));
  BOOST_CHECK_EQUAL(false, flagEmpty.isActive(ePrueba2::flag07));
}

BOOST_AUTO_TEST_CASE(EnumFlags_clear)
{
  EnumFlags<ePrueba2> flag(ePrueba2::flag02 | ePrueba2::flag03);
  BOOST_CHECK(flag.isActive(ePrueba2::flag02));
  BOOST_CHECK(flag.isActive(ePrueba2::flag03));

  flag.clear();
  BOOST_CHECK_EQUAL(false, flag.isActive(ePrueba2::flag02));
  BOOST_CHECK_EQUAL(false, flag.isActive(ePrueba2::flag03));
}

BOOST_AUTO_TEST_CASE(EnumFlags_flagOn) 
{
  EnumFlags<ePrueba3> flag2;

  BOOST_CHECK_EQUAL(false, flag2.isActive(ePrueba3::flag03));

  flag2.flagOn(ePrueba3::flag03);

  BOOST_CHECK(flag2.isActive(ePrueba3::flag03));

  //Prueba a activar un flag ya activo
  flag2.flagOn(ePrueba3::flag03);
  BOOST_CHECK(flag2.isActive(ePrueba3::flag03));
}

BOOST_AUTO_TEST_CASE(EnumFlags_flagOff) 
{
  EnumFlags<ePrueba3> flag2(ePrueba3::flag15);

  BOOST_CHECK(flag2.isActive(ePrueba3::flag15));

  flag2.flagOff(ePrueba3::flag15);

  BOOST_CHECK_EQUAL(false, flag2.isActive(ePrueba3::flag15));

  //Prueba a desactivar un flag ya desactivado
  flag2.flagOff(ePrueba3::flag15);
  BOOST_CHECK_EQUAL(false, flag2.isActive(ePrueba3::flag15));
}

BOOST_AUTO_TEST_CASE(EnumFlags_switchFlag)
{
  EnumFlags<ePrueba3> flag2(ePrueba3::flag15 | ePrueba3::flag06 );
  BOOST_CHECK(flag2.isActive(ePrueba3::flag15));
  BOOST_CHECK(flag2.isActive(ePrueba3::flag06));
  BOOST_CHECK_EQUAL(false, flag2.isActive(ePrueba3::flag01));
  BOOST_CHECK_EQUAL(false, flag2.isActive(ePrueba3::flag03));

  flag2.switchFlag(ePrueba3::flag15);
  flag2.switchFlag(ePrueba3::flag06);
  flag2.switchFlag(ePrueba3::flag01);
  flag2.switchFlag(ePrueba3::flag03);

  BOOST_CHECK_EQUAL(false, flag2.isActive(ePrueba3::flag15));
  BOOST_CHECK_EQUAL(false, flag2.isActive(ePrueba3::flag06));
  BOOST_CHECK(flag2.isActive(ePrueba3::flag01));
  BOOST_CHECK(flag2.isActive(ePrueba3::flag03));
}


// Flags<T>

// Constructor Flags_8
BOOST_AUTO_TEST_CASE(Flags_8_constructor)
{
  Flags_8 flag8;
  BOOST_CHECK_EQUAL(0, static_cast<EnumFlags<ePrueba2>::Type>(flag8.flags()));
  for (int i = 0; i < 8; i++)
    BOOST_CHECK_EQUAL(false, flag8.isActive(i));
}

// Constructor Flags_16
BOOST_AUTO_TEST_CASE(Flags_16_constructor)
{
  Flags_16 flag16;
  BOOST_CHECK_EQUAL(0, static_cast<EnumFlags<ePrueba2>::Type>(flag16.flags()));
  for (int i = 0; i < 16; i++)
    BOOST_CHECK_EQUAL(false, flag16.isActive(i));
}

// Constructor Flags_32
BOOST_AUTO_TEST_CASE(Flags_32_constructor)
{
  Flags_32 flag32;
  BOOST_CHECK_EQUAL(0, static_cast<EnumFlags<ePrueba2>::Type>(flag32.flags()));
  for (int i = 0; i < 32; i++)
    BOOST_CHECK_EQUAL(false, flag32.isActive(i));
}

// Constructor Flags_64
BOOST_AUTO_TEST_CASE(Flags_64_constructor)
{
  Flags_64 flag64;
  BOOST_CHECK_EQUAL(0, static_cast<EnumFlags<ePrueba2>::Type>(flag64.flags()));
  for (int i = 0; i < 64; i++)
    BOOST_CHECK_EQUAL(false, flag64.isActive(i));
}

// Constructor copia
BOOST_AUTO_TEST_CASE(Flags_copy_constructor)
{
  Flags_16 flag16;
  flag16.flagOn(3);

  Flags<uint16_t> flag_copy(flag16);
  for (int i = 0; i < 16; i++) {
    if (i == 3)
      BOOST_CHECK(flag_copy.isActive(i));
    else 
      BOOST_CHECK_EQUAL(false, flag_copy.isActive(i));
  }
}

// Construcción con inicialización de lista
BOOST_AUTO_TEST_CASE(Flags_initializer_list_constructor)
{
  Flags_32 flag_list{ 0, 3, 7, 4 };
  BOOST_CHECK(flag_list.isActive(0));
  BOOST_CHECK_EQUAL(false, flag_list.isActive(1));
  BOOST_CHECK_EQUAL(false, flag_list.isActive(2));
  BOOST_CHECK(flag_list.isActive(3));
  BOOST_CHECK(flag_list.isActive(4));
  BOOST_CHECK_EQUAL(false, flag_list.isActive(5));
  BOOST_CHECK_EQUAL(false, flag_list.isActive(6));
  BOOST_CHECK(flag_list.isActive(7));
  
}

BOOST_AUTO_TEST_CASE(Flags_assign)
{
  Flags_8 flag1;
  flag1.flagOn(1);

  Flags_8 flag2 = flag1;

  BOOST_CHECK_EQUAL(false, flag2.isActive(0));
  BOOST_CHECK(flag2.isActive(1));
  BOOST_CHECK_EQUAL(false, flag2.isActive(2));
  BOOST_CHECK_EQUAL(false, flag2.isActive(3));
  BOOST_CHECK_EQUAL(false, flag2.isActive(4));
  BOOST_CHECK_EQUAL(false, flag2.isActive(5));
  BOOST_CHECK_EQUAL(false, flag2.isActive(6));
  BOOST_CHECK_EQUAL(false, flag2.isActive(7));
}

BOOST_AUTO_TEST_CASE(Flags_clear)
{
  Flags_32 flag;
  flag.flagOn(2);
  flag.switchFlag(3);

  BOOST_CHECK(flag.isActive(2));
  BOOST_CHECK(flag.isActive(3));

  flag.clear();

  BOOST_CHECK_EQUAL(false, flag.isActive(2));
  BOOST_CHECK_EQUAL(false, flag.isActive(3));
}

BOOST_AUTO_TEST_CASE(Flags_flagOn)
{
  Flags_64 flag2;
  BOOST_CHECK_EQUAL(false, flag2.isActive(15));
  
  flag2.flagOn(15);

  BOOST_CHECK(flag2.isActive(15));

  //Prueba a activar un flag ya activo
  flag2.flagOn(3);
  BOOST_CHECK(flag2.isActive(3));
}

BOOST_AUTO_TEST_CASE(Flags_flagOff)
{
  Flags_64 flag2{15};

  BOOST_CHECK(flag2.isActive(15));

  flag2.flagOff(15);

  BOOST_CHECK_EQUAL(false, flag2.isActive(15));

  //Prueba a desactivar un flag ya desactivado
  flag2.flagOff(15);
  BOOST_CHECK_EQUAL(false, flag2.isActive(15));
}

BOOST_AUTO_TEST_CASE(Flags_switchFlag) 
{
  Flags_16 flag_16;
  flag_16.switchFlag(15);
  flag_16.switchFlag(6);
  BOOST_CHECK(flag_16.isActive(15));
  BOOST_CHECK(flag_16.isActive(6));
  BOOST_CHECK_EQUAL(false, flag_16.isActive(1));
  BOOST_CHECK_EQUAL(false, flag_16.isActive(3));

  flag_16.switchFlag(15);
  flag_16.switchFlag(6);
  flag_16.switchFlag(1);
  flag_16.switchFlag(3);

  BOOST_CHECK_EQUAL(false, flag_16.isActive(15));
  BOOST_CHECK_EQUAL(false, flag_16.isActive(6));
  BOOST_CHECK(flag_16.isActive(1));
  BOOST_CHECK(flag_16.isActive(3));
}

BOOST_AUTO_TEST_CASE(Flags_bad_data)
{
  Flags_16 flag_16;
  flag_16.flagOn(5);
  flag_16.flagOn(25); // Se asigna un valor fuera de rango
  BOOST_CHECK_EQUAL(false, flag_16.isActive(25));
  BOOST_CHECK(flag_16.isActive(5));
}

