#include "headers.h"
<<<<<<< HEAD
//-----------------------------------------------------------------------------
// Procedure:	vendorextnGetSectionInfo
//
// Description: Finds the type of NVM used.
//
// Arguments:
//		Adapter    - ptr to Adapter object instance
//		pNVMType   - ptr to NVM type.
// Returns:
//		STATUS_SUCCESS/STATUS_FAILURE
//
//-----------------------------------------------------------------------------
INT vendorextnGetSectionInfo(PVOID  pContext,PFLASH2X_VENDORSPECIFIC_INFO pVendorInfo)
=======
/*
 * Procedure:	vendorextnGetSectionInfo
 *
 * Description: Finds the type of NVM used.
 *
 * Arguments:
 *		Adapter    - ptr to Adapter object instance
 *		pNVMType   - ptr to NVM type.
 * Returns:
 *		STATUS_SUCCESS/STATUS_FAILURE
 *
 */
INT vendorextnGetSectionInfo(PVOID  pContext, struct bcm_flash2x_vendor_info *pVendorInfo)
>>>>>>> refs/remotes/origin/master
{
	return STATUS_FAILURE;
}

<<<<<<< HEAD
//-----------------------------------------------------------------------------
// Procedure:   vendorextnInit
//
// Description: Initializing the vendor extension NVM interface
//
// Arguments:
//              Adapter   - Pointer to MINI Adapter Structure.

// Returns:
//              STATUS_SUCCESS/STATUS_FAILURE
//
//-----------------------------------------------------------------------------
INT vendorextnInit(PMINI_ADAPTER Adapter)
=======
/*
 * Procedure:   vendorextnInit
 *
 * Description: Initializing the vendor extension NVM interface
 *
 * Arguments:
 *              Adapter   - Pointer to MINI Adapter Structure
 * Returns:
 *             STATUS_SUCCESS/STATUS_FAILURE
 *
 *
 */
INT vendorextnInit(struct bcm_mini_adapter *Adapter)
>>>>>>> refs/remotes/origin/master
{
	return STATUS_SUCCESS;
}

<<<<<<< HEAD
//-----------------------------------------------------------------------------
// Procedure:   vendorextnExit
//
// Description: Free the resource associated with vendor extension NVM interface
//
// Arguments:
//              Adapter   - Pointer to MINI Adapter Structure.

// Returns:
//              STATUS_SUCCESS/STATUS_FAILURE
//
//-----------------------------------------------------------------------------
INT vendorextnExit(PMINI_ADAPTER Adapter)
=======
/*
 * Procedure:   vendorextnExit
 *
 * Description: Free the resource associated with vendor extension NVM interface
 *
 * Arguments:
 *
 * Returns:
 *              STATUS_SUCCESS/STATUS_FAILURE
 *
 *
 */
INT vendorextnExit(struct bcm_mini_adapter *Adapter)
>>>>>>> refs/remotes/origin/master
{
	return STATUS_SUCCESS;
}

<<<<<<< HEAD
//------------------------------------------------------------------------
// Procedure:	vendorextnIoctl
//
// Description: 	execute the vendor extension specific ioctl
//
//Arguments:
//		Adapter -Beceem private Adapter Structure
//		cmd 	-vendor extension specific Ioctl commad
//		arg		-input parameter sent by vendor
//
// Returns:
//		CONTINUE_COMMON_PATH in case it is not meant to be processed by vendor ioctls
//		STATUS_SUCCESS/STATUS_FAILURE as per the IOCTL return value
//
//--------------------------------------------------------------------------
INT vendorextnIoctl(PMINI_ADAPTER Adapter, UINT cmd, ULONG arg)
=======
/*
 * Procedure:	vendorextnIoctl
 *
 * Description: execute the vendor extension specific ioctl
 *
 * Arguments:
 *		Adapter -Beceem private Adapter Structure
 *		cmd	-vendor extension specific Ioctl commad
 *		arg	-input parameter sent by vendor
 *
 * Returns:
 *		CONTINUE_COMMON_PATH in case it is not meant to be processed by vendor ioctls
 *		STATUS_SUCCESS/STATUS_FAILURE as per the IOCTL return value
 */

INT vendorextnIoctl(struct bcm_mini_adapter *Adapter, UINT cmd, ULONG arg)
>>>>>>> refs/remotes/origin/master
{
	return CONTINUE_COMMON_PATH;
}



<<<<<<< HEAD
//------------------------------------------------------------------
// Procedure:	vendorextnReadSection
//
// Description: Reads from a section of NVM
//
// Arguments:
//		pContext - ptr to Adapter object instance
//		pBuffer - Read the data from Vendor Area to this buffer
//		SectionVal   - Value of type of Section
//		Offset - Read from the Offset of the Vendor Section.
//		numOfBytes - Read numOfBytes from the Vendor section to Buffer
//
// Returns:
//		STATUS_SUCCESS/STATUS_FAILURE
//
//------------------------------------------------------------------

INT vendorextnReadSection(PVOID  pContext, PUCHAR pBuffer, FLASH2X_SECTION_VAL SectionVal,
=======
/*
 * Procedure:	vendorextnReadSection
 *
 * Description: Reads from a section of NVM
 *
 * Arguments:
 *		pContext - ptr to Adapter object instance
 *		pBuffer - Read the data from Vendor Area to this buffer
 *		SectionVal   - Value of type of Section
 *		Offset - Read from the Offset of the Vendor Section.
 *		numOfBytes - Read numOfBytes from the Vendor section to Buffer
 *
 * Returns:
 *		STATUS_SUCCESS/STATUS_FAILURE
 */

INT vendorextnReadSection(PVOID  pContext, PUCHAR pBuffer, enum bcm_flash2x_section_val SectionVal,
>>>>>>> refs/remotes/origin/master
			UINT offset, UINT numOfBytes)
{
	return STATUS_FAILURE;
}



<<<<<<< HEAD
//------------------------------------------------------------------
// Procedure:	vendorextnWriteSection
//
// Description: Write to a Section of NVM
//
// Arguments:
//		pContext - ptr to Adapter object instance
//		pBuffer - Write the data provided in the buffer
//		SectionVal   - Value of type of Section
//		Offset - Writes to the Offset of the Vendor Section.
//		numOfBytes - Write num Bytes after reading from pBuffer.
//		bVerify - the Buffer Written should be verified.
//
// Returns:
//		STATUS_SUCCESS/STATUS_FAILURE
//
//------------------------------------------------------------------
INT vendorextnWriteSection(PVOID  pContext, PUCHAR pBuffer, FLASH2X_SECTION_VAL SectionVal,
			UINT offset, UINT numOfBytes, BOOLEAN bVerify)
=======
/*
 * Procedure:	vendorextnWriteSection
 *
 * Description: Write to a Section of NVM
 *
 * Arguments:
 *		pContext - ptr to Adapter object instance
 *		pBuffer - Write the data provided in the buffer
 *		SectionVal   - Value of type of Section
 *		Offset - Writes to the Offset of the Vendor Section.
 *		numOfBytes - Write num Bytes after reading from pBuffer.
 *		bVerify - the Buffer Written should be verified.
 *
 * Returns:
 *		STATUS_SUCCESS/STATUS_FAILURE
 */
INT vendorextnWriteSection(PVOID  pContext, PUCHAR pBuffer, enum bcm_flash2x_section_val SectionVal,
			UINT offset, UINT numOfBytes, bool bVerify)
>>>>>>> refs/remotes/origin/master
{
	return STATUS_FAILURE;
}



<<<<<<< HEAD
//------------------------------------------------------------------
// Procedure:	vendorextnWriteSectionWithoutErase
//
// Description: Write to a Section of NVM without erasing the sector
//
// Arguments:
//		pContext - ptr to Adapter object instance
//		pBuffer - Write the data provided in the buffer
//		SectionVal   - Value of type of Section
//		Offset - Writes to the Offset of the Vendor Section.
//		numOfBytes - Write num Bytes after reading from pBuffer.
//
// Returns:
//		STATUS_SUCCESS/STATUS_FAILURE
//
//------------------------------------------------------------------
INT vendorextnWriteSectionWithoutErase(PVOID  pContext, PUCHAR pBuffer, FLASH2X_SECTION_VAL SectionVal,
=======
/*
 * Procedure:	vendorextnWriteSectionWithoutErase
 *
 * Description: Write to a Section of NVM without erasing the sector
 *
 * Arguments:
 *		pContext - ptr to Adapter object instance
 *		pBuffer - Write the data provided in the buffer
 *		SectionVal   - Value of type of Section
 *		Offset - Writes to the Offset of the Vendor Section.
 *		numOfBytes - Write num Bytes after reading from pBuffer.
 *
 * Returns:
 *		STATUS_SUCCESS/STATUS_FAILURE
 */
INT vendorextnWriteSectionWithoutErase(PVOID  pContext, PUCHAR pBuffer, enum bcm_flash2x_section_val SectionVal,
>>>>>>> refs/remotes/origin/master
			UINT offset, UINT numOfBytes)
{
	return STATUS_FAILURE;
}
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
