#include <gtest/gtest.h>
#include "core/utils.h"

using namespace TL;


//TEST(isDirectoryTest, Expect)
//{
//  //EXPECT_TRUE(isDirectory("D:/Desarrollo"));
//  //EXPECT_TRUE(isDirectory("D:/Desarrollo/tidop/test"));
//  //EXPECT_TRUE(isDirectory("D:\\Desarrollo"));
//  //EXPECT_TRUE(isDirectory("D:\\Desarrollo\\tidop\\test"));
//  //EXPECT_FALSE(isDirectory("D:/Desarrollo_"));
//  //EXPECT_FALSE(isDirectory("D:/Desarrollo/no_existe_dir"));
//  //EXPECT_FALSE(isDirectory("D:\\Desarrollo_"));
//  //EXPECT_FALSE(isDirectory("D:\\Desarrollo\\no_existe_dir"));
//
//  //// Con barra final
//  //EXPECT_TRUE(isDirectory("D:/Desarrollo/"));
//  //EXPECT_TRUE(isDirectory("D:/Desarrollo/tidop/test/"));
//  //EXPECT_TRUE(isDirectory("D:\\Desarrollo\\"));
//  //EXPECT_TRUE(isDirectory("D:\\Desarrollo\\tidop\\test\\"));
//
//  //// Con espacios
//  //EXPECT_TRUE(isDirectory("D:/Desarrollo/Nueva carpeta"));
//  //EXPECT_TRUE(isDirectory("D:\\Desarrollo\\Nueva carpeta"));
//  //EXPECT_TRUE(isDirectory("D:/Desarrollo/Nueva carpeta/"));
//  //EXPECT_TRUE(isDirectory("D:\\Desarrollo\\Nueva carpeta\\"));
//
//}
//
//TEST(isDirectoryTest, Null)
//{
//  EXPECT_FALSE(isDirectory(""));
//}
//
//TEST(createDir, ExistDir)
//{
//  char name[TL_MAX_PATH];
//  EXPECT_EQ(0, getFileDriveDir(getRunfile(), name, TL_MAX_PATH));
//  //Se prueba a crear un directorio ya existente
//  EXPECT_EQ(1, createDir(name));
//}
//
////TEST(createDir, NewDir){
////  //Borro por si acaso existia
////  deleteDir("D:\\Desarrollo\\tidop\\test\\create_dir");
////  EXPECT_EQ(0,createDir("D:\\Desarrollo\\tidop\\test\\create_dir\\1"));
////  EXPECT_EQ(0,createDir("D:/Desarrollo/tidop/test/create_dir/1/2"));
////}
////
////TEST(DeleteDir, ExistDir){
////  EXPECT_EQ(0,deleteDir("D:\\Desarrollo\\tidop\\test\\create_dir\\1\\2"));
////  EXPECT_EQ(0,deleteDir("D:/Desarrollo/tidop/test/create_dir"));
////}
////
////TEST(DeleteDir, NoDir){
////  EXPECT_EQ(1,deleteDir("D:\\Desarrollo\\tidop\\test\\create_dir"));
////}
//
///* ---------------------------------------------------------------------------------- */
///*                        Test Operaciones con cadenas                                */
///* ---------------------------------------------------------------------------------- */
//
//TEST(splitToNumbers, NullString){
//  std::vector<int> vOut;
//  EXPECT_EQ(0, splitToNumbers("", vOut));
//  EXPECT_EQ(0, vOut.size());
//  std::vector<double> vOutD;
//  EXPECT_EQ(0, splitToNumbers("", vOutD));
//  EXPECT_EQ(0, vOutD.size());
//}
//
//
//TEST(splitToNumbers, DefaultSeparatorCharacter)
//{
//  std::vector<int> vOut;
//  EXPECT_EQ(0, splitToNumbers("1,2,3,4", vOut));
//  EXPECT_EQ(4, vOut.size());
//  EXPECT_EQ(1, vOut[0]);
//  EXPECT_EQ(2, vOut[1]);
//  EXPECT_EQ(3, vOut[2]);
//  EXPECT_EQ(4, vOut[3]);
//  vOut.resize(0);
//  EXPECT_EQ(0, splitToNumbers("0,5,6", vOut));
//  EXPECT_EQ(3, vOut.size());
//  EXPECT_EQ(0, vOut[0]);
//  EXPECT_EQ(5, vOut[1]);
//  EXPECT_EQ(6, vOut[2]);
//  std::vector<double> vOutD;
//  EXPECT_EQ(0, splitToNumbers("1.0,3.5,5.2", vOutD));
//  EXPECT_EQ(3, vOutD.size());
//  EXPECT_EQ(1.0, vOutD[0]);
//  EXPECT_EQ(3.5, vOutD[1]);
//  EXPECT_EQ(5.2, vOutD[2]);
//  vOutD.resize(0);
//  EXPECT_EQ(0, splitToNumbers("234.6,235253.6", vOutD));
//  EXPECT_EQ(2, vOutD.size());
//  EXPECT_EQ(234.6, vOutD[0]);
//  EXPECT_EQ(235253.6, vOutD[1]);
//}
//
//TEST(splitToNumbers, CustomSeparatorCharacter)
//{
//  std::vector<int> vOut;
//  EXPECT_EQ(0, splitToNumbers("1#2#3#4", vOut, "#"));
//  EXPECT_EQ(4, vOut.size());
//  EXPECT_EQ(1, vOut[0]);
//  EXPECT_EQ(2, vOut[1]);
//  EXPECT_EQ(3, vOut[2]);
//  EXPECT_EQ(4, vOut[3]);
//  vOut.resize(0);
//  EXPECT_EQ(0, splitToNumbers("0-5-6", vOut, "-"));
//  EXPECT_EQ(3, vOut.size());
//  EXPECT_EQ(0, vOut[0]);
//  EXPECT_EQ(5, vOut[1]);
//  EXPECT_EQ(6, vOut[2]);
//  std::vector<double> vOutD;
//  EXPECT_EQ(0, splitToNumbers("1.0 3.5 5.2", vOutD, " "));
//  EXPECT_EQ(3, vOutD.size());
//  EXPECT_EQ(1.0, vOutD[0]);
//  EXPECT_EQ(3.5, vOutD[1]);
//  EXPECT_EQ(5.2, vOutD[2]);
//  vOutD.resize(0);
//  EXPECT_EQ(0, splitToNumbers("234.6@235253.6", vOutD, "@"));
//  EXPECT_EQ(2, vOutD.size());
//  EXPECT_EQ(234.6, vOutD[0]);
//  EXPECT_EQ(235253.6, vOutD[1]);
//
//  vOut.resize(0);
//  EXPECT_EQ(1, splitToNumbers("345@45", vOut, ","));
//  EXPECT_EQ(0, vOut.size());
//  vOutD.resize(0);
//  EXPECT_EQ(1, splitToNumbers("234.6@235253.6", vOutD, ","));
//  EXPECT_EQ(0, vOutD.size());
//}
//
//TEST(replaceString, test1)
//{
//
//  std::string cadena = "Prueba cadena";
//  replaceString(&cadena, " ", "_");
//  EXPECT_STREQ("Prueba_cadena", cadena.c_str());
//  replaceString(&cadena, " ", " ");
//  ASSERT_STRNE("Prueba cadena", cadena.c_str());
//}
//
//TEST(getFileDir, ValidDir)
//{
//
//  const char *file = "D:\\path\\file.ext";
//  char file_dir[TL_MAX_PATH];
//  EXPECT_EQ(0, getFileDir(file, file_dir, TL_MAX_PATH));
//  EXPECT_STREQ("\\path\\", file_dir);
//  const char *file2 = "D:/path/file.ext";
//  EXPECT_EQ(0, getFileDir(file2, file_dir, TL_MAX_PATH));
//  EXPECT_STREQ("/path/", file_dir);
//}
//
//TEST(getFileDir, InvalidDir)
//{
//
//  const char *file = "";
//  char file_dir[TL_MAX_PATH];
//  EXPECT_EQ(0, getFileDir(file, file_dir, TL_MAX_PATH));
//  EXPECT_STREQ("", file_dir);
//  const char *file2 = "file.ext";
//  EXPECT_EQ(0, getFileDir(file2, file_dir, TL_MAX_PATH));
//  EXPECT_STREQ("", file_dir);
//}
//
//TEST(getFileDrive, ValidDir)
//{
//
//  const char *path = "D:\\path\\file.ext";
//  char drive[TL_MAX_PATH];
//  EXPECT_EQ(0, getFileDrive(path, drive, TL_MAX_PATH));
//  EXPECT_STREQ("D:", drive);
//  const char *file2 = "D:/path/file.ext";
//  EXPECT_EQ(0, getFileDrive(file2, drive, TL_MAX_PATH));
//  EXPECT_STREQ("D:", drive);
//}
//
//TEST(getFileExtension, ValidExtension){
//
//  const char *path = "D:\\path\\file.ext";
//  char ext[TL_MAX_PATH];
//  EXPECT_EQ(0, getFileExtension(path, ext, TL_MAX_PATH));
//  EXPECT_STREQ(".ext", ext);
//  const char *file2 = "D:/path/file.ext";
//  EXPECT_EQ(0, getFileExtension(file2, ext, TL_MAX_PATH));
//  EXPECT_STREQ(".ext", ext);
//}
//
//TEST(getFileExtension, InvalidExtension){
//
//  const char *path = "D:\\path\\file";
//  char ext[TL_MAX_PATH];
//  EXPECT_EQ(0, getFileExtension(path, ext, TL_MAX_PATH));
//  EXPECT_STREQ("", ext);
//  const char *file2 = "D:/path/file";
//  EXPECT_EQ(0, getFileExtension(file2, ext, TL_MAX_PATH));
//  EXPECT_STREQ("", ext);
//}
//
//TEST(getFileName, ValidName)
//{
//  const char *path = "D:\\path\\file.ext";
//  char name[TL_MAX_PATH];
//  EXPECT_EQ(0, getFileName(path, name, TL_MAX_PATH));
//  EXPECT_STREQ("file", name);
//  const char *path2 = "D:/path/file.ext";
//  EXPECT_EQ(0, getFileName(path2, name, TL_MAX_PATH));
//  EXPECT_STREQ("file", name);
//}
//
//TEST(getFileDriveDir, ValidPath)
//{
//  const char *path = "D:\\path\\file.ext";
//  char name[TL_MAX_PATH];
//  EXPECT_EQ(0, getFileDriveDir(path, name, TL_MAX_PATH));
//  EXPECT_STREQ("D:\\path\\", name);
//  const char *path2 = "D:/path/file.ext";
//  EXPECT_EQ(0, getFileDriveDir(path2, name, TL_MAX_PATH));
//  EXPECT_STREQ("D:/path/", name);
//}
//
//TEST(changeFileName, ValidPath)
//{
//  const char *path = "D:\\path\\file.ext";
//  char new_path[TL_MAX_PATH];
//  EXPECT_EQ(0, changeFileName(path, "new_file_name", new_path, TL_MAX_PATH));
//  EXPECT_STREQ("D:\\path\\new_file_name.ext", new_path);
//  const char *path2 = "D:/path/file.ext";
//  EXPECT_EQ(0, changeFileName(path2, "new_file_name", new_path, TL_MAX_PATH));
//  EXPECT_STREQ("D:/path/new_file_name.ext", new_path);
//}
//
//TEST(changeFileExtension, ValidPath)
//{
//  const char *path = "D:\\path\\file.ext";
//  char new_path[TL_MAX_PATH];
//  EXPECT_EQ(0, changeFileExtension(path, "aaa", new_path, TL_MAX_PATH));
//  EXPECT_STREQ("D:\\path\\file.aaa", new_path);
//  const char *path2 = "D:/path/file.ext";
//  EXPECT_EQ(0, changeFileExtension(path2, "bbb", new_path, TL_MAX_PATH));
//  EXPECT_STREQ("D:/path/file.bbb", new_path);
//}
//
//TEST(changeFileNameAndExtension, ValidPath)
//{
//  const char *path = "D:\\path\\file.ext";
//  char new_path[TL_MAX_PATH];
//  EXPECT_EQ(0, changeFileNameAndExtension(path, "new_name.aaa", new_path, TL_MAX_PATH));
//  EXPECT_STREQ("D:\\path\\new_name.aaa", new_path);
//  const char *path2 = "D:/path/file.ext";
//  EXPECT_EQ(0, changeFileNameAndExtension(path2, "new_name.bbb", new_path, TL_MAX_PATH));
//  EXPECT_STREQ("D:/path/new_name.bbb", new_path);
//}
//
//TEST(split, NullString)
//{
//  std::vector<std::string> out;
//  EXPECT_EQ(0, split("", out));
//  EXPECT_EQ(0, out.size());
//}
//
//
//TEST(split, DefaultSeparatorCharacter)
//{
//  std::vector<std::string> out;
//  EXPECT_EQ(0, split("cad1,cad2,cad3", out));
//  EXPECT_EQ(3, out.size());
//  EXPECT_EQ("cad1", out[0]);
//  EXPECT_EQ("cad2", out[1]);
//  EXPECT_EQ("cad3", out[2]);
//}
//
//
//TEST(split, CustomSeparatorCharacter)
//{
//  std::vector<std::string> out;
//  EXPECT_EQ(0, split("cad1#cad2#cad3", out, "#"));
//  EXPECT_EQ(3, out.size());
//  EXPECT_EQ("cad1", out[0]);
//  EXPECT_EQ("cad2", out[1]);
//  EXPECT_EQ("cad3", out[2]);
//}
//
//
//TEST(Path, DefaultConstructor)
//{
//  Path path;
//  // Path esta vacio con lo cual el path actual tiene que estar vacio
//  std::vector<std::string> _path = path.currentPath();
//  EXPECT_EQ(0, _path.size());
//  //No tiene que dar error...
//  path.down();
//  path.up();
//}
//
//TEST(Path, Constructor)
//{
//  Path path1("D:\\Desarrollo\\Libs");
//  std::vector<std::string> _path = path1.currentPath();
//  EXPECT_EQ(3, _path.size());
//  Path path2("D:/Desarrollo/Libs/");
//  std::vector<std::string> _path2 = path2.currentPath();
//  EXPECT_EQ(3, _path2.size());
//}

//TEST(Path, CopyConstructor){
//
//}
//
//TEST(Path, parse){
//
//}


