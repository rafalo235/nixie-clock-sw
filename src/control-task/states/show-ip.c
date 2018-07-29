/*
 show-ip.c

  Created on: Jul 29, 2018
      Author: Rafał Olejniczak
 */

#include "control-task/states/states.h"
#include "control-task/control-task.h"
#include "esp/esp.h"
#include "esp/esp_sta.h"
#include "display/display.h"

void ShowIpState(void *sm, tUtils_Signal sig)
{
  tControlContext *context = sm;
  static esp_ip_t ip, gw, nm;
  static int i = 0;

  switch (sig)
  {
  case UTILS_ENTER_STATE :
  {
    esp_sta_copy_ip(&ip, &gw, &nm);
    context->position = 0;
    i = 0;
    break;
  }
  case CONTROL_ACTION_SECOND_TICK :
  {
    uint8_t show = ip.ip[context->position];

    Display_Print(show / 100, show % 100);

    if (2 == i++)
    {
      i = 0;
      if (4 == (context->position++))
      {
        STATE_CHANGE(context, &IdleState);
      }
    }
    break;
  }
  case UTILS_LEAVE_STATE :
  {
    context->position = 1;
    break;
  }
  }
}
