/*  partmp.h
 *
 *  This include file contains all the constants and structures associated
 *  with the Multiprocessing Support in the Partition Manager.
 *
 *  COPYRIGHT (c) 1989, 1990, 1991, 1992, 1993, 1994.
 *  On-Line Applications Research Corporation (OAR).
 *  All rights assigned to U.S. Government, 1994.
 *
 *  This material may be reproduced by or for the U.S. Government pursuant
 *  to the copyright license under the clause at DFARS 252.227-7013.  This
 *  notice must appear in all copies of this file and its derivatives.
 *
 *  $Id$
 */

#ifndef __RTEMS_PARTITION_MP_h
#define __RTEMS_PARTITION_MP_h

#ifdef __cplusplus
extern "C" {
#endif

#include <rtems/score/mppkt.h>
#include <rtems/score/object.h>
#include <rtems/rtems/options.h>
#include <rtems/score/thread.h>

#include <rtems/rtems/part.h>

/*
 *  The following enumerated type defines the list of
 *  remote partition operations.
 */

typedef enum {
  PARTITION_MP_ANNOUNCE_CREATE        =  0,
  PARTITION_MP_ANNOUNCE_DELETE        =  1,
  PARTITION_MP_EXTRACT_PROXY          =  2,
  PARTITION_MP_GET_BUFFER_REQUEST     =  3,
  PARTITION_MP_GET_BUFFER_RESPONSE    =  4,
  PARTITION_MP_RETURN_BUFFER_REQUEST  =  5,
  PARTITION_MP_RETURN_BUFFER_RESPONSE =  6
}   Partition_MP_Remote_operations;

/*
 *  The following data structure defines the packet used to perform
 *  remote partition operations.
 */

typedef struct {
  rtems_packet_prefix             Prefix;
  Partition_MP_Remote_operations  operation;
  rtems_name                      name;
  void                           *buffer;
  Objects_Id                      proxy_id;
}   Partition_MP_Packet;

/*
 *  _Partition_MP_Send_process_packet
 *
 *  DESCRIPTION:
 *
 *  This routine performs a remote procedure call so that a
 *  process operation can be performed on another node.
 */

void _Partition_MP_Send_process_packet (
  Partition_MP_Remote_operations operation,
  Objects_Id                     partition_id,
  rtems_name                     name,
  Objects_Id                     proxy_id
);

/*
 *  _Partition_MP_Send_request_packet
 *
 *  DESCRIPTION:
 *
 *  This routine performs a remote procedure call so that a
 *  directive operation can be initiated on another node.
 */

rtems_status_code _Partition_MP_Send_request_packet (
  Partition_MP_Remote_operations  operation,
  Objects_Id                      partition_id,
  void                           *buffer
);

/*
 *  _Partition_MP_Send_response_packet
 *
 *  DESCRIPTION:
 *
 *  This routine performs a remote procedure call so that a
 *  directive can be performed on another node.
 */

void _Partition_MP_Send_response_packet (
  Partition_MP_Remote_operations  operation,
  Objects_Id                      partition_id,
  Thread_Control                 *the_thread
);

/*
 *
 *  _Partition_MP_Process_packet
 *
 *  DESCRIPTION:
 *
 *  This routine performs the actions specific to this package for
 *  the request from another node.
 */

void _Partition_MP_Process_packet (
  rtems_packet_prefix *the_packet_prefix
);

/*
 *  _Partition_MP_Send_object_was_deleted
 *
 *  DESCRIPTION:
 *
 *  This routine is invoked indirectly by the thread queue
 *  when a proxy has been removed from the thread queue and
 *  the remote node must be informed of this.
 *
 *  This routine is not needed by the Partition since a partition
 *  cannot be deleted when buffers are in use.
 */

/*
 *  _Partition_MP_Send_extract_proxy
 *
 *  DESCRIPTION:
 *
 *  This routine is invoked when a task is deleted and it
 *  has a proxy which must be removed from a thread queue and
 *  the remote node must be informed of this.
 */

void _Partition_MP_Send_extract_proxy (
  Thread_Control *the_thread
);

/*
 *  _Partition_MP_Get_packet
 *
 *  DESCRIPTION:
 *
 *  This function is used to obtain a partition mp packet.
 */

Partition_MP_Packet *_Partition_MP_Get_packet ( void );

#ifdef __cplusplus
}
#endif

#endif
/* end of file */
