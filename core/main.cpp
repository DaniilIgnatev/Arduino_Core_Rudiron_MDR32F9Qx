/*
  main.cpp - Main loop for Arduino sketches
  Copyright (c) 2005-2013 Arduino Team.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified for Arduino_Core_Rudiron by Daniil Ignatev on 08.03.2022
*/

#include <Arduino.h>
#include "rudiron/tasks_timer.h"

TasksTimer tasksTimer;

int main(void)
{
  init();

  initVariant();

#if defined(USBCON)
  USBDevice.attach();
#endif

  setup();

  for (;;)
  {
    loop();
    if (serialEventRun)
    {
      serialEventRun();
    }

    if (!tasksTimer.empty())
    {
      tasksTimer.tick();
    }
  }

  return 0;
}
