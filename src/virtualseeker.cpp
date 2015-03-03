/*
  Zipios++ - a small C++ library that provides easy access to .zip files.
  Copyright (C) 2000-2015  Thomas Sondergaard

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
*/

/** \file
 * Implementation file that defines VirtualSeeker.
 */

#include "zipios++/virtualseeker.h"

#include "zipios++/zipiosexceptions.h"


namespace zipios
{


/** \class VirtualSeeker
 *
 * The virtual seeker class is a simple definition of an object
 * that keeps track of a set of specified (virtual) file pointers
 * that mark start and end of a file inside another.
 *
 * An example of its use (and its reason for existence) is to
 * keep track of the file endings of a Zip file embedded in another
 * file (see the appendzip tool and the ZipFile::openEmbeddedZipFile()
 * function).
 */


/** \brief Create a virtual seeker.
 *
 * This constructor defines a virtual seeker start and end offset
 * on initialization.
 *
 * \warning
 * If the offsets are left undefined (both set to zero) then the virtual
 * seeker is viewed as a transparent seeker, meaning that it seeks in
 * the input streams as if it did not exist.
 *
 * \exception InvalidException
 * The start offset must be before or equal to the end offset or
 * this exception is raised.
 *
 * \param[in] start_offset  The start offset of the embedded file.
 * \param[in] end_offset  The end offset of the embedded file.
 */
VirtualSeeker::VirtualSeeker(offset_t start_offset, offset_t end_offset)
    : m_start_offset(start_offset)
    , m_end_offset(end_offset)
{
    if(m_start_offset > m_end_offset)
    {
        throw InvalidException("VirtualSeeker::VirtualSeeker(): the start offset cannot be larged than the end offset.");
    }
}


/** \brief Set the offsets of the virtual seeker.
 *
 * This function can be used to change the virtual seeker offsets.
 *
 * \exception InvalidException
 * The start offset must be before or equal to the end offset or
 * this exception is raised.
 *
 * \param[in] start_offset  The new start offset.
 * \param[in] end_offset  The new end offset.
 */
void VirtualSeeker::setOffsets(off_t start_offset, off_t end_offset)
{
    if(start_offset > end_offset)
    {
        throw InvalidException("VirtualSeeker::VirtualSeeker(): the start offset cannot be larged than the end offset.");
    }

    m_start_offset = start_offset;
    m_end_offset = end_offset;
}


/** \brief Retrieve the current offsets.
 *
 * This function retrieves the start and end offsets from the virtual
 * seeker object.
 *
 * \param[out] start_offset  Returns the start offset.
 * \param[out] end_offset  Returns the end offset.
 */
void VirtualSeeker::getOffsets(off_t& start_offset, off_t& end_offset) const
{
    start_offset = m_start_offset;
    end_offset = m_end_offset;
}


/** \brief Return the start offset.
 *
 * This function returns a copy of the start offset.
 *
 * \return The start offset.
 */
VirtualSeeker::offset_t VirtualSeeker::startOffset() const
{
    return m_start_offset;
}


/** \brief Return the end offset.
 *
 * This function returns a copy of the end offset.
 *
 * \return The end offset.
 */
VirtualSeeker::offset_t VirtualSeeker::endOffset() const
{
    return m_end_offset;
}


/** \brief Seek within the embedded file.
 *
 * This function changes the file pointer in \p is to the position
 * specified in offset.
 *
 * The direction can be indicated by \p sd.
 *
 * \param[in,out] is  The stream which pointer is to be changed.
 * \param[in] offset  The offset represent the exact position, or
 *                    a relative position (depending on \p sd).
 * \param[in] sd  The stream direction to use.
 */
void VirtualSeeker::vseekg(std::istream &is, offset_t offset, std::ios::seekdir sd) const
{
    switch(sd)
    {
    case std::ios::cur:
        is.seekg(offset, sd);
        break;

    case std::ios::beg:
        is.seekg(offset + m_start_offset,  sd);
        break;

    case std::ios::end:
        // TBD: is that really correct?
        //      should it not be "m_end_offset - offset" instead?
        is.seekg(offset - m_end_offset, sd);
        break;

    default:
        throw std::logic_error("VirtualSeekManager::vseekg(): error - unknown seekdir");

    }
}


/** \brief Current position within the sub-file.
 *
 * This function calculates the position (file current pointer) within
 * the embedded file in the specified stream.
 *
 * \param[in] is  The stream to get the position from.
 *
 * \return The stream offset within the embedded file.
 */
std::streampos VirtualSeeker::vtellg(std::istream& is) const
{
    //return static_cast<off_t>(is.tellg()) - m_start_offset;
    std::streampos pos(is.tellg() - m_start_offset);
    if(pos < 0
    || (m_end_offset > m_start_offset && m_end_offset != 0 && pos > m_end_offset - m_start_offset))
    {
        // position is out of range
        pos = -1;
    }
    return pos;
}


} // zipios namespace
// vim: ts=4 sw=4 et