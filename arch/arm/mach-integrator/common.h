<<<<<<< HEAD
void integrator_init_early(void);
void integrator_reserve(void);
<<<<<<< HEAD
=======
void integrator_restart(char, const char *);
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/reboot.h>
#include <linux/amba/serial.h>
extern struct amba_pl010_data ap_uart_data;
void integrator_init_early(void);
int integrator_init(bool is_cp);
void integrator_reserve(void);
void integrator_restart(enum reboot_mode, const char *);
void integrator_init_sysfs(struct device *parent, u32 id);
>>>>>>> refs/remotes/origin/master
