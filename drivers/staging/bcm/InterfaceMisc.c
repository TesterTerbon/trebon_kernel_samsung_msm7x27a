#include "headers.h"

<<<<<<< HEAD
<<<<<<< HEAD
INT
InterfaceRDM(PS_INTERFACE_ADAPTER psIntfAdapter,
            UINT addr,
            PVOID buff,
            INT len)
{
	int retval = 0;
	USHORT usRetries = 0 ;
	if(psIntfAdapter == NULL )
	{
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter,DBG_TYPE_PRINTK, 0, 0,"Interface Adapter is NULL");
		return -EINVAL ;
	}

	if(psIntfAdapter->psAdapter->device_removed == TRUE)
	{
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter,DBG_TYPE_PRINTK, 0, 0,"Device got removed");
		return -ENODEV;
	}

	if((psIntfAdapter->psAdapter->StopAllXaction == TRUE) && (psIntfAdapter->psAdapter->chip_id >= T3LPB))
	{
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter,DBG_TYPE_OTHERS, RDM, DBG_LVL_ALL,"Currently Xaction is not allowed on the bus");
		return -EACCES;
	}

	if(psIntfAdapter->bSuspended ==TRUE || psIntfAdapter->bPreparingForBusSuspend == TRUE)
	{
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter,DBG_TYPE_OTHERS, RDM, DBG_LVL_ALL,"Bus is in suspended states hence RDM not allowed..");
		return -EACCES;
	}
	psIntfAdapter->psAdapter->DeviceAccess = TRUE ;
	do {
		retval = usb_control_msg(psIntfAdapter->udev,
				usb_rcvctrlpipe(psIntfAdapter->udev,0),
	            0x02,
	            0xC2,
	            (addr & 0xFFFF),
	            ((addr >> 16) & 0xFFFF),
				buff,
	            len,
	            5000);

		usRetries++ ;
		if(-ENODEV == retval)
		{
			psIntfAdapter->psAdapter->device_removed =TRUE;
			break;
		}

	}while((retval < 0) && (usRetries < MAX_RDM_WRM_RETIRES ) );

	if(retval < 0)
	{
			BCM_DEBUG_PRINT(psIntfAdapter->psAdapter,DBG_TYPE_OTHERS, RDM, DBG_LVL_ALL, "RDM failed status :%d, retires :%d", retval,usRetries);
			psIntfAdapter->psAdapter->DeviceAccess = FALSE ;
			return retval;
	}
	else
	{
			BCM_DEBUG_PRINT(psIntfAdapter->psAdapter,DBG_TYPE_OTHERS, RDM, DBG_LVL_ALL, "RDM sent %d", retval);
			psIntfAdapter->psAdapter->DeviceAccess = FALSE ;
			return STATUS_SUCCESS;
	}
}

INT
InterfaceWRM(PS_INTERFACE_ADAPTER psIntfAdapter,
            UINT addr,
            PVOID buff,
            INT len)
{
	int retval = 0;
	USHORT usRetries = 0 ;

	if(psIntfAdapter == NULL )
	{
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter,DBG_TYPE_PRINTK, 0, 0, "Interface Adapter  is NULL");
		return -EINVAL;
	}
	if(psIntfAdapter->psAdapter->device_removed == TRUE)
	{

		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter,DBG_TYPE_PRINTK, 0, 0,"Device got removed");
		return -ENODEV;
	}

	if((psIntfAdapter->psAdapter->StopAllXaction == TRUE) && (psIntfAdapter->psAdapter->chip_id >= T3LPB))
	{
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter,DBG_TYPE_OTHERS, WRM, DBG_LVL_ALL,"Currently Xaction is not allowed on the bus...");
		return -EACCES;
	}

	if(psIntfAdapter->bSuspended ==TRUE || psIntfAdapter->bPreparingForBusSuspend == TRUE)
	{
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter,DBG_TYPE_OTHERS, WRM, DBG_LVL_ALL,"Bus is in suspended states hence RDM not allowed..");
		return -EACCES;
	}
	psIntfAdapter->psAdapter->DeviceAccess = TRUE ;
	do{
		retval = usb_control_msg(psIntfAdapter->udev,
				usb_sndctrlpipe(psIntfAdapter->udev,0),
	            0x01,
	            0x42,
	            (addr & 0xFFFF),
	            ((addr >> 16) & 0xFFFF),
				buff,
	            len,
	            5000);

		usRetries++ ;
		if(-ENODEV == retval)
		{
			psIntfAdapter->psAdapter->device_removed = TRUE ;
			break;
		}

	}while((retval < 0) && ( usRetries < MAX_RDM_WRM_RETIRES));

	if(retval < 0)
	{
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter,DBG_TYPE_OTHERS, WRM, DBG_LVL_ALL, "WRM failed status :%d, retires :%d", retval, usRetries);
		psIntfAdapter->psAdapter->DeviceAccess = FALSE ;
		return retval;
	}
	else
	{
		psIntfAdapter->psAdapter->DeviceAccess = FALSE ;
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter,DBG_TYPE_OTHERS, WRM, DBG_LVL_ALL, "WRM sent %d", retval);
		return STATUS_SUCCESS;

	}

}

INT
BcmRDM(PVOID arg,
			UINT addr,
			PVOID buff,
			INT len)
=======
INT InterfaceRDM(PS_INTERFACE_ADAPTER psIntfAdapter,
		UINT addr,
		PVOID buff,
		INT len)
{
	int bytes;
	USHORT usRetries = 0;

	if (psIntfAdapter == NULL) {
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_PRINTK, 0, 0, "Interface Adapter is NULL");
		return -EINVAL;
	}
=======
int InterfaceRDM(struct bcm_interface_adapter *psIntfAdapter,
		unsigned int addr,
		void *buff,
		int len)
{
	int bytes;

	if (!psIntfAdapter)
		return -EINVAL;
>>>>>>> refs/remotes/origin/master

	if (psIntfAdapter->psAdapter->device_removed == TRUE) {
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_PRINTK, 0, 0, "Device got removed");
		return -ENODEV;
	}

	if ((psIntfAdapter->psAdapter->StopAllXaction == TRUE) && (psIntfAdapter->psAdapter->chip_id >= T3LPB))	{
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_OTHERS, RDM, DBG_LVL_ALL, "Currently Xaction is not allowed on the bus");
		return -EACCES;
	}

	if (psIntfAdapter->bSuspended == TRUE || psIntfAdapter->bPreparingForBusSuspend == TRUE) {
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_OTHERS, RDM, DBG_LVL_ALL, "Bus is in suspended states hence RDM not allowed..");
		return -EACCES;
	}
	psIntfAdapter->psAdapter->DeviceAccess = TRUE;

<<<<<<< HEAD
	do {
		bytes = usb_control_msg(psIntfAdapter->udev,
					usb_rcvctrlpipe(psIntfAdapter->udev, 0),
					0x02,
					0xC2,
					(addr & 0xFFFF),
					((addr >> 16) & 0xFFFF),
					buff,
					len,
					5000);

		usRetries++;
		if (-ENODEV == bytes) {
			psIntfAdapter->psAdapter->device_removed = TRUE;
			break;
		}

	} while ((bytes < 0) && (usRetries < MAX_RDM_WRM_RETIRES));

	if (bytes < 0)
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_OTHERS, RDM, DBG_LVL_ALL, "RDM failed status :%d, retires :%d", bytes, usRetries);
	else
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_OTHERS, RDM, DBG_LVL_ALL, "RDM sent %d", bytes);

	psIntfAdapter->psAdapter->DeviceAccess = FALSE;
	return bytes;
}

INT InterfaceWRM(PS_INTERFACE_ADAPTER psIntfAdapter,
		UINT addr,
		PVOID buff,
		INT len)
{
	int retval = 0;
	USHORT usRetries = 0;

	if (psIntfAdapter == NULL) {
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_PRINTK, 0, 0, "Interface Adapter  is NULL");
		return -EINVAL;
	}
=======
	bytes = usb_control_msg(psIntfAdapter->udev,
				usb_rcvctrlpipe(psIntfAdapter->udev, 0),
				0x02,
				0xC2,
				(addr & 0xFFFF),
				((addr >> 16) & 0xFFFF),
				buff,
				len,
				5000);

	if (-ENODEV == bytes)
		psIntfAdapter->psAdapter->device_removed = TRUE;

	if (bytes < 0)
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_OTHERS, RDM, DBG_LVL_ALL, "RDM failed status :%d", bytes);
	else
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_OTHERS, RDM, DBG_LVL_ALL, "RDM sent %d", bytes);

	psIntfAdapter->psAdapter->DeviceAccess = false;
	return bytes;
}

int InterfaceWRM(struct bcm_interface_adapter *psIntfAdapter,
		unsigned int addr,
		void *buff,
		int len)
{
	int retval = 0;

	if (!psIntfAdapter)
		return -EINVAL;
>>>>>>> refs/remotes/origin/master

	if (psIntfAdapter->psAdapter->device_removed == TRUE) {
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_PRINTK, 0, 0, "Device got removed");
		return -ENODEV;
	}

	if ((psIntfAdapter->psAdapter->StopAllXaction == TRUE) && (psIntfAdapter->psAdapter->chip_id >= T3LPB)) {
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_OTHERS, WRM, DBG_LVL_ALL, "Currently Xaction is not allowed on the bus...");
		return -EACCES;
	}

	if (psIntfAdapter->bSuspended == TRUE || psIntfAdapter->bPreparingForBusSuspend == TRUE) {
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_OTHERS, WRM, DBG_LVL_ALL, "Bus is in suspended states hence RDM not allowed..");
		return -EACCES;
	}

	psIntfAdapter->psAdapter->DeviceAccess = TRUE;

<<<<<<< HEAD
	do {
		retval = usb_control_msg(psIntfAdapter->udev,
					usb_sndctrlpipe(psIntfAdapter->udev, 0),
					0x01,
					0x42,
					(addr & 0xFFFF),
					((addr >> 16) & 0xFFFF),
					buff,
					len,
					5000);

		usRetries++;
		if (-ENODEV == retval) {
			psIntfAdapter->psAdapter->device_removed = TRUE;
			break;
		}

	} while ((retval < 0) && (usRetries < MAX_RDM_WRM_RETIRES));

	if (retval < 0)	{
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_OTHERS, WRM, DBG_LVL_ALL, "WRM failed status :%d, retires :%d", retval, usRetries);
		psIntfAdapter->psAdapter->DeviceAccess = FALSE;
		return retval;
	} else {
		psIntfAdapter->psAdapter->DeviceAccess = FALSE;
=======
	retval = usb_control_msg(psIntfAdapter->udev,
				usb_sndctrlpipe(psIntfAdapter->udev, 0),
				0x01,
				0x42,
				(addr & 0xFFFF),
				((addr >> 16) & 0xFFFF),
				buff,
				len,
				5000);

	if (-ENODEV == retval)
		psIntfAdapter->psAdapter->device_removed = TRUE;

	if (retval < 0)	{
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_OTHERS, WRM, DBG_LVL_ALL, "WRM failed status :%d", retval);
		psIntfAdapter->psAdapter->DeviceAccess = false;
		return retval;
	} else {
		psIntfAdapter->psAdapter->DeviceAccess = false;
>>>>>>> refs/remotes/origin/master
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_OTHERS, WRM, DBG_LVL_ALL, "WRM sent %d", retval);
		return STATUS_SUCCESS;
	}
}

<<<<<<< HEAD
INT BcmRDM(PVOID arg,
	UINT addr,
	PVOID buff,
	INT len)
>>>>>>> refs/remotes/origin/cm-10.0
{
	return InterfaceRDM((PS_INTERFACE_ADAPTER)arg, addr, buff, len);
}

<<<<<<< HEAD
INT
BcmWRM(PVOID arg,
			UINT addr,
			PVOID buff,
			INT len)
=======
INT BcmWRM(PVOID arg,
	UINT addr,
	PVOID buff,
	INT len)
>>>>>>> refs/remotes/origin/cm-10.0
{
	return InterfaceWRM((PS_INTERFACE_ADAPTER)arg, addr, buff, len);
}

<<<<<<< HEAD


INT Bcm_clear_halt_of_endpoints(PMINI_ADAPTER Adapter)
{
	PS_INTERFACE_ADAPTER psIntfAdapter = (PS_INTERFACE_ADAPTER)(Adapter->pvInterfaceAdapter);
	INT status = STATUS_SUCCESS ;

	/*
		 usb_clear_halt - tells device to clear endpoint halt/stall condition
		 @dev: device whose endpoint is halted
		 @pipe: endpoint "pipe" being cleared
		 @ Context: !in_interrupt ()

		usb_clear_halt is the synchrnous call and returns 0 on success else returns with error code.
		This is used to clear halt conditions for bulk and interrupt endpoints only.
		 Control and isochronous endpoints never halts.

		Any URBs  queued for such an endpoint should normally be unlinked by the driver
		before clearing the halt condition.

	*/

	//Killing all the submitted urbs to different end points.
	Bcm_kill_all_URBs(psIntfAdapter);


	//clear the halted/stalled state for every end point
	status = usb_clear_halt(psIntfAdapter->udev,psIntfAdapter->sIntrIn.int_in_pipe);
	if(status != STATUS_SUCCESS)
			BCM_DEBUG_PRINT(Adapter,DBG_TYPE_OTHERS, INTF_INIT, DBG_LVL_ALL, "Unable to Clear Halt of Interrupt IN end point. :%d ", status);

	status = usb_clear_halt(psIntfAdapter->udev,psIntfAdapter->sBulkIn.bulk_in_pipe);
	if(status != STATUS_SUCCESS)
			BCM_DEBUG_PRINT(Adapter,DBG_TYPE_OTHERS, INTF_INIT, DBG_LVL_ALL, "Unable to Clear Halt of Bulk IN end point. :%d ", status);

	status = usb_clear_halt(psIntfAdapter->udev,psIntfAdapter->sBulkOut.bulk_out_pipe);
	if(status != STATUS_SUCCESS)
			BCM_DEBUG_PRINT(Adapter,DBG_TYPE_OTHERS, INTF_INIT, DBG_LVL_ALL, "Unable to Clear Halt of Bulk OUT end point. :%d ", status);

	return status ;
}


=======
INT Bcm_clear_halt_of_endpoints(PMINI_ADAPTER Adapter)
{
	PS_INTERFACE_ADAPTER psIntfAdapter = (PS_INTERFACE_ADAPTER)(Adapter->pvInterfaceAdapter);
	INT status = STATUS_SUCCESS;
=======
int BcmRDM(void *arg,
	unsigned int addr,
	void *buff,
	int len)
{
	return InterfaceRDM((struct bcm_interface_adapter *)arg, addr, buff, len);
}

int BcmWRM(void *arg,
	unsigned int addr,
	void *buff,
	int len)
{
	return InterfaceWRM((struct bcm_interface_adapter *)arg, addr, buff, len);
}

int Bcm_clear_halt_of_endpoints(struct bcm_mini_adapter *Adapter)
{
	struct bcm_interface_adapter *psIntfAdapter = (struct bcm_interface_adapter *)(Adapter->pvInterfaceAdapter);
	int status = STATUS_SUCCESS;
>>>>>>> refs/remotes/origin/master

	/*
	 * usb_clear_halt - tells device to clear endpoint halt/stall condition
	 * @dev: device whose endpoint is halted
	 * @pipe: endpoint "pipe" being cleared
	 * @ Context: !in_interrupt ()
	 *
	 * usb_clear_halt is the synchrnous call and returns 0 on success else returns with error code.
	 * This is used to clear halt conditions for bulk and interrupt endpoints only.
	 * Control and isochronous endpoints never halts.
	 *
	 * Any URBs  queued for such an endpoint should normally be unlinked by the driver
	 * before clearing the halt condition.
	 *
	 */

	/* Killing all the submitted urbs to different end points. */
	Bcm_kill_all_URBs(psIntfAdapter);

	/* clear the halted/stalled state for every end point */
	status = usb_clear_halt(psIntfAdapter->udev, psIntfAdapter->sIntrIn.int_in_pipe);
	if (status != STATUS_SUCCESS)
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, INTF_INIT, DBG_LVL_ALL, "Unable to Clear Halt of Interrupt IN end point. :%d ", status);

	status = usb_clear_halt(psIntfAdapter->udev, psIntfAdapter->sBulkIn.bulk_in_pipe);
	if (status != STATUS_SUCCESS)
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, INTF_INIT, DBG_LVL_ALL, "Unable to Clear Halt of Bulk IN end point. :%d ", status);

	status = usb_clear_halt(psIntfAdapter->udev, psIntfAdapter->sBulkOut.bulk_out_pipe);
	if (status != STATUS_SUCCESS)
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS, INTF_INIT, DBG_LVL_ALL, "Unable to Clear Halt of Bulk OUT end point. :%d ", status);

	return status;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
VOID Bcm_kill_all_URBs(PS_INTERFACE_ADAPTER psIntfAdapter)
{
	struct urb *tempUrb = NULL;
	UINT i;

<<<<<<< HEAD
	/**
	  *     usb_kill_urb - cancel a transfer request and wait for it to finish
 	  *     @urb: pointer to URB describing a previously submitted request,
 	  *	  returns nothing as it is void returned API.
 	  *
	  * 	 This routine cancels an in-progress request. It is guaranteed that
 	  *     upon return all completion handlers will have finished and the URB
 	  *     will be totally idle and available for reuse

 	  *	This routine may not be used in an interrupt context (such as a bottom
 	  *	 half or a completion handler), or when holding a spinlock, or in other
 	  *	 situations where the caller can't schedule().
	  *
	**/

	/* Cancel submitted Interrupt-URB's */
	if(psIntfAdapter->psInterruptUrb != NULL)
	{
		if(psIntfAdapter->psInterruptUrb->status == -EINPROGRESS)
				 usb_kill_urb(psIntfAdapter->psInterruptUrb);
	}

	/* Cancel All submitted TX URB's */
	for(i = 0; i < MAXIMUM_USB_TCB; i++)
	{
		tempUrb = psIntfAdapter->asUsbTcb[i].urb;
		if(tempUrb)
		{
			if(tempUrb->status == -EINPROGRESS)
=======
=======
void Bcm_kill_all_URBs(struct bcm_interface_adapter *psIntfAdapter)
{
	struct urb *tempUrb = NULL;
	unsigned int i;

>>>>>>> refs/remotes/origin/master
	/*
	 * usb_kill_urb - cancel a transfer request and wait for it to finish
	 * @urb: pointer to URB describing a previously submitted request,
	 * returns nothing as it is void returned API.
	 *
	 * This routine cancels an in-progress request. It is guaranteed that
	 * upon return all completion handlers will have finished and the URB
	 * will be totally idle and available for reuse
	 *
	 * This routine may not be used in an interrupt context (such as a bottom
	 * half or a completion handler), or when holding a spinlock, or in other
	 * situations where the caller can't schedule().
	 *
	 */

	/* Cancel submitted Interrupt-URB's */
<<<<<<< HEAD
	if (psIntfAdapter->psInterruptUrb != NULL) {
=======
	if (psIntfAdapter->psInterruptUrb) {
>>>>>>> refs/remotes/origin/master
		if (psIntfAdapter->psInterruptUrb->status == -EINPROGRESS)
			usb_kill_urb(psIntfAdapter->psInterruptUrb);
	}

	/* Cancel All submitted TX URB's */
	for (i = 0; i < MAXIMUM_USB_TCB; i++) {
		tempUrb = psIntfAdapter->asUsbTcb[i].urb;
		if (tempUrb) {
			if (tempUrb->status == -EINPROGRESS)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				usb_kill_urb(tempUrb);
		}
	}

<<<<<<< HEAD
<<<<<<< HEAD
	for(i = 0; i < MAXIMUM_USB_RCB; i++)
	{
		tempUrb = psIntfAdapter->asUsbRcb[i].urb;
		if(tempUrb)
		{
			if(tempUrb->status == -EINPROGRESS)
					usb_kill_urb(tempUrb);
=======
=======
>>>>>>> refs/remotes/origin/master
	for (i = 0; i < MAXIMUM_USB_RCB; i++) {
		tempUrb = psIntfAdapter->asUsbRcb[i].urb;
		if (tempUrb) {
			if (tempUrb->status == -EINPROGRESS)
				usb_kill_urb(tempUrb);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		}
	}

	atomic_set(&psIntfAdapter->uNumTcbUsed, 0);
	atomic_set(&psIntfAdapter->uCurrTcb, 0);

	atomic_set(&psIntfAdapter->uNumRcbUsed, 0);
	atomic_set(&psIntfAdapter->uCurrRcb, 0);
}

<<<<<<< HEAD
VOID putUsbSuspend(struct work_struct *work)
{
<<<<<<< HEAD
	PS_INTERFACE_ADAPTER psIntfAdapter = NULL ;
	struct usb_interface *intf = NULL ;
	psIntfAdapter = container_of(work, S_INTERFACE_ADAPTER,usbSuspendWork);
	intf=psIntfAdapter->interface ;

	if(psIntfAdapter->bSuspended == FALSE)
		usb_autopm_put_interface(intf);

=======
	PS_INTERFACE_ADAPTER psIntfAdapter = NULL;
	struct usb_interface *intf = NULL;
	psIntfAdapter = container_of(work, S_INTERFACE_ADAPTER, usbSuspendWork);
	intf = psIntfAdapter->interface;

	if (psIntfAdapter->bSuspended == FALSE)
		usb_autopm_put_interface(intf);
>>>>>>> refs/remotes/origin/cm-10.0
=======
void putUsbSuspend(struct work_struct *work)
{
	struct bcm_interface_adapter *psIntfAdapter = NULL;
	struct usb_interface *intf = NULL;
	psIntfAdapter = container_of(work, struct bcm_interface_adapter, usbSuspendWork);
	intf = psIntfAdapter->interface;

	if (psIntfAdapter->bSuspended == false)
		usb_autopm_put_interface(intf);
>>>>>>> refs/remotes/origin/master
}

