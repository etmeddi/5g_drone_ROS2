/**
 * @file px4_controller.cpp
 * @author Sem van der Hoeven (sem.hoeven@gmail.com)
 * @brief Controller node to contol the PX4 using attitude or trajectory setpoints.
 * It subscribes to the /drone/set_attitude topic to receive control commands
 */

#include <chrono>
#include <math.h>

#include "rclcpp/rclcpp.hpp"

#include <px4_msgs/msg/vehicle_attitude_setpoint.hpp>
#include <px4_msgs/msg/trajectory_setpoint.hpp>
#include <px4_msgs/msg/vehicle_command.hpp>
#include <px4_msgs/msg/vehicle_control_mode.hpp>
#include <px4_msgs/msg/vehicle_attitude.hpp>
#include <px4_msgs/msg/vehicle_local_position.hpp>

#include <drone_services/srv/set_attitude.hpp>
#include <drone_services/srv/set_trajectory.hpp>
#include <drone_services/srv/arm_drone.hpp>
#include <drone_services/srv/disarm_drone.hpp>
#include <drone_services/msg/drone_control_mode.hpp>

#include <std_srvs/srv/empty.hpp>

#include "drone_control_modes.h"

#define DEFAULT_YAW_SPEED 0.6 // default turning speed in radians per second
#define D_SPEED(x) -x - 9.81  // a speed up of x m/s has to be ajusted for the earths gravity

using namespace std::chrono_literals;

class PX4Controller : public rclcpp::Node
{
public:
    PX4Controller() : Node("px4_controller")
    {

        rmw_qos_profile_t qos_profile = rmw_qos_profile_sensor_data;
        auto qos = rclcpp::QoS(rclcpp::QoSInitialization(qos_profile.history, 5), qos_profile);

        // create a publisher on the vehicle attitude setpoint topic
        vehicle_setpoint_publisher_ = this->create_publisher<px4_msgs::msg::VehicleAttitudeSetpoint>("/fmu/in/vehicle_attitude_setpoint", 10);
        vehicle_command_publisher_ = this->create_publisher<px4_msgs::msg::VehicleCommand>("/fmu/in/vehicle_command", 10);
        trajectory_setpoint_publisher = this->create_publisher<px4_msgs::msg::TrajectorySetpoint>("/fmu/in/trajectory_setpoint", 10);
        // offboard_control_mode_publisher_ = this->create_publisher<px4_msgs::msg::OffboardControlMode>("/fmu/in/offboard_control_mode", 10);

        vehicle_attitude_subscription_ = this->create_subscription<px4_msgs::msg::VehicleAttitude>("/fmu/out/vehicle_attitude", qos, std::bind(&PX4Controller::on_attitude_receive, this, std::placeholders::_1));
        vehicle_local_position_subscription_ = this->create_subscription<px4_msgs::msg::VehicleLocalPosition>("/fmu/out/vehicle_local_position", qos, std::bind(&PX4Controller::on_local_position_receive, this, std::placeholders::_1));
        control_mode_subscription_ = this->create_subscription<drone_services::msg::DroneControlMode>("/drone/control_mode", qos, std::bind(&PX4Controller::on_control_mode_receive, this, std::placeholders::_1));

        set_attitude_service_ = this->create_service<drone_services::srv::SetAttitude>("/drone/set_attitude", std::bind(&PX4Controller::handle_attitude_setpoint, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        set_trajectory_service_ = this->create_service<drone_services::srv::SetTrajectory>("/drone/set_trajectory", std::bind(&PX4Controller::handle_trajectory_setpoint, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        disarm_service_ = this->create_service<drone_services::srv::DisarmDrone>("/drone/disarm", std::bind(&PX4Controller::handle_disarm_request, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        arm_service_ = this->create_service<drone_services::srv::ArmDrone>("/drone/arm", std::bind(&PX4Controller::handle_arm_request, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        // create timer to send vehicle attitude setpoints every second
        timer_ = this->create_wall_timer(100ms, std::bind(&PX4Controller::send_setpoint, this));

        start_time_ = this->get_clock()->now().seconds();
        RCLCPP_INFO(this->get_logger(), "finished initializing at %f seconds.", start_time_);
    }

private:
    rclcpp::Publisher<px4_msgs::msg::VehicleAttitudeSetpoint>::SharedPtr vehicle_setpoint_publisher_;
    rclcpp::Publisher<px4_msgs::msg::TrajectorySetpoint>::SharedPtr trajectory_setpoint_publisher;
    rclcpp::Publisher<px4_msgs::msg::VehicleCommand>::SharedPtr vehicle_command_publisher_;
    rclcpp::Subscription<px4_msgs::msg::VehicleAttitude>::SharedPtr vehicle_attitude_subscription_;
    rclcpp::Subscription<px4_msgs::msg::VehicleLocalPosition>::SharedPtr vehicle_local_position_subscription_;
    rclcpp::Subscription<drone_services::msg::DroneControlMode>::SharedPtr control_mode_subscription_;

    rclcpp::Service<drone_services::srv::SetAttitude>::SharedPtr set_attitude_service_;
    rclcpp::Service<drone_services::srv::SetTrajectory>::SharedPtr set_trajectory_service_;
    rclcpp::Service<drone_services::srv::DisarmDrone>::SharedPtr disarm_service_;
    rclcpp::Service<drone_services::srv::ArmDrone>::SharedPtr arm_service_;

    // rclcpp::Publisher<px4_msgs::msg::OffboardControlMode>::SharedPtr offboard_control_mode_publisher_;

    rclcpp::TimerBase::SharedPtr timer_;
    double start_time_;
    bool has_sent_status = false;
    bool user_in_control = false; // if user has taken over control
    bool armed = false;
    bool has_swithed = false;
    int setpoint_count = 0;
    float thrust = 0.0;
    bool ready_to_fly = false;
    float cur_yaw = 0;
    bool new_setpoint = false; // for printing new q_d when a new setpoint has been received

    float last_setpoint[3] = {0, 0, 0};
    float last_angle = 0;  // angle in radians (-PI .. PI)
    float last_thrust = 0; // default 10% thrust for when the drone gets armed

    float velocity[3] = {0, 0, 0};
    float position[3] = {0, 0, 0};

    float base_q[4] = {0, 0, 0, 0};
    int base_q_amount = 0;

    char current_control_mode = CONTROL_MODE_ATTITUDE; // start with attitude control

    bool start_trajectory = false;
    const float omega = 0.3; // angular speed of the POLAR trajectory
    const float K = 2;       // [m] gain that regulates the spiral pitch

    const float rho_0 = 2;
    const float theta_0 = 0;
    const float p0_z = -5.0;
    float p0_x = rho_0 * cos(theta_0);
    float p0_y = rho_0 * sin(theta_0);
    float des_x = p0_x, des_y = p0_y, des_z = p0_z;
    float dot_des_x = 0.0, dot_des_y = 0.0;
    float gamma = M_PI_4;

    float X;
    float Y;

    uint32_t discrete_time_index = 0;

    // result quaternion
    std::array<float, 4> q = {0, 0, 0, 0};

    void handle_attitude_setpoint(
        const std::shared_ptr<rmw_request_id_t> request_header,
        const std::shared_ptr<drone_services::srv::SetAttitude::Request> request,
        const std::shared_ptr<drone_services::srv::SetAttitude::Response> response)
    {
        if (armed)
        {
            if (request->yaw == 0 && request->pitch == 0 && request->roll == 0 && request->thrust == 0)
            {
                last_setpoint[0] = degrees_to_radians(request->yaw);
                last_setpoint[1] = degrees_to_radians(request->pitch);
                last_setpoint[2] = degrees_to_radians(request->roll);
                last_thrust = request->thrust;
                RCLCPP_INFO(this->get_logger(), "STOPPING MOTORS");
            }
            else
            {
                last_setpoint[0] += degrees_to_radians(request->yaw);
                last_setpoint[1] += degrees_to_radians(request->pitch);
                last_setpoint[2] += degrees_to_radians(request->roll);
                last_thrust += request->thrust;
                if (last_thrust > 1)
                    last_thrust = 1;
                else if (last_thrust < 0)
                    last_thrust = 0;
            }

            RCLCPP_INFO(this->get_logger(), "got values: yaw:%f pitch:%f roll:%f thrust:%f", request->yaw, request->pitch, request->roll, request->thrust);
            RCLCPP_INFO(this->get_logger(), "New setpoint: yaw:%f pitch:%f roll:%f thrust:%f", last_setpoint[0], last_setpoint[1], last_setpoint[2], last_thrust);

            response->success = true;
        }
        else
        {
            last_thrust = 0;
            last_setpoint[0] = 0;
            last_setpoint[1] = 0;
            last_setpoint[2] = 0;
            response->success = false;
        }
    }

    void handle_trajectory_setpoint(
        const std::shared_ptr<rmw_request_id_t> request_header,
        const std::shared_ptr<drone_services::srv::SetTrajectory::Request> request,
        const std::shared_ptr<drone_services::srv::SetTrajectory::Response> response)
    {
        if (!(request->control_mode == CONTROL_MODE_VELOCITY || request->control_mode == CONTROL_MODE_POSITION))
        {
            RCLCPP_INFO(this->get_logger(), "Got invalid trajectory control mode: %d", request->control_mode);
            response->success = false;
        }
        else
        {
            RCLCPP_INFO(this->get_logger(), "Got new trajectory setpoint with control mode: %d", request->control_mode);
            if (request->control_mode == CONTROL_MODE_VELOCITY)
            {
                for (int i = 0; i < 3; i++)
                {
                    velocity[i] += request->values[i];
                }
                RCLCPP_INFO(this->get_logger(), "Got new velocity setpoint. %f %f %f", velocity[0], velocity[1], velocity[2]);
            }
            else if (request->control_mode == CONTROL_MODE_POSITION)
            {
                for (int i = 0; i < 3; i++)
                {
                    position[i] = request->values[i];
                }
                RCLCPP_INFO(this->get_logger(), "Got new position setpoint: %f %f %f", position[0], position[1], position[2]);
            }

            last_angle = request->yaw;
            new_setpoint = true;
            RCLCPP_INFO(this->get_logger(), "Yaw: %f", last_angle);
            response->success = true;
        }
    }

    /**
     * @brief disarms the drone when a call to the disarm service is made
     *
     * @param request_header the header for the service request
     * @param request the request (empty)
     * @param response the response (empty)
     */
    void handle_disarm_request(
        const std::shared_ptr<rmw_request_id_t> request_header,
        const std::shared_ptr<drone_services::srv::DisarmDrone::Request> request,
        const std::shared_ptr<drone_services::srv::DisarmDrone::Response> response)
    {
        RCLCPP_INFO(this->get_logger(), "Got disarm request...");

        if (armed)
        {
            armed = false;
            user_in_control = false;
            publish_vehicle_command(px4_msgs::msg::VehicleCommand::VEHICLE_CMD_COMPONENT_ARM_DISARM, 0.0, 0);
            response->success = true;
        }
        else
        {
            RCLCPP_ERROR(this->get_logger(), "Got disarm request but drone was not armed!");
            response->success = false;
        }
    }

    /**
     * @brief arms the drone when a call to the arm service is made
     *
     * @param request_header the header for the service request
     * @param request the request (empty)
     * @param response the response (empty)
     */
    void handle_arm_request(
        const std::shared_ptr<rmw_request_id_t> request_header,
        const std::shared_ptr<drone_services::srv::ArmDrone::Request> request,
        const std::shared_ptr<drone_services::srv::ArmDrone::Response> response)
    {
        RCLCPP_INFO(this->get_logger(), "Got arm request...");

        if (!armed)
        {
            this->publish_vehicle_command(px4_msgs::msg::VehicleCommand::VEHICLE_CMD_DO_SET_MODE, 1, 6);
            RCLCPP_INFO(this->get_logger(), "Set to offboard mode");
            // arm the drone
            this->publish_vehicle_command(px4_msgs::msg::VehicleCommand::VEHICLE_CMD_COMPONENT_ARM_DISARM, 1.0, 0);

            RCLCPP_INFO(this->get_logger(), "Arm command sent");
            this->last_thrust = -0.1; // spin motors at 10% thrust
            armed = true;
            response->success = true;
        }
        else
        {
            RCLCPP_ERROR(this->get_logger(), "Arm request received but the drone is already armed!");
            response->success = false;
        }
    }

    void send_attitude_setpoint()
    {

        // set message to enable attitude
        auto msg = px4_msgs::msg::VehicleAttitudeSetpoint();

        // move up?
        msg.thrust_body[0] = 0; // north
        msg.thrust_body[1] = 0; // east
        msg.thrust_body[2] = -last_thrust;

        calculate_quaternion(q, last_setpoint[0], last_setpoint[1], last_setpoint[2]);
        // set quaternion
        msg.q_d[0] = q.at(0);
        msg.q_d[1] = base_q[1] + q.at(1);
        msg.q_d[2] = base_q[2] + q.at(2);
        msg.q_d[3] = base_q[3] + q.at(3);

        msg.yaw_sp_move_rate = 0;
        msg.reset_integral = false;
        msg.fw_control_yaw_wheel = false;

        msg.timestamp = this->get_clock()->now().nanoseconds() / 1000;
        vehicle_setpoint_publisher_->publish(msg);
    }

    void send_velocity_setpoint()
    {
        auto msg = px4_msgs::msg::TrajectorySetpoint();

        msg.velocity[0] = velocity[0];
        msg.velocity[1] = velocity[1];
        msg.velocity[2] = -velocity[2];
        for (int i = 0; i < 3; i++)
        {
            msg.position[i] = NAN;
        }

        publish_trajectory_setpoint(msg);
    }

    void send_position_setpoint()
    {
        auto msg = px4_msgs::msg::TrajectorySetpoint();

        msg.position = {des_x, des_y, des_z};
        msg.velocity = {dot_des_x, dot_des_y, 0.0};
        msg.yaw = gamma; //-3.14; // [-PI:PI]

        msg.timestamp = this->get_clock()->now().nanoseconds() / 1000;
        trajectory_setpoint_publisher->publish(msg);
    }

    void publish_trajectory_setpoint(px4_msgs::msg::TrajectorySetpoint msg)
    {
        msg.yaw = last_angle;
        msg.yawspeed = DEFAULT_YAW_SPEED;
        msg.timestamp = this->get_clock()->now().nanoseconds() / 1000;
        trajectory_setpoint_publisher->publish(msg);
    }

    /**
     * @brief Send setpoints to PX4. First, 20 setpoints will be sent before offboard mode will be engaged and the drone will be armed.
     *
     */
    void send_setpoint()
    {
        // the spiral, in polar coordinates (theta, rho), is given by
        // theta = theta_0 + omega*t
        // rho = rho_0 + K*theta
        float theta = theta_0 + omega * 0.1 * discrete_time_index;
        float rho = rho_0 + K * theta;

        // from polar to cartesian coordinates
        des_x = rho * cos(theta);
        des_y = rho * sin(theta);

        // velocity computation
        float dot_rho = K * omega;
        dot_des_x = dot_rho * cos(theta) - rho * sin(theta) * omega;
        dot_des_y = dot_rho * sin(theta) + rho * cos(theta) * omega;
        // desired heading direction
        gamma = atan2(dot_des_y, dot_des_x);

        if (!user_in_control)
        {
            // RCLCPP_INFO(this->get_logger(), "Sending idle attitude setpoint");
            send_attitude_setpoint();
        }
        else
        {
            if (current_control_mode == CONTROL_MODE_ATTITUDE)
            {
                // RCLCPP_INFO(this->get_logger(), "Sending attitude setpoint");
                send_attitude_setpoint();
            }
            else
            {
                if (!new_setpoint)
                {
                    return;
                }
                if (current_control_mode == CONTROL_MODE_VELOCITY)
                {
                    // RCLCPP_INFO(this->get_logger(), "Sending velocity setpoint");
                    send_velocity_setpoint();
                }
                else if (current_control_mode == CONTROL_MODE_POSITION)
                {
                    // RCLCPP_INFO(this->get_logger(), "Sending position setpoint");
                    send_position_setpoint();
                }
                new_setpoint = false;
            }
        }
        if (start_trajectory)
        {
            discrete_time_index++;
        }
    }

    void on_attitude_receive(const px4_msgs::msg::VehicleAttitude::SharedPtr msg)
    {
        /*
        average q:
        - 0.7313786745071411
        - 0.005042835604399443
        - 0.0002370707516092807
        - 0.6819528937339783
        */
        if (!armed)
        {
            if (base_q_amount > 2)
            {
                base_q[1] = (base_q[1] + msg->q[1]) / 2;
                base_q[2] = (base_q[2] + msg->q[2]) / 2;
                base_q[3] = (base_q[3] + msg->q[3]) / 2;
            }
            else
            {
                base_q[1] = msg->q[1];
                base_q[2] = msg->q[2];
                base_q[3] = msg->q[3];
                base_q_amount++;
            }

            RCLCPP_INFO(this->get_logger(), "base_q: %f %f %f %f", base_q[0], base_q[1], base_q[2], base_q[3]);
        }
    }

    void on_local_position_receive(const px4_msgs::msg::VehicleLocalPosition::SharedPtr msg)
    {
        X = msg->x;
        Y = msg->y;
        float Z = msg->z;
        if (!start_trajectory && (p0_x + 1.0 > X && p0_x - 1.0 < X) && (p0_y + 1.0 > Y && p0_y - 1.0 < Y) && (p0_z + 1.0 > Z && p0_z - 1.0 < Z))
        {
            start_trajectory = true;
            RCLCPP_INFO(this->get_logger(), "start trajectory");
        }
    }

    void on_control_mode_receive(const drone_services::msg::DroneControlMode::SharedPtr msg)
    {
        if (msg->control_mode >= CONTROL_MODE_MIN && msg->control_mode <= CONTROL_MODE_MAX)
        {
            current_control_mode = msg->control_mode;
            RCLCPP_INFO(this->get_logger(), "Got valid control mode");
            user_in_control = true; // user has taken over control
        }
        else
        {
            RCLCPP_ERROR(this->get_logger(), "Received invalid control mode %d", msg->control_mode);
        }
        RCLCPP_INFO(this->get_logger(), "Control mode set to %d", current_control_mode);
    }

    /**
     * @brief Publish vehicle commands
     * @param command   Command code (matches VehicleCommand and MAVLink MAV_CMD codes)
     * @param param1    Command parameter 1
     * @param param2    Command parameter 2
     */
    void publish_vehicle_command(uint16_t command, float param1, float param2)
    {
        px4_msgs::msg::VehicleCommand msg{};
        msg.param1 = param1;
        msg.param2 = param2;
        msg.command = command;
        msg.target_system = 1;
        msg.target_component = 1;
        msg.source_system = 1;
        msg.source_component = 1;
        msg.from_external = true;
        msg.timestamp = this->get_clock()->now().nanoseconds() / 1000;
        vehicle_command_publisher_->publish(msg);
    }

    /**
     * @brief calculates a quaternion based on the given input values
     *
     * @param ptr array of result quaternion. Must be of size 4.
     * @param heading desired heading (yaw) in radians.
     * @param attitude desired attitude (pitch) in radians.
     * @param bank desired bank (roll) in radians.
     */
    static void calculate_quaternion(std::array<float, 4> &ptr, float heading, float attitude, float bank)
    {

        float c1 = cos(heading / 2);
        float c2 = cos(attitude / 2);
        float c3 = cos(bank / 2);
        float s1 = sin(heading / 2);
        float s2 = sin(attitude / 2);
        float s3 = sin(bank / 2);

        float c1c2 = c1 * c2;
        float s1s2 = s1 * s2;

        ptr.at(0) = c1c2 * c3 - s1s2 * s3;       // w
        ptr.at(1) = c1c2 * s3 + s1s2 * c3;       // x
        ptr.at(2) = s1 * c2 * c3 + c1 * s2 * s3; // y
        ptr.at(3) = c1 * s2 * c3 - s1 * c2 * s3; // z
    }

    /**
     * @brief converts degrees to radians
     *
     * @param deg angle in degrees
     * @return float new angle in radians
     */
    static float degrees_to_radians(float deg)
    {
        return deg * (M_PI / 180.0);
    }
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::Node::SharedPtr node = std::make_shared<PX4Controller>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
