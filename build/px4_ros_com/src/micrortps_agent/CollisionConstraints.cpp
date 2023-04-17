// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*!
 * @file CollisionConstraints.cpp
 * This source file contains the definition of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#ifdef _WIN32
// Remove linker warning LNK4221 on Visual Studio
namespace { char dummy; }
#endif

#include "CollisionConstraints.h"
#include <fastcdr/Cdr.h>

#include <fastcdr/exceptions/BadParamException.h>
using namespace eprosima::fastcdr::exception;

#include <utility>


px4_msgs::msg::CollisionConstraints::CollisionConstraints()
{
    // m_timestamp com.eprosima.idl.parser.typecode.PrimitiveTypeCode@12591ac8
    m_timestamp = 0;
    // m_original_setpoint com.eprosima.idl.parser.typecode.AliasTypeCode@5a7fe64f
    memset(&m_original_setpoint, 0, (2) * 4);
    // m_adapted_setpoint com.eprosima.idl.parser.typecode.AliasTypeCode@5a7fe64f
    memset(&m_adapted_setpoint, 0, (2) * 4);

}

px4_msgs::msg::CollisionConstraints::~CollisionConstraints()
{



}

px4_msgs::msg::CollisionConstraints::CollisionConstraints(const CollisionConstraints &x)
{
    m_timestamp = x.m_timestamp;
    m_original_setpoint = x.m_original_setpoint;
    m_adapted_setpoint = x.m_adapted_setpoint;
}

px4_msgs::msg::CollisionConstraints::CollisionConstraints(CollisionConstraints &&x)
{
    m_timestamp = x.m_timestamp;
    m_original_setpoint = std::move(x.m_original_setpoint);
    m_adapted_setpoint = std::move(x.m_adapted_setpoint);
}

px4_msgs::msg::CollisionConstraints& px4_msgs::msg::CollisionConstraints::operator=(const CollisionConstraints &x)
{

    m_timestamp = x.m_timestamp;
    m_original_setpoint = x.m_original_setpoint;
    m_adapted_setpoint = x.m_adapted_setpoint;

    return *this;
}

px4_msgs::msg::CollisionConstraints& px4_msgs::msg::CollisionConstraints::operator=(CollisionConstraints &&x)
{

    m_timestamp = x.m_timestamp;
    m_original_setpoint = std::move(x.m_original_setpoint);
    m_adapted_setpoint = std::move(x.m_adapted_setpoint);

    return *this;
}

size_t px4_msgs::msg::CollisionConstraints::getMaxCdrSerializedSize(size_t current_alignment)
{
    size_t initial_alignment = current_alignment;


    current_alignment += 8 + eprosima::fastcdr::Cdr::alignment(current_alignment, 8);


    current_alignment += ((2) * 4) + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);


    current_alignment += ((2) * 4) + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);



    return current_alignment - initial_alignment;
}

size_t px4_msgs::msg::CollisionConstraints::getCdrSerializedSize(const px4_msgs::msg::CollisionConstraints& data, size_t current_alignment)
{
    (void)data;
    size_t initial_alignment = current_alignment;


    current_alignment += 8 + eprosima::fastcdr::Cdr::alignment(current_alignment, 8);


    if ((2) > 0)
    {
        current_alignment += ((2) * 4) + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);
    }

    if ((2) > 0)
    {
        current_alignment += ((2) * 4) + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);
    }


    return current_alignment - initial_alignment;
}

void px4_msgs::msg::CollisionConstraints::serialize(eprosima::fastcdr::Cdr &scdr) const
{

    scdr << m_timestamp;
    scdr << m_original_setpoint;

    scdr << m_adapted_setpoint;

}

void px4_msgs::msg::CollisionConstraints::deserialize(eprosima::fastcdr::Cdr &dcdr)
{

    dcdr >> m_timestamp;
    dcdr >> m_original_setpoint;

    dcdr >> m_adapted_setpoint;

}

/*!
 * @brief This function sets a value in member timestamp
 * @param _timestamp New value for member timestamp
 */
void px4_msgs::msg::CollisionConstraints::timestamp(uint64_t _timestamp)
{
m_timestamp = _timestamp;
}

/*!
 * @brief This function returns the value of member timestamp
 * @return Value of member timestamp
 */
uint64_t px4_msgs::msg::CollisionConstraints::timestamp() const
{
    return m_timestamp;
}

/*!
 * @brief This function returns a reference to member timestamp
 * @return Reference to member timestamp
 */
uint64_t& px4_msgs::msg::CollisionConstraints::timestamp()
{
    return m_timestamp;
}

/*!
 * @brief This function copies the value in member original_setpoint
 * @param _original_setpoint New value to be copied in member original_setpoint
 */
void px4_msgs::msg::CollisionConstraints::original_setpoint(const px4_msgs::msg::float__2 &_original_setpoint)
{
m_original_setpoint = _original_setpoint;
}

/*!
 * @brief This function moves the value in member original_setpoint
 * @param _original_setpoint New value to be moved in member original_setpoint
 */
void px4_msgs::msg::CollisionConstraints::original_setpoint(px4_msgs::msg::float__2 &&_original_setpoint)
{
m_original_setpoint = std::move(_original_setpoint);
}

/*!
 * @brief This function returns a constant reference to member original_setpoint
 * @return Constant reference to member original_setpoint
 */
const px4_msgs::msg::float__2& px4_msgs::msg::CollisionConstraints::original_setpoint() const
{
    return m_original_setpoint;
}

/*!
 * @brief This function returns a reference to member original_setpoint
 * @return Reference to member original_setpoint
 */
px4_msgs::msg::float__2& px4_msgs::msg::CollisionConstraints::original_setpoint()
{
    return m_original_setpoint;
}
/*!
 * @brief This function copies the value in member adapted_setpoint
 * @param _adapted_setpoint New value to be copied in member adapted_setpoint
 */
void px4_msgs::msg::CollisionConstraints::adapted_setpoint(const px4_msgs::msg::float__2 &_adapted_setpoint)
{
m_adapted_setpoint = _adapted_setpoint;
}

/*!
 * @brief This function moves the value in member adapted_setpoint
 * @param _adapted_setpoint New value to be moved in member adapted_setpoint
 */
void px4_msgs::msg::CollisionConstraints::adapted_setpoint(px4_msgs::msg::float__2 &&_adapted_setpoint)
{
m_adapted_setpoint = std::move(_adapted_setpoint);
}

/*!
 * @brief This function returns a constant reference to member adapted_setpoint
 * @return Constant reference to member adapted_setpoint
 */
const px4_msgs::msg::float__2& px4_msgs::msg::CollisionConstraints::adapted_setpoint() const
{
    return m_adapted_setpoint;
}

/*!
 * @brief This function returns a reference to member adapted_setpoint
 * @return Reference to member adapted_setpoint
 */
px4_msgs::msg::float__2& px4_msgs::msg::CollisionConstraints::adapted_setpoint()
{
    return m_adapted_setpoint;
}

size_t px4_msgs::msg::CollisionConstraints::getKeyMaxCdrSerializedSize(size_t current_alignment)
{
    size_t current_align = current_alignment;






    return current_align;
}

bool px4_msgs::msg::CollisionConstraints::isKeyDefined()
{
   return false;
}

void px4_msgs::msg::CollisionConstraints::serializeKey(eprosima::fastcdr::Cdr &scdr) const
{
    (void) scdr;
     
     
     
}


