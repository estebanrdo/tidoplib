#include <stdio.h>
#include <memory>
#include <iomanip>

#include <tidop/core/console.h>
#include <tidop/core/messages.h>
#include <tidop/geospatial/crs.h>
#include <tidop/img/imgreader.h>
#include <tidop/vect/vectwriter.h>
#include <tidop/graphic/layer.h>
#include <tidop/graphic/entities/polygon.h>
#include <tidop/graphic/datamodel.h>
#include <tidop/geospatial/camera.h>
#include <tidop/geospatial/photo.h>
#include <tidop/geospatial/footprint.h>
#include <tidop/geospatial/util.h>
#include <tidop/vect/vectreader.h> // Para la lectura de la huella de vuelo
// filesystem
#if (__cplusplus >= 201703L)
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#endif

using namespace tl;
using namespace geospatial;

int main(int argc, char** argv)
{

  fs::path app_path = argv[0];
  std::string cmd_name = app_path.stem().string();

  // Consola
  Console &console = Console::instance();
  console.setTitle(cmd_name);
  console.setConsoleUnicode();
  console.setFontHeight(14);
  console.setLogLevel(MessageLevel::msg_verbose);
  MessageManager::instance().addListener(&console);

  std::string bundle_file;
  std::string image_list;
  std::string image_path;
  std::string crs;
  std::string mdt;
  std::string footprint_file;
  std::string offset_file;
  double cx = 0.;
  double cy = 0.;

  Command cmd(cmd_name, "Huella de vuelo");
  cmd.push_back(std::make_shared<ArgumentStringRequired>("bundle_file", 'b', "Fichero bundle", &bundle_file));
  cmd.push_back(std::make_shared<ArgumentStringRequired>("image_list", 'i', "Listado de imagenes", &image_list));
  cmd.push_back(std::make_shared<ArgumentStringOptional>("image_path", 'p', "Ruta de imagenes", &image_path));
  cmd.push_back(std::make_shared<ArgumentStringRequired>("crs", 'c', "Código EPSG", &crs));
  cmd.push_back(std::make_shared<ArgumentStringRequired>("mdt", 'm', "Modelo digital del terreno", &mdt));
  cmd.push_back(std::make_shared<ArgumentStringRequired>("footprint_file", 'f', "Fichero Shapefile con la huella de vuelo", &footprint_file));
  cmd.push_back(std::make_shared<ArgumentStringOptional>("offset_file", "Fichero con el offset a aplicar a las cámaras", &offset_file));
  cmd.push_back(std::make_shared<ArgumentDoubleOptional>("cx", "Punto principal x. Por defecto la mitad de la anchura de las imágenes", &cx));
  cmd.push_back(std::make_shared<ArgumentDoubleOptional>("cy", "Punto principal y. Por defecto la mitad de la altura de las imágenes", &cy));

  cmd.addExample(cmd_name + " --bundle_file bundle.rd.out --image_list bundle.rd.out.list.txt --image_path visualize --crs EPSG:25830 --mdt mdt.tif --footprint_file footprint.shp");

  Command::Status status = cmd.parse(argc, argv);
  if (status == Command::Status::parse_error ) {
    return 1;
  } else if (status == Command::Status::show_help) {
    return 0;
  } else if (status == Command::Status::show_licence) {
    return 0;
  } else if (status == Command::Status::show_version) {
    return 0;
  }

  try {

    /// Lectura del offset
    
    Point3D offset; // (272021.250, 4338368.076, 379.370);

    {
      std::ifstream ifs;
      ifs.open(offset_file, std::ifstream::in);
      if (ifs.is_open()) {
      
        ifs >> offset.x >> offset.y >> offset.z;

        ifs.close();
      }
    }

    /// Carga de imagenes 

    if (!fs::exists(image_list)) throw std::runtime_error("Image list not found");

    std::vector<std::string> images;

    std::ifstream ifs;
    ifs.open(image_list, std::ifstream::in);
    if (ifs.is_open()) {

      std::string line;

      while (std::getline(ifs, line)) {
        
        if (fs::exists(line)) {
          images.push_back(line);
        } else {
          std::string image = std::string(image_path).append("\\").append(line);
          
          if (fs::exists(image)) {
            images.push_back(image);
          } else {
            std::string err = "Image not found: ";
            err.append(image);
            throw std::runtime_error(err.c_str());
          }

        }
        

      }

      ifs.close();
    }

    /// Fin carga de imagenes 

    std::vector<Photo> photos;
    

    /// Lectura de fichero bundle

    std::unique_ptr<ImageReader> imageReader;

    if (!fs::exists(bundle_file)) throw std::runtime_error("Bundle file not found");

    ifs.open(bundle_file, std::ifstream::in);
    if (ifs.is_open()) {
    
      std::string line;
      std::getline(ifs, line); // Salto primera linea

      std::getline(ifs, line);

      std::stringstream ss(line);

      int camera_count;
      int feature_count;
      ss >> camera_count >> feature_count;

      TL_ASSERT(camera_count == images.size(), "ERROR");

      for (size_t i = 0; i < camera_count; i++) {
      //while (std::getline(ifs, line)) {
        
        imageReader = ImageReaderFactory::createReader(images[i]);
        imageReader->open();
        int width = 0;
        int height = 0;
        if (imageReader->isOpen()) {
          height = imageReader->rows();
          width = imageReader->cols();
        }


        std::getline(ifs, line);
        ss.str(line);
        ss.clear();

        double focal;
        double k1;
        double k2;
        ss >> focal >> k1 >> k2;

        TL_TODO("¿Necesito algo de Camera o sólo de Calibration?")
        Camera camera;
        //camera.setMake("SONY");
        //camera.setModel("ILCE-6000");
        //camera.setFocal(16);
        camera.setHeight(height);
        camera.setWidth(width);
        camera.setType("Radial");
        //camera.setSensorSize(23.5);
        std::shared_ptr<Calibration> calibration = CalibrationFactory::create(camera.type());
        calibration->setParameter(Calibration::Parameters::focal, focal);
        if (cx == 0. && cy == 0.) {
          cx = width / 2;
          cy = height / 2;
        } 
        calibration->setParameter(Calibration::Parameters::cx, cx);        
        calibration->setParameter(Calibration::Parameters::cy, cy);
        calibration->setParameter(Calibration::Parameters::k1, k1);
        calibration->setParameter(Calibration::Parameters::k2, k2);
        camera.setCalibration(calibration);

        std::getline(ifs, line);
        ss.str(line);
        ss.clear();
        double r00;
        double r01;
        double r02;
        ss >> r00 >> r01 >> r02;
        std::getline(ifs, line);
        ss.str(line);
        ss.clear();
        double r10;
        double r11;
        double r12;
        ss >> r10 >> r11 >> r12;
        std::getline(ifs, line);
        ss.str(line);
        ss.clear();
        double r20;
        double r21;
        double r22;
        ss >> r20 >> r21 >> r22;

        math::RotationMatrix<double> rotation_matrix;
        rotation_matrix.at(0, 0) = r00;
        rotation_matrix.at(0, 1) = r01;
        rotation_matrix.at(0, 2) = r02;
        rotation_matrix.at(1, 0) = r10;
        rotation_matrix.at(1, 1) = r11;
        rotation_matrix.at(1, 2) = r12;
        rotation_matrix.at(2, 0) = r20;
        rotation_matrix.at(2, 1) = r21;
        rotation_matrix.at(2, 2) = r22;

        std::getline(ifs, line);
        ss.str(line);
        ss.clear();
        double tx;
        double ty;
        double tz;
        ss >> tx >> ty >> tz;
        
        //Point3D position(tx, ty, tz);
        //Point3D position(-5.7208 + 272021.61, -17.8296 + 4338369.137, 0.166741 + 314.874);
        //Point3D offset(272021.250, 4338368.076, 379.370);
        Point3D position;

        // Paso de la transformación de mundo a imagen a imagen mundo

        math::RotationMatrix<double> rotation_transpose = rotation_matrix.transpose();

        position.x = -(rotation_transpose.at(0, 0) * tx +
                       rotation_transpose.at(0, 1) * ty +
                       rotation_transpose.at(0, 2) * tz) + offset.x;
        position.y = -(rotation_transpose.at(1, 0) * tx +
                       rotation_transpose.at(1, 1) * ty +
                       rotation_transpose.at(1, 2) * tz) + offset.y;
        position.z = -(rotation_transpose.at(2, 0) * tx +
                       rotation_transpose.at(2, 1) * ty +
                       rotation_transpose.at(2, 2) * tz) + offset.z;


        Photo::Orientation orientation(position, rotation_matrix);
        Photo photo(images[i]);
        photo.setCamera(camera);
        photo.setOrientation(orientation);
        //if (images[i].compare("C:\\Users\\esteban\\Documents\\Inspector\\Projects\\Madrigalejo\\images\\image_2020-08-04 12_45_42.jpg") == 0)
          photos.push_back(photo);
      }

      ifs.close();
    }

    /// Fin lectura de fichero bundle

    Footprint footprint(mdt);
    footprint.run(photos, footprint_file);

  } catch (const std::exception &e) {
    msgError(e.what());
  } 



  /// Prueba de lectura de huella de vuelo y selección de los fotogramas que contienen un punto

  PointD pt(272013.665, 4338378.212);
  std::map<double, std::shared_ptr<graph::GPolygon>> entities;
  int n_best_entities = 5;
  std::unique_ptr<VectorReader> vectorReader = VectorReaderFactory::createReader(footprint_file);
  vectorReader->open();
  if (vectorReader->isOpen()) {

    size_t size = vectorReader->layersCount();

    if (size >= 1) {
      std::shared_ptr<graph::GLayer> layer = vectorReader->read(0);

      for (const auto &entity : *layer) {
        graph::GraphicEntity::Type type = entity->type();
        if (type == graph::GraphicEntity::Type::polygon_2d) {

          std::shared_ptr<graph::GPolygon> polygon = std::dynamic_pointer_cast<graph::GPolygon>(entity);
          if (polygon->isInner(pt)) {
            PointD center = polygon->window().center();
            double distance = tl::distance(center, pt);
            entities[distance] = polygon;
          }

        } else {
          msgError("No es un fichero de huella de vuelo");
          break;
        }
        
      }
    }
    
    vectorReader->close();
  }

  //int entities_contain_point = entities.size();
  //for (auto entity : entities) {
  //  if (n_best_entities == 0) break;
  //  //msgInfo("distance: %lf", entity.first);
  //  std::shared_ptr<graph::GPolygon> polygon = entity.second;
  //  std::shared_ptr<experimental::TableRegister> data = polygon->data();
  //  for (size_t i = 0; i < data->size(); i++) {
  //    std::string value = data->value(i);
  //    msgInfo("image: %s", value.c_str());
  //  }
  //  --n_best_entities;
  //}

  return 0;
}
