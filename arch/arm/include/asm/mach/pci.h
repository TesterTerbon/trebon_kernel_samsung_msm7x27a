/*
 *  arch/arm/include/asm/mach/pci.h
 *
 *  Copyright (C) 2000 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __ASM_MACH_PCI_H
#define __ASM_MACH_PCI_H

<<<<<<< HEAD
struct pci_sys_data;
struct pci_bus;
=======
#include <linux/ioport.h>

struct pci_sys_data;
struct pci_ops;
struct pci_bus;
struct device;
>>>>>>> refs/remotes/origin/master

struct hw_pci {
#ifdef CONFIG_PCI_DOMAINS
	int		domain;
#endif
<<<<<<< HEAD
	struct list_head buses;
	int		nr_controllers;
=======
	struct pci_ops	*ops;
	int		nr_controllers;
	void		**private_data;
>>>>>>> refs/remotes/origin/master
	int		(*setup)(int nr, struct pci_sys_data *);
	struct pci_bus *(*scan)(int nr, struct pci_sys_data *);
	void		(*preinit)(void);
	void		(*postinit)(void);
	u8		(*swizzle)(struct pci_dev *dev, u8 *pin);
<<<<<<< HEAD
<<<<<<< HEAD
	int		(*map_irq)(struct pci_dev *dev, u8 slot, u8 pin);
=======
	int		(*map_irq)(const struct pci_dev *dev, u8 slot, u8 pin);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	int		(*map_irq)(const struct pci_dev *dev, u8 slot, u8 pin);
	resource_size_t (*align_resource)(struct pci_dev *dev,
					  const struct resource *res,
					  resource_size_t start,
					  resource_size_t size,
					  resource_size_t align);
	void		(*add_bus)(struct pci_bus *bus);
	void		(*remove_bus)(struct pci_bus *bus);
>>>>>>> refs/remotes/origin/master
};

/*
 * Per-controller structure
 */
struct pci_sys_data {
#ifdef CONFIG_PCI_DOMAINS
	int		domain;
#endif
	struct list_head node;
	int		busnr;		/* primary bus number			*/
	u64		mem_offset;	/* bus->cpu memory mapping offset	*/
	unsigned long	io_offset;	/* bus->cpu IO mapping offset		*/
	struct pci_bus	*bus;		/* PCI bus				*/
<<<<<<< HEAD
<<<<<<< HEAD
	struct resource *resource[3];	/* Primary PCI bus resources		*/
					/* Bridge swizzling			*/
	u8		(*swizzle)(struct pci_dev *, u8 *);
					/* IRQ mapping				*/
	int		(*map_irq)(struct pci_dev *, u8, u8);
=======
	struct list_head resources;	/* root bus resources (apertures)       */
=======
	struct list_head resources;	/* root bus resources (apertures)       */
	struct resource io_res;
	char		io_res_name[12];
>>>>>>> refs/remotes/origin/master
					/* Bridge swizzling			*/
	u8		(*swizzle)(struct pci_dev *, u8 *);
					/* IRQ mapping				*/
	int		(*map_irq)(const struct pci_dev *, u8, u8);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	struct hw_pci	*hw;
=======
					/* Resource alignement requirements	*/
	resource_size_t (*align_resource)(struct pci_dev *dev,
					  const struct resource *res,
					  resource_size_t start,
					  resource_size_t size,
					  resource_size_t align);
	void		(*add_bus)(struct pci_bus *bus);
	void		(*remove_bus)(struct pci_bus *bus);
>>>>>>> refs/remotes/origin/master
	void		*private_data;	/* platform controller private data	*/
};

/*
<<<<<<< HEAD
 * This is the standard PCI-PCI bridge swizzling algorithm.
 */
#define pci_std_swizzle pci_common_swizzle

/*
 * Call this with your hw_pci struct to initialise the PCI system.
 */
void pci_common_init(struct hw_pci *);
=======
 * Call this with your hw_pci struct to initialise the PCI system.
 */
void pci_common_init_dev(struct device *, struct hw_pci *);

/*
 * Compatibility wrapper for older platforms that do not care about
 * passing the parent device.
 */
static inline void pci_common_init(struct hw_pci *hw)
{
	pci_common_init_dev(NULL, hw);
}

/*
 * Setup early fixed I/O mapping.
 */
#if defined(CONFIG_PCI)
extern void pci_map_io_early(unsigned long pfn);
#else
static inline void pci_map_io_early(unsigned long pfn) {}
#endif
>>>>>>> refs/remotes/origin/master

/*
 * PCI controllers
 */
<<<<<<< HEAD
extern int iop3xx_pci_setup(int nr, struct pci_sys_data *);
extern struct pci_bus *iop3xx_pci_scan_bus(int nr, struct pci_sys_data *);
extern void iop3xx_pci_preinit(void);
extern void iop3xx_pci_preinit_cond(void);

extern int dc21285_setup(int nr, struct pci_sys_data *);
extern struct pci_bus *dc21285_scan_bus(int nr, struct pci_sys_data *);
extern void dc21285_preinit(void);
extern void dc21285_postinit(void);

extern int via82c505_setup(int nr, struct pci_sys_data *);
extern struct pci_bus *via82c505_scan_bus(int nr, struct pci_sys_data *);
extern void via82c505_init(void *sysdata);

extern int pci_v3_setup(int nr, struct pci_sys_data *);
extern struct pci_bus *pci_v3_scan_bus(int nr, struct pci_sys_data *);
extern void pci_v3_preinit(void);
extern void pci_v3_postinit(void);

=======
extern struct pci_ops iop3xx_ops;
extern int iop3xx_pci_setup(int nr, struct pci_sys_data *);
extern void iop3xx_pci_preinit(void);
extern void iop3xx_pci_preinit_cond(void);

extern struct pci_ops dc21285_ops;
extern int dc21285_setup(int nr, struct pci_sys_data *);
extern void dc21285_preinit(void);
extern void dc21285_postinit(void);

>>>>>>> refs/remotes/origin/master
#endif /* __ASM_MACH_PCI_H */
