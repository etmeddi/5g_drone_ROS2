#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <iostream>

#include "rclcpp/rclcpp.hpp"

#include <terabee/ITerarangerFactory.hpp>
#include <terabee/ITerarangerMultiflex.hpp>
#include <terabee/DistanceData.hpp>

using terabee::DistanceData;

using namespace std::chrono_literals;

std::ostream &operator<<(std::ostream &os, const DistanceData &d)
{
  os << "[";
  for (size_t i = 0; i < d.distance.size(); i++)
  {
    os << d.distance[i] << ", ";
  }
  os << "\b\b"
     << " ]";
  return os;
}

class MultiflexReader : public rclcpp::Node
{
public:
  MultiflexReader()
      : Node("multiflex_reader")
  {
    factory = terabee::ITerarangerFactory::getFactory();
    multiflex = factory->createTerarangerMultiflex("/dev/ttyACM0");

    if (!multiflex)
    {
      RCLCPP_ERROR(this->get_logger(), "Failed to create multiflex");
      return;
    }

    if (!multiflex->initialize())
    {
      RCLCPP_ERROR(this->get_logger(), "Failed to initialize multiflex");
      return;
    }

    if (!multiflex->configureNumberOfSensors(0x3f))
    {
      RCLCPP_ERROR(this->get_logger(), "Failed to set the number of sensors to 6!");
      return;
    }

    RCLCPP_INFO(this->get_logger(), "Multiflex initialized");

    timer_ = this->create_wall_timer(500ms, std::bind(&MultiflexReader::read_multiflex_data, this));
  }

private:
  void read_multiflex_data()
  {
    std::cout << "Distance = " << multiflex->getDistance() << std::endl;
  }

  std::unique_ptr<terabee::ITerarangerFactory> factory;
  std::unique_ptr<terabee::ITerarangerMultiflex> multiflex;

  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MultiflexReader>());
  rclcpp::shutdown();
  return 0;
}
