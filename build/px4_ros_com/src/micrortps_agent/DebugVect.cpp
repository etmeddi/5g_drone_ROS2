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
 * @file DebugVect.cpp
 * This source file contains the definition of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#ifdef _WIN32
// Remove linker warning LNK4221 on Visual Studio
namespace { char dummy; }
#endif

#include "DebugVect.h"
#include <fastcdr/Cdr.h>

#include <fastcdr/exceptions/BadParamException.h>
using namespace eprosima::fastcdr::exception;

#include <utility>


px4_msgs::msg::DebugVect::DebugVect()
{
    // m_timestamp com.eprosima.idl.parser.typecode.PrimitiveTypeCode@305b7c14
    m_timestamp = 0;
    // m_name com.eprosima.idl.parser.typecode.AliasTypeCode@6913c1fb
    memset(&m_name, 0, (10) * 1);
    // m_x com.eprosima.idl.parser.typecode.PrimitiveTypeCode@66d18979
    m_x = 0.0;
    // m_y com.eprosima.idl.parser.typecode.PrimitiveTypeCode@bccb269
    m_y = 0.0;
    // m_z com.eprosima.idl.parser.typecode.PrimitiveTypeCode@609cd4d8
    m_z = 0.0;

}

px4_msgs::msg::DebugVect::~DebugVect()
{





}

px4_msgs::msg::DebugVect::DebugVect(const DebugVect &x)
{
    m_timestamp = x.m_timestamp;
    m_name = x.m_name;
    m_x = x.m_x;
    m_y = x.m_y;
    m_z = x.m_z;
}

px4_msgs::msg::DebugVect::DebugVect(DebugVect &&x)
{
    m_timestamp = x.m_timestamp;
    m_name = std::move(x.m_name);
    m_x = x.m_x;
    m_y = x.m_y;
    m_z = x.m_z;
}

px4_msgs::msg::DebugVect& px4_msgs::msg::DebugVect::operator=(const DebugVect &x)
{

    m_timestamp = x.m_timestamp;
    m_name = x.m_name;
    m_x = x.m_x;
    m_y = x.m_y;
    m_z = x.m_z;

    return *this;
}

px4_msgs::msg::DebugVect& px4_msgs::msg::DebugVect::operator=(DebugVect &&x)
{

    m_timestamp = x.m_timestamp;
    m_name = std::move(x.m_name);
    m_x = x.m_x;
    m_y = x.m_y;
    m_z = x.m_z;

    return *this;
}

size_t px4_msgs::msg::DebugVect::getMaxCdrSerializedSize(size_t current_alignment)
{
    size_t initial_alignment = current_alignment;


    current_alignment += 8 + eprosima::fastcdr::Cdr::alignment(current_alignment, 8);


    current_alignment += ((10) * 1) + eprosima::fastcdr::Cdr::alignment(current_alignment, 1);


    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);


    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);


    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);



    return current_alignment - initial_alignment;
}

size_t px4_msgs::msg::DebugVect::getCdrSerializedSize(const px4_msgs::msg::DebugVect& data, size_t current_alignment)
{
    (void)data;
    size_t initial_alignment = current_alignment;


    current_alignment += 8 + eprosima::fastcdr::Cdr::alignment(current_alignment, 8);


    if ((10) > 0)
    {
        current_alignment += ((10) * 1) + eprosima::fastcdr::Cdr::alignment(current_alignment, 1);
    }

    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);


    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);


    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);



    return current_alignment - initial_alignment;
}

void px4_msgs::msg::DebugVect::serialize(eprosima::fastcdr::Cdr &scdr) const
{

    scdr << m_timestamp;
    scdr << m_name;

    scdr << m_x;
    scdr << m_y;
    scdr << m_z;
}

void px4_msgs::msg::DebugVect::deserialize(eprosima::fastcdr::Cdr &dcdr)
{

    dcdr >> m_timestamp;
    dcdr >> m_name;

    dcdr >> m_x;
    dcdr >> m_y;
    dcdr >> m_z;
}

/*!
 * @brief This function sets a value in member timestamp
 * @param _timestamp New value for member timestamp
 */
void px4_msgs::msg::DebugVect::timestamp(uint64_t _timestamp)
{
m_timestamp = _timestamp;
}

/*!
 * @brief This function returns the value of member timestamp
 * @return Value of member timestamp
 */
uint64_t px4_msgs::msg::DebugVect::timestamp() const
{
    return m_timestamp;
}

/*!
 * @brief This function returns a reference to member timestamp
 * @return Reference to member timestamp
 */
uint64_t& px4_msgs::msg::DebugVect::timestamp()
{
    return m_timestamp;
}

/*!
 * @brief This function copies the value in member name
 * @param _name New value to be copied in member name
 */
void px4_msgs::msg::DebugVect::name(const px4_msgs::msg::uint8__10 &_name)
{
m_name = _name;
}

/*!
 * @brief This function moves the value in member name
 * @param _name New value to be moved in member name
 */
void px4_msgs::msg::DebugVect::name(px4_msgs::msg::uint8__10 &&_name)
{
m_name = std::move(_name);
}

/*!
 * @brief This function returns a constant reference to member name
 * @return Constant reference to member name
 */
const px4_msgs::msg::uint8__10& px4_msgs::msg::DebugVect::name() const
{
    return m_name;
}

/*!
 * @brief This function returns a reference to member name
 * @return Reference to member name
 */
px4_msgs::msg::uint8__10& px4_msgs::msg::DebugVect::name()
{
    return m_name;
}
/*!
 * @brief This function sets a value in member x
 * @param _x New value for member x
 */
void px4_msgs::msg::DebugVect::x(float _x)
{
m_x = _x;
}

/*!
 * @brief This function returns the value of member x
 * @return Value of member x
 */
float px4_msgs::msg::DebugVect::x() const
{
    return m_x;
}

/*!
 * @brief This function returns a reference to member x
 * @return Reference to member x
 */
float& px4_msgs::msg::DebugVect::x()
{
    return m_x;
}

/*!
 * @brief This function sets a value in member y
 * @param _y New value for member y
 */
void px4_msgs::msg::DebugVect::y(float _y)
{
m_y = _y;
}

/*!
 * @brief This function returns the value of member y
 * @return Value of member y
 */
float px4_msgs::msg::DebugVect::y() const
{
    return m_y;
}

/*!
 * @brief This function returns a reference to member y
 * @return Reference to member y
 */
float& px4_msgs::msg::DebugVect::y()
{
    return m_y;
}

/*!
 * @brief This function sets a value in member z
 * @param _z New value for member z
 */
void px4_msgs::msg::DebugVect::z(float _z)
{
m_z = _z;
}

/*!
 * @brief This function returns the value of member z
 * @return Value of member z
 */
float px4_msgs::msg::DebugVect::z() const
{
    return m_z;
}

/*!
 * @brief This function returns a reference to member z
 * @return Reference to member z
 */
float& px4_msgs::msg::DebugVect::z()
{
    return m_z;
}


size_t px4_msgs::msg::DebugVect::getKeyMaxCdrSerializedSize(size_t current_alignment)
{
    size_t current_align = current_alignment;








    return current_align;
}

bool px4_msgs::msg::DebugVect::isKeyDefined()
{
   return false;
}

void px4_msgs::msg::DebugVect::serializeKey(eprosima::fastcdr::Cdr &scdr) const
{
    (void) scdr;
     
     
     
     
     
}


