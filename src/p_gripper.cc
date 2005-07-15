/*
 *  Player - One Hell of a Robot Server
 *  Copyright (C) 2004, 2005 Richard Vaughan
 *                      
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

/*
 * Desc: A plugin driver for Player that gives access to Stage devices.
 * Author: Richard Vaughan
 * Date: 10 December 2004
 * CVS: $Id: p_gripper.cc,v 1.1 2005-07-15 03:41:37 rtv Exp $
 */


#include "p_driver.h"

#include "playerclient.h" // for the dumb pioneer gripper command defines

//
// GRIPPER INTERFACE
//

void GripperCommand( Interface* device, void* src, size_t len )
{
  if( len == sizeof(player_gripper_cmd_t) )
    {
      player_gripper_cmd_t* pcmd = (player_gripper_cmd_t*)src;
      //printf("GripperCommand: %d\n", pcmd->cmd);
      // Pass it to stage:
      stg_gripper_cmd_t cmd; 
      cmd.cmd = STG_GRIPPER_CMD_NOP;
      cmd.arg = 0;

      switch( pcmd->cmd )
	{
	case GRIPclose: cmd.cmd = STG_GRIPPER_CMD_CLOSE; break;
	case GRIPopen:  cmd.cmd = STG_GRIPPER_CMD_OPEN; break;
	case LIFTup:    cmd.cmd = STG_GRIPPER_CMD_UP; break;
	case LIFTdown:  cmd.cmd = STG_GRIPPER_CMD_DOWN; break;
	  //default:
	  //printf( "Stage: player gripper command %d is not implemented\n", 
	  //  pcmd->cmd );
	}

      //cmd.cmd  = pcmd->cmd;
      cmd.arg = pcmd->arg;

      //stg_model_set_command( device->mod, &cmd, sizeof(cmd) ) ;
      stg_model_set_property( device->mod, "gripper_cmd", &cmd, sizeof(cmd));
    }
  else
    PRINT_ERR2( "wrong size gripper command packet (%d/%d bytes)",
		(int)len, (int)sizeof(player_position_cmd_t) );
}

void GripperData( Interface* device, void* data, size_t len )
{
  //puts( "publishing gripper data\n" );
  
  if( len == sizeof(stg_gripper_data_t) )
    {
      stg_gripper_data_t* sdata = (stg_gripper_data_t*)data;

      player_gripper_data_t pdata;
      memset( &pdata, 0, sizeof(pdata) );
            
      // set the proper bits
      
      pdata.beams = 0;
      pdata.beams |=  sdata->outer_break_beam ? 0x04 : 0x00;
      pdata.beams |=  sdata->inner_break_beam ? 0x08 : 0x00;
      
      pdata.state = 0;  
      pdata.state |= (sdata->paddles == STG_GRIPPER_PADDLE_OPEN) ? 0x01 : 0x00;
      pdata.state |= (sdata->paddles == STG_GRIPPER_PADDLE_CLOSED) ? 0x02 : 0x00;
      pdata.state |= ((sdata->paddles == STG_GRIPPER_PADDLE_OPENING) ||
		      (sdata->paddles == STG_GRIPPER_PADDLE_CLOSING))  ? 0x04 : 0x00;
      
      pdata.state |= (sdata->lift == STG_GRIPPER_LIFT_UP) ? 0x10 : 0x00;
      pdata.state |= (sdata->lift == STG_GRIPPER_LIFT_DOWN) ? 0x20 : 0x00;
      pdata.state |= ((sdata->lift == STG_GRIPPER_LIFT_UPPING) ||
		      (sdata->lift == STG_GRIPPER_LIFT_DOWNING))  ? 0x040 : 0x00;
      
      //pdata.state |= sdata->lift_error ? 0x80 : 0x00;
      //pdata.state |= sdata->gripper_error ? 0x08 : 0x00;

      device->driver->PutData( device->id, &pdata, sizeof(pdata), NULL); 
    }
}

void GripperConfig( Interface* device, void* client, void* buffer, size_t len )
{
  printf("got gripper request\n");
}


