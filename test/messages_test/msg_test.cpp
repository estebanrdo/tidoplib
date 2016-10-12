#include <gtest/gtest.h>
#include "..\..\src\core\messages.h"
#include "..\..\src\utils.h"

using namespace I3D;


TEST(Message, Default) {

  // Valores por defecto
  EXPECT_EQ(MessageLevel::MSG_ERROR,Message::get().getMessageLevel());
  EXPECT_EQ(MessageOutput::MSG_CONSOLE,Message::get().getMessageOutput());

  //Print por defecto
  Message::message("%s", "Prueba defecto").print();
  Message::setMessageLevel(MessageLevel::MSG_DEBUG);

  Message::message("%s", "Prueba Warning").print(MessageLevel::MSG_WARNING, MessageOutput::MSG_CONSOLE | MessageOutput::MSG_LOG);
  
  consolePrintDebug("%s", "Prueba consola Debug");
  consolePrintVerbose("%s", "Prueba consola Verbose");
  consolePrintInfo("%s", "Prueba consola Info");
  consolePrintWarning("%s", "Prueba consola Warning");
  consolePrintError("%s", "Prueba consola Error");
  
  logPrintDebug("%s", "Prueba log Debug");
  logPrintVerbose("%s", "Prueba log Verbose");
  logPrintInfo("%s", "Prueba log Info");
  logPrintWarning("%s", "Prueba log Warning");
  logPrintError("%s", "Prueba log Error");
}

GTEST_API_ int main(int argc, char **argv) {
  printf("Running main() from gtest_main.cc\n");
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
