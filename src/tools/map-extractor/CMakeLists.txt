# mangos-one is a full featured server for World of Warcraft in its first
# expansion version, supporting clients for patch 2.4.3.
#
# Copyright (C) 2005-2013  MaNGOS project <http://getmangos.com>
#
# ***** BEGIN GPL LICENSE BLOCK *****
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
# ***** END GPL LICENSE BLOCK *****
#
# World of Warcraft, and all World of Warcraft or Warcraft art, images,
# and lore are copyrighted by Blizzard Entertainment, Inc.

set(EXECUTABLE_NAME map-extractor)

add_subdirectory (libmpq)
add_subdirectory (loadlib)

include_directories (
    ${CMAKE_CURRENT_SOURCE_DIR}/libmpq
    ${CMAKE_CURRENT_SOURCE_DIR}/loadlib
    ${CMAKE_BINARY_DIR}
)

link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/libmpq
    ${CMAKE_CURRENT_SOURCE_DIR}/loadlib
)

add_executable (${EXECUTABLE_NAME} dbcfile.cpp mpq_libmpq.cpp System.cpp)
target_link_libraries (${EXECUTABLE_NAME} libmpq loadlib)
install(TARGETS ${EXECUTABLE_NAME} DESTINATION ${BIN_DIR})
