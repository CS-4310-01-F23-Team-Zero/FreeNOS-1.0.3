/*
 * Copyright (C) 2015 Niek Linnenbank
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "IntController.h"

IntController::IntController()
    : m_base(0)
{
}

uint IntController::getBase() const
{
    return m_base;
}

IntController::Result IntController::nextPending(uint & irq)
{
    return NotFound;
}

bool IntController::isTriggered(uint irq)
{
    return false;
}

IntController::Result IntController::send(const uint targetCoreId, const uint irq)
{
    return IOError;
}
